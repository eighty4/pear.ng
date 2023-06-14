project(pear-cli)

set(LIBS pear_screen_capture)

if (NOT WIN32)
    find_package(fmt CONFIG REQUIRED)
    list(APPEND LIBS fmt::fmt)
endif ()

add_executable(pear_cli pear_cli.cc)
target_compile_features(pear_cli PUBLIC ${PEAR_COMPILE_FEATURES})
target_include_directories(pear_cli PRIVATE .)
target_link_libraries(pear_cli PRIVATE ${LIBS})
