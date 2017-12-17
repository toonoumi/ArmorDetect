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

typedef vector<Mat> Matvector;

AD_Util::AD_Util(){}

AD_Util::~AD_Util(){}

bool AD_Util::display_Img(Mat frame){}
 
bool AD_Util::draw_Aim(AimPoint aim, Mat&frame){}

bool AD_Util::draw_Arrow(DirectionHint hint, Mat&frame){}
 
float AD_Util::find_Exposure_Adjustment(Matvector frames){}

float AD_Util::find_Temperature_Adjustment(Matvector frames){}

HSVRange AD_Util::find_Red_HSVRange(Matvector frames){}

std::pair<HSVRange, bool> AD_Util::find_Blue_HSVRange(Matvector frames)
{
	// Previous HSVRange values that worked
	static vector<HSVRange> previous;
	Matvector frames_hsv;
	
 	// blur image to reduce false positives then covert image to HSV format
	for (auto& frame : frames)
	{
		Mat tmp;
		GaussianBlur(frame, tmp, Size(9, 9), 3, 3);
		cvtColor(tmp, tmp, COLOR_BGR2HSV);
		frames_hsv.push_back(tmp);
	}	
										 
	// Try previously successful ranges first
	for (const auto& range : previous)
	{
		array<Matvector, 2> results = testFrames(range, frames);
		Matvector passed ();
	
		if (results[0].size() >= (frames.size() * FRAMES_THRES))
			return pair<HSVRange, bool>(range, true);
	}


	// Initialize default ranges to start with
	HSVRange cand {.LowH = BLUE_HUE_LOW, .HighH = BLUE_HUE_HIGH, 
								 .LowS = BLUE_SAT_LOW, .HighS = BLUE_SAT_HIGH, 
								 .LowV = BLUE_VAL_LOW, .HighV = BLUE_VAL_HIGH };
								 
	Matvector test_frames (frames_hsv), passed, failed;
	bool calibrated = false;
	
	#ifdef DEBUG
		std::cout<< "starting range finder\n\n";
		std::cout<< "# of Frames: " << frames.size() << "\n";
		std::cout<< "# to pass: " << frames.size() * FRAMES_THRES << "\n";
		int pass = 0;
	#endif

	while (!calibrated && ( cand.LowS > BLUE_SAT_MIN || cand.LowV > BLUE_VAL_MIN ))
	{ 
		array<Matvector, 2> results = testFrames(cand, test_frames);
		passed.insert(passed.end(), results[0].begin(), results[0].end());
		
		#ifdef DEBUG
			std::cout<< "--Pass #" << pass++ << "\n";					
			std::cout<< "--Passing frames: " << passed.size() << "\n";
		#endif
		
		if (passed.size() >= (frames.size() * FRAMES_THRES))
		{ 
			calibrated = true; 
			previous.push_back(cand);
		}
		else
		{
			// Reduce value by predetermined percentage
			cand.LowS -= TUNER_MEDIUM(255);
			cand.LowV -= TUNER_COARSE(255);
			test_frames = results[1];
		} 
		
		#ifdef DEBUG
			std::cout<< "--Low sat: "  << cand.LowS << "\n";
			std::cout<< "--Low val: " << cand.LowV << "\n\n";
		#endif
																			
	}									
	
	#ifdef DEBUG
			destroyWindow("Mask");
			destroyWindow("Image (HSV)");
		#endif 
									  
	return pair<HSVRange, bool>(cand, calibrated);
}

array<Matvector,2> AD_Util::testFrames(const HSVRange& range, const Matvector& frames)
{

	Matvector passed, failed;
	array<Matvector, 2> results;
		
	for (const auto& frame : frames)
		{
			// find pixels that match range
			Mat mask;
			Scalar lower (range.LowH, range.LowS, range.LowV);
			Scalar upper (range.HighH, range.HighS, range.HighV);			
			inRange(frame, lower, upper, mask);
			
			// use a predetermined area of the image for calibration
			Range rows = ROI_RANGE(frame.rows);
			Range cols = ROI_RANGE(frame.cols);
			Mat roi = mask(rows, cols);
			
			uint32_t  pixelsfound = 0;

			// count the number of pixels found in ROI
			for (unsigned int i = 0; i < roi.rows; i++)
			{
				uint8_t* row = roi.ptr<uint8_t>(i);
	
				for (unsigned int j = 0; j < roi.cols; j++)
					if (row[j]) pixelsfound++; 
			}
			
			#ifdef DEBUG
				std::cout<< "\tPixels needed: " << roi.total() * PIXELS_THRES << "\n";
				std::cout<< "\tPixels found: " << pixelsfound << "\n";			
			#endif
			
			if (pixelsfound >= (roi.total() * PIXELS_THRES))
				{ 
					passed.push_back(frame); 
					#ifdef	DEBUG
						std::cout<< "\tPassed\n";					
					#endif
				}
			else
				{ 
					failed.push_back(frame);	
					#ifdef DEBUG
						std::cout<< "\tfailed\n"; 										
					#endif
				}
				
			#ifdef DEBUG
				namedWindow("Mask", WINDOW_NORMAL);
				imshow("Mask", mask);
				namedWindow("Image (HSV)", WINDOW_NORMAL);
				imshow("Image (HSV)", frame);

				waitKey(0);		
			#endif							
		}
		
		return {passed, failed};

}
/* AD_Util_cpp */
