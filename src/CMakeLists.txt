set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/camera_calibration.hpp")

# set(SOURCE_FILES camera_calibration.cpp ${HEADER_LIST})

add_library(CamCalibLib SHARED STATIC camera_calibration.cpp ${HEADER_LIST})
target_link_libraries(CamCalibLib m ${OpenCV_LIBS} Eigen3::Eigen)

install(TARGETS CamCalibLib DESTINATION ${INSTALL_LIB_DIR})
# install(FILES ${HEADER_LIST} DESTINATION ${INSTALL_INCLUDE_DIR})

