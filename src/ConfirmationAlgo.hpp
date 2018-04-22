//
//  ConfirmationAlgo.hpp
//  ArmorDetect
//
//  Created by Jason Lu on 3/18/18.
//

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <stack>
#include <chrono>
#include <queue>
#include <vector>
#ifndef ConfirmationAlgo_h
#define ConfirmationAlgo_h

using namespace std;
using namespace cv;

/**
 *  To get the distance from point 0 to point A
 *  @param pointO
 *      starting point
 *  @param pointA
 *      ending point
 *  @return
 *      return the distance between two point
 */
double getDistance (CvPoint pointO,CvPoint pointA )
{
    double distance;
    distance = powf((pointO.x - pointA.x),2) + powf((pointO.y - pointA.y),2);
    distance = sqrtf(distance);
    
    return distance;
}


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
void thresholding(Mat&src,Mat&dst,HSVRange range={0,255,0,100,150,255}){
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
 *  If the image sent contains a circle in the image
 *
 *  @param src
 *      the source image
 *  @return
 *      if the image contains a circle returns true, otherwise false;
 *
 */
bool contains_circle(Mat&src){
    if(src.rows<10||src.cols<10){
        return false;
    }
    thresholding(src, src);
   
    vector<vector<Point> > contours;
    findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);
    for(int i=0;i<contours.size();i++){
        if(contours[i].size()<20){
            continue; //skiping the small counts
        }
        Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
        if(box.size.width*box.size.height < src.cols*src.rows/3){
            continue;
            
        }
        if(box.size.width/box.size.height < 0.8 || box.size.width/box.size.height > 1.2){
            continue;
        }
        return true;
    }
    return false;
}
/*
 to record the lockon status for the lockon functionality.
 */
enum LockonStatus{INVALID, VALID, LOCKED_ON, URGENT};

//------------------definition of armor unit--------------------
struct _ArmorUnit{
    Point p1;
    Point p2;
    int confirmation=0; //to store how many times it has been confirmed.
    bool varified=false; //if it contains 66 percent of more confirmation, it is true.
    LockonStatus status = INVALID;
    uint64_t timestamp=0;
    int num = 0;
    _ArmorUnit(){}
   _ArmorUnit(Point p1,Point p2,int confirmation, bool varified, LockonStatus status, uint64_t timestamp):p1(p1),p2(p2),confirmation(confirmation),varified(varified),status(status),timestamp(timestamp) {}


};
struct cmp{
    bool operator()(_ArmorUnit A, _ArmorUnit B)
    {
        if (A.status == B.status)
            return (abs(A.p2.x - A.p1.x)*(A.p2.y - A.p1.y) < abs(B.p2.x - B.p1.x)*(B.p2.y - B.p1.y));
        else if (A.status == INVALID && B.status == VALID)
            return true;
        //else if (A.status != LOCKED_ON && B.status == LOCKED_ON)
            //return true;
        else return false;
    }

};
typedef struct _ArmorUnit ArmorUnit;

