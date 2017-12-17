//
//  AD_Util.h
//  ArmorDetect
//
//  Created by Jason Lu on 10/12/17.
//

#ifndef AD_Util_h
#define AD_Util_h
#include <utility>
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc.hpp"
//#include <vector>


#define BLUE_HUE_LOW 		105
#define BLUE_HUE_HIGH 	125
#define BLUE_SAT_LOW 		200
#define BLUE_SAT_HIGH 	255
#define BLUE_VAL_LOW 		200
#define BLUE_VAL_HIGH		255
#define BLUE_SAT_MIN 		85
#define BLUE_VAL_MIN 		50
#define U_FLOOR(x)			( (x) < 0 ? 0 : (x) )
#define U8_CEILING(x)		( (x) > 255 ? 255 : (x) )  
#define ROI_CONSTANT 		3
#define ROI(x) 					((x) / ROI_CONSTANT)
#define ROI_RANGE(x) 		Range(ROI(x), (x) - ROI(x))
#define PIXELS_THRES 		0.65
#define FRAMES_THRES 		0.70
#define TUNER_MEDIUM(x)	((x) * 0.10)
#define TUNER_COARSE(x)	((x) * 0.15)


using namespace cv;

struct _AimPoint{
    float x;
    float y;
    float offset_x;
    float offset_y;
};
typedef struct _AimPoint AimPoint;

struct _WindowRect{
    int x;
    int y;
    int width;
    int height;
};

typedef struct _WindowRect WindowRect;

struct _HSVRange{
    int LowH;
    int HighH;
    int LowS;
    int HighS;
    int LowV;
    int HighV;
};
typedef struct _HSVRange HSVRange;

enum DirectionHint{MOVE_UP=1,MOVE_DOWN,MOVE_LEFT,MOVE_RIGHT};

class AD_Util{
public:
    /**
     * The constructor of AD_Util class
     *
     */
    AD_Util();
    /**
     * Deconstructor of AD_Util class
     */
    ~AD_Util();
    /**
     * This is to display a video or a image. Instead of using imshow
     * we will be using this function. It will organize the windows so that
     * they do not overlap each other when displaying on screen
     * @param frame
     *      The Mat image to display
     * @return
     *      if the image is successfully displayed
     */
    bool display_Img(Mat frame);
    /**
     * This will draw a aim object on a frame, like a first personal shooting game HUD
     * offset is for zeroing purpose.
     * make it cool
     *
     * @param frame
     *      The Mat image to draw the HUD (in BGR)
     * @param aim
     *      The location to draw the aimming HUD
     * @return
     *      if the HUD is drew
     */
    bool draw_Aim(AimPoint aim, Mat&frame);
    /**
     * This will draw arrows to the directions we want to go. (Draw it by the borders of the
     * frame, make it a decent UI)
     * ex: If the Aim is on the left of our target, client code will invoke this function
     * to draw a right arrow.
     * @param frame
     *      The Mat image to draw the hint (in BGR)
     * @param hint
     *      The direction himt we want to draw
     * @return
     *      if the HUD is drew
     */
    bool draw_Arrow(DirectionHint hint, Mat&frame);
    /**
     * This will find exposure adjustment that our program needs to make to the camera
     * for it to clearly see objects. Hint: calculate several frames and average the results.
     *
     *
     * scale: -12 to 12
     * @param frames
     *      The Mat images for it to make decision
     * @return
     *      the adjustment it needs to make to current exposure
     */
    float find_Exposure_Adjustment(std::vector<Mat> frames);
    /**
     * This will find temperature adjustment that our program needs to make to the camera
     * for it to clearly see objects and identify colors.
     * Hint: calculate several frames and average the results.
     *
     *
     * scale: -12 to 12
     * @param frames
     *      The Mat images for it to make decision (in BGR)
     * @return
     *      the adjustment it needs to make to current exposure
     */
    float find_Temperature_Adjustment(std::vector<Mat> frames);
    /**
     * This will find correct HSV range for identifying the concept "RED object"
     * for calibration purpose. Place a plain red object in front of the camera,
     * until it sees majority of the points in a frame to pass the threshhold, report the range
     * *May requires understanding of PID algorithm
     * @param frames
     *      The Mat images for it to make decision (in HSV)
     * @return
     *      The correct HSV range to identify red objects
     */
    HSVRange find_Red_HSVRange(std::vector<Mat> frames);
    /**
     * This will find correct HSV range for identifying the concept "BLUE objects"
     * for calibration purpose. Place a plain blue object in front of the camera,
     * until it sees majority of the points in a frame to pass the threshhold, report the range
     * *May requires understanding of PID algorithm
     * @param frames
     *      The Mat images for it to make decision (in HSV)
     * @return
     *      The correct HSV range to identify blue objects
     */
   std::pair<HSVRange, bool> find_Blue_HSVRange(std::vector<Mat> frames);
    /**
     *cvtype
     */
     

private:
    std::vector<WindowRect> windowsDisplayed;
		std::array<std::vector<Mat>,2> testFrames(const HSVRange& range, const std::vector<Mat>& frames);
   
};

#endif /* AD_Util_h */