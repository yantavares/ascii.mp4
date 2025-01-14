#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <pthread.h>
#include <queue>
#include <condition_variable>
#include <unistd.h>
#include <atomic>
#include <sys/ioctl.h>
#include <string>
#include <cmath>
#include <iomanip>
#include <limits>
#include <filesystem>

#define BUFFER_SIZE 2000

struct FrameBuffer
{
    std::queue<std::vector<std::string>> buffer;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
};

struct WorkerArgs
{
    const cv::Mat *frame;
    const std::map<char, cv::Mat> *font_images;
    int font_size;
    int start_row;
    int end_row;
    std::vector<std::string> *characters_grid;
};

FrameBuffer frame_buffer;
std::atomic<bool> done(false);
std::atomic<int> processed_frames(0);

std::string formatNumber(int num, int length)
{
    std::ostringstream oss;
    oss << std::setw(length) << std::setfill('0') << num;
    return oss.str();
}

std::map<char, cv::Mat> load_font_images(const std::string &font_dir)
{
    std::map<char, cv::Mat> font_images;
    for (const auto &entry : std::filesystem::directory_iterator(font_dir))
    {
        if (entry.path().extension() == ".png")
        {
            std::string filename = entry.path().stem().string();
            int char_code;
            try
            {
                char_code = std::stoi(filename);
            }
            catch (const std::invalid_argument &ia)
            {
                std::cerr << "Invalid argument: " << ia.what() << '\n';
                continue;
            }
            catch (const std::out_of_range &oor)
            {
                std::cerr << "Out of Range error: " << oor.what() << '\n';
                continue;
            }

            if (char_code < 0 || char_code > 255)
            {
                std::cerr << "Character code out of valid range: " << char_code << '\n';
                continue;
            }

            char char_code_char = static_cast<char>(char_code);
            cv::Mat img = cv::imread(entry.path(), cv::IMREAD_GRAYSCALE);
            if (img.empty())
            {
                std::cerr << "Failed to load image for char code " << char_code_char << " at path " << entry.path() << std::endl;
                continue;
            }

            font_images[char_code_char] = img;
        }
    }
    return font_images;
}

char compare_matrices(const cv::Mat &segment, const std::map<char, cv::Mat> &font_images)
{
    double min_distance = std::numeric_limits<double>::max();
    char best_match_char = 0;

    for (const auto &[char_code, font_image] : font_images)
    {
        if (!segment.empty() && !font_image.empty() && segment.type() == font_image.type() && segment.size() == font_image.size())
        {
            cv::bitwise_not(segment, segment); // Not sure why, but this works better

            double distance = cv::norm(segment, font_image, cv::NORM_L2SQR);

            if (distance < min_distance)
            {
                min_distance = distance;
                best_match_char = char_code;
            }
        }
        else
        {
            std::cerr << "Incompatible or empty images for char " << char_code << std::endl;
        }
    }
    if (best_match_char <= 0 || best_match_char > 127)
    {
        best_match_char = '?';
    }
    return best_match_char;
}

void *worker_thread(void *args)
{
    WorkerArgs *worker_args = static_cast<WorkerArgs *>(args);
    const cv::Mat *frame = worker_args->frame;
    const std::map<char, cv::Mat> *font_images = worker_args->font_images;
    int font_size = worker_args->font_size;
    int start_row = worker_args->start_row;
    int end_row = worker_args->end_row;
    std::vector<std::string> *characters_grid = worker_args->characters_grid;

    for (int j = start_row; j < end_row; j += font_size)
    {
        std::string row_chars;
        for (int i = 0; i <= frame->cols - font_size; i += font_size)
        {
            cv::Rect region(i, j, font_size, font_size);
            cv::Mat segment = (*frame)(region);
            char best_match_char = compare_matrices(segment, *font_images);
            row_chars += best_match_char;
        }
        (*characters_grid)[j / font_size] = row_chars;
    }
    return nullptr;
}

std::pair<int, int> get_terminal_size()
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
    {
        return {w.ws_col, w.ws_row};
    }
    else
    {
        return {80, 24};
    }
}
void *consumer_thread(void *args)
{
    int terminal_width, terminal_height;
    std::tie(terminal_width, terminal_height) = get_terminal_size();

    while (!done || !frame_buffer.buffer.empty())
    {
        pthread_mutex_lock(&frame_buffer.mutex);

        while (frame_buffer.buffer.empty() && !done)
        {
            pthread_cond_wait(&frame_buffer.not_empty, &frame_buffer.mutex);
        }

        if (!frame_buffer.buffer.empty())
        {
            // Verifique o tamanho do buffer e pause o consumidor se necessário
            if (frame_buffer.buffer.size() < 10 && !done) // Buffer quase vazio
            {
                pthread_mutex_unlock(&frame_buffer.mutex);
                usleep(50000); // Aguarde 50ms para o produtor reabastecer
                pthread_mutex_lock(&frame_buffer.mutex);
            }

            std::vector<std::string> frame_chars = frame_buffer.buffer.front();
            frame_buffer.buffer.pop();
            pthread_cond_signal(&frame_buffer.not_full);
            pthread_mutex_unlock(&frame_buffer.mutex);

            for (const auto &row : frame_chars)
            {
                std::cout << row << std::endl;
            }
            std::cout << "\033[H\033[J"; // Clear terminal
        }
        else
        {
            pthread_mutex_unlock(&frame_buffer.mutex);
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    std::string video = "videos/SampleVideo.mp4";
    std::string font_dir = "fonts/ComicMono_chars";
    int font_size = 11;

    cv::VideoCapture cap(video);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file" << std::endl;
        return -1;
    }

    frame_buffer = {std::queue<std::vector<std::string>>(), PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};

    std::map<char, cv::Mat> font_images = load_font_images(font_dir);
    int terminal_width, terminal_height;
    std::tie(terminal_width, terminal_height) = get_terminal_size();

    pthread_t consumer;
    pthread_create(&consumer, nullptr, consumer_thread, nullptr);

    cv::Mat frame;
    int frame_count = 0;

    while (cap.read(frame))
    {
        cv::Mat gray_frame;
        cv::resize(frame, gray_frame, cv::Size(terminal_width * font_size, terminal_height * font_size));
        cv::cvtColor(gray_frame, gray_frame, cv::COLOR_BGR2GRAY);

        std::vector<std::string> characters_grid(terminal_height);

        pthread_t workers[4];
        WorkerArgs args[4];

        for (int i = 0; i < 4; ++i)
        {
            args[i] = {&gray_frame, &font_images, font_size, i * (terminal_height / 4) * font_size, (i + 1) * (terminal_height / 4) * font_size, &characters_grid};
            pthread_create(&workers[i], nullptr, worker_thread, &args[i]);
        }

        for (int i = 0; i < 4; ++i)
        {
            pthread_join(workers[i], nullptr);
        }

        pthread_mutex_lock(&frame_buffer.mutex);

        while (frame_buffer.buffer.size() >= BUFFER_SIZE)
        {
            pthread_cond_wait(&frame_buffer.not_full, &frame_buffer.mutex);
        }

        // Adicione ao buffer
        frame_buffer.buffer.push(characters_grid);
        pthread_cond_signal(&frame_buffer.not_empty);
        pthread_mutex_unlock(&frame_buffer.mutex);

        ++frame_count;
    }

    done = true;
    pthread_cond_signal(&frame_buffer.not_empty);

    pthread_join(consumer, nullptr);
    cap.release();

    std::cout << "Processed " << frame_count << " frames." << std::endl;
    return 0;
}