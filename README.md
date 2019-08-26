# Pinhole Camera Calibration
This library is working specifically for undistorting images that taken from a pinhole camera.

## experiment result
<p align="center">
  <img src="testimages/input/tstimg8.jpg" width="350" height="350"> <img src="testimages/output/0.jpg" width="350" height="350">
</p>

## How to use it
### 1. Git clone it to local
```
$cd <directory>
$git clone https://github.com/ZhangAoCanada/Camera-Undistortion
```

### 2. Put all calibration images into the right directory
```
$./calibration_image/
```

### 3. Remove the directory "build"
```
$sudo rm -r ./build
```

### 4. Open the configure file and do the changes if neccesary
```
$nano ./config/parameters.txt
```
Note: change the diretory of the images that are about to be undistorted correctly. Also, please specify the output directory.

### 5. Build the whole project
```
$mkdir build
$cd build
$cmake ..
$make && make install
```

### 6. Run calibration
```
$cd bin
```
If you are running it first time:
```
$./CalibrateImages recalibrate
```
If not
```
$./CalibrateImages
```

### Optional varifying step
After first time running it,go to the directory "calibration_para_saved" to find out the cameraMatrix and distCoeffs have been written in "libration_para.yml".


