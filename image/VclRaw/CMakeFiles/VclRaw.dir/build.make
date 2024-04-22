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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home_D/Documents/Dual_ISO_project/release_Eurographics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home_D/Documents/Dual_ISO_project/release_Eurographics

# Include any dependencies generated for this target.
include image/VclRaw/CMakeFiles/VclRaw.dir/depend.make

# Include the progress variables for this target.
include image/VclRaw/CMakeFiles/VclRaw.dir/progress.make

# Include the compile flags for this target's objects.
include image/VclRaw/CMakeFiles/VclRaw.dir/flags.make

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o: image/VclRaw/CMakeFiles/VclRaw.dir/flags.make
image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o: image/VclRaw/VclRaw.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home_D/Documents/Dual_ISO_project/release_Eurographics/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o"
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/VclRaw.dir/VclRaw.cpp.o -c /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw/VclRaw.cpp

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VclRaw.dir/VclRaw.cpp.i"
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw/VclRaw.cpp > CMakeFiles/VclRaw.dir/VclRaw.cpp.i

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VclRaw.dir/VclRaw.cpp.s"
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw/VclRaw.cpp -o CMakeFiles/VclRaw.dir/VclRaw.cpp.s

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.requires:
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.requires

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.provides: image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.requires
	$(MAKE) -f image/VclRaw/CMakeFiles/VclRaw.dir/build.make image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.provides.build
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.provides

image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.provides.build: image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o

# Object files for target VclRaw
VclRaw_OBJECTS = \
"CMakeFiles/VclRaw.dir/VclRaw.cpp.o"

# External object files for target VclRaw
VclRaw_EXTERNAL_OBJECTS =

image/VclRaw/libVclRaw.a: image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o
image/VclRaw/libVclRaw.a: image/VclRaw/CMakeFiles/VclRaw.dir/build.make
image/VclRaw/libVclRaw.a: image/VclRaw/CMakeFiles/VclRaw.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libVclRaw.a"
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && $(CMAKE_COMMAND) -P CMakeFiles/VclRaw.dir/cmake_clean_target.cmake
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VclRaw.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
image/VclRaw/CMakeFiles/VclRaw.dir/build: image/VclRaw/libVclRaw.a
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/build

image/VclRaw/CMakeFiles/VclRaw.dir/requires: image/VclRaw/CMakeFiles/VclRaw.dir/VclRaw.cpp.o.requires
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/requires

image/VclRaw/CMakeFiles/VclRaw.dir/clean:
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw && $(CMAKE_COMMAND) -P CMakeFiles/VclRaw.dir/cmake_clean.cmake
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/clean

image/VclRaw/CMakeFiles/VclRaw.dir/depend:
	cd /home_D/Documents/Dual_ISO_project/release_Eurographics && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home_D/Documents/Dual_ISO_project/release_Eurographics /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw /home_D/Documents/Dual_ISO_project/release_Eurographics /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw /home_D/Documents/Dual_ISO_project/release_Eurographics/image/VclRaw/CMakeFiles/VclRaw.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : image/VclRaw/CMakeFiles/VclRaw.dir/depend

