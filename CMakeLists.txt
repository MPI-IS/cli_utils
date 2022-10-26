cmake_minimum_required(VERSION 3.5)
project(cli_utils VERSION 1.0.0)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# stop build on first error
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wfatal-errors -Werror=return-type")

find_package(Boost REQUIRED COMPONENTS program_options)


## Libraries

add_library(program_options
    src/program_options.cpp
)
target_include_directories(program_options PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(program_options PUBLIC
    Boost::program_options
)


## Tests

include(CTest)
if (BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)

    add_executable(test_cli_utils
        test/main.cpp
        test/test_program_options.cpp
    )
    target_link_libraries(test_cli_utils
        program_options
        GTest::gtest
    )
    # declare the test as gtest
    gtest_add_tests(TARGET test_cli_utils)
endif()


## Installation

install(DIRECTORY include/${PROJECT_NAME}/
        DESTINATION include/${PROJECT_NAME})
install(
    TARGETS
        program_options
    EXPORT ${PROJECT_NAME}Targets
)

# Generate and install package config files
install(EXPORT
    ${PROJECT_NAME}Targets
    DESTINATION lib/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION lib/cmake/${PROJECT_NAME}
)
