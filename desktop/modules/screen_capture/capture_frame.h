#ifndef PEAR_SCREEN_CAPTURE_CAPTURE_FRAME_H
#define PEAR_SCREEN_CAPTURE_CAPTURE_FRAME_H

#include "capture_source.h"
#include <memory>
#include <string>

namespace pear::capture {

    class CaptureFrame {
    public:
        static std::shared_ptr<CaptureFrame> ForFrameData(std::shared_ptr<CaptureSource> source, uint8_t *data,
                                                          int data_size, int width, int height);

        static std::shared_ptr<CaptureFrame> ForFrameData(std::shared_ptr<CaptureSource> source, uint8_t *data,
                                                          int data_size, int stride, int width, int height);

        CaptureFrame(std::shared_ptr<CaptureSource> source, uint8_t *data, int data_size, int stride,
                     int width, int height);

        ~CaptureFrame() {
            delete[] data_;
        }

        std::shared_ptr<CaptureSource> Source() {
            return source_;
        }

        uint8_t *Data() {
            return data_;
        }

        [[nodiscard]] int DataSize() const {
            return data_size_;
        }

        [[nodiscard]] int Height() const {
            return height_;
        }

        [[nodiscard]] int Stride() const {
            return stride_;
        }

        [[nodiscard]] int Width() const {
            return width_;
        }

    private:
        std::shared_ptr<CaptureSource> source_;
        uint8_t *data_;
        int data_size_;
        int height_;
        int stride_;
        int width_;
    };

} // pear::capture

#endif // PEAR_SCREEN_CAPTURE_CAPTURE_FRAME_H
