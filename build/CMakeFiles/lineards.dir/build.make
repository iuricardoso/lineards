# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake.exe

# The command to remove a file.
RM = /usr/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/g/Meu Drive/1. Ensino/Materiais/lineards/src"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/g/Meu Drive/1. Ensino/Materiais/lineards/build"

# Include any dependencies generated for this target.
include CMakeFiles/lineards.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/lineards.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/lineards.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lineards.dir/flags.make

CMakeFiles/lineards.dir/lineards.c.o: CMakeFiles/lineards.dir/flags.make
CMakeFiles/lineards.dir/lineards.c.o: /g/Meu\ Drive/1.\ Ensino/Materiais/lineards/src/lineards.c
CMakeFiles/lineards.dir/lineards.c.o: CMakeFiles/lineards.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir="/g/Meu Drive/1. Ensino/Materiais/lineards/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lineards.dir/lineards.c.o"
	/mingw64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/lineards.dir/lineards.c.o -MF CMakeFiles/lineards.dir/lineards.c.o.d -o CMakeFiles/lineards.dir/lineards.c.o -c "/g/Meu Drive/1. Ensino/Materiais/lineards/src/lineards.c"

CMakeFiles/lineards.dir/lineards.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/lineards.dir/lineards.c.i"
	/mingw64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/g/Meu Drive/1. Ensino/Materiais/lineards/src/lineards.c" > CMakeFiles/lineards.dir/lineards.c.i

CMakeFiles/lineards.dir/lineards.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/lineards.dir/lineards.c.s"
	/mingw64/bin/cc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/g/Meu Drive/1. Ensino/Materiais/lineards/src/lineards.c" -o CMakeFiles/lineards.dir/lineards.c.s

# Object files for target lineards
lineards_OBJECTS = \
"CMakeFiles/lineards.dir/lineards.c.o"

# External object files for target lineards
lineards_EXTERNAL_OBJECTS =

liblineards.a: CMakeFiles/lineards.dir/lineards.c.o
liblineards.a: CMakeFiles/lineards.dir/build.make
liblineards.a: CMakeFiles/lineards.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir="/g/Meu Drive/1. Ensino/Materiais/lineards/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library liblineards.a"
	$(CMAKE_COMMAND) -P CMakeFiles/lineards.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lineards.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lineards.dir/build: liblineards.a
.PHONY : CMakeFiles/lineards.dir/build

CMakeFiles/lineards.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lineards.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lineards.dir/clean

CMakeFiles/lineards.dir/depend:
	cd "/g/Meu Drive/1. Ensino/Materiais/lineards/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/g/Meu Drive/1. Ensino/Materiais/lineards/src" "/g/Meu Drive/1. Ensino/Materiais/lineards/src" "/g/Meu Drive/1. Ensino/Materiais/lineards/build" "/g/Meu Drive/1. Ensino/Materiais/lineards/build" "/g/Meu Drive/1. Ensino/Materiais/lineards/build/CMakeFiles/lineards.dir/DependInfo.cmake" "--color=$(COLOR)"
.PHONY : CMakeFiles/lineards.dir/depend

