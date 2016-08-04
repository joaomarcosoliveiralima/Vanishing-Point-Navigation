##############################
PROJECT( FFME )
##############################
cmake_minimum_required (VERSION 2.6)

# Packages
find_package( OpenCV REQUIRED )

# Includes
include_directories(	
	${OPENCV_INCLUDE_DIR}
	${PROJECT_BINARY_DIR}
)

set(SOURCE_FILES 
	main.cpp
	main_func.cpp
	main_func.h
)


# Add executable and target link libraries
ADD_EXECUTABLE( FFME ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( FFME ${OpenCV_LIBS})

