# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/andrecajus/Desktop/ProjectImgAlgorithm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/andrecajus/Desktop/ProjectImgAlgorithm/bin

# Include any dependencies generated for this target.
include calibration/CMakeFiles/calib.dir/depend.make

# Include the progress variables for this target.
include calibration/CMakeFiles/calib.dir/progress.make

# Include the compile flags for this target's objects.
include calibration/CMakeFiles/calib.dir/flags.make

calibration/CMakeFiles/calib.dir/calibration.cpp.o: calibration/CMakeFiles/calib.dir/flags.make
calibration/CMakeFiles/calib.dir/calibration.cpp.o: ../calibration/calibration.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object calibration/CMakeFiles/calib.dir/calibration.cpp.o"
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/calib.dir/calibration.cpp.o -c /home/andrecajus/Desktop/ProjectImgAlgorithm/calibration/calibration.cpp

calibration/CMakeFiles/calib.dir/calibration.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/calib.dir/calibration.cpp.i"
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/andrecajus/Desktop/ProjectImgAlgorithm/calibration/calibration.cpp > CMakeFiles/calib.dir/calibration.cpp.i

calibration/CMakeFiles/calib.dir/calibration.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/calib.dir/calibration.cpp.s"
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/andrecajus/Desktop/ProjectImgAlgorithm/calibration/calibration.cpp -o CMakeFiles/calib.dir/calibration.cpp.s

calibration/CMakeFiles/calib.dir/calibration.cpp.o.requires:
.PHONY : calibration/CMakeFiles/calib.dir/calibration.cpp.o.requires

calibration/CMakeFiles/calib.dir/calibration.cpp.o.provides: calibration/CMakeFiles/calib.dir/calibration.cpp.o.requires
	$(MAKE) -f calibration/CMakeFiles/calib.dir/build.make calibration/CMakeFiles/calib.dir/calibration.cpp.o.provides.build
.PHONY : calibration/CMakeFiles/calib.dir/calibration.cpp.o.provides

calibration/CMakeFiles/calib.dir/calibration.cpp.o.provides.build: calibration/CMakeFiles/calib.dir/calibration.cpp.o

# Object files for target calib
calib_OBJECTS = \
"CMakeFiles/calib.dir/calibration.cpp.o"

# External object files for target calib
calib_EXTERNAL_OBJECTS =

calib: calibration/CMakeFiles/calib.dir/calibration.cpp.o
calib: calibration/CMakeFiles/calib.dir/build.make
calib: /usr/local/lib/libopencv_videostab.so.3.1.0
calib: /usr/local/lib/libopencv_videoio.so.3.1.0
calib: /usr/local/lib/libopencv_video.so.3.1.0
calib: /usr/local/lib/libopencv_superres.so.3.1.0
calib: /usr/local/lib/libopencv_stitching.so.3.1.0
calib: /usr/local/lib/libopencv_shape.so.3.1.0
calib: /usr/local/lib/libopencv_photo.so.3.1.0
calib: /usr/local/lib/libopencv_objdetect.so.3.1.0
calib: /usr/local/lib/libopencv_ml.so.3.1.0
calib: /usr/local/lib/libopencv_imgproc.so.3.1.0
calib: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
calib: /usr/local/lib/libopencv_highgui.so.3.1.0
calib: /usr/local/lib/libopencv_flann.so.3.1.0
calib: /usr/local/lib/libopencv_features2d.so.3.1.0
calib: /usr/local/lib/libopencv_core.so.3.1.0
calib: /usr/local/lib/libopencv_calib3d.so.3.1.0
calib: /usr/local/lib/libopencv_features2d.so.3.1.0
calib: /usr/local/lib/libopencv_ml.so.3.1.0
calib: /usr/local/lib/libopencv_highgui.so.3.1.0
calib: /usr/local/lib/libopencv_videoio.so.3.1.0
calib: /usr/local/lib/libopencv_imgcodecs.so.3.1.0
calib: /usr/local/lib/libopencv_flann.so.3.1.0
calib: /usr/local/lib/libopencv_video.so.3.1.0
calib: /usr/local/lib/libopencv_imgproc.so.3.1.0
calib: /usr/local/lib/libopencv_core.so.3.1.0
calib: calibration/CMakeFiles/calib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable ../calib"
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/calib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
calibration/CMakeFiles/calib.dir/build: calib
.PHONY : calibration/CMakeFiles/calib.dir/build

calibration/CMakeFiles/calib.dir/requires: calibration/CMakeFiles/calib.dir/calibration.cpp.o.requires
.PHONY : calibration/CMakeFiles/calib.dir/requires

calibration/CMakeFiles/calib.dir/clean:
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration && $(CMAKE_COMMAND) -P CMakeFiles/calib.dir/cmake_clean.cmake
.PHONY : calibration/CMakeFiles/calib.dir/clean

calibration/CMakeFiles/calib.dir/depend:
	cd /home/andrecajus/Desktop/ProjectImgAlgorithm/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/andrecajus/Desktop/ProjectImgAlgorithm /home/andrecajus/Desktop/ProjectImgAlgorithm/calibration /home/andrecajus/Desktop/ProjectImgAlgorithm/bin /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration /home/andrecajus/Desktop/ProjectImgAlgorithm/bin/calibration/CMakeFiles/calib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : calibration/CMakeFiles/calib.dir/depend

