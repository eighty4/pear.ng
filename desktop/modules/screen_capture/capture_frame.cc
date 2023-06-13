#include "capture_frame.h"
#include <utility>

namespace pear::capture {

    std::shared_ptr<CaptureFrame> CaptureFrame::ForFrameData(std::shared_ptr<CaptureSource> source, uint8_t *data,
                                                      int data_size, int width, int height) {
        return ForFrameData(std::move(source), data, data_size, width * 4, width, height);
    }

    std::shared_ptr<CaptureFrame> CaptureFrame::ForFrameData(std::shared_ptr<CaptureSource> source, uint8_t *data,
                                                      int data_size, int stride, int width, int height) {
        return std::make_shared<CaptureFrame>(std::move(source), data, data_size, stride, width, height);
    }

    CaptureFrame::CaptureFrame(std::shared_ptr<CaptureSource> source, uint8_t *data, int data_size,
                               int stride, int width, int height)
            : source_(std::move(source)),
              data_(data),
              data_size_(data_size),
              height_(height),
              stride_(stride),
              width_(width) {}

} // pear::capture
