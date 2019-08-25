#include "camera_calibration.hpp"
#include <stdlib.h>

namespace JAIcam
{
    void CalibrateCam::ReadParameters(
        String  filename, int *board_width, 
        int *board_height, float *square_size
    ) 
    {
        /***********************************************************************
         * Args:
         *      board_width         ->      how many grids in the width of chessboard
         *      board_height        ->      how many grids in the height of chessboard
         *      square_size         ->      size of each square of chessboard
         * 
         * Function:
         *      Read the parameters configure file, and set the values to the
         *      corresponding variables.
        ************************************************************************/ 

        // define necessary variables
        ifstream parameters;
        string content;
        string parameter_name;
        int space_len;
        int tmp1, tmp2;
        float tmp3;

        // open the configure file
        parameters.open(filename);
        if (parameters.is_open()) {
            // get txt file line by line
            while ( getline(parameters, content) ) {
                // split the lines to 2 parts, saparated by the first space
                space_len = content.find(" ");
                parameter_name = content.substr(0, space_len);
                content.erase(0, space_len + 1);
                // assign values
                if (parameter_name == "board_width") {
                    tmp1 = stoi(content); 
                    *board_width = tmp1;
                } else if (parameter_name == "board_height") {
                    tmp2 = stoi(content);                 
                    *board_height = tmp2;
                } else if (parameter_name == "square_size") {
                    tmp3 = stof(content); 
                    *square_size = tmp3;
                }
            }
            parameters.close();
        } else {
            cerr << "Please check the directory or the content of the file." << endl;
            terminate();
        }
    }

    void CalibrateCam::LoadImageAndFindChessboard(
        int board_width, int board_height, float square_size,
        int num_imgs, String img_dir, vector<String> all_images
    ) 
    {
        /***********************************************************************
         * Args:
         *      board_width         ->      how many grids in the width of chessboard
         *      board_height        ->      how many grids in the height of chessboard
         *      square_size         ->      size of each square of chessboard
         *      num_imgs            ->      how many images for camera calibration
         *      img_dir             ->      image directory
         * 
         * Functions:
         *      find the chess board corners of the calibration images, and use it
         *      for calculating camera matrice.
        ************************************************************************/ 

        // variables for debug (could be deleted in the future)
        // Size display_size = Size(600, 600);
        // Mat current_img_display;

        // declare necessary variables
        Size board_size = Size(board_width, board_height);
        int board_n = board_width * board_height;
        bool found;

        // find the chessboard (and plot it)
        for (int i = 0; i < num_imgs; i++) {
            img = imread(all_images[i], IMREAD_COLOR);
            cvtColor(img, gray, CV_BGR2GRAY);
            img_size = gray.size();

            found = false;

            vector<Point3f> obj;
            for( int j = 0; j < board_height; j++ ) {
                for( int k = 0; k < board_width; k++ ) {
                    obj.push_back(Point3f(double( (float)k * square_size ), double( (float)j * square_size ), 0));                
                }
            }

            found = findChessboardCorners(img, board_size, corners, 
                        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
            if (found) {
                cornerSubPix(gray, corners, Size(5, 5), Size(-1, -1), 
                        TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
                drawChessboardCorners(img, board_size, corners, found);
                image_points.push_back(corners);
                object_points.push_back(obj);
            }
            // resize(img, current_img_display, display_size);
            // imshow("aaa", current_img_display);
            // waitKey(1000);
        }
    }

    double CalibrateCam::CalibrateCamera() 
    {
        /***********************************************************************
         * Args:
         *      cameraMatrix        ->      to get the camera matrix after function
         *      distCoeffs          ->      to get the distortion coefficient after function
         *      img_size            ->      necessary parameter for the calibration
         * 
         * Functions:
         *      calibrate camera and return camera matrix and distortion coefficient.
        ************************************************************************/ 
        vector<Mat> rvecs, tvecs;
        double re_proj;

        Mat map1, map2;
        FileStorage fs("../calibration_para_saved/libration_para.yml", FileStorage::WRITE);

        re_proj = calibrateCamera(object_points, image_points, img_size, 
                                    cameraMatrix, distCoeffs, rvecs, tvecs, flag);

        fs << "cameraMatrix" << cameraMatrix;
        fs << "distCoeffs" << distCoeffs;
        fs.release();

        return re_proj;
    }

    void CalibrateCam::ProcessingCalibration() {
        /***********************************************************************
         * Functions:
         *      Go through the process of calibration and save the parameters
        ************************************************************************/ 

        // declare variables for finding chessboard
        int board_width;
        int board_height;
        float square_size;
        int num_imgs;
        double re_projection_err;
        String config_file = "../config/parameters.txt";
        // declare variables for iamge directory
        String img_dir = "../calibration_image/";
        vector<String> all_images;

        // read configure file and assign values
        CalibrateCam::ReadParameters(config_file, &board_width, &board_height, &square_size);

        // read all images in the image direcotry
        glob(img_dir, all_images);
        num_imgs = all_images.size();

        // test function
        CalibrateCam::LoadImageAndFindChessboard(board_width, board_height, square_size,
                                    num_imgs, img_dir, all_images);

        re_projection_err = CalibrateCam::CalibrateCamera();
    }

    void DoCalibration::ReadCalibrationParameters()
    {
        /***********************************************************************
         * Args:
         *      parameters_filename ->      yml file for reading the cameraMatrix and distCoeffs
         *      cameraMatrix        ->      camera matrix 
         *      distCoeffs          ->      distortion coefficient 
         * 
         * Functions:
         *      Read the camera matrix and distortion coefficients from yml file.
        ************************************************************************/ 
        // read parameters
        FileStorage read_parameters("../calibration_para_saved/libration_para.yml", FileStorage::READ);

        read_parameters["cameraMatrix"] >> cameraMatrix;
        read_parameters["distCoeffs"] >> distCoeffs;
    }

    bool DoCalibration::CheckParaEligible()
    {
        /***********************************************************************
         * Args:
         *      Eligible     ->      whether there are values in cameraMatrix and distCoeffs
         * 
         * Functions:
         *      Check the values of cameraMatrix and distCoeffs.
        ************************************************************************/ 
        int cameraMatrix_size, distCoeffs_size;

        DoCalibration::ReadCalibrationParameters();

        cameraMatrix_size = cameraMatrix.size().width;
        distCoeffs_size = distCoeffs.size().width;

        if (cameraMatrix_size == 0 || distCoeffs_size == 0) {
            Eligible = false;
        } else {
            Eligible = true;
        }

        return Eligible;
    }

    void DoCalibration::CalibrateImageDir() 
    {
        /***********************************************************************
         * Args:
         *      ImagesDirectory     ->      directory for images to be calibrated
         *      cameraMatrix        ->      camera matrix 
         *      distCoeffs          ->      distortion coefficient 
         * 
         * Functions:
         *      read all images from directory and calibration them all then save them.
        ************************************************************************/ 
        vector<String> all_images;
        Mat img, img_undist;
        int num_imgs;
        string saving_image;

        DoCalibration::ReadCalibrationParameters();

        glob(reading_directory, all_images);
        num_imgs = all_images.size();

        if (num_imgs == 0) {
            cerr << "Please check your input and output directory." << endl;
            terminate();
        }

        for (int i = 0; i < num_imgs; i++) {
            img = imread(all_images[i], IMREAD_COLOR);
            undistort(img, img_undist, cameraMatrix, distCoeffs);
            saving_image = saving_directory + "/" + to_string(i) + ".jpg";
            imwrite(saving_image, img_undist);
        }

    }
}