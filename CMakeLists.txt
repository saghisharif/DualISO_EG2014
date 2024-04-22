CMAKE_MINIMUM_REQUIRED(VERSION 2.8.8)
PROJECT(dualISO)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

FIND_PACKAGE (OpenEXR REQUIRED)
INCLUDE_DIRECTORIES(${OpenEXR_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenEXR_LIBRARY_DIRS})
SET( OPENEXR_LIBRARIES Half IlmImf Iex Imath IlmThread)

INCLUDE_DIRECTORIES(/usr/include/eigen3)

find_package( OpenCV REQUIRED )
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
LINK_DIRECTORIES(${OpenCV_LIBRARY_DIRS})


add_subdirectory(image/VclExr)
add_subdirectory(opensource/rawspeed)
add_subdirectory(image/VclRaw)
add_subdirectory(util/VclMatlab)
add_subdirectory(opensource/arg)

set(SOURCEFILES main.cpp VclBayerPatterns.cpp VclKernelRegression.cpp VclShadingCorrection.cpp)
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

ADD_EXECUTABLE(dualISO ${SOURCEFILES})
target_link_libraries(dualISO ${OPENEXR_LIBRARIES} arg VclExr VclRaw exiv2 jpeg rawspeed VclMatlab hdf5 matio opencv_core opencv_calib3d opencv_imgproc opencv_highgui)



