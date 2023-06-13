#include "screen_capturer.h"
#include <utility>

#ifdef PEAR_MAC
#include "screen_capture/mac/screen_capturer_mac.h"
#include "screen_capture/mac/window_capturer_mac.h"
#elif defined(PEAR_WIN)
#include "screen_capture/win/gdi_screen_capturer.h"
#include "screen_capture/win/gdi_window_capturer.h"
#endif

namespace pear::capture {

#ifdef PEAR_MAC
    typedef ScreenCapturerMac PlatformScreenCapturer;
    typedef WindowCapturerMac PlatformWindowCapturer;
#elif defined(PEAR_WIN)
    typedef GdiScreenCapturer PlatformScreenCapturer;
    typedef GdiWindowCapturer PlatformWindowCapturer;
#endif

    ScreenCapturer *ScreenCapturer::Create(std::shared_ptr<CaptureSource> source) {
        if (source->type == Screen) {
            return new PlatformScreenCapturer(std::move(source));
        } else {
            return new PlatformWindowCapturer(std::move(source));
        }
    }

    ScreenCapturer::ScreenCapturer(std::shared_ptr<CaptureSource> source)
            : source_(std::move(source)) {}

    std::shared_ptr<CaptureFrame> ScreenCapturer::Capture() {
        printf("not implemented\n");
        exit(1);
    }

    std::shared_ptr<CaptureSource> ScreenCapturer::Source() {
        return source_;
    }

} // pear::capture
