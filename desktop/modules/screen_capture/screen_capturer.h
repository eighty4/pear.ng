#ifndef PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_H
#define PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_H

#include "capture_frame.h"
#include "capture_source.h"
#include <memory>
#include <string>

namespace pear::capture {

    class ScreenCapturer {
    public:
        static ScreenCapturer *Create(std::shared_ptr<CaptureSource> source);

        explicit ScreenCapturer(std::shared_ptr<CaptureSource> source);

        virtual std::shared_ptr<CaptureFrame> Capture();

        std::shared_ptr<CaptureSource> Source();

    protected:
        std::shared_ptr<CaptureSource> source_;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_SCREEN_CAPTURER_H
