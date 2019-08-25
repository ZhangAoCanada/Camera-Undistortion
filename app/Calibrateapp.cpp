#include <camera_calibration.hpp>
#include <iostream>

using namespace cv;

void GetImagesDirectory(
    string *filename, string *IMG_direcotry, string *result_directory
)
{
    ifstream parameters;
    string content;
    string parameter_name;
    string tmp1, tmp2;
    int space_len;
    
    // open the configure file
    parameters.open(*filename);
    if (parameters.is_open()) {
        // get txt file line by line
        while ( getline(parameters, content) ) {
            // split the lines to 2 parts, saparated by the first space
            space_len = content.find(" ");
            parameter_name = content.substr(0, space_len);
            content.erase(0, space_len + 1);
            // assign values
            if (parameter_name == "image_directory") {
                tmp1 = content;
                *IMG_direcotry = tmp1;
            } else if (parameter_name == "output_directory") {
                tmp2 = content;
                *result_directory = tmp2;
            }
        }
        parameters.close();
    } else {
        cerr << "Please check the directory or the content of the file." << endl;
        terminate();
    }
}

int main(int argc, char ** argv) {
    JAIcam::CalibrateCam calibration_process;
    string config_file = "../config/parameters.txt";
    string IMG_direcotry, result_directory;
    int size1, size2;
    string first_input;
    bool recalibrate_input = false;
    bool check_out;

    GetImagesDirectory(&config_file, &IMG_direcotry, &result_directory);
    size1 = IMG_direcotry.size();
    size2 = result_directory.size();

    if (size1 == 0 || size2 == 0) {
        cerr << "Please check your input and output directory." << endl;
        terminate();
    }

    JAIcam::DoCalibration calibrate_all_images(IMG_direcotry, result_directory);

    check_out = calibrate_all_images.CheckParaEligible();

    if (argc >=2) {
        first_input = argv[1];
        if (first_input == "recalibrate") {
            recalibrate_input = true;
        } else {
            recalibrate_input = false;
        }
    }

    if (!check_out || recalibrate_input) {
        calibration_process.ProcessingCalibration();
    }

    calibrate_all_images.CalibrateImageDir();

    return 0;
}