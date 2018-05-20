/*******************************************************
 * File name: ConfirmationAlgo.h
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


#ifndef _CONFIRMATIONALGO_H
#define _CONFIRMATIONALGO_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// Standard library
#include <vector>

// OpenCV library
#include "opencv2/core.hpp"

/* ************************************************************************** */
/* Section: Types                                                             */
/* ************************************************************************** */

/*
 to record the lockon status for the lockon functionality.
 */
enum LockonStatus {
  INVALID, VALID, LOCKED_ON, URGENT
};

struct ArmorUnit {
  cv::Point p1;
  cv::Point p2;
  int confirmation = 0; //to store how many times it has been confirmed.
  bool verified = false; //if it contains 66 percent of more confirmation, it is true.
  LockonStatus status = INVALID;
  uint64_t timestamp = 0;
  int num = 0;

  ArmorUnit() { }

  ArmorUnit(cv::Point p1, cv::Point p2, int confirmation, bool verified, LockonStatus status, uint64_t timestamp) : p1(p1), p2(p2), confirmation(confirmation), verified(verified), status(status), timestamp(timestamp) { }
};

struct cmp {

  bool operator()(ArmorUnit A, ArmorUnit B) {
    if (A.status == B.status)
      return (abs(A.p2.x - A.p1.x)*(A.p2.y - A.p1.y) < abs(B.p2.x - B.p1.x)*(B.p2.y - B.p1.y));
    else if (A.status == INVALID && B.status == VALID)
      return true;
      //else if (A.status != LOCKED_ON && B.status == LOCKED_ON)
      //return true;
    else return false;
  }

};

/* ************************************************************************** */
/* Section: Class Declaration                                                 */
/* ************************************************************************** */
class ArmorRegistry {
 public:
  ArmorRegistry();

  ~ArmorRegistry();

  /**
   * True is returned if the armor is registered.
   *  @param item
   *          the armor unit to register
   * @return
   *     True if the armor was successfully registered, false otherwise.
   */
  bool register_armor(ArmorUnit item);

  /**
   * Returns the current armor unit registry.
   *  @return
   *      returns a vector of ArmorUnits, i.e the registry.
   */
  std::vector<ArmorUnit> get_registered_armor();

  /**
   *
   */
  void update_timestamp();

  /**
   *
   */
  void getPriorityTarget();


 private:
  std::vector<ArmorUnit> armorUnitList;
  int number_of_registration;
  const int confirmation_threshold = 3;
  uint64_t frameCount;

  /**
   *  To get the distance from point 0 to point A
   *  @param pointO
   *      starting point
   *  @param pointA
   *      ending point
   *  @return
   *      return the distance between two point
   */
  double getDistance(CvPoint pointO, CvPoint pointA);

  /**
   *  If the image sent contains a circle in the image
   *
   *  @param src
   *      the source image
   *  @return
   *      True if the image contains a circle, false otherwise.
   *
   */
  bool contains_circle(cv::Mat& src);

  /*
   * procedure to clean the invalid identified armor.
   * in this case, armor without update will be removed.
   */
  void clean_invalid_entry();

  /**
   *  To judge if a passed armorunit is a valid one to register
   *
   *  @param item
   *          the armor unit passed for judgment
   *  @return
   *          True if it is valid, false otherwise.
   *
   */
  bool is_valid_to_register(ArmorUnit item);

  /**
   *  To match a passed armor unit to the current registry
   *
   *  @param item
   *          the armor unit for matching
   *  @return
   *          the index of the current registry that the armor unit matched to
   */
  int match_to_registry(ArmorUnit item);

}; // Armor Registry

#endif // _CONFIRMATIONALGO_H
