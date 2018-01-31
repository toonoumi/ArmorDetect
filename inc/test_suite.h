#pragma once
#include "AD_Util.h"

/*
 Functions for testing FindBlueHSVRange and FindRedHSVRange
 */

// Test with images
void test_FindHSVRange(char* images[], int sz, bool color);

// Test with camera
void test_FindHSVRange(int id, bool color);
bool testImage(const Mat& roi, const HSVRange& range, float thres);
std::pair<HSVRange, bool> calibrate(VideoCapture& cam, bool color);

/*
 Functions for testing threshMask
 */
// Test with camera
void test_threshMask(int id);
