# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.9.4_1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.9.4_1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jasonlu/Documents/Projects/ArmorDetect

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild

# Include any dependencies generated for this target.
include CMakeFiles/ArmorDetect.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ArmorDetect.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ArmorDetect.dir/flags.make

CMakeFiles/ArmorDetect.dir/main.cpp.o: CMakeFiles/ArmorDetect.dir/flags.make
CMakeFiles/ArmorDetect.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ArmorDetect.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ArmorDetect.dir/main.cpp.o -c /Users/jasonlu/Documents/Projects/ArmorDetect/main.cpp

CMakeFiles/ArmorDetect.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ArmorDetect.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jasonlu/Documents/Projects/ArmorDetect/main.cpp > CMakeFiles/ArmorDetect.dir/main.cpp.i

CMakeFiles/ArmorDetect.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ArmorDetect.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jasonlu/Documents/Projects/ArmorDetect/main.cpp -o CMakeFiles/ArmorDetect.dir/main.cpp.s

CMakeFiles/ArmorDetect.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/ArmorDetect.dir/main.cpp.o.requires

CMakeFiles/ArmorDetect.dir/main.cpp.o.provides: CMakeFiles/ArmorDetect.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/ArmorDetect.dir/build.make CMakeFiles/ArmorDetect.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/ArmorDetect.dir/main.cpp.o.provides

CMakeFiles/ArmorDetect.dir/main.cpp.o.provides.build: CMakeFiles/ArmorDetect.dir/main.cpp.o


# Object files for target ArmorDetect
ArmorDetect_OBJECTS = \
"CMakeFiles/ArmorDetect.dir/main.cpp.o"

# External object files for target ArmorDetect
ArmorDetect_EXTERNAL_OBJECTS =

ArmorDetect: CMakeFiles/ArmorDetect.dir/main.cpp.o
ArmorDetect: CMakeFiles/ArmorDetect.dir/build.make
ArmorDetect: /usr/local/lib/libopencv_dnn.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_ml.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_objdetect.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_shape.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_stitching.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_superres.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_videostab.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_calib3d.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_features2d.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_flann.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_highgui.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_photo.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_video.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_videoio.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_imgcodecs.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_imgproc.3.3.0.dylib
ArmorDetect: /usr/local/lib/libopencv_core.3.3.0.dylib
ArmorDetect: CMakeFiles/ArmorDetect.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ArmorDetect"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ArmorDetect.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ArmorDetect.dir/build: ArmorDetect

.PHONY : CMakeFiles/ArmorDetect.dir/build

CMakeFiles/ArmorDetect.dir/requires: CMakeFiles/ArmorDetect.dir/main.cpp.o.requires

.PHONY : CMakeFiles/ArmorDetect.dir/requires

CMakeFiles/ArmorDetect.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ArmorDetect.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ArmorDetect.dir/clean

CMakeFiles/ArmorDetect.dir/depend:
	cd /Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jasonlu/Documents/Projects/ArmorDetect /Users/jasonlu/Documents/Projects/ArmorDetect /Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild /Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild /Users/jasonlu/Documents/Projects/ArmorDetect/mkflBuild/CMakeFiles/ArmorDetect.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ArmorDetect.dir/depend