//-------------------definition of Armor Registery--------------------
class ArmorRegistery{
private:
    vector<ArmorUnit> armorUnitList;
    int number_of_registration = 0;
    const int confirmation_threshold=3;
    uint64_t frameCount=0;
    /*
     * procedure to clean the invalid identified armor.
     * in this case, armor without update will be removed.
     */
    void clean_invalid_entry(){
        for(int i=0; i < armorUnitList.size();i++){
            //armorUnitList[i].confirmation-=confirmation_threshold*1.2;
            if(abs(frameCount-armorUnitList[i].timestamp)>5){
                armorUnitList[i].varified=false;
            }
            if(!armorUnitList[i].varified){
                armorUnitList.erase(armorUnitList.begin()+i);
            }
        }
    }
    /**
     *  To judge if a passed armorunit is a valid one to register
     *
     *  @param item
     *          the armor unit passed for judgement
     *  @return
     *          if it is valid.
     *
     */
    bool is_valid_to_register(ArmorUnit item){
        
        if(item.p1.x<0||item.p1.y<0||item.p2.x<0||item.p2.y<0){
            cout<<"Armor not registered for invalid position"<<endl;
            return false;
        }
        if(item.confirmation!=0){
            cout<<"Armor not registered for abundent info"<<endl;
            return false;
        }
        if(item.varified){
            cout<<"Armor not registered for skipping the registery process."<<endl;
            return false;
        }
        if(item.status!=INVALID){
            cout<<"Armor not registered for skipping the registery process."<<endl;
            return false;
        }
        return true;
    }
    /**
     *  To match a passed armor unit to the current registery
     *
     *  @param item
     *          the armor unit for matching
     *  @return
     *          the index of the current registery that the armor unit matched to
     */
    int match_to_registery(ArmorUnit item){
        const int max_score = 125;
        int *scores = new int[armorUnitList.size()];
        for(int i=0;i<armorUnitList.size();i++){
            int difference1 = getDistance(item.p1,armorUnitList[i].p1);
            int difference2 = getDistance(item.p2,armorUnitList[i].p2);
            scores[i]=max_score-difference1+max_score-difference2;
        }
        int best_fit_index=-1;
        int best_fit_score=INT_MIN;
        for(int i=0;i<armorUnitList.size();i++){
            if(scores[i]>0&&scores[i]>best_fit_score){
                best_fit_index=i;
                best_fit_score=scores[i];
            }
        }
        delete[]scores;
        return best_fit_index;
    }
public:
    ArmorRegistery(){
        
    }
    ~ArmorRegistery(){
        
    }
    /**
     * True is returned if the armor is registered.
     *  @param item
     *          the armor unit to register
     */
    bool register_armor(ArmorUnit item){
        if(!is_valid_to_register(item)){
            cout<<"Armor registration declined."<<endl;
            return false;
        }
        number_of_registration++;
        if(number_of_registration>3*confirmation_threshold){
            number_of_registration=0;
            //frameCount=0;
            clean_invalid_entry();
        }
        if(armorUnitList.size()==0){
            armorUnitList.push_back(item);
            return true;
        }
        int match_result = match_to_registery(item);
        if(match_result == -1){         //coult not find match, treat as newly discovered armor
            armorUnitList.push_back(item);
            return true;
        }
        //if match is complete, update the registered info
        armorUnitList[match_result].p1=item.p1;
        armorUnitList[match_result].p2=item.p2;
        armorUnitList[match_result].confirmation++;
        armorUnitList[match_result].timestamp=frameCount;
        if(armorUnitList[match_result].confirmation<0){
            armorUnitList[match_result].confirmation=confirmation_threshold; //overflow dealing
        }
        if(armorUnitList[match_result].confirmation>confirmation_threshold){
            armorUnitList[match_result].varified=true;
            if(armorUnitList[match_result].status==INVALID){
                armorUnitList[match_result].status=VALID;
            }
        }
        return true;
    }
    /**
     *  @return
     *      return the current armor unit registery
     */
    vector<ArmorUnit> get_registered_armor(){
        
        
        return armorUnitList;
    }
    
    void update_timestamp(){
        frameCount++;
        if(frameCount%8==0){
            //frameCount=0;
            clean_invalid_entry();
        }
    }
    void getPriorityTarget()
    {
        if (armorUnitList.empty()) {

        }
        else
        {
            int i = 1;
            bool valid = false;
            for (auto & it : armorUnitList)
            {
                if (it.status != INVALID)
                {
                    valid = true;
                }
                it.num = i;
                i++;
            }
            if (valid == true)
            {
                priority_queue<_ArmorUnit, std::vector<_ArmorUnit>, cmp> pq;
                for (auto & it : armorUnitList)
                {
                    pq.push(it);
                }
                for (auto & it : armorUnitList)
                {
                    if (pq.top().num == it.num)
                    {
                        it.status = LOCKED_ON;
                        break;
                    }
                }
            }
        }


    }

};


#endif /* ConfirmationAlgo_h */
