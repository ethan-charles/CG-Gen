# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.30

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\xiaol\Desktop\CG-Gen\HW2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\xiaol\Desktop\CG-Gen\build

# Include any dependencies generated for this target.
include CMakeFiles/RayTracing.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/RayTracing.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/RayTracing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/RayTracing.dir/flags.make

CMakeFiles/RayTracing.dir/main.cpp.obj: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/main.cpp.obj: C:/Users/xiaol/Desktop/CG-Gen/HW2/main.cpp
CMakeFiles/RayTracing.dir/main.cpp.obj: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/RayTracing.dir/main.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/main.cpp.obj -MF CMakeFiles\RayTracing.dir\main.cpp.obj.d -o CMakeFiles\RayTracing.dir\main.cpp.obj -c C:\Users\xiaol\Desktop\CG-Gen\HW2\main.cpp

CMakeFiles/RayTracing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/main.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaol\Desktop\CG-Gen\HW2\main.cpp > CMakeFiles\RayTracing.dir\main.cpp.i

CMakeFiles/RayTracing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/main.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaol\Desktop\CG-Gen\HW2\main.cpp -o CMakeFiles\RayTracing.dir\main.cpp.s

CMakeFiles/RayTracing.dir/Scene.cpp.obj: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Scene.cpp.obj: C:/Users/xiaol/Desktop/CG-Gen/HW2/Scene.cpp
CMakeFiles/RayTracing.dir/Scene.cpp.obj: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/RayTracing.dir/Scene.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Scene.cpp.obj -MF CMakeFiles\RayTracing.dir\Scene.cpp.obj.d -o CMakeFiles\RayTracing.dir\Scene.cpp.obj -c C:\Users\xiaol\Desktop\CG-Gen\HW2\Scene.cpp

CMakeFiles/RayTracing.dir/Scene.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Scene.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaol\Desktop\CG-Gen\HW2\Scene.cpp > CMakeFiles\RayTracing.dir\Scene.cpp.i

CMakeFiles/RayTracing.dir/Scene.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Scene.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaol\Desktop\CG-Gen\HW2\Scene.cpp -o CMakeFiles\RayTracing.dir\Scene.cpp.s

CMakeFiles/RayTracing.dir/Accel.cpp.obj: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Accel.cpp.obj: C:/Users/xiaol/Desktop/CG-Gen/HW2/Accel.cpp
CMakeFiles/RayTracing.dir/Accel.cpp.obj: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/RayTracing.dir/Accel.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Accel.cpp.obj -MF CMakeFiles\RayTracing.dir\Accel.cpp.obj.d -o CMakeFiles\RayTracing.dir\Accel.cpp.obj -c C:\Users\xiaol\Desktop\CG-Gen\HW2\Accel.cpp

CMakeFiles/RayTracing.dir/Accel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Accel.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaol\Desktop\CG-Gen\HW2\Accel.cpp > CMakeFiles\RayTracing.dir\Accel.cpp.i

CMakeFiles/RayTracing.dir/Accel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Accel.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaol\Desktop\CG-Gen\HW2\Accel.cpp -o CMakeFiles\RayTracing.dir\Accel.cpp.s

CMakeFiles/RayTracing.dir/Math.cpp.obj: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Math.cpp.obj: C:/Users/xiaol/Desktop/CG-Gen/HW2/Math.cpp
CMakeFiles/RayTracing.dir/Math.cpp.obj: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/RayTracing.dir/Math.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Math.cpp.obj -MF CMakeFiles\RayTracing.dir\Math.cpp.obj.d -o CMakeFiles\RayTracing.dir\Math.cpp.obj -c C:\Users\xiaol\Desktop\CG-Gen\HW2\Math.cpp

CMakeFiles/RayTracing.dir/Math.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Math.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaol\Desktop\CG-Gen\HW2\Math.cpp > CMakeFiles\RayTracing.dir\Math.cpp.i

CMakeFiles/RayTracing.dir/Math.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Math.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaol\Desktop\CG-Gen\HW2\Math.cpp -o CMakeFiles\RayTracing.dir\Math.cpp.s

CMakeFiles/RayTracing.dir/Ray.cpp.obj: CMakeFiles/RayTracing.dir/flags.make
CMakeFiles/RayTracing.dir/Ray.cpp.obj: C:/Users/xiaol/Desktop/CG-Gen/HW2/Ray.cpp
CMakeFiles/RayTracing.dir/Ray.cpp.obj: CMakeFiles/RayTracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/RayTracing.dir/Ray.cpp.obj"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/RayTracing.dir/Ray.cpp.obj -MF CMakeFiles\RayTracing.dir\Ray.cpp.obj.d -o CMakeFiles\RayTracing.dir\Ray.cpp.obj -c C:\Users\xiaol\Desktop\CG-Gen\HW2\Ray.cpp

CMakeFiles/RayTracing.dir/Ray.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/RayTracing.dir/Ray.cpp.i"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\xiaol\Desktop\CG-Gen\HW2\Ray.cpp > CMakeFiles\RayTracing.dir\Ray.cpp.i

CMakeFiles/RayTracing.dir/Ray.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/RayTracing.dir/Ray.cpp.s"
	C:\PROGRA~1\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\xiaol\Desktop\CG-Gen\HW2\Ray.cpp -o CMakeFiles\RayTracing.dir\Ray.cpp.s

# Object files for target RayTracing
RayTracing_OBJECTS = \
"CMakeFiles/RayTracing.dir/main.cpp.obj" \
"CMakeFiles/RayTracing.dir/Scene.cpp.obj" \
"CMakeFiles/RayTracing.dir/Accel.cpp.obj" \
"CMakeFiles/RayTracing.dir/Math.cpp.obj" \
"CMakeFiles/RayTracing.dir/Ray.cpp.obj"

# External object files for target RayTracing
RayTracing_EXTERNAL_OBJECTS =

RayTracing.exe: CMakeFiles/RayTracing.dir/main.cpp.obj
RayTracing.exe: CMakeFiles/RayTracing.dir/Scene.cpp.obj
RayTracing.exe: CMakeFiles/RayTracing.dir/Accel.cpp.obj
RayTracing.exe: CMakeFiles/RayTracing.dir/Math.cpp.obj
RayTracing.exe: CMakeFiles/RayTracing.dir/Ray.cpp.obj
RayTracing.exe: CMakeFiles/RayTracing.dir/build.make
RayTracing.exe: CMakeFiles/RayTracing.dir/linkLibs.rsp
RayTracing.exe: CMakeFiles/RayTracing.dir/objects1.rsp
RayTracing.exe: CMakeFiles/RayTracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable RayTracing.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\RayTracing.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/RayTracing.dir/build: RayTracing.exe
.PHONY : CMakeFiles/RayTracing.dir/build

CMakeFiles/RayTracing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\RayTracing.dir\cmake_clean.cmake
.PHONY : CMakeFiles/RayTracing.dir/clean

CMakeFiles/RayTracing.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\xiaol\Desktop\CG-Gen\HW2 C:\Users\xiaol\Desktop\CG-Gen\HW2 C:\Users\xiaol\Desktop\CG-Gen\build C:\Users\xiaol\Desktop\CG-Gen\build C:\Users\xiaol\Desktop\CG-Gen\build\CMakeFiles\RayTracing.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/RayTracing.dir/depend

