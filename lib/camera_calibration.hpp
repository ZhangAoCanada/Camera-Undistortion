#include <vector>
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

namespace JAIcam
{
    class CalibrateCam
    {
        std::vector<std::vector<cv::Point3f>> object_points;
        std::vector<std::vector<cv::Point2f>> image_points;
        std::vector<cv::Point2f> corners;

        cv::Mat cameraMatrix, distCoeffs;
        cv::Mat img, gray;
        cv::Size img_size;

        int flag;

    public:
        CalibrateCam() : flag(0) {};

        void ReadParameters(String  filename, int *board_width, 
                            int *board_height, float *square_size);
        void LoadImageAndFindChessboard(int board_width, int board_height, 
                            float square_size, int num_imgs, String img_dir, 
                            vector<String> all_images) ;
        double CalibrateCamera();
        void Processing_Calibration();
    };
}