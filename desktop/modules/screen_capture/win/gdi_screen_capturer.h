#ifndef PEAR_SCREEN_CAPTURE_GDI_SCREEN_CAPTURER_H
#define PEAR_SCREEN_CAPTURE_GDI_SCREEN_CAPTURER_H

#include "../capture_frame.h"
#include "../screen_capturer.h"

namespace pear::capture {
    class GdiScreenCapturer : public ScreenCapturer {
    public:
        explicit GdiScreenCapturer(std::shared_ptr<CaptureSource> source);

        std::shared_ptr<CaptureFrame> Capture() override;

    private:
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_GDI_SCREEN_CAPTURER_H
