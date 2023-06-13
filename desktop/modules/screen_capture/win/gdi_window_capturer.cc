#include "gdi_window_capturer.h"
#include <Windows.h>
#include <iostream>
#include <memory>

namespace pear::capture {

    GdiWindowCapturer::GdiWindowCapturer(std::shared_ptr<CaptureSource> source)
            : ScreenCapturer(std::move(source)) {}

    std::shared_ptr<CaptureFrame> GdiWindowCapturer::Capture() {
        HWND window_id = reinterpret_cast<HWND>(source_->id);
        HDC window_dc = GetWindowDC(window_id);
        if (!window_dc) {
            std::cout << "failed to get window dc" << std::endl;
            exit(1);
        }

        RECT rect;
        if (!GetWindowRect(window_id, &rect)) {
            std::cout << "failed to get window size" << std::endl;
            exit(1);
        }

        int img_width = rect.right - rect.left;
        int img_height = rect.bottom - rect.top;

        HDC mem_dc = CreateCompatibleDC(window_dc);
        HBITMAP mem_hbm = CreateCompatibleBitmap(window_dc, img_width, img_height);

        if (!BitBlt(mem_dc,
                    0, 0,
                    img_width,
                    img_height,
                    window_dc,
                    rect.left, rect.top,
                    SRCCOPY | CAPTUREBLT)) {
            DWORD error_code = GetLastError();
            std::cout << "bit blt error " << error_code << std::endl;
            if (error_code == 5) {
                DeleteObject(mem_hbm);
                DeleteDC(mem_dc);
                ReleaseDC(nullptr, window_dc);
                return nullptr;
            } else {
                exit(1);
            }
        }


        SelectObject(mem_dc, mem_hbm);

        BITMAP mem_bmp;
        GetObject(mem_hbm, sizeof(BITMAP), &mem_bmp);
        BITMAPINFOHEADER bi;
        bi.biSize = sizeof(BITMAPINFOHEADER);
        bi.biWidth = mem_bmp.bmWidth;
        bi.biHeight = -mem_bmp.bmHeight;
        bi.biPlanes = 1;
        bi.biBitCount = 32;
        bi.biCompression = BI_RGB;
        bi.biSizeImage = 0;
        bi.biXPelsPerMeter = 0;
        bi.biYPelsPerMeter = 0;
        bi.biClrUsed = 0;
        bi.biClrImportant = 0;

        long bmp_size = mem_bmp.bmWidth * mem_bmp.bmHeight * 4;
        auto data = new uint8_t[bmp_size];
        GetDIBits(mem_dc, mem_hbm, 0, (UINT) mem_bmp.bmHeight, data, (BITMAPINFO *) &bi, DIB_RGB_COLORS);

        DeleteObject(mem_hbm);
        DeleteDC(mem_dc);
        ReleaseDC(nullptr, window_dc);
        return CaptureFrame::ForFrameData(source_, data, bmp_size, img_width, img_height);
    }

} // pear::capture
