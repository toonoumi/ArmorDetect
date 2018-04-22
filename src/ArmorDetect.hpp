#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stack>
#include <chrono>
//#include <unistd.h>
#include "AD_Util.h"
#include "ConfirmationAlgo.hpp"
#include "RMVideoCapture.hpp"
#include "CommPort.hpp"

#define DEBUG 	1
#define CAMERA	0
//#define COMM_PORT_NOT_CONNECTED 1


/** @function main */
//---------------------define constant-----------------------

#define CONTOURS_POINT_COUNT_THRESHOLD 150


//---------------------define constant-----------------------
//#include "AD_Util.h"
using namespace std;
using namespace cv;

enum Video_Mode {
    FRAME_NONE = 0,
    FRAME_720_60 = 1,
    FRAME_480_30 = 2,
    FRAME_480_120 = 3
};

struct _LightBar{
    int minY;
    int maxY;
    Vec2d equation;
    bool paired=false;
};
typedef struct _LightBar LightBar;

class ArmorDetect{
private:
    ArmorRegistery ar;

#ifndef COMM_PORT_NOT_CONNECTED
    CommPort comm;
#endif
    Point center=Point(640/2,480/2);
public:
    /**
     *  To threshold the image into binary image
     *
     *  @param src
     *          the original image for thresholding
     *  @param dst
     *          the frame to deliever the result
     *  @param range
     *          the range for thresholding hsv
     *
     */
    void thresholding(Mat&src,Mat&dst,HSVRange range={75,130,0,255,150,255}){
        int iLowH = range.LowH;
        int iHighH = range.HighH;
        
        int iLowS = range.LowS;   //smaller S lighter it can go
        int iHighS = range.HighS;
        //int iCompS = 30;

        int iLowV = range.LowV;    //smaller V blacker it can go
        int iHighV = range.HighV;
       // int iCompV = 240;

        Mat imgHSV;
        vector<Mat> hsvSplit;
        Mat imgGray;
        cvtColor(src, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
        cvtColor(src, imgGray, COLOR_BGR2GRAY);
        //因为我们读取的是彩色图，直方图均衡化需要在HSV空间做
        split(imgHSV, hsvSplit);
        //equalizeHist(hsvSplit[2],hsvSplit[2]);
        merge(hsvSplit,imgHSV);
        Mat imgThresholded,imgThresholded2,imgThresholded3;
        
        inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
        //inRange(imgGray, 80, 255, imgThresholded2);
        //inRange(imgHSV, Scalar(iLowH, iLowS, iCompV), Scalar(iHighH, iHighS, iHighV), imgThresholded3);
        //imgThresholded=imgThresholded2;  //comment this like to switch from gray scale to hsv
         //Threshold the image
        Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(imgThresholded, imgThresholded, MORPH_OPEN, element);
        
        //闭操作 (连接一些连通域)
        morphologyEx(imgThresholded, imgThresholded, MORPH_CLOSE, element);
        dst=imgThresholded;
    }

   
void thresholdingRed(Mat&src,Mat&dst,HSVRange range = {80,100,0,255,200,255})
	{
		AD_Util util;
		Mat src_inv = ~src; // Invert the image

		GaussianBlur(src_inv, src_inv, Size(9, 9), 3, 3);
		dst = util.threshMask(src_inv, range, COLOR_BGR2HSV);
	}

    /**
     *  To draw the detected lightbars on the frame, and register the valid lightbars.
     *
     *  @param contours
     *          the grouped points selected for fitting the lines.
     *  @param dst
     *          the frame to draw the lines.
     *  @param allLightBars
     *          output vector to put the detected lightbars in
     *          equation form
     *
     */
    void drawFit(vector<vector<Point>> contours,Mat&dst, vector<LightBar>&allLightBars){
        for(int i=0;i<contours.size();i++){
            if(contours[i].size()<30){
                continue;
            }
            Vec4d rst;
            fitLine(contours[i], rst, CV_DIST_L2, 0, 0.01,0.01);
            long int c=rst[3]-(rst[1]/rst[0])*rst[2];
            int y1=dst.rows;
            int y2=0;

            //find biggest Y
            for(int j=0;j<contours[i].size();j++){
                if(contours[i][j].y>y2){
                    y2=contours[i][j].y;
                }
            }
            //find smallest Y
            for(int j=0;j<contours[i].size();j++){
                if(contours[i][j].y<y1){
                    y1=contours[i][j].y;
                }
            }

            int x1=(y1-c)/(rst[1]/rst[0]);
            int x2=(y2-c)/(rst[1]/rst[0]);
            if(x1<0||x2<0||x1>dst.cols||x2>dst.cols){
                cout<<"fucked."<<endl;
                break;
            }
            if(abs(rst[1]/rst[0])>1){  //if the slope is big enough
#ifdef DEBUG
                line(dst,Point(x1,y1),Point(x2,y2),Scalar(255,0,255),1);
#endif
                LightBar newLightBar;
                newLightBar.minY=y1;
                newLightBar.maxY=y2;
                newLightBar.equation=Vec2d(rst[1]/rst[0],c);
                allLightBars.push_back(newLightBar);
            }
        }
    }
    /**
     *  group connected points and output them to csv files, with different
     *  groups seperated by sep, sep
     *
     *  @param contours
     *          the grouped points selected for output
     *  @param fname
     *          the csv file to output to
     *
     *
     */
    void pointsToCSV(vector<vector<Point>> contours,string fname){
        ofstream out(fname);
        for(int i=0;i<contours.size();i++){
            if(contours[i].size()<40){
                continue;
            }
            for(int j=0;j<contours[i].size();j++){
                out<<contours[i][j].x<<", "<<contours[i][j].y<<endl;
            }
            out<<"sep, sep"<<endl;
        }
        out.close();
    }

    bool isInRange(Point center, Point tgt, int range){
        return (tgt.x>=center.x-range) && (tgt.x<=center.x+range) &&
            (tgt.y>=center.y-range) && (tgt.y<=center.y+range);
    }
    /**
     *  group connected points and output them to csv files, with different
     *  groups seperated by sep, sep
     *
     *  @param oframe
     *          the frame selected for output
     *  @param fname
     *          the csv file to output to
     *
     *
     */
    void pointsToCSV(Mat oframe, string fname){
        ofstream out(fname);
        Mat frame;
        oframe.copyTo(frame);
        vector<vector<Point>> contours;
        vector<Point> collection;
        for(int i=0;i<frame.cols;i++){
            for(int j=0;j<frame.rows;j++){
                if(frame.at<uchar>(i,j)!=0){
                    collection.push_back(Point(i,j));
                }
            }
        }
        while(!collection.empty()){
            stack<Point> s;
            s.push(collection.back());
            collection.pop_back();
            vector<Point> v;
            while(!s.empty()){
                Point p=s.top();
                s.pop();
                v.push_back(p);
                for(int i=0;i<collection.size();i++){
                    if(isInRange(p,collection[i],20)){
                        //v.push_back(collection[i]);
                        s.push(Point(collection[i].x,collection[i].y));
                        collection.erase(collection.begin()+i);

                    }
                }
            }
            contours.push_back(v);
        }

        for(int i=0;i<contours.size();i++){
            if(contours[i].size()<300){
                continue;
            }
            for(int j=0;j<contours[i].size();j++){
                out<<contours[i][j].x<<", "<<contours[i][j].y<<endl;
            }
            out<<"sep, sep"<<endl;
        }
        out.close();
    }
    /**
     *  This function is to pair similar lightbars for analyze.
     *  @param allLightBars
     *          all light bars that is detected without pairing
     *  @return
     *          return the paired lightbars in vector of vectors, which the inner
     *          vector represents the recognized pair, which conatains 2 lightbars.
     *
     */
     vector<vector<LightBar>> pairing(vector<LightBar> allLightBars){
         vector<vector<LightBar>> rtn;
         int verticalOffset=50;
         float slopeOffset=5000;
         //float constantOffset=50000;
         int differenceOffset=50;

        for(int i=0;i<allLightBars.size();i++){
            if(allLightBars[i].paired){
                //continue;
            }
            for(int j=i+1;j<allLightBars.size();j++){
                if(allLightBars[j].paired){
                     //continue;
                }
                int score=0;
                int topDifference=allLightBars[i].minY - allLightBars[j].minY;
                int bottomDifference=allLightBars[i].maxY - allLightBars[j].maxY;
                if(abs(allLightBars[i].minY - allLightBars[j].minY)<verticalOffset){
                    score++;
                }
                if(abs(allLightBars[i].maxY - allLightBars[j].maxY)<verticalOffset){
                    score++;
                }
                if(abs(abs(allLightBars[i].equation[0]) - abs(allLightBars[j].equation[0]))<slopeOffset){
                    score++;
                }
                if(abs(topDifference+bottomDifference)<differenceOffset){
                    score++;
                }
                /*
                if(abs(allLightBars[i].equation[1] - allLightBars[j].equation[1])<constantOffset){
                    score++;
                }*/
                if(score>2){
                    vector<LightBar> v;
                    LightBar lb1;
                    LightBar lb2;
                    lb1.minY=allLightBars[i].minY;
                    lb1.maxY=allLightBars[i].maxY;
                    lb1.equation=allLightBars[i].equation;
                    lb2.minY=allLightBars[j].minY;
                    lb2.maxY=allLightBars[j].maxY;
                    lb2.equation=allLightBars[j].equation;
                    //copy all data
                    allLightBars[i].paired=true;
                    allLightBars[j].paired=true;
                    //delete original
                    v.push_back(lb1);
                    v.push_back(lb2);
                    rtn.push_back(v);
                    break;
                }
            }
        }
        return rtn;
        
     }
    
    
    void drawRegisteredArmor(vector<ArmorUnit>armors,Mat&dst){
        for(int i=0;i<armors.size();i++){
            if(armors[i].status==VALID){
                line(dst,armors[i].p1,armors[i].p2,Scalar(0,255,0),2);
                line(dst,Point(armors[i].p1.x,armors[i].p2.y),Point(armors[i].p2.x,armors[i].p1.y),Scalar(0,255,0),2);
            }
            if(armors[i].status==LOCKED_ON){
                line(dst,armors[i].p1,armors[i].p2,Scalar(255,0,255),2);
                line(dst,Point(armors[i].p1.x,armors[i].p2.y),Point(armors[i].p2.x,armors[i].p1.y),Scalar(255,0,255),2);
            }
            if(armors[i].status==URGENT){
                line(dst,armors[i].p1,armors[i].p2,Scalar(0,0,255),2);
                line(dst,Point(armors[i].p1.x,armors[i].p2.y),Point(armors[i].p2.x,armors[i].p1.y),Scalar(0,0,255),2);
            }
                                           
        }
        
    }
    
    
    
    /**
     *  To draw the detected lightbars in pairs on the frame.
     *
     *  @param pairs
     *          the paird lightbars to draw
     *  @param dst
     *          the frame to draw the rectangles.
     *
     *
     */
    
    void drawPair(vector<vector<LightBar>> pairs, Mat&dst){
        for(int i=0;i<pairs.size();i++){
            int x1 = (pairs[i][0].minY - pairs[i][0].equation[1])/pairs[i][0].equation[0];
            int x2 = (pairs[i][1].maxY - pairs[i][1].equation[1])/pairs[i][1].equation[0];
            int difference = pairs[i][1].maxY - pairs[i][0].minY;
            
            Point p1(x1,pairs[i][0].minY-.6*difference);
            Point p2(x2,pairs[i][1].maxY+.6*difference);
            //cut the frame to the confirm algorithm for verification
            rectangle(dst, p1, p2, Scalar(0,255,255),1);
            //-----------------register armor-----------------------
            ArmorUnit newArmor;
            newArmor.p1=p1;
            newArmor.p2=p2;
            ar.register_armor(newArmor);
            
            //------------------register armor end---------------------
            //Mat cut = dst(Rect(p1,p2));
            /*if(contains_circle(cut)){
                //the cut is confirmed
                rectangle(dst, p1, p2, Scalar(0,255,0),2);
                //ar.register_armor(newArmor);
            }*/
        }
        ar.getPriorityTarget();
        vector<ArmorUnit> armors=ar.get_registered_armor();

        drawRegisteredArmor(armors, dst);

#ifndef COMM_PORT_NOT_CONNECTED
        sendDifference(armors);
#endif

    }

    void sendDifference(vector<ArmorUnit> items){
#ifndef COMM_PORT_NOT_CONNECTED
        for(int i=0;i<items.size();i++){
            if(items[i].status== LOCKED_ON){
                Point p((items[i].p1.x+items[i].p2.x)/2,(items[i].p1.y+items[i].p2.y)/2);
                Point pd(p.x-center.x,-(p.y-center.y));
                comm.sendDifference(pd);
                cout<<"UART sent: "<<pd.x<<" "<<pd.y<<endl;
                return;
            }
        }
        comm.sendDifference(Point(0,0));
#endif
    }
    
    
    /**
     *  Constructor that opens the camera and start the detection process.
     *
     */
    ArmorDetect(){
        //VideoCapture cap("1749.mp4");
        //VideoCapture cap("IMG_6586.mp4");
        /*VideoCapture cap(CAMERA);
        cap.set(CV_CAP_PROP_EXPOSURE, -4);
        //cap.set(CAP_PROP_BRIGHTNESS, 0);
        cap.set(CAP_PROP_FRAME_WIDTH , 640);
         cap.set(CAP_PROP_FRAME_HEIGHT , 480);
        //cap.set(CV_CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G') );
        cap.set(CV_CAP_PROP_FPS , 120);
        //cap.set(CAP_PROP_TEMPERATURE  , -100)*/;
        RMVideoCapture cap("/dev/video0", 3);


            cap.setVideoFormat(640, 480, 1);
            cap.setExposureTime(0, 60);//settings->exposure_time);
            cap.setVideoFPS(60);
            cap.startStream();
            cap.info();
        while(1){
            Mat image;
            cap>>image;
            if(image.rows<10||image.cols<10){
                waitKey(27);
                continue;
            }
            auto start = std::chrono::high_resolution_clock::now();
            
            vector<LightBar> allLightBars; //to record light bars recognized
            resize(image, image, Size(640,480));
            Mat contoursImg;
            Mat contoursImg2;
            Mat bluredImg;
            bluredImg=image;//TODO: preprocessing bluring absent
            //convert the image into gray scale

            Mat binaryImg;
            thresholding(bluredImg,binaryImg);
            vector<vector<Point>> contours;
            //find contours of the threshold image
            findContours(binaryImg, contours, RETR_LIST, CHAIN_APPROX_NONE);
            drawFit(contours,bluredImg,allLightBars); //draw the fit line onto the frame (HSV algorithm);
            cout<<allLightBars.size()<<" Light-bars recognized."<<endl;
            vector<vector<LightBar>> pairs;
            pairs=pairing(allLightBars); //this is the pairs that came out of the result
            cout<<"There are "<<pairs.size()<<" pairs of lightbars."<<endl;
            //draw pairs, and register identified armor
            drawPair(pairs, bluredImg);
            ar.update_timestamp();
#ifdef DEBUG
            namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
            imshow( "Display window", bluredImg );
            
            namedWindow( "Display window binary", WINDOW_AUTOSIZE );// Create a window for display.
            imshow( "Display window binary", binaryImg );

            
            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time: " << elapsed.count() << " s\n";
#endif
            char c = waitKey(6);                                          // Wait for a keystroke in the window
            if(c == 27)
            	break;
        }
        
        cout<<"RoboGrinders Wins"<<endl;
        
    }
};


