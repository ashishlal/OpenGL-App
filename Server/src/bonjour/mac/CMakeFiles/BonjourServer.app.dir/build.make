# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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
CMAKE_COMMAND = "/Applications/CMake 2.8-3.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-3.app/Contents/bin/cmake" -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-3.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server

# Utility rule file for BonjourServer.app.

src/bonjour/mac/CMakeFiles/BonjourServer.app: src/bonjour/mac/cmd1

src/bonjour/mac/cmd1:
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating cmd1"
	cd /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/src/bonjour/mac && xcodebuild -project BonjourServer.xcodeproj -alltargets && cp -r build/Release/BonjourServer.app /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/bin/mac

BonjourServer.app: src/bonjour/mac/CMakeFiles/BonjourServer.app
BonjourServer.app: src/bonjour/mac/cmd1
BonjourServer.app: src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/build.make
.PHONY : BonjourServer.app

# Rule to build all files generated by this target.
src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/build: BonjourServer.app
.PHONY : src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/build

src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/clean:
	cd /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/src/bonjour/mac && $(CMAKE_COMMAND) -P CMakeFiles/BonjourServer.app.dir/cmake_clean.cmake
.PHONY : src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/clean

src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/depend:
	cd /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/src/bonjour/mac /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/src/bonjour/mac /Users/Ashish/mygithub/OpenGL-Broadcast-App-Maintenance/Server/src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/bonjour/mac/CMakeFiles/BonjourServer.app.dir/depend

