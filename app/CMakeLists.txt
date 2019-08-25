
# set(SOURCE_FILES Calibrateapp.cpp)

add_executable(CalibrateImages Calibrateapp.cpp)
target_link_libraries(CalibrateImages CamCalibLib)
target_link_libraries(CalibrateImages m ${OpenCV_LIBS})
install(TARGETS CalibrateImages DESTINATION ${INSTALL_BIN_DIR})