#ifndef PEAR_SCREEN_CAPTURE_GDI_WINDOW_CAPTURER_H
#define PEAR_SCREEN_CAPTURE_GDI_WINDOW_CAPTURER_H

#include "../capture_frame.h"
#include "../screen_capturer.h"

namespace pear::capture {
    class GdiWindowCapturer : public ScreenCapturer {
    public:
        explicit GdiWindowCapturer(std::shared_ptr<CaptureSource> source);

        std::shared_ptr<CaptureFrame> Capture() override;

    private:
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_GDI_WINDOW_CAPTURER_H
