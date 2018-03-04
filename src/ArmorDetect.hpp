#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stack>
//#include <unistd.h>
#include "AD_Util.h"
#define DEBUG_MODE 1


/** @function main */
//---------------------define constant-----------------------

#define CONTOURS_POINT_COUNT_THRESHOLD 150


//---------------------define constant-----------------------
//#include "AD_Util.h"
using namespace std;
using namespace cv;

struct _LightBar{
    int minY;
    int maxY;
    Vec2f equation;
    bool paired=false;
};
typedef struct _LightBar LightBar;

class ArmorDetect{
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
    void thresholding(Mat&src,Mat&dst,HSVRange range={75,130,0,255,240,255}){
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
        equalizeHist(hsvSplit[2],hsvSplit[2]);
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

   
    /**
     *  To draw the detected lightbars on the frame.
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
            if(contours[i].size()<40){
                continue;
            }
            Vec4f rst;
            fitLine(contours[i], rst, CV_DIST_L2, 0, 0.01,0.01);
            int c=rst[3]-(rst[1]/rst[0])*rst[2];
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
            if(abs(rst[1]/rst[0])>8){  //if the slope is big enough
#ifdef DEBUG_MODE
                line(dst,Point(x1,y1),Point(x2,y2),Scalar(255,0,255),1);
#endif
                LightBar newLightBar;
                newLightBar.minY=y1;
                newLightBar.maxY=y2;
                newLightBar.equation=Vec2f(rst[1]/rst[0],c);
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
     *  @param allLightBars
     *          all light bars that is detected without pairing
     *  @return
     *          return the paired lightbars in vector of vectors, which the inner
     *          vector represents the recognized pair, which conatains 2 lightbars.
     *
     */
     vector<vector<LightBar>> pairing(vector<LightBar> allLightBars){
         vector<vector<LightBar>> rtn;
         int verticalOffset=20;
         float slopeOffset=100;
         //float constantOffset=50000;
         int differenceOffset=10;

        for(int i=0;i<allLightBars.size();i++){
            if(allLightBars[i].paired){
                continue;
            }
            for(int j=i+1;j<allLightBars.size();j++){
                if(allLightBars[j].paired){
                     continue;
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
            Point p1(x1,pairs[i][0].minY);
            Point p2(x2,pairs[i][1].maxY);
            rectangle(dst, p1, p2, Scalar(0,255,255),2);
        }
    }
    
    
    /**
     *  Constructor that opens the camera and start the detection process.
     *
     */
    ArmorDetect(){
        VideoCapture cap(0);
        cap.set(CAP_PROP_EXPOSURE, -13);
        //cap.set(CAP_PROP_BRIGHTNESS, 0);
        cap.set(CAP_PROP_FRAME_WIDTH , 640);
         cap.set(CAP_PROP_FRAME_HEIGHT , 480);
        cap.set(CV_CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G') );
        //cap.set(CAP_PROP_HUE , 120);
        //cap.set(CAP_PROP_TEMPERATURE  , -100);
        while(1){
            Mat image;
            cap>>image;
            vector<LightBar> allLightBars; //to record light bars recognized
            //resize(image, image, Size(640,480));
            Mat contoursImg;
            Mat contoursImg2;
            Mat bluredImg;
            bluredImg=image;//TODO: preprocessing bluring absent
            //convert the image into gray scale
            cvtColor(bluredImg,contoursImg,CV_BGR2GRAY);
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
            //draw pairs
            drawPair(pairs, bluredImg);
            
            
            //draw AimPoint
            AD_Util myAD_Util;
            AimPoint aim;
            aim.x=image.rows/2+70;
            aim.y=image.cols/2-90;
            bool JasonSB;
            JasonSB=myAD_Util.draw_Aim(aim, image);
            while (!JasonSB){}

            
#ifdef DEBUG_MODE
            namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
            imshow( "Display window", bluredImg );
            
            namedWindow( "Display window binary", WINDOW_AUTOSIZE );// Create a window for display.
            imshow( "Display window binary", binaryImg );
#endif
            
            
            waitKey(15);                                          // Wait for a keystroke in the window
        }
        //cout<<"RoboGrinders Wins"<<endl;
        
    }
};


