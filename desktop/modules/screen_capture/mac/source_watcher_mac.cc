#include "screen_capture/source_watcher.h"
#include <CoreFoundation/CFNumber.h>
#include <CoreGraphics/CGDirectDisplay.h>

namespace pear::capture {

    std::string getWindowString(CFDictionaryRef dictionary_ref, CFStringRef key_ref) {
        auto string_ref = reinterpret_cast<CFStringRef>(CFDictionaryGetValue(dictionary_ref, key_ref));
        if (string_ref == nullptr) {
            return "";
        }
        CFIndex string_length = CFStringGetLength(string_ref);
        CFIndex buf_length = string_length + 1;
        CFStringEncoding encoding = CFStringGetFastestEncoding(string_ref);
        char *string_buf = new char[buf_length];
        bool success = CFStringGetCString(string_ref, string_buf, buf_length, encoding);
        if (!success) {
            delete[] string_buf;
            return "";
        }
        string_buf[string_length] = '\0';
        std::string result(string_buf);
        delete[] string_buf;
        return result;
    }

    std::string getWindowName(CFDictionaryRef window_ref) {
        auto window_owner_name = getWindowString(window_ref, kCGWindowOwnerName);
        auto window_name = getWindowString(window_ref, kCGWindowName);
        if (window_name.length() > 0) {
            window_owner_name.append(" ");
            window_owner_name.append(window_name);
        }
        return window_owner_name;
    }

    SourceId getWindowId(CFDictionaryRef window_ref) {
        auto window_id = reinterpret_cast<CFNumberRef>(CFDictionaryGetValue(window_ref, kCGWindowNumber));
        if (!window_id) {
            return 0;
        }
        CGWindowID id;
        if (!CFNumberGetValue(window_id, kCFNumberIntType, &id)) {
            return 0;
        }
        return id;
    }

    bool isValidWindow(CFDictionaryRef window_ref) {
        auto window_layer = reinterpret_cast<CFNumberRef>(CFDictionaryGetValue(window_ref, kCGWindowLayer));
        if (!window_layer) {
            return false;
        }
        int layer;
        if (!CFNumberGetValue(window_layer, kCFNumberIntType, &layer)) {
            return false;
        }
        return layer == 0;
    }

    std::vector<std::shared_ptr<CaptureSource>> SourceWatcher::PollForAvailableSources(SourcePollType poll_type) {
        std::vector<std::shared_ptr<CaptureSource>> result;

        if (poll_type != WindowSourcesOnly) {
            uint32_t display_count;
            CGGetActiveDisplayList(10, nullptr, &display_count);
            auto displays = new CGDirectDisplayID[display_count];
            CGGetActiveDisplayList(10, displays, &display_count);
            for (int i = 0; i < display_count; i++) {
                auto display_name = i == 0 ? "Primary Screen" : "Screen " + std::to_string(i + 1);
                auto source = std::make_shared<CaptureSource>(displays[i], display_name, Screen);
                result.push_back(std::move(source));
            }
            delete[] displays;
        }

        if (poll_type != ScreenSourcesOnly) {
            CFArrayRef windows = CGWindowListCopyWindowInfo(
                    kCGWindowListOptionOnScreenOnly | kCGWindowListExcludeDesktopElements, kCGNullWindowID);
            if (windows) {
                for (CFIndex i = 0; i < CFArrayGetCount(windows); i++) {
                    auto window_ref = reinterpret_cast<CFDictionaryRef>(CFArrayGetValueAtIndex(windows, i));
                    if (window_ref && isValidWindow(window_ref)) {
                        std::string name = getWindowName(window_ref);
                        SourceId id = getWindowId(window_ref);
                        auto source = std::make_shared<CaptureSource>(id, name, Window);
                        result.push_back(std::move(source));
                    }
                }
                CFRelease(windows);
            }
        }

        return result;
    }

} // pear::capture
