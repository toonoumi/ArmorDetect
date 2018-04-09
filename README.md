# ArmorDetect

Authored by RoboGrinder's Computer Vision sub-team.

The repository contains sofware that will be used to creatd an autonomous robot for the RoboMaster competition. The robot, dubbed the "Armor Bot", must detect and fire upon enemy robots in the arena. As stated in the 2018 Competition Rules, it cannot be manualy operated.

OpenCV is leveraged in the development of the CV application.
## Effect and Results

Go ahead and check the repository for image and demonstration for the results and outcome.

## Installing OpenCV

Below is a link to tutorials provided by the OpenCV contributors with instructions on how to set up OpenCV on various systems. They are not updated often but they are the most simple. If you run into trouble try the other links below.

https://docs.opencv.org/master/df/d65/tutorial_table_of_content_introduction.html

Other tutorial that were found useful:

https://www.learnopencv.com/install-opencv3-on-ubuntu/
http://www.learnopencv.com/install-opencv3-on-windows/

Some instructions on how the set up OpenCV with XCode. Thank you to Youming Qin (@QinYouming), a.k.a Aaron, for prodviding these.

If you are trying to setup openCV with Xcode:
1.  Download the latest openCV library from Github
2.  Compile the source code with Cmake
3.  Watch this video and follow the instruction:
      https://www.youtube.com/watch?time_continue=148&v=XJeP1juuHHY
4.  The default location of the header files is: /usr/local/include
The default location of the library files is : /usr/local/lib
The additional linker flags are: -lopencv_calib3d -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_video -lopencv_videoio -lopencv_videostab
    Note, do not include -lopencv_ts for additional linker flags
5.  If you have further questions, please contact Youming Qin at qyoumi9@vt.edu. GL HF
