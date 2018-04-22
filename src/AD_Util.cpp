//
//  AD_Util.h
//  ArmorDetect
//														Dates
//  Created by: Jason Lu			10/12/17.
//			Edited: Frank Eyenga 	12/17/2017 
//


#ifdef DEBUG
#include <iostream>
#endif
 
#include "AD_Util.h"

using std::pair;
using std::vector;
using std::array;

typedef std::vector<Mat> Matvector;

AD_Util::AD_Util(){}

AD_Util::~AD_Util(){}

bool AD_Util::display_Img(Mat frame){}
 
bool AD_Util::draw_Aim(AimPoint aim, Mat&frame){}

bool AD_Util::draw_Arrow(DirectionHint hint, Mat&frame){}
 
float AD_Util::find_Exposure_Adjustment(Matvector frames){}

float AD_Util::find_Temperature_Adjustment(Matvector frames){}

bool AD_Util::calibrateRange(const Matvector& frames, HSVRange& range, Color clr) 
{
	Matvector regions;

	for(const auto frm: frames)
	{
		Range r_rows = ROI_RANGE(frm.rows);
		Range r_cols = ROI_RANGE(frm.cols);
		Mat tmp (frm, r_rows, r_cols);
		Mat roi;
		tmp.copyTo(roi);
		
		if(clr == RED)
			roi = ~roi;
	
		cvtColor(roi, roi, COLOR_BGR2HSV);
		regions.push_back(roi);
	}

	range = findHSVRange(regions, clr);
	return (range.LowH >= 0);
}

HSVRange AD_Util::findHSVRange(Matvector& frames, Color clr)
{
	HSVRange cand;
	size_t list_size = frames.size();
	int passed = 0;

	// Initialize default ranges to start with
	if(clr == RED)
	{
		cand.LowH = CYAN_HUE_LOW;
		cand.HighH = CYAN_HUE_HIGH;
	}
	else 
	{
		cand.LowH = BLUE_HUE_LOW;
		cand.HighH = BLUE_HUE_HIGH;
	}
	
	
	cand.LowS = SAT_LOW;
	cand.HighS = SAT_HIGH;
	cand.LowV = VAL_LOW;
	cand.HighV = VAL_HIGH;

	#ifdef DEBUG
		std::cout<< "starting range finder\n\n";
		std::cout<< "# of Frames: " << frames.size() << "\n";
		std::cout<< "# to pass: " << frames.size() * FRAMES_THRES << "\n";
		int pass = 0;
	#endif

	while(cand.LowS > SAT_MIN && cand.LowV > VAL_MIN )
	{ 
		// Reduce value by predetermined percentage
		cand.LowS -= TUNER_MEDIUM(255);
		cand.LowV -= TUNER_COARSE(255);

		for(int i = 0; i < list_size; i++)
		{
			Mat tmp = frames[i];

			// If test is successful, increment counter, swap with back and delete
			if(testFrame(tmp, cand)) 
			{
				passed++;
				frames[i] = frames.back();
				frames.back() = tmp;
				frames.pop_back();
			}
		}
		
		if(passed < list_size * FRAMES_THRES)
			return cand;
			
		#ifdef DEBUG
			std::cout<< "--Pass #" << pass++ << "\n";					
			std::cout<< "--Passing frames: " << passed << "\n";
		#endif
		
		
		#ifdef DEBUG
			std::cout<< "--Low sat: "  << cand.LowS << "\n";
			std::cout<< "--Low val: " << cand.LowV << "\n\n";
		#endif
	}									
	
	return HSVRange {-1, -1, -1, -1, -1, -1};							  

}


Mat AD_Util::threshMask(Mat& frame, HSVRange& range, unsigned int code) 
{
	Mat mask;

	if(code <= COLOR_COLORCVT_MAX)
		cvtColor(frame, frame, code);
		
	Scalar lower (range.LowH, range.LowS, range.LowV);
	Scalar upper (range.HighH, range.HighS, range.HighV);			
	inRange(frame, lower, upper, mask);
	
	return mask;
}


Matvector AD_Util::bgrToHSV(Matvector& frames, bool blur)
{
	Matvector frames_hsv;
	
	for (auto& frame : frames)
	{
		Mat tmp;

		if (blur)
			GaussianBlur(frame, tmp, Size(9, 9), 3, 3);
		
		cvtColor(tmp, tmp, COLOR_BGR2HSV);
		frames_hsv.push_back(tmp);
	}
	
	return frames_hsv;
}

bool AD_Util::testFrame(const Mat frame, const HSVRange& range)
{
	// use a predetermined area of the image for calibration
	Range r_rows = ROI_RANGE(frame.rows);
	Range r_cols = ROI_RANGE(frame.cols);
	Mat roi (frame, r_rows, r_cols);
	roi = ~roi;

	// find pixels that match range
	Mat mask;
	Scalar lower (range.LowH, range.LowS, range.LowV);
	Scalar upper (range.HighH, range.HighS, range.HighV);			
	inRange(roi, lower, upper, mask);
	
	int cols = mask.cols, rows = mask.rows, pixelsfound = 0;
	
	if(mask.isContinuous())
	{
		cols *= rows;
		rows = 1;
	}

	// count the number of pixels found in ROI
	for (unsigned int i = 0; i < rows; i++)
	{
		uint8_t* row = mask.ptr<uint8_t>(i);

		for (unsigned int j = 0; j < cols; j++)
			if (row[j]) 
				pixelsfound++; 
	}
	
	#ifdef DEBUG
		std::cout<< "\tPixels needed: " << mask.total() * PIXELS_THRES << "\n";
		std::cout<< "\tPixels found: " << pixelsfound << "\n";			

		namedWindow("Mask", WINDOW_NORMAL);
		imshow("Mask", mask);
		namedWindow("Image (HSV)", WINDOW_NORMAL);
		imshow("Image (HSV)", frame);

		waitKey(0);		
	
		destroyWindow("Mask");
		destroyWindow("Image (HSV)");
	#endif
	
	if (pixelsfound >= (mask.total() * PIXELS_THRES))
	{ 
		#ifdef	DEBUG
			std::cout<< "\tPassed\n";					
		#endif
		return true;
	}

	#ifdef DEBUG
		std::cout<< "\tfailed\n"; 										
	#endif
	return false;
}
/* AD_Util_cpp */
