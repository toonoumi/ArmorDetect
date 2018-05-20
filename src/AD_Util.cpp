/*******************************************************
 * File name: ArmorDetectUtil.cpp
 *
 * Description:
 *
 *
 * Created by:  Jason Lu
 * Edited by:   Frank Eyenga
 *
 * Created:   2017-10-12
 * Last Edit: 2018-05-18
 */


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// Standard library
#include <vector>
#include <utility>
#include <iostream>

// OpenCV library
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"

// local library/files
#include "AD_Util.h"

/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
#define DEBUG

/* ************************************************************************** */
/* Section: Types                                                             */
/* ************************************************************************** */
using std::vector;

using namespace cv;

typedef std::vector<Mat> VecMat;

/* ************************************************************************** */
/* Section: Function Definitions                                              */

/* ************************************************************************** */
AD_Util::AD_Util() { } //constructor

AD_Util::~AD_Util() { } //destructor

bool AD_Util::display_Img(Mat frame) { } //display_Img

bool AD_Util::draw_Aim(AimPoint aim, Mat&frame) { } //draw_Aim

bool AD_Util::draw_Arrow(DirectionHint hint, Mat&frame) { } //draw_Arrow

float AD_Util::find_Exposure_Adjustment(VecMat frames) { } //find_Exposure_Adjustment

float AD_Util::find_Temperature_Adjustment(VecMat frames) { } //find_Temperature_Adjustment

bool AD_Util::calibrateRange(const VecMat& frames, HSVRange& range, Color clr)
{
 VecMat regions; // The region of the frame used to calibrate
 HSVRange cand;

 for (const auto frm : frames)
 {
   Mat roi;
   Range r_rows = ROI_RANGE(frm.rows);
   Range r_cols = ROI_RANGE(frm.cols);
   Mat(frm, r_rows, r_cols).copyTo(roi);

   if (clr == RED) // if red, invert before converting
     roi = ~roi;

   cvtColor(roi, roi, COLOR_BGR2HSV);
   regions.push_back(roi);
 }

 cand = findHSVRange(regions, clr);

 if (cand.max(0) >= 0);
 {
   range = cand;
   return true;
 }

 return false;
} //calibrateRange

HSVRange AD_Util::findHSVRange(VecMat& frames, const Color& clr)
{
 HSVRange range;
 size_t list_size = frames.size();
 int passed = 0; // Number of frames that have passed

 // Initialize starting value for range
 range.min = Scalar(BLUE_HUE_LOW, SAT_LOW, VAL_LOW);
 range.max = Scalar(BLUE_HUE_HIGH, SAT_HIGH, VAL_HIGH);

 if (clr == RED)
 {
   range.min(0) = CYAN_HUE_LOW;
   range.max(0) = CYAN_HUE_HIGH;
 }

#ifdef DEBUG
 std::cout << "starting range finder\n\n";
 std::cout << "# of Frames: " << list_size << "\n";
 std::cout << "# to pass: " << list_size * FRAMES_THRES << "\n";
 int pass = 0;
#endif

 while (range.min(1) > SAT_MIN && range.min(2) > VAL_MIN)
 {
   for (int i = 0; i < list_size; i++)
   {
     Mat tmp = frames[i];

     // If successful, increment counter, swap with back and delete
     if (testFrame(tmp, range))
     {
       passed++;
       frames[i] = frames.back();
       frames.back() = tmp;
       frames.pop_back();
     }
   }

   if (passed >= list_size * FRAMES_THRES)
     return range;

#ifdef DEBUG
   std::cout << "--Pass #" << pass++ << "\n";
   std::cout << "--Passing frames: " << passed << "\n";
   std::cout << "--Low sat: " << range.min(1) << "\n";
   std::cout << "--Low val: " << range.min(2) << "\n\n";
#endif
 }

 return HSVRange{ Scalar(-1, -1, -1), Scalar(-1, -1, -1)};

} //findHSVRange

Mat AD_Util::threshMask(Mat& frame, HSVRange& range, unsigned int code)
{
 Mat mask;

 if (code <= COLOR_COLORCVT_MAX)
   cvtColor(frame, frame, code);

 inRange(frame, range.min, range.max, mask);

 return mask;
} //threshMask

VecMat AD_Util::bgrToHSV(VecMat& frames, bool blur)
{
 VecMat frames_hsv;

 for (auto& frame : frames)
 {
   Mat tmp;

   if (blur)
     GaussianBlur(frame, tmp, Size(9, 9), 3, 3);

   cvtColor(tmp, tmp, COLOR_BGR2HSV);
   frames_hsv.push_back(tmp);
 }

 return frames_hsv;
} //bgrToHSV

bool AD_Util::testFrame(const Mat frame, const HSVRange& range)
{
 // find pixels that match range
 Mat mask;
 inRange(frame, range.min, range.max, mask);

 int cols = mask.cols, rows = mask.rows, pixelsfound = 0;

 if (mask.isContinuous())
 {
   cols *= rows;
   rows = 1;
 }

 // count the number of pixels detected
 for (unsigned int i = 0; i < rows; i++)
 {
   uint8_t* row = mask.ptr<uint8_t>(i);

   for (unsigned int j = 0; j < cols; j++)
     if (row[j])
       pixelsfound++;
 }

#ifdef DEBUG
 std::cout << "\tPixels needed: " << mask.total() * PIXELS_THRES << "\n";
 std::cout << "\tPixels found: " << pixelsfound << "\n";

 namedWindow("Mask", WINDOW_NORMAL);
 imshow("Mask", mask);
 namedWindow("Image (HSV)", WINDOW_NORMAL);
 imshow("Image (HSV)", frame);

 waitKey(0);

 destroyWindow("Mask");
 destroyWindow("Image (HSV)");
#endif

 if (pixelsfound >= (mask.total() * PIXELS_THRES))
 {
#ifdef DEBUG
   std::cout << "\tPassed\n";
#endif
   return true;
 }

#ifdef DEBUG
 std::cout << "\tfailed\n";
#endif
 return false;
} //testFrame

/* AD_Util_cpp */
