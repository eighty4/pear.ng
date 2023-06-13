#include "gdi_screen_capturer.h"
#include <Windows.h>
#include <iostream>
#include <memory>

namespace pear::capture {

    GdiScreenCapturer::GdiScreenCapturer(std::shared_ptr<CaptureSource> source)
            : ScreenCapturer(std::move(source)) {}

    std::shared_ptr<CaptureFrame> GdiScreenCapturer::Capture() {
        // todo crop because GetDC(nullptr) returns a device context for all monitors on a combined virtual screen
        HDC screen_dc = GetDC(nullptr);
        int actual_height = GetDeviceCaps(screen_dc, DESKTOPVERTRES);
        int actual_width = GetDeviceCaps(screen_dc, DESKTOPHORZRES);
        int img_height = actual_height;
        int img_width = actual_width;

        HDC mem_dc = CreateCompatibleDC(screen_dc);
        HBITMAP mem_hbm = CreateCompatibleBitmap(screen_dc, img_width, img_height);

        if (!BitBlt(mem_dc,
                    0, 0,
                    actual_width, actual_height,
                    screen_dc,
                    0, 0,
                    SRCCOPY | CAPTUREBLT)) {
            DWORD error_code = GetLastError();
            std::cout << "bit blt error " << error_code << std::endl;
            if (error_code == 5) {
                DeleteObject(mem_hbm);
                DeleteDC(mem_dc);
                ReleaseDC(nullptr, screen_dc);
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
        ReleaseDC(nullptr, screen_dc);
        return CaptureFrame::ForFrameData(source_, data, bmp_size, img_width, img_height);
    }

} // pear::capture
