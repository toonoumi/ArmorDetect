//
//  AD_Util.h
//  ArmorDetect
//														Dates
//  Created by: Jason Lu			10/12/17.
//			Edited: Frank Eyenga 	03/13/18.
//


#ifndef AD_Util_h
#define AD_Util_h
#include <vector>
#include <utility>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"


// Default values for Blue and Red HSV range
#define BLUE_HUE_LOW   	75
#define BLUE_HUE_HIGH	130
#define CYAN_HUE_LOW   	0
#define CYAN_HUE_HIGH	8

#define SAT_LOW   		200
#define SAT_HIGH		255
#define VAL_LOW			200
#define VAL_HIGH 		255
#define SAT_MIN   		85
#define VAL_MIN   		50


#define U_FLOOR(x)   ( (x) < 0 ? 0 : (x) )
#define U8_CEILING(x)  ( (x) > 255 ? 255 : (x) )
#define ROI_CONSTANT   3
#define ROI(x)     ((x) / ROI_CONSTANT)
#define ROI_RANGE(x)   Range(ROI(x), (x) - ROI(x))
#define PIXELS_THRES   0.65
#define FRAMES_THRES   0.70
#define TUNER_MEDIUM(x)     ((x) * 0.10)
#define TUNER_COARSE(x)     ((x) * 0.15)


using namespace cv;

struct _AimPoint {
  float x;
  float y;
  float offset_x;
  float offset_y;
};
typedef struct _AimPoint AimPoint;

struct _WindowRect {
  int x;
  int y;
  int width;
  int height;
};

typedef struct _WindowRect WindowRect;

struct _HSVRange {
  int LowH;
  int HighH;
  int LowS;
  int HighS;
  int LowV;
  int HighV;
};
typedef struct _HSVRange HSVRange;

enum DirectionHint {
  MOVE_UP = 1, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
};

enum Color { BlUE = 0, RED };

class AD_Util {
public:
  /**
   * The constructor of AD_Util class
   *
   */
  AD_Util();

  /**
   * Deconstructor of AD_Util class
   */
  ~AD_Util();

  /**
   * This is to display a video or a image. Instead of using imshow
   * we will be using this function. It will organize the windows so that
   * they do not overlap each other when displaying on screen
   * @param frame
   *      The Mat image to display
   * @return
   *      if the image is successfully displayed
   */
  bool display_Img(Mat frame);

  /**
   * This will draw a aim object on a frame, like a first personal shooting game HUD
   * offset is for zeroing purpose.
   * make it cool
   *
   * @param frame
   *      The Mat image to draw the HUD (in BGR)
   * @param aim
   *      The location to draw the aimming HUD
   * @return
   *      if the HUD is drew
   */
  bool draw_Aim(AimPoint aim, Mat&frame);
  /**
   * This will draw arrows to the directions we want to go. (Draw it by the borders of the
   * frame, make it a decent UI)
   * ex: If the Aim is on the left of our target, client code will invoke this function
   * to draw a right arrow.
   * @param frame
   *      The Mat image to draw the hint (in BGR)
   * @param hint
   *      The direction himt we want to draw
   * @return
   *      if the HUD is drew
   */
  bool draw_Arrow(DirectionHint hint, Mat&frame);

  bool calibrateRange(const std::vector<Mat>& frames, HSVRange& range, Color clr);
  /**
   * This will find exposure adjustment that our program needs to make to the camera
   * for it to clearly see objects. Hint: calculate several frames and average the results.
   *
   *
   * scale: -12 to 12
   * @param frames
   *      The Mat images for it to make decision
   * @return
   *      the adjustment it needs to make to current exposure
   */
  float find_Exposure_Adjustment(std::vector<Mat> frames);

  /**
   * This will find temperature adjustment that our program needs to make to the camera
   * for it to clearly see objects and identify colors.
   * Hint: calculate several frames and average the results.
   *
   *
   * scale: -12 to 12
   * @param frames
   *      The Mat images for it to make decision (in BGR)
   * @return
   *      the adjustment it needs to make to current exposure
   */
  float find_Temperature_Adjustment(std::vector<Mat> frames);

  /**
   * This will find correct HSV range for identifying the concept "RED object"
   * for calibration purpose. Place a plain red object in front of the camera,
   * until it sees majority of the points in a frame to pass the threshhold, report the range
   * *May requires understanding of PID algorithm
   * @param frames
   *      The Mat images for it to make decision (in HSV)
   * @return
   *      The correct HSV range to identify red objects
   */
HSVRange findHSVRange(std::vector<Mat>& frames, Color clr);

  /**
   * This will find correct HSV range for identifying the concept "BLUE objects"
   * for calibration purpose. Place a plain blue object in front of the camera,
   * until it sees majority of the points in a frame to pass the threshhold, report the range
   * *May requires understanding of PID algorithm
   * @param frames
   *      The Mat images for it to make decision (in HSV)
   * @return
   *      The correct HSV range to identify blue objects
   */


  /**
   * Converts images from RGB/BGR to HSV color format, applies a custom threshold, and
   * returns the resulting binary images. In other words, each image will be
   * converted and it's pixels will be checked against the given range. Every
   * pixel who's HSV values are within that range will be set(1), all others will be
   * cleared (0). These binary images are referred to as "mask."
   * @param frame
   *       A Mat images formated in the RGB/BGR color space
   * @param
   *      The range of HSV value that will be used to threshold the images
   * @return
   *      A vector of binary Mats that resulted from the threshold operation
   */
Mat threshMask(Mat& frame, HSVRange& range, unsigned int code = COLOR_COLORCVT_MAX + 1); 


private:
  std::vector<WindowRect> windowsDisplayed;

  /**
   * Converts images formated using the BGR(Blue-Green-Red) color space to their
   * HSV equivalent. Note that the typical color format for images is RGB, but
   * openCV automatically converts a RGB image to BGR when it is loaded.
   * @param frames
   *      A vector of Mat images formated in the RGB/BGR color space
   * @param blur
   *      A boolean value. If true then a Gaussian blur will be applied to
   *      each image before converting it, otherwise only the conversion is done.
   * @return
   */
  std::vector<Mat> bgrToHSV(std::vector<Mat>& frames, bool blur);

bool testFrame(const Mat frame, const HSVRange& range);

};

#endif /* AD_Util_h */
