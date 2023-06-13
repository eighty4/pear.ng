#include "../source_watcher.h"
#include <codecvt>
#include <string>
#include "Windows.h"
#include "dwmapi.h"

namespace pear::capture {

    std::wstring getWindowTitle(HWND hwnd) {
        int size = GetWindowTextLengthW(hwnd);
        if (size > 0) {
            size += 1;
            auto name_buf = new wchar_t[size];
            GetWindowTextW(hwnd, name_buf, size);
            std::wstring name_str;
            name_str.append(name_buf);
            delete[] name_buf;
            return name_str;
        } else {
            return L"unnamed window";
        }
    }

    BOOL isApplicationWindow(HWND hwnd) {

        if (!IsWindowVisible(hwnd)) {
            return FALSE;
        }

        HWND hwndTry = GetAncestor(hwnd, GA_ROOTOWNER);
        HWND hwndWalk = nullptr;
        while (hwndTry != hwndWalk) {
            hwndWalk = hwndTry;
            hwndTry = GetLastActivePopup(hwndWalk);
            if (IsWindowVisible(hwndTry))
                break;
        }
        if (hwndWalk != hwnd) {
            return FALSE;
        }

        TITLEBARINFO title_bar;
        title_bar.cbSize = sizeof(title_bar);
        GetTitleBarInfo(hwnd, &title_bar);
        if (title_bar.rgstate[0] & STATE_SYSTEM_INVISIBLE) {
            return FALSE;
        }
        if (title_bar.rgstate[0] & STATE_SYSTEM_OFFSCREEN) {
            return FALSE;
        }
        if (title_bar.rgstate[0] & STATE_SYSTEM_UNAVAILABLE) {
            return FALSE;
        }

        if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) {
            return FALSE;
        }

        int cloaked;
        DwmGetWindowAttribute(hwnd, DWMWA_CLOAKED, &cloaked, sizeof(cloaked));
        if (cloaked != 0) {
            return FALSE;
        }

        return TRUE;
    }

    BOOL CALLBACK enumScreensFunc(HMONITOR hmonitor, [[maybe_unused]] HDC hdc,
                                  [[maybe_unused]] LPRECT lprect, LPARAM lparam) {
        auto result = (std::vector<std::shared_ptr<CaptureSource>> *) lparam;
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(hmonitor, &info);
        int id = static_cast<int>(result->size());
        auto title = id == 0 ? "Primary Screen" : "Screen " + std::to_string(id);
        auto screen = std::make_shared<CaptureSource>(id, title, Screen);
        result->push_back(std::move(screen));
        return TRUE;
    }

    std::string wstring_to_utf8(const std::wstring &str) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> wstring_to_utf8;
        return wstring_to_utf8.to_bytes(str);
    }

    BOOL CALLBACK enumWindowsFunc(HWND hwnd, LPARAM lparam) {
        auto result = (std::vector<std::shared_ptr<CaptureSource>> *) lparam;
        if (isApplicationWindow(hwnd)) {
            auto window_title = getWindowTitle(hwnd);
            auto id = reinterpret_cast<intptr_t>(hwnd);
            auto title = wstring_to_utf8(window_title); // special and non-english chars lost
            auto window = std::make_shared<CaptureSource>(id, title, Window);
            result->push_back(std::move(window));
        }
        return TRUE;
    }

    std::vector<std::shared_ptr<CaptureSource>> SourceWatcher::PollForAvailableSources(SourcePollType poll_type) {
        std::vector<std::shared_ptr<CaptureSource>> result;
        if (poll_type != WindowSourcesOnly) {
            EnumDisplayMonitors(nullptr, nullptr, enumScreensFunc, (LPARAM) &result);
        }
        if (poll_type != ScreenSourcesOnly) {
            EnumWindows(enumWindowsFunc, (LPARAM) & result);
        }
        return result;
    }

} // pear::capture
