//
//  AD_Util.h
//  ArmorDetect
//														Dates
//  Created by: Jason Lu			10/12/17.
//			Edited: Frank Eyenga 	12/17/2017
//          Edited: Youming Qin     03/03/2018
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

bool AD_Util::display_Img(Mat frame){
    return false;
}
 
bool AD_Util::draw_Aim(AimPoint aim, Mat&frame){
    
    
//            //Method 1 圈加点 dot and circle
//            Point p1(aim.x,aim.y);
//            Point p2(aim.x,aim.y+1);
//            circle(frame, p1, 25, Scalar(13,15,185),2);
//            line(frame, p1, p2, Scalar(13,15,185),2);
    
            //Method 2 圈加叉 dot and cross
            Point p1(aim.x,aim.y);
            Point p2(aim.x,aim.y+1);
            circle(frame, p1, 25, Scalar(13,15,185),2);
            arrowedLine(frame, p2, p1, Scalar(13,15,185),2,8,0,10);
            drawMarker(frame, p1, Scalar(13,15,185),1,20,2,8);
    
    
//            //Method 3 四倍镜 circle and triangle
//            Point p1(aim.x,aim.y);
//            Point p2(aim.x,aim.y+1);
//            circle(frame, p1, 25, Scalar(13,15,185),2);
//            arrowedLine(frame, p2, p1, Scalar(13,15,185),2,8,0,10);
    
    
            return true;
}

bool AD_Util::draw_Arrow(DirectionHint hint, Mat&frame){
    return false;
}
 
float AD_Util::find_Exposure_Adjustment(Matvector frames){
    return false;
}

float AD_Util::find_Temperature_Adjustment(Matvector frames){
    return false;
}

std::pair<HSVRange, bool> AD_Util::find_Red_HSVRange(Matvector frames)
{
	// Previous HSVRange values that workedA
	static vector<HSVRange> previous;
	// Convert to HSV
	Matvector frames_hsv (bgrToHSV(frames, true));
									 
	// Try previously successful ranges first
	for (const auto& range : previous)
	{
		vector<Matvector> results = testFrames(range, frames);
	
		if (results.front().size() >= (frames.size() * FRAMES_THRES))
			return pair<HSVRange, bool>(range, true);
	}


	// Initialize default ranges to start with
	HSVRange cand_low {.LowH = RED_HUE_LOW, .HighH = RED_HUE_HIGH, 
				   		 .LowS = RED_SAT_LOW, .HighS = RED_SAT_HIGH, 
				   		 .LowV = RED_VAL_LOW, .HighV = RED_VAL_HIGH };

	HSVRange cand_upp {.LowH = 172, .HighH = 180, 
				   		 .LowS = RED_SAT_LOW, .HighS = RED_SAT_HIGH, 
				   		 .LowV = RED_VAL_LOW, .HighV = RED_VAL_HIGH };
								 
	Matvector test_frames (frames_hsv), passed, failed;
	bool calibrated = false;
	
	#ifdef DEBUG
		std::cout<< "starting range finder\n\n";
		std::cout<< "# of Frames: " << frames.size() << "\n";
		std::cout<< "# to pass: " << frames.size() * FRAMES_THRES << "\n";
		int pass = 0;
	#endif

	// Try to lower end of the spectrum
	while (!calibrated && ( cand_low.LowS > RED_SAT_MIN || cand_low.LowV > RED_VAL_MIN ))
	{ 
		vector<Matvector> results_low (testFrames(cand_low, test_frames));
		passed.insert(passed.end(), results_low.front().begin(), results_low.front().end());
		
		#ifdef DEBUG
			std::cout<< "--Pass #" << pass++ << "\n";					
			std::cout<< "--Passing frames: " << passed.size() << "\n";
		#endif
		
		if (passed.size() >= (frames.size() * FRAMES_THRES))
		{ 
			calibrated = true; 
			previous.push_back(cand_low);
		}
		else
		{
			// Reduce value by predetermined percentage
			cand_low.LowS -= TUNER_MEDIUM(255);
			cand_low.LowV -= TUNER_COARSE(255);
			test_frames = results_low.back();
		} 
		
		#ifdef DEBUG
			std::cout<< "--Low sat: "  << cand_low.LowS << "\n";
			std::cout<< "--Low val: " << cand_low.LowV << "\n\n";
		#endif
																			
	}									
	
	if(calibrated)
		return pair<HSVRange, bool>(cand_low, calibrated);
		
	// Try upper end of sprectrum
	while (!calibrated && ( cand_upp.LowS > RED_SAT_MIN || cand_upp.LowV > RED_VAL_MIN ))
	{ 
		vector<Matvector> results_upp (testFrames(cand_upp, test_frames));
		passed.insert(passed.end(), results_upp.front().begin(), results_upp.front().end());
		
		#ifdef DEBUG
			std::cout<< "--Pass #" << pass++ << "\n";					
			std::cout<< "--Passing frames: " << passed.size() << "\n";
		#endif
		
		if (passed.size() >= (frames.size() * FRAMES_THRES))
		{ 
			calibrated = true; 
			previous.push_back(cand_upp);
		}
		else
		{
			// Reduce value by predetermined percentage
			cand_upp.LowS -= TUNER_MEDIUM(255);
			cand_upp.LowV -= TUNER_COARSE(255);
			test_frames = results_upp.back();
		} 
		
		#ifdef DEBUG
			std::cout<< "--Low sat: "  << cand_upp.LowS << "\n";
			std::cout<< "--Low val: " << cand_upp.LowV << "\n\n";
		#endif
																			
	}
								  
	return pair<HSVRange, bool>(cand_upp, calibrated);
}

std::pair<HSVRange, bool> AD_Util::find_Blue_HSVRange(Matvector frames)
{
	// Previous HSVRange values that worked
	static vector<HSVRange> previous;
	// Convert to HSV
	Matvector frames_hsv (bgrToHSV(frames, true));
									 
	// Try previously successful ranges first
	for (const auto& range : previous)
	{
		vector<Matvector> results = testFrames(range, frames);
	
		if (results.front().size() >= (frames.size() * FRAMES_THRES))
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
		vector<Matvector> results (testFrames(cand, test_frames));
		passed.insert(passed.end(), results.front().begin(), results.front().end());
		
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
			test_frames = results.back();
		} 
		
		#ifdef DEBUG
			std::cout<< "--Low sat: "  << cand.LowS << "\n";
			std::cout<< "--Low val: " << cand.LowV << "\n\n";
		#endif
																			
	}									
	 
									  
	return pair<HSVRange, bool>(cand, calibrated);
}

Matvector AD_Util::threshMask(Matvector& frames, HSVRange& range)
{
	Matvector frames_hsv (bgrToHSV(frames, true)), masks;
		
	for (const auto& frame : frames_hsv)
	{
		Mat mask;
		Scalar lower (range.LowH, range.LowS, range.LowV);
		Scalar upper (range.HighH, range.HighS, range.HighV);			
		inRange(frame, lower, upper, mask);
		masks.push_back(mask);	
	}
	
	return masks;
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

vector<Matvector> AD_Util::testFrames(const HSVRange& range, const Matvector& frames)
{
	Matvector passed, failed;
		
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
		
		#ifdef DEBUG
			destroyWindow("Mask");
			destroyWindow("Image (HSV)");
		#endif
		
		return vector<Matvector>({passed, failed});

}
/* AD_Util_cpp */
