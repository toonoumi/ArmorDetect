/*******************************************************
 * File name: ConfirmationAlgo.cpp
 *
 * Description:
 *
 *
 * Created by:  Jason Lu
 * Edited by:   Frank Eyenga
 *
 * Created:   2018-03-18
 * Last Edit: 2018-05-18
 */


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// Standard library
#include <iostream>
#include <vector>
#include <queue>

// OpenCV library
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"

// Local library/files
#include "ConfirmationAlgo.h"
#include "ArmorDetect.h"

/* ************************************************************************** */
/* Section: Types                                                             */
/* ************************************************************************** */
using std::cout;
using std::vector;
using std::endl;
using std::priority_queue;

using namespace cv;

/* ************************************************************************** */
/* Section: Function Definitions                                              */
/* ************************************************************************** */
ArmorRegistry::ArmorRegistry()
{
  number_of_registration = 0;
  frameCount = 0;
}

ArmorRegistry::~ArmorRegistry() { } //constructor

bool ArmorRegistry::register_armor(ArmorUnit item)
{
  if (!is_valid_to_register(item))
  {
      cout << "Armor registration declined." << endl;
      return false;
  }

  number_of_registration++;
  if (number_of_registration > 3 * confirmation_threshold)
  {
      number_of_registration = 0;
      //frameCount=0;
      clean_invalid_entry();
  }

  if (armorUnitList.size() == 0)
  {
      armorUnitList.push_back(item);
      return true;
  }

  int match_result = match_to_registry(item);
  if (match_result == -1)
  { //coult not find match, treat as newly discovered armor
      armorUnitList.push_back(item);
      return true;
  }

  //if match is complete, update the registered info
  armorUnitList[match_result].p1 = item.p1;
  armorUnitList[match_result].p2 = item.p2;
  armorUnitList[match_result].confirmation++;
  armorUnitList[match_result].timestamp = frameCount;

  if (armorUnitList[match_result].confirmation < 0)
  {
      //overflow dealing
      armorUnitList[match_result].confirmation = confirmation_threshold;
  }

  if (armorUnitList[match_result].confirmation > confirmation_threshold)
  {
      armorUnitList[match_result].verified = true;
      if (armorUnitList[match_result].status == INVALID)
      {
          armorUnitList[match_result].status = VALID;
      }
  }

  return true;
} //destructor

vector<ArmorUnit> ArmorRegistry::get_registered_armor()
{
  return armorUnitList;
} //get_registered_armor

void ArmorRegistry::update_timestamp()
{
  frameCount++;
  if (frameCount % 8 == 0)
  {
      //frameCount=0;
      clean_invalid_entry();
  }
} //update_timestamp

void ArmorRegistry::getPriorityTarget()
{
  if (armorUnitList.empty())
  {

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
     priority_queue<ArmorUnit, vector<ArmorUnit>, cmp> pq;
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
} //getPriorityTarget

double ArmorRegistry::getDistance(CvPoint pointO, CvPoint pointA)
{
  double distance;
  distance = powf((pointO.x - pointA.x), 2) + powf((pointO.y - pointA.y), 2);
  distance = sqrtf(distance);

  return distance;
} //getDistance

bool ArmorRegistry::contains_circle(Mat& src)
{
  if (src.rows < 10 || src.cols < 10)
  {
      return false;
  }

  ArmorDetect().thresholding(src, src);
  vector<vector < Point>> contours;
  findContours(src, contours, RETR_LIST, CHAIN_APPROX_NONE);

  for (int i = 0; i < contours.size(); i++)
  {
      if (contours[i].size() < 20)
      {
          continue; //skiping the small counts
      }

      Mat pointsf;
      Mat(contours[i]).convertTo(pointsf, CV_32F);
      RotatedRect box = fitEllipse(pointsf);

      if (box.size.width * box.size.height < src.cols * src.rows / 3)
      {
          continue;
      }

      if (box.size.width / box.size.height < 0.8 || box.size.width / box.size.height > 1.2)
      {
          continue;
      }

      return true;
  }

  return false;
} //contains_circle

void ArmorRegistry::clean_invalid_entry()
{
  for (int i = 0; i < armorUnitList.size(); i++)
  {
      //armorUnitList[i].confirmation-=confirmation_threshold*1.2;
      if (abs(frameCount - armorUnitList[i].timestamp) > 5)
      {
          armorUnitList[i].verified = false;
      }

      if (!armorUnitList[i].verified)
      {
          armorUnitList.erase(armorUnitList.begin() + i);
      }
  }
} //clean_invalid_entry

bool ArmorRegistry::is_valid_to_register(ArmorUnit item)
{
  if (item.p1.x < 0 || item.p1.y < 0 || item.p2.x < 0 || item.p2.y < 0)
  {
      cout << "Armor not registered for invalid position" << endl;
      return false;
  }

  if (item.confirmation != 0)
  {
      cout << "Armor not registered for abundent info" << endl;
      return false;
  }

  if (item.verified)
  {
      cout << "Armor not registered for skipping the registery process." << endl;
      return false;
  }

  if (item.status != INVALID)
  {
      cout << "Armor not registered for skipping the registery process." << endl;
      return false;
  }

  return true;
} //is_valid_to_register

int ArmorRegistry::match_to_registry(ArmorUnit item)
{
  const int max_score = 125;
  int *scores = new int[armorUnitList.size()];

  for (int i = 0; i < armorUnitList.size(); i++)
  {
      int difference1 = getDistance(item.p1, armorUnitList[i].p1);
      int difference2 = getDistance(item.p2, armorUnitList[i].p2);
      scores[i] = max_score - difference1 + max_score - difference2;
  }

  int best_fit_index = -1;
  int best_fit_score = INT_MIN;

  for (int i = 0; i < armorUnitList.size(); i++)
  {
      if (scores[i] > 0 && scores[i] > best_fit_score)
      {
          best_fit_index = i;
          best_fit_score = scores[i];
      }
  }

  delete[]scores;
  return best_fit_index;
} //match_to_registry

// End of file