cmake_minimum_required (VERSION 2.8.7)
project (fft_plot)
# The version number.
include(GNUInstallDirs)
set (fft_plot_VERSION_MAJOR 0)
set (fft_plot_VERSION_MINOR 1)
set (CMAKE_C_STANDARD 99)
set (CMAKE_BUILD_TYPE Debug)
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
find_library(FFTW_LIBRARIES NAMES fftw fftw3 libfftw libfftw3)
find_library(IIO_LIBRARIES NAMES iio libiio )
find_path(IIO_INCLUDE_DIRS iio.h PATHS)

include_directories(
	${IIO_INCLUDE_DIRS}
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

# add the executable
add_executable(fft_plot main.c basic_graph.c)

target_link_libraries( fft_plot
	-lm
	${IIO_LIBRARIES}
	fftw3
)

install(TARGETS fft_plot RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
