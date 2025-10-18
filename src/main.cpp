// src/main.cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

void print_usage(const char* prog) {
    std::cout << "Usage: " << prog << " <input> <output> [options]\n"
              << "Options:\n"
              << "  --gray    Convert to grayscale\n"
              << "  --blur    Use Gaussian blur\n"
              << "  --canny    Use Canny edge detection\n"
              << "  --rotate 90    Use image 90angle rotation\n"
              << "  --rotate 180    Use image 180angle rotation\n"
              << "  --rotate 270    Use image 270angle rotation\n"
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        print_usage(argv[0]);
        return -1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    cv::Mat img = cv::imread(input_path);
    if (img.empty()) {
        std::cerr << "Error: Could not load image " << input_path << std::endl;
        return -1;
    }

    cv::Mat result = img.clone();

    // Parse options
    for (int i = 3; i < argc; ++i) {
        std::string opt = argv[i];
        if (opt == "--gray") {
            if (result.channels() == 3) {
                cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            }
        }else if (opt == "--blur") {
            cv::GaussianBlur(result, result, cv::Size(15, 15), 0);
        }else if (opt == "--canny") {
            // Canny 边缘检测通常需要灰度图，先确保图像为单通道
            if (result.channels() == 3) {
                cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
            }
            // 应用 Canny 算法（阈值可根据实际需求调整）
            cv::Canny(result, result, 30, 90);
        }else if (opt == "--rotate") {
            int angle;
            std::string next_opt= argv[i+1];
            angle = std::stoi(next_opt);
            

            // 检查角度是否合法
            if (angle != 90 && angle != 180 && angle != 270) {
                std::cerr << "错误：仅支持 90°、180°、270° 旋转" << std::endl;
                return 1;
            }

            // 根据角度执行旋转
            cv::Mat rotated;
            switch (angle) {
                case 90:
                    // 顺时针旋转90度（OpenCV默认旋转方向）
                    cv::rotate(result, result, cv::ROTATE_90_CLOCKWISE);
                    break;
                case 180:
                    cv::rotate(result, result, cv::ROTATE_180);
                    break;
                case 270:
                    // 顺时针旋转270度 = 逆时针旋转90度
                    cv::rotate(result, result, cv::ROTATE_90_COUNTERCLOCKWISE);
                    break;
            }

            i++;

        }else {
            std::cerr << "Unknown option: " << opt << std::endl;
            print_usage(argv[0]);
            return -1;
        }
    }

    if (!cv::imwrite(output_path, result)) {
        std::cerr << "Error: Could not save image to " << output_path << std::endl;
        return -1;
    }

    std::cout << "Success! Saved to " << output_path << std::endl;
    return 0;
}
