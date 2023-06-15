project(pear-cli)

set(SOURCES pear_cli.cc)

set(LIBS
        pear_net_tcp
        pear_screen_capture
        )

add_executable(pear_cli ${SOURCES})
target_compile_features(pear_cli PUBLIC ${PEAR_COMPILE_FEATURES})
target_link_libraries(pear_cli ${LIBS})
