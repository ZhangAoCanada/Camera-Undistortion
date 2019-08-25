#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace JAIcam
{
    class CalibrateCam
    {
        vector<vector<Point3f>> object_points;
        vector<vector<Point2f>> image_points;
        vector<Point2f> corners;

        Mat cameraMatrix, distCoeffs;
        Mat img, gray;
        Size img_size;

        int flag;

    public:
        CalibrateCam() : flag(0) {};

        void ReadParameters(String  filename, int *board_width, 
                            int *board_height, float *square_size);
        void LoadImageAndFindChessboard(int board_width, int board_height, 
                            float square_size, int num_imgs, String img_dir, 
                            vector<String> all_images) ;
        double CalibrateCamera();
        void ProcessingCalibration();
    };

    class DoCalibration
    {
        string reading_directory, saving_directory;
        Mat cameraMatrix, distCoeffs;
        bool Eligible;
    public:
        DoCalibration(string filename1, string filename2) : reading_directory(filename1), saving_directory(filename2) {};

        void ReadCalibrationParameters();
        bool CheckParaEligible();
        void CalibrateImageDir();
    };
}