/*******************************************************
 * File name: ArmorDetectUtil.h
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

#ifndef _ARMORDETECTUTIL_H
#define _ARMORDETECTUTIL_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// Standard library
#include <vector>
#include <utility>

// OpenCV library
#include "opencv2/opencv.hpp"

/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
// Default values for Blue and Red HSV range
#define BLUE_HUE_LOW  75
#define BLUE_HUE_HIGH 130
#define CYAN_HUE_LOW  75
#define CYAN_HUE_HIGH 110

#define SAT_LOW   200
#define SAT_HIGH  255
#define VAL_LOW   200
#define VAL_HIGH  255
#define SAT_MIN   85
#define VAL_MIN   50

#define PIXELS_THRES  0.65
#define FRAMES_THRES  0.70

#define ROI_CONSTANT  3
#define ROI(x)        ((x) / ROI_CONSTANT)
#define ROI_RANGE(x)  Range(ROI(x), (x) - ROI(x))

#define U_FLOOR(x)    ((x) < 0 ? 0 : (x) )
#define U8_CEILING(x) ((x) > 255 ? 255 : (x) )

#define TUNER_MEDIUM(x) ((x) * 0.10)
#define TUNER_COARSE(x) ((x) * 0.15)

/* ************************************************************************** */
/* Section: Types                                                             */
/* ************************************************************************** */
struct AimPoint {
  float x;
  float y;
  float offset_x;
  float offset_y;
};

struct WindowRect {
  int x;
  int y;
  int width;
  int height;
};

struct HSVRange {
  cv::Scalar min;
  cv::Scalar max;
};

enum DirectionHint {
  MOVE_UP = 1, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
};

enum Color {
  BlUE = 0, RED
};

/* ************************************************************************** */
/* Section: Class Declaration                                                 */

/* ************************************************************************** */
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
  bool display_Img(cv::Mat frame);

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
  bool draw_Aim(AimPoint aim, cv::Mat& frame);

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
  bool draw_Arrow(DirectionHint hint, cv::Mat& frame);

  /**
   * Finds the correct range of values to idenify a certain color (Blue or Red).
   * @param frame
   *       A list of Mat images formated in the RGB/BGR color space
   * @param range
   *      A reference to an HSVRange variable. This variable will be set by the function.
   * @param clr
          The color to test for
   * @return
   *      true if the calibration was successful; {@code range} will be set
          to the values that successfully detected most of the pixels.
   *      False otherwise; the {@code range} will be unchanged.
   */
  bool calibrateRange(const std::vector<cv::Mat>& frames, HSVRange& range, Color clr);

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
  float find_Exposure_Adjustment(std::vector<cv::Mat> frames);

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
  float find_Temperature_Adjustment(std::vector<cv::Mat> frames);

  /**
   * This will find HSV range needed to identify RED or BLUE objects.
   * Used for calibration purposes.
   *
   * A plain red or blue object must be place in the center-front view of the
   * camera until it sees enough of the points in the frames to pass the
   * threshhold.
   *
   * May require an understanding of PID algorithm
   *
   * @param frames
   *      A vector of Mat images used for testing (in HSV)
   * @param clr
   *      The color to test for
   * @return
   *      The HSV range needed to detect color. If a valid range could not be
   *      found, return an negative range.
   */
  HSVRange findHSVRange(std::vector<cv::Mat>& frames, const Color& clr);

  /**
   * Converts images from RGB/BGR to HSV color format, applies a custom threshold, and
   * returns the resulting binary images.
   *
   * In other words, each image will be converted and it's pixels will be checked
   * against the given range. Every pixel who's HSV values are within that range
   * will be set(1), all others will be cleared (0).
   *
   * @param frame
   *       A Mat images formated in the RGB/BGR color space
   * @param
   *      The range of HSV value that will be used to threshold the images
   * @return
   *      A vector of binary Mats that resulted from the threshold operation
   */
  cv::Mat threshMask(cv::Mat& frame, HSVRange& range, unsigned int code = cv::COLOR_COLORCVT_MAX + 1);

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
  std::vector<cv::Mat> bgrToHSV(std::vector<cv::Mat>& frames, bool blur);

  /**
   *
   * @param frame
   * @param range
   * @return
   */
  bool testFrame(const cv::Mat frame, const HSVRange& range);
};

#endif //_ARMORDETECTUTIL_H
