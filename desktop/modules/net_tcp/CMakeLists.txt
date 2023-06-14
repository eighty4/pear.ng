project(pear-net-tcp)

if (APPLE OR LINUX)
    set(SOURCES tcp_socket_posix.cc)
elseif (WIN32)
    set(SOURCES tcp_socket_win.cc)
endif ()

if (APPLE)
    find_library(APPLICATION_SERVICES ApplicationServices)
    set(LIBS ${APPLICATION_SERVICES})
elseif (LINUX)
elseif (WIN32)
    set(LIBS
            wsock32
            ws2_32
            mswsock
            advapi32
            )
endif ()

add_library(pear_net_tcp STATIC ${SOURCES})
target_compile_features(pear_net_tcp PUBLIC ${PEAR_COMPILE_FEATURES})
target_include_directories(pear_net_tcp PUBLIC ..)
target_link_libraries(pear_net_tcp PUBLIC ${LIBS})
