#include_directories(../inc)

add_executable(ArmorDetect AD_Util.cpp ArmorDetect.hpp main.cpp CommPort.hpp ConfirmationAlgo.hpp RMVideoCapture.cpp test_suite.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries( ArmorDetect ${OpenCV_LIBS} )
