/*************************************************************************
 * This file is part of input-overlay
 * git.vrsal.xyz/alex/input-overlay
 * Copyright 2023 univrsal <uni@vrsal.xyz>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/

#include "uihook_log.hpp"
#include <fstream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <obs-module.h>
#include <filesystem>

namespace uihook_log {

static std::mutex log_mutex;
static std::ofstream log_file;
static bool logging_enabled = false;

void shutdown()
{
    std::lock_guard<std::mutex> lock(log_mutex);
    if (log_file.is_open()) {
        log_file.close();
    }
}

void start_logging(const std::string &recording_path)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    std::filesystem::path rec_path(recording_path);

    std::filesystem::path log_path = rec_path.parent_path() / (rec_path.stem().string() + "_input_overlay.log");

    log_file.open(log_path.string(), std::ios_base::app);

    if (!log_file.is_open()) {

        blog(LOG_ERROR, "[input-overlay] Failed to open log file: %s", log_path.string().c_str());

        return;
    }

    logging_enabled = true;
}

void stop_logging()

{

    std::lock_guard<std::mutex> lock(log_mutex);

    if (log_file.is_open()) {

        log_file << " - Logging stopped" << std::endl;

        log_file.close();
    }

    logging_enabled = false;
}

void log_uihook_event(const uiohook_event *event)
{
    if (!event)
        return;

    std::lock_guard<std::mutex> lock(log_mutex);
    if (!log_file.is_open() || !logging_enabled)
        return;

    log_file << event->time << " - ";

    switch (event->type) {
    case EVENT_KEY_PRESSED:
    case EVENT_KEY_RELEASED:
        log_file << (event->type == EVENT_KEY_PRESSED ? "Key Pressed: " : "Key Released: ")
                 << event->data.keyboard.keycode << std::endl;
        break;
    case EVENT_MOUSE_CLICKED:
    case EVENT_MOUSE_PRESSED:
    case EVENT_MOUSE_RELEASED:
        log_file << "Mouse "
                 << (event->type == EVENT_MOUSE_CLICKED   ? "Clicked: "
                     : event->type == EVENT_MOUSE_PRESSED ? "Pressed: "
                                                          : "Released: ")
                 << "Button " << event->data.mouse.button << " at (" << event->data.mouse.x << ", "
                 << event->data.mouse.y << ")" << std::endl;
        break;
    case EVENT_MOUSE_MOVED:
    case EVENT_MOUSE_DRAGGED:
        log_file << (event->type == EVENT_MOUSE_MOVED ? "Mouse Moved: " : "Mouse Dragged: ") << "("
                 << event->data.mouse.x << ", " << event->data.mouse.y << ")" << std::endl;
        break;
    case EVENT_MOUSE_WHEEL:
        log_file << "Mouse Wheel: "
                 << "Amount " << event->data.wheel.amount << ", Rotation " << event->data.wheel.rotation << std::endl;
        break;
    default:
        log_file << "Unknown event type: " << event->type << std::endl;
    }
}

} // namespace uihook_log