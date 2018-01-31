//
//  AD_Util.h
//  ArmorDetect
//														Dates
//  Created by: Frank Eyenga 	12/17/2017
//


#include<iostream>
#include <cctype>
#include <cstdlib>
#include "opencv2/videoio.hpp"
#include "test_suite.h"



// The std namespace is HUGE. Doing it this way reduces compile time
using std::vector;
using std::cout;
using std::array;
using std::pair;

void test_FindHSVRange(char* images[], int sz, bool color)
{
  vector<Mat> images_bgr;

  for (unsigned int i = 0; i < sz; i++)
  {
      // Read in image
      Mat img, img_hsv;
      img = imread(images[i], IMREAD_COLOR);

      if (!img.data)
      {
          cout << "Could not find or open \"";
          cout << images[i] << "\"\n";
          std::exit(EXIT_FAILURE);
      }

      images_bgr.push_back(img);
  }

  // Calibrate HSV threshhold
  pair<HSVRange, bool> hsv_range;
  if (color)
      hsv_range = AD_Util().find_Blue_HSVRange(images_bgr);
  else
      hsv_range = AD_Util().find_Red_HSVRange(images_bgr);

  if (!hsv_range.second) cout << "Calibration Failed\n";

  // Use range to remove non-desired pixels and display results
  HSVRange range = hsv_range.first;
  for (const auto& image : images_bgr)
  {
      Mat image_hsv, mask, output;

      cvtColor(image, image_hsv, COLOR_BGR2HSV);
      Scalar lowbnd(range.LowH, range.LowS, range.LowV);
      Scalar uppbnd(range.HighH, range.HighS, range.HighV);
      inRange(image_hsv, lowbnd, uppbnd, mask);
      bitwise_and(image, image, output, mask);

      namedWindow("Original", WINDOW_NORMAL);
      imshow("Original", image);
      namedWindow("Result", WINDOW_NORMAL);
      imshow("Result", output);

      for (char c = waitKey(0); c != 32; c = waitKey(0))
          if (c == 27) return;
  }
}

void test_FindHSVRange(int id, bool color)
{
  VideoCapture cam;

  if (!cam.open(id))
  {
      cout << "Could not open camera\n";
      std::exit(EXIT_FAILURE);
  }

  HSVRange range{.LowH = 110, .HighH = 130,
      .LowS = 100, .HighS = 255,
      .LowV = 100, .HighV = 255};

  cout << "Press \"Esc\" to exit\nPress the spacebar to (re)calibrate\n";

  while (1)
  {
      Mat frame;
      if (!cam.read(frame))
      {
          cout << "Error reading frame\n";
          return;
      }

      // Convert frame and mask pixels
      Mat frame_hsv, mask;
      cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
      Scalar lowbnd(range.LowH, range.LowS, range.LowV);
      Scalar uppbnd(range.HighH, range.HighS, range.HighV);
      inRange(frame_hsv, lowbnd, uppbnd, mask);

      // Place a retangle in the center of the frame.
      // Color of retangle will change if most of the pixels in it are blue
      Range rows = ROI_RANGE(frame.rows);
      Range cols = ROI_RANGE(frame.cols);

      if (testImage(frame_hsv(rows, cols), range, .50))
      {
          rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(0, 255, 0), 4);
      }
      else
      {
          rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(255, 255, 255), 4);
      }

      namedWindow("Camera Feed", WINDOW_NORMAL);
      imshow("Camera Feed", frame);
      namedWindow("Mask", WINDOW_NORMAL);
      imshow("Mask", mask);

      char c = waitKey(5);
      if (c == 27)
      {
          return;
      }
      else if (c == 32)
      {
          pair<HSVRange, bool> hsv_range = calibrate(cam, color);
          if (hsv_range.second)
          {
              cout << "Calibration Succeeded\n";
              range = hsv_range.first;
          }
          else
          {
              cout << "Calibration failed\n";
          }
      }
  }

}

pair<HSVRange, bool> calibrate(VideoCapture& cam, bool color)
{
  cout << "Place a blue or red object in the center of the screen\n";
  cout << "Calibrating...\n";

  vector<Mat> frames;
  for (Mat next_frame; frames.size() < 10;)
  {
      if (!cam.read(next_frame))
      {
          cout << "Error reading frame\n";
          return pair<HSVRange, bool>(HSVRange(), false);
      }
      frames.push_back(next_frame);
  }

  if (color)
      return AD_Util().find_Blue_HSVRange(frames);

  return AD_Util().find_Red_HSVRange(frames);

}

bool testImage(const Mat& roi, const HSVRange& range, float thres)
{
  // find pixels that match range
  Mat mask;
  uint32_t pixelsfound = 0;

  Scalar lower(range.LowH, range.LowS, range.LowV);
  Scalar upper(range.HighH, range.HighS, range.HighV);
  inRange(roi, lower, upper, mask);

  // count the number of pixels found in ROI
  for (unsigned int i = 0; i < mask.rows; i++)
  {
      uint8_t* row = mask.ptr<uint8_t>(i);

      for (unsigned int j = 0; j < mask.cols; j++)
          if (row[j]) pixelsfound++;
  }

  return ( pixelsfound >= (roi.total() * thres)) ? true : false;
}

void test_threshMask(int id)
{
  VideoCapture cam;

  if (!cam.open(id))
  {
      cout << "Could not open camera\n";
      std::exit(EXIT_FAILURE);
  }

  HSVRange range{.LowH = 110, .HighH = 130,
      .LowS = 100, .HighS = 255,
      .LowV = 100, .HighV = 255};

  while (1)
  {
      vector<Mat> frames;
      for (Mat next_frame; frames.size() < 10;)
      {
          if (!cam.read(next_frame))
              cout << "Error reading frame\n";

          frames.push_back(next_frame);
      }

      frames = AD_Util().threshMask(frames, range);

      for (const auto& mask : frames)
      {
          namedWindow("Mask", WINDOW_NORMAL);
          imshow("Mask", mask);

          char c = waitKey(0);
          if (c == 27)
              return;
      }


  }
}
//End of file
