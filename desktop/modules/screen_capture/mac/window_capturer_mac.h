#ifndef PEAR_SCREEN_CAPTURE_WINDOW_CAPTURER_MAC_H
#define PEAR_SCREEN_CAPTURE_WINDOW_CAPTURER_MAC_H

#include "../capture_frame.h"
#include "../screen_capturer.h"

namespace pear::capture {

    class WindowCapturerMac : public ScreenCapturer {
    public:
        explicit WindowCapturerMac(std::shared_ptr<CaptureSource> source);

        std::shared_ptr<CaptureFrame> Capture() override;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_WINDOW_CAPTURER_MAC_H
