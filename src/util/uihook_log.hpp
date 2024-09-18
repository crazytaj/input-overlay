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

#pragma once

#include <uiohook.h>

#include <string>

namespace uihook_log {

void shutdown();

void start_logging(const std::string &recording_path);

void stop_logging();

void log_uihook_event(const uiohook_event *event);

} // namespace uihook_log