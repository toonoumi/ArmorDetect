/*******************************************************
 * File name: RMVideoCapture.h
 *
 * Description:
 *
 *
 * Created by:  Author1
 * Edited by:   Contributor1, Contributor2
 *
 * Created:   yyyy-mm-dd
 * Last Edit: yyyy-mm-dd
 */


#ifndef _RMVIDEOCAPTURE_H
#define _RMVIDEOCAPTURE_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
// OpenCV library
#include "opencv2/core.hpp"

/* ************************************************************************** */
/* Section: Class Declaration                                                 */
/* ************************************************************************** */
class RMVideoCapture {
 public:
  /**
   *
   * @param device
   * @param size_buffer
   */
  RMVideoCapture(const char* device, int size_buffer = 1);

  /**
   *
   * @param image
   * @return
   */
  RMVideoCapture& operator>>(cv::Mat& image);

  /**
   *
   */
  ~RMVideoCapture();

  /**
   *
   * @return
   */
  bool startStream();

  /**
   *
   * @return
   */
  bool closeStream();

  /**
   *
   * @param auto_exp
   * @param t
   * @return
   */
  bool setExposureTime(bool auto_exp, int t);

  /**
   *
   * @param width
   * @param height
   * @param mjpg
   * @return
   */
  bool setVideoFormat(int width, int height, bool mjpg = 1);

  /**
   *
   * @param width
   * @param height
   * @param mjpg
   * @return
   */
  bool changeVideoFormat(int width, int height, bool mjpg = 1);

  /**
   *
   * @param width
   * @param height
   * @return
   */
  bool getVideoSize(int& width, int& height);

  /**
   *
   * @param fps
   * @return
   */
  bool setVideoFPS(int fps);

  /**
   *
   * @param bsize
   * @return
   */
  bool setBufferSize(int bsize);

  /**
   *
   */
  void restartCapture();

  /**
   *
   * @return
   */
  int getFrameCount();

  /**
   *
   */
  void info();

 private:
  unsigned int capture_width, capture_height;
  unsigned int buffer_size, buffr_idx;
  unsigned int format;
  unsigned int cur_frame;
  int fd;

  const char* video_path;

  struct MapBuffer {
    void* ptr;
    unsigned int size;
  };
  MapBuffer* mb;

  /**
   *
   * @param data
   * @param image
   */
  void cvtRaw2Mat(const void* data, cv::Mat& image);

  /**
   *
   * @return
   */
  bool refreshVideoFormat();

  /**
   *
   * @return
   */
  bool initMMap();

  /**
   *
   * @param fd
   * @param request
   * @param arg
   * @return
   */
  int xioctl(int fd, int request, void* arg);
};

#endif //_RMVIDEOCAPTURE_H