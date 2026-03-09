cmake_minimum_required(VERSION 3.30)
project(Calc VERSION 0.0.1
    DESCRIPTION "A simple calculator library/app. Provides a CLI and a GUI version" 
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fix clang not supporting std::expected 
# and macro defined issue with gtest
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
    add_compile_definitions(__cpp_concepts=202002L)
    add_compile_options(-Wno-builtin-macro-redefined)
endif()

list(FIND CMAKE_CXX_COMPILE_FEATURES cxx_std_23 cxx23_supported)
if (cxx23_supported EQUAL -1)
    message(FATAL_ERROR "C++ 23 Standard not supported by compiler")
endif()

option(BUILD_TESTS "Build tests using Google Test" ON)
option(BUILD_BENCHMARKS "Build benchmarks using Google Benchmark" OFF)
option(BUILD_FUZZERS "Build fuzz tests using libFuzzer" OFF)
option(BUILD_LANG_BINDINGS "Build language bindings" OFF)
option(ENABLE_SANITIZERS "Enable UB sanitizers" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
option(ENABLE_CLANG_FORMAT "Enable auto source formatting" OFF)

message("Build type is ${CMAKE_BUILD_TYPE}")

# Helper utility to tag available targets
include(${CMAKE_SOURCE_DIR}/cmake/SpecialTargets.cmake)

# Enable project wide clang-tidy check
# Can set target level properties: CXX_CLANG_TIDY
if (ENABLE_CLANG_TIDY)
    message("Enabling clang-tidy static analysis check")
    find_program(CLANG-TIDY_PATH clang-tidy REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG-TIDY_PATH};-checks=*")
    add_compile_options(-Wall -Werror -Wextra)
endif()

# Adds a custom 'format' target, uses clang-format
if (ENABLE_CLANG_FORMAT)
    include(${CMAKE_SOURCE_DIR}/cmake/Format.cmake)
    Format(DIRECTORIES src test)
    register_special_target(format)
endif()

if (ENABLE_SANITIZERS)
    include(CheckCXXCompilerFlag)
    set(flag undefined)
    check_cxx_compiler_flag(-fsanitize=${flag} HAS_SANITIZER_FLAG)
    if (HAS_SANITIZER_FLAG)
        message("Adding undefined sanitizer flag")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=${flag} -fno-omit-frame-pointer")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=${flag} -fno-omit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=${flag}")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fsanitize=${flag}")
    else()
        message(WARNING "Undefined flag not supported for compiler")
    endif()
endif()

# Add doxygen target to generate docs
find_package(Doxygen)
if (DOXYGEN_FOUND)
    register_special_target(docs)
    set(DOXYGEN_PROJECT_NAME Calculator)
    set(DOXYGEN_PROJECT_BRIEF "Calculator App")
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_HTML_OUTPUT ${CMAKE_SOURCE_DIR}/docs) 
    set(DOXYGEN_SOURCE_BROWSER YES)
    set(DOXYGEN_INLINE_SOURCES YES)
    set(DOXYGEN_HAVE_DOTS YES)
    set(DOXYGEN_CLASS_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_GRAPHICAL_HIERARCHY YES)
    doxygen_add_docs(docs ${CMAKE_SOURCE_DIR}/src/libs/core 
        COMMENT "Generating HTML documentation")
endif()

if (BUILD_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()

if (BUILD_BENCHMARKS)
    add_subdirectory(bench)
endif()

if (BUILD_FUZZERS)
    add_subdirectory(fuzz)
endif()

# Add source mandatorily duh
add_subdirectory(src)

# Create CalcConfig.cmake file
include(CMakePackageConfigHelpers)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/calc)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CalcConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/CalcConfig.cmake"
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}
    PATH_VARS LIB_INSTALL_DIR
)

# Create a CalcConfigVersion.cmake file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/CalcConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install export target under namespace, creates CalcTargets.cmake
install(EXPORT CalcTargets NAMESPACE Calc:: DESTINATION ${LIB_INSTALL_DIR})

# Install the config file to required path
install(FILES 
    "${CMAKE_CURRENT_BINARY_DIR}/CalcConfig.cmake" 
    "${CMAKE_CURRENT_BINARY_DIR}/CalcConfigVersion.cmake" 
    DESTINATION ${LIB_INSTALL_DIR})

if(AVAILABLE_SPECIAL_TARGETS)
    list(JOIN AVAILABLE_SPECIAL_TARGETS ", " SPL_TARGETS)
    message(STATUS "Special targets available: ${SPL_TARGETS}")
endif()

# CPack config; Use: cpack -G "TGZ;DEB" -B packages
set(CPACK_PACKAGE_NAME "Calculator")
set(CPACK_PACKAGE_VENDOR "Naresh Jagadeesan")
set(CPACK_PACKAGE_DESCRIPTION "A simple Calculator written in C++")
set(CPACK_PACKAGE_CONTACT "37257700+Infinage@users.noreply.github.com")
set(CPACK_GENERATOR TGZ) 
include(CPack)
