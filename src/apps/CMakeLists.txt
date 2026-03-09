add_executable(calc calc_cli.cc)
target_link_libraries(calc PRIVATE calculator)

include(FetchContent)
FetchContent_Declare(webview SYSTEM
    GIT_REPOSITORY https://github.com/webview/webview
    GIT_TAG 9a608c2ce21521f2e1006df2003970cf02871ede)
FetchContent_MakeAvailable(webview)

# Remove the terminal showing up on start
add_executable(calc-gui calc_gui.cc)
set_target_properties(calc-gui PROPERTIES WIN32_EXECUTABLE $<BOOL:${WIN32}>)

target_link_libraries(calc-gui PRIVATE calculator webview::core)

# Tie executables to an export target
install(TARGETS calc calc-gui RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
