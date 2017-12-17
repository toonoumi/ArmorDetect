#include<iostream>
#include <cctype>
#include <string>
#include <cstdlib>
#include "opencv2/videoio.hpp"
#include "AD_Util.h"



using std::vector;
using std::cout;
using std::array;
using std::pair;

void checkArgs(int argc, char* const argv[]);

// Test functions with image files or video stream
void test_FindBlueRange(const char* images[], int sz);
void test_FindBlueRange(int id);

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

void test_FindBlueRange(const char* images[], int sz)
{
	vector<Mat> images_bgr, images_hsv;
	
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
		GaussianBlur (img, img, Size(9, 9), 3, 3); // blur images to reduce false positives
		cvtColor(img, img_hsv, COLOR_BGR2HSV); // Covert image to HSV format
		images_hsv.push_back(img_hsv);
	}
	
	// Calibrate HSV threshhold
	AD_Util ad;
	std::pair<HSVRange, bool> blue_hsv = ad.find_Blue_HSVRange(images_hsv);
	
	if (!blue_hsv.second)
	{
		string input;
		cout << "Calibration Failed\n";
		cout << "Continue?(y or n): \n";
		std::getline(cin, input);
		if ( (input.compare("y") && input.compare("Y")) != 0 )
		{
			std::exit(EXIT_SUCCESS);
		}	
	}
	
		// Use range to remove non-desired pixels and display results
	HSVRange range = blue_hsv.first;
	for (int i = 0; i < images_hsv.size(); i++)
	{
		Mat mask, output;
		Scalar lowbnd (range.LowH, range.LowS, range.LowV);
		Scalar uppbnd (range.HighH, range.HighS, range.HighV);
		inRange(images_hsv[i], lowbnd, uppbnd, mask);
		bitwise_and(images_bgr[i], images_bgr[i], output, mask);
		
		namedWindow("image original", WINDOW_NORMAL);
		imshow("image original", images_bgr[i]);
		namedWindow("result", WINDOW_NORMAL);
		imshow("result", output);
			
		for (char c = waitKey(0); c != 32; c = waitKey(0))
		{ if (c == 27 )	{ return; } }
		
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
	
	HSVRange range = {.LowH = BLUE_HUE_LOW,
									 .HighH = BLUE_HUE_HIGH, 
									 .LowS = BLUE_SAT_LOW, .HighS = BLUE_SAT_HIGH, 
									 .LowV = BLUE_VAL_LOW, .HighV = BLUE_VAL_HIGH };
									  
	while (1)
	{
		Mat frame;
		if (!cam.read(frame))
		{
			cout << "Could not read frame\n";
			return;
		}
		
		// Convert frame
		Mat frame_hsv;
	 	cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
	 	
	 	vector<Mat> tmp ({frame_hsv});
		array<vector<Mat>, 2> results = AD_Util().testFrames(range, tmp);
	 	std::pair<HSVRange, bool> blue_hsv = ad.find_Blue_HSVRange(images_hsv);
	 	
	 	// Add rectangle used for color detections
		Range rows = ROI_RANGE(frame.rows);
		Range cols = ROI_RANGE(frame.cols);
		
		if (!results.front().empty())
		{
			rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(0,255,0), 7);
		}else
		{
			rectangle(frame, Point(cols.start, rows.start), Point(cols.end, rows.end), Scalar(0,0,0), 7);
			
			// Calibrate range
			vector<Mat> frames_hsv;
			for (Mat next_frame;frames_hsv.size()<=5;)
			{
				cam.read(next_frame);
				cvtColor(next_frame, frame_hsv, COLOR_BGR2HSV);
				frames_hsv.push_back(frame_hsv);
			}

			//range = AD_Util().find_Blue_HSVRange(frames_hsv);
		}
		
		Mat mask;
//		Scalar lowbnd (range.LowH, range.LowS, range.LowV);
//		Scalar uppbnd (range.HighH, range.HighS, range.HighV);
		inRange(frame_hsv, lowbnd, uppbnd, mask);
		
		namedWindow("image original", WINDOW_NORMAL);
		imshow("image original", frame);
		namedWindow("mask", WINDOW_NORMAL);
		imshow("mask", mask);
			
		char c = waitKey(5);
		 if (c == 27 ) { return; }	
	}
	
}	

//End of file
