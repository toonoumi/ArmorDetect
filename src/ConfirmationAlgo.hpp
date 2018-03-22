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
#ifndef ConfirmationAlgo_h
#define ConfirmationAlgo_h

using namespace std;
using namespace cv;

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
void thresholding(Mat&src,Mat&dst,HSVRange range={0,255,0,50,240,255}){
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

bool contains_circle(Mat&src){
    thresholding(src, src);
    vector<vector<Point>> contours;
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

#endif /* ConfirmationAlgo_h */
