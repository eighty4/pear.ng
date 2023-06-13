#ifndef PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_MAC_H
#define PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_MAC_H

#include "../capture_frame.h"
#include "../screen_capturer.h"

namespace pear::capture {

    class ScreenCapturerMac : public ScreenCapturer {
    public:
        explicit ScreenCapturerMac(std::shared_ptr<CaptureSource> source);

        std::shared_ptr<CaptureFrame> Capture() override;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_MAC_H
