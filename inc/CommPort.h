/*******************************************************
 * File name:
 *
 * Description:
 *
 *
 * Created by:  Jason Lu
 * Edited by:   Frank Eyenga
 *
 * Created:   2018-03-30
 * Last Edit: 2018-05-18
 */


#ifndef _COMMPORT_H
#define _COMMPORT_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// OpenCV library
#include "opencv2/core.hpp"

/* ************************************************************************** */
/* Section: Class Declaration                                                 */

/* ************************************************************************** */
class CommPort {
 public:
  /**
   * default constructor
   */
  CommPort();

  /**
   *
   * @param baudRate
   */
  CommPort(int baudRate);


  /**
   * Destructor
   */
  ~CommPort();

  /**
   *
   * @return
   */
  int getBaudRate();

  /**
   *
   * @param baud_rate
   */
  void setBaudRate(int baud_rate);

  /**
   *
   * @param point_of_difference
   */
  void sendDifference(cv::Point point_of_difference);

  /**
   *
   * @return
   */
  bool requestTeamInfo();

 private:
  int baudRate;
  int cv_uplink;
  int cv_downlink;
};

#endif //_COMMPORT_H