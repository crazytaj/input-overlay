// hid_event_log.cpp
#include "hid_event_log.hpp"
#include <mutex>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

std::mutex log_mutex;
const char *log_file_path = "C:/Users/Tariq/Desktop/log_file.txt";

std::string get_current_timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

void log_mouse_event_thread_safe(const uiohook_event &event)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    std::ofstream log_file(log_file_path, std::ios_base::app);
    if (log_file.is_open()) {
        log_file << get_current_timestamp() << " - Mouse event: ";
        switch (event.type) {
        case EVENT_MOUSE_CLICKED:
        case EVENT_MOUSE_PRESSED:
        case EVENT_MOUSE_RELEASED:
            log_file << "Click event, button: " << event.data.mouse.button << std::endl;
            break;
        case EVENT_MOUSE_MOVED:
        case EVENT_MOUSE_DRAGGED:
            log_file << "Move event, x: " << event.data.mouse.x << ", y: " << event.data.mouse.y << std::endl;
            break;
        default:
            log_file << "Unknown mouse event" << std::endl;
            break;
        }
        log_file.close();
    }
}