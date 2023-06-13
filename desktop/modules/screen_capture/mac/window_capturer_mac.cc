#include "window_capturer_mac.h"
#include <ApplicationServices/ApplicationServices.h>
#include <iostream>
#include <utility>

namespace pear::capture {

    WindowCapturerMac::WindowCapturerMac(std::shared_ptr<CaptureSource> source)
            : ScreenCapturer(std::move(source)) {}

    std::shared_ptr<CaptureFrame> WindowCapturerMac::Capture() {
        CGImageRef image = CGWindowListCreateImage(
                CGRectNull,
                kCGWindowListOptionIncludingWindow,
                source_->id,
                kCGWindowImageNominalResolution | kCGWindowImageBoundsIgnoreFraming
        );
        if (image == nullptr) {
            std::cout << source_->key << " capture null" << std::endl;
            exit(1);
        }
        size_t width = CGImageGetWidth(image);
        size_t height = CGImageGetHeight(image);
        if (width == 0 && height == 0) {
            std::cout << source_->key << " capture returned empty image" << std::endl;
            exit(1);
        }
        size_t bits_per_pixel = CGImageGetBitsPerPixel(image);
        if (bits_per_pixel != 32) {
            std::cout << source_->key << " " << bits_per_pixel << " not 32 bpp image" << std::endl;
            exit(1);
        }
        CGDataProviderRef data_provider = CGImageGetDataProvider(image);
        CFDataRef data_pointer = CGDataProviderCopyData(data_provider);
        CGImageRelease(image);
        int image_buffer_len = static_cast<int>(CFDataGetLength(data_pointer));
        const uint8_t *image_buffer = CFDataGetBytePtr(data_pointer);
        auto image_buffer_copy = new uint8_t[image_buffer_len];
        for (int i = 0; i < image_buffer_len; i++) {
            image_buffer_copy[i] = image_buffer[i];
        }
        auto frame = CaptureFrame::ForFrameData(source_, image_buffer_copy, image_buffer_len, width, height);
        CFRelease(data_pointer);
        return frame;
    }

} // pear::capture
