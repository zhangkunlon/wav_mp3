#include <iostream>
#include <filesystem>
#include <string.h>
#include <getopt.h>
#include <atomic>
#include <thread>
#include <chrono>

extern "C"
{
    #include "wav2mp3.h"
}

namespace fs = std::filesystem;

std::atomic<int> g_is_running = 0;
/**
 * @brief  wav转mp3
 * @param  input  输入文件名
 * @param  output 输出文件名
 * @return 0:成功，-1:失败
 * @note   wav需要采样率8k，双声道
*/
int convert(std::string infname, std::string outfname)
{

    char* in = new char[infname.size() + 1];
    char* out = new char[outfname.size() + 1];
    memccpy(in, infname.c_str(), '\0', infname.size() + 1);
    memccpy(out, outfname.c_str(), '\0', outfname.size() + 1);
    
    // strcpy(in, infname.c_str());
    // strcpy(out, outfname.c_str());
    int ret = wav2mp3(in, out);

    delete[] in;
    delete[] out;
    --g_is_running;
    return ret;
}




int main(int argc, char** argv)
{
    // 记录程序开始时间
    time_t start_time = time(NULL);


    // 命令行参数
    std::string input_dir;
    std::string output_dir;
    int thread_num = 1;
    int ch;
    for (; (ch = getopt(argc, argv, "n:i:o:h")) != -1;) {
        switch (ch) {
        case 'o':
            output_dir = optarg;
            break;
        case 'i':
            input_dir = optarg;
            break;
        case 'n':
            thread_num = atoi(optarg);
            break;
        case 'h':
            printf(
                "Usage: %s [OPTION]... FILE\n"
                "Input Dirctory wav -> mp3.\n"
                "for example: ./wav2mp3 -o OutputDir -i InputDir\n"
                "Options:\n"
                "  -o FILE \n"
                "  -i FILE \n"
                "  -n thread_num \n"
                "\n"
                "  -h display this help and exit\n",
                argv[0]);
            break;
        default: 
            printf("Unknown option: %c\n", optopt);
            printf("Try '%s -h' for more information.\n", argv[0]);
            return 0;
        }
    }
    
    // 遍历文件夹内的文件，将wav转换为MP3
    int count = 0;
    if (fs::exists(input_dir) && fs::is_directory(input_dir) && 
        fs::exists(output_dir) && fs::is_directory(output_dir)) {
        for (const auto& entry : fs::directory_iterator(input_dir)) {
            if (entry.is_directory()) {
                std::cout << "Folder: " << entry.path().filename() << std::endl;
            }
            else if (entry.is_regular_file() && entry.path().filename().extension().string() == ".wav") {
                
                std::string filename = entry.path().filename().string();
                std::string input_filename = entry.path().string();
                filename = filename.substr(0, filename.find_last_of("."));
                std::string output_filename = output_dir + "/" + filename + ".mp3";
                
                if (input_filename == output_filename)
                    continue;
                else
                {
                    // wav2mp3(input_filename.c_str(), output_filename.c_str());
                    ++g_is_running; 
                    std::thread t(convert, input_filename, output_filename);
                    t.detach();
                }    
                count++;
            }
            else {
                std::cout << "File: " << entry.path().filename() << " is not a wav file\n";
            }
            
            while (g_is_running.load() >= thread_num)
            {   
            }
        }
    }
    else {
        std::cout << "Folder: " << input_dir << " does not exist!\n";
    }

    while (g_is_running > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // 记录程序结束时间
    size_t duration = time(NULL) - start_time;
    if (duration > 0)
    {
        std::cout << count << " 个wav文件, " << "共计用时: " << duration/3600 << " h " << duration/60 << " m " << duration%60 << " s\n";
    }
    
    return 0;
}
