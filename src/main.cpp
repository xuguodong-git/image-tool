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
