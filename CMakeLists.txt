cmake_minimum_required(VERSION 3.19)

# --------------------------- 
# Basic cmake for install
# ---------------------------

project(logging VERSION 1.0 DESCRIPTION logging LANGUAGES CXX)

add_library(logging OBJECT src/logging.cpp include/logger_ostream.h)

# install stuff
target_include_directories(logging INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/logging-config-version.cmake"
    VERSION ${VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(TARGETS logging
    EXPORT logging_targets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/logging-config.cmake.in" 
    "${PROJECT_BINARY_DIR}/logging-config.cmake" 
    INSTALL_DESTINATION lib/cmake/stacktrace
)

install(EXPORT logging_targets DESTINATION lib/cmake/logging)
install(FILES "${PROJECT_BINARY_DIR}/logging-config-version.cmake" "${PROJECT_BINARY_DIR}/logging-config.cmake" 
    DESTINATION lib/cmake/logging
)

find_package(stacktrace CONFIG REQUIRED)
target_link_libraries(logging INTERFACE stacktrace)

# test
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

add_executable(logging-test "${PROJECT_SOURCE_DIR}/examples/main.cpp")
add_dependencies(logging-test logging)

include_directories("${PROJECT_BINARY_DIR}/include/")
include_directories("${PROJECT_SOURCE_DIR}/include/")
set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(logging-test logging)
