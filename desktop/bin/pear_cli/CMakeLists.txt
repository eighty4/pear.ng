project(pear-cli)

add_executable(pear_cli pear_cli.cc)
target_compile_features(pear_cli PUBLIC ${PEAR_COMPILE_FEATURES})
target_link_libraries(pear_cli pear_screen_capture)
