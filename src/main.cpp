#include<iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "opencv2/videoio.hpp"
#include "AD_Util.h"
#include "test_suite.h"




using std::vector;
using std::cout;
using std::array;
using std::pair;


int main(int argc, char* argv[])
{
	checkArgs(argc, argv);
	
	if (strcmp("-F", argv[1]) == 0)
		test_FindBlueRange(argv+2, argc - 2);
	else
		test_FindBlueRange(strtol(argv[2], nullptr, 0));

	return EXIT_SUCCESS;
}



void checkArgs(int argc, char* const argv[])
{
	 if(argc == 3 && (strcmp("-F", argv[1]) == 0 || 
	 								 (strcmp("-C", argv[1]) == 0 && isdigit(argv[2][0])) ))
		{ return;}
		else if (argc > 3 && strcmp("-F", argv[1]) == 0)
		{ return; }
		
		cout << "Usage: ";
	 	cout << argv[0];
	 	cout << " -C [camera id number] | -F [path to image(s)]\n";
	 	 
	 	std::exit(EXIT_FAILURE);
}

void test_FindBlueRange(char* images[], int sz)
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
	pair<HSVRange, bool> blue_hsv = AD_Util().find_Blue_HSVRange(images_bgr);
	
	if (!blue_hsv.second) cout << "Calibration Failed\n";	
	
	// Use range to remove non-desired pixels and display results
	HSVRange range = blue_hsv.first;
	for (const auto& image : images_bgr)
	{
		Mat image_hsv, mask, output;
		
		cvtColor(image,image_hsv, COLOR_BGR2HSV);
		Scalar lowbnd (range.LowH, range.LowS, range.LowV);
		Scalar uppbnd (range.HighH, range.HighS, range.HighV);
		inRange(image_hsv, lowbnd, uppbnd, mask);
		bitwise_and(image, image, output, mask);
		
		namedWindow("Original", WINDOW_NORMAL);
		imshow("Original", image);
		namedWindow("Result", WINDOW_NORMAL);
		imshow("Result", output);
			
		for (char c = waitKey(0); c != 32; c = waitKey(0))
			if (c == 27 )	return; 		
	}
}

void test_FindBlueRange(int id)
{
	VideoCapture cam;
	 
	if(!cam.open(id))
	{
		cout << "Could not open camera\n";
		std::exit(EXIT_FAILURE);
	}	
	
	HSVRange range {.LowH = 110, .HighH = 130, 
								 .LowS = 100, .HighS = 255, 
								 .LowV = 100, .HighV = 255 };
								 
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
		Scalar lowbnd (range.LowH, range.LowS, range.LowV);
		Scalar uppbnd (range.HighH, range.HighS, range.HighV);
		inRange(frame_hsv, lowbnd, uppbnd, mask);
		
		// Place a retangle in the center of the frame.
		// Color of retangle will change if most of the pixels in it are blue
		Range rows = ROI_RANGE(frame.rows);
		Range cols = ROI_RANGE(frame.cols);
		
		if (testImage(frame_hsv(rows, cols), range, .50))
		{
			rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(0,255,0), 7);
		}else
		{
			rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(0,0,0), 7);	
		}
		
		namedWindow("Camera Feed", WINDOW_NORMAL);
		imshow("Camera Feed", frame);
		namedWindow("Mask", WINDOW_NORMAL);
		imshow("Mask", mask);
		
		char c = waitKey(5);
		if (c == 27 ) { return; }
		 else if (c == 32)
		 {
			 	pair<HSVRange, bool> blue_hsv = calibrate(cam);
				if (blue_hsv.second)
				{ 
					cout << "Calibration Succeeded\n"; 
					range = blue_hsv.first;
				}
				else { cout << "Calibration failed\n"; }
		 }	
	}
	
}	

pair<HSVRange, bool> calibrate(VideoCapture& cam)
{
	cout << "Place a blue object in the center of the screen\n";
	cout << "Calibrating...\n";
	
	vector<Mat> frames;
	for (Mat next_frame;frames.size() < 10;)
	{
		if (!cam.read(next_frame))
		{
			cout << "Error reading frame\n";
			return pair<HSVRange, bool>(HSVRange(), false);
		}
		frames.push_back(next_frame);
	}

	return AD_Util().find_Blue_HSVRange(frames);
	
}

bool testImage(const Mat& roi, const HSVRange& range, float thres)
{
	// find pixels that match range
	Mat mask;
	uint32_t  pixelsfound = 0;
	
	Scalar lower (range.LowH, range.LowS, range.LowV);
	Scalar upper (range.HighH, range.HighS, range.HighV);			
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
//End of file
