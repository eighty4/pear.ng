project(pear-screen-capture)

set(SOURCES
        capture_frame.cc
        screen_capturer.cc
        source_watcher.cc
        )

if (APPLE)
    find_library(APPLICATION_SERVICES ApplicationServices)
    set(LIBS ${APPLICATION_SERVICES})
    list(APPEND SOURCES
            mac/screen_capturer_mac.cc
            mac/source_watcher_mac.cc
            mac/window_capturer_mac.cc
            )
elseif (LINUX)
elseif (WIN32)
    set(LIBS crypt32 dwmapi)
    list(APPEND SOURCES
            win/gdi_screen_capturer.cc
            win/gdi_window_capturer.cc
            win/source_watcher_win.cc
            )
endif ()

add_library(pear_screen_capture STATIC ${SOURCES})
target_compile_features(pear_screen_capture PUBLIC ${PEAR_COMPILE_FEATURES})
target_include_directories(pear_screen_capture PUBLIC ..)
target_link_libraries(pear_screen_capture PUBLIC ${LIBS})
