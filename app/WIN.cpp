#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>

// Characteres usados no vídeo
const std::string ASCII_CHARS = "@#W$21abc?!;:+=-,._ ";

char brightness_to_ascii(unsigned char brightness)
{
    int index = (brightness * (ASCII_CHARS.size() - 1)) / 255;
    return ASCII_CHARS[index];
}

void process_and_display_frame(const cv::Mat &frame, int output_width, int output_height)
{
    cv::Mat resized_frame;
    cv::resize(frame, resized_frame, cv::Size(output_width, output_height));

    cv::Mat gray_frame;
    cv::cvtColor(resized_frame, gray_frame, cv::COLOR_BGR2GRAY);

    cv::Mat inverted_frame;
    cv::bitwise_not(gray_frame, inverted_frame);

    std::system("cls");

    for (int y = 0; y < inverted_frame.rows; ++y)
    {
        for (int x = 0; x < inverted_frame.cols; ++x)
        {
            unsigned char brightness = inverted_frame.at<unsigned char>(y, x);
            std::cout << brightness_to_ascii(brightness);
        }
        std::cout << std::endl;
    }
}

std::pair<int, int> get_terminal_size()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {
        int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return {columns, rows};
    }
    else
    {
        std::cerr << "Unable to detect terminal size. Defaulting to 80x24." << std::endl;
        return {80, 24};
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: " << argv[0] << " <Caminho_do_arquivo>" << std::endl;
        return 1;
    }

    std::string video_path = argv[1];

    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file: " << video_path << std::endl;
        return -1;
    }

    auto [terminal_width, terminal_height] = get_terminal_size();

    const double CHARACTER_ASPECT_RATIO = 1;
    int output_width = terminal_width;
    int output_height = static_cast<int>(terminal_height * CHARACTER_ASPECT_RATIO);

    double fps = cap.get(cv::CAP_PROP_FPS);
    int frame_delay = static_cast<int>(1000 / fps);

    cv::Mat frame;
    while (cap.read(frame))
    {
        process_and_display_frame(frame, output_width, output_height);

        std::this_thread::sleep_for(std::chrono::milliseconds(frame_delay));
    }

    cap.release();
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Video playback completed." << std::endl;

    return 0;
}
