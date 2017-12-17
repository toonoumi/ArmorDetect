#pragma once
#include "AD_Util.h"

// Test functions with image files or video stream
void test_FindBlueRange(char* images[], int sz);
void test_FindBlueRange(int id);
bool testImage(const Mat& roi, const HSVRange& range, float thres);
std::pair<HSVRange, bool> calibrate(VideoCapture& cam);
