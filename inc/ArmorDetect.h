/*******************************************************
 * File name: ArmorDectect.h
 *
 * Description:
 *
 *
 * Created by:  Jason Lu
 * Edited by:   Frank Eyenga
 *
 * Created:   yyyy-mm-dd
 * Last Edit: 2018-05-18
 */

#ifndef _ARMORDETECT_H
#define _ARMORDETECT_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// standard library
#include <vector>

// OpenCV library
#include "opencv2/core.hpp"

// Local library/files
#include "AD_Util.h"
#include "CommPort.h"
#include "ConfirmationAlgo.h"

/* ************************************************************************** */
/* Section: Types                                                        */
/* ************************************************************************** */
enum Video_Mode {
  FRAME_NONE = 0,
  FRAME_720_60,
  FRAME_480_30,
  FRAME_480_120,
};

struct LightBar {
  int minY;
  int maxY;
  cv::Vec2d equation;
  bool paired = false;
};

/* ************************************************************************** */
/* Section: Class Declaration                                                 */
/* ************************************************************************** */
class ArmorDetect {
public:
  /**
   *  Constructor that opens the camera and start the detection process.
   *
   */
  ArmorDetect();

  /**
   *  Destructor
   */
  ~ArmorDetect();

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
  void thresholding(cv::Mat&src, cv::Mat&dst, HSVRange range = {cv::Scalar(75, 130, 0), cv::Scalar(255, 150, 255)});

  void thresholdingRed(cv::Mat&src, cv::Mat&dst, HSVRange range = {cv::Scalar(80, 100, 0), cv::Scalar(255, 200, 255)});

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
  void drawFit(std::vector<std::vector<cv::Point>> contours, cv::Mat&dst, std::vector<LightBar>&allLightBars);

  /**
   *  group connected points and output them to csv files, with different
   *  groups seperated by sep, sep
   *
   *  @param contours
   *          the grouped points selected for output
   *  @param fname
   *          the csv file to output to
   *
   */
  void pointsToCSV(std::vector<std::vector<cv::Point>> contours, std::string fname);

  /**
   *
   * @param center
   * @param tgt
   * @param range
   * @return
   */
  bool isInRange(cv::Point center, cv::Point tgt, int range);

  /**
   *  group connected points and output them to csv files, with different
   *  groups seperated by sep, sep
   *
   *  @param oframe
   *          the frame selected for output
   *  @param fname
   *          the csv file to output to
   *
   */
  void pointsToCSV(cv::Mat oframe, std::string fname);

  /**
   *  This function is to pair similar lightbars for analyze.
   *
   *  @param allLightBars
   *          all light bars that is detected without pairing
   *  @return
   *          The paired lightbars in vector of vectors, which the inner
   *          vector represents the recognized pair, which conatains 2 lightbars.
   *
   */
  std::vector<std::vector<LightBar>> pairing(std::vector<LightBar> allLightBars);

  /**
   *
   * @param armors
   * @param dst
   */
  void drawRegisteredArmor(std::vector<ArmorUnit>armors, cv::Mat&dst);

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

  void drawPair(std::vector<std::vector<LightBar>> pairs, cv::Mat&dst);

  /**
   *
   * @param items
   */
  void sendDifference(std::vector<ArmorUnit> items);

  /**
   */
  void launch();

private:
  ArmorRegistry ar;
  CommPort comm;

}; // ArmorDetect

#endif // _ARMORDECTECT_H