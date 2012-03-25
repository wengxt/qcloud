cmake_minimum_required(VERSION 2.6)

project(qcloud)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})
include_directories(${PROJECT_SOURCE_DIR}/lib)

add_definitions(${KDE4_DEFINITIONS})
find_package(QCA2 REQUIRED)
find_package(QOAuth REQUIRED)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -Wl,--no-undefined ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

set(QCLOUD_PLUGIN_DIR ${QT_PLUGINS_DIR}/qcloud)

add_subdirectory(lib)
add_subdirectory(backend)
add_subdirectory(tool)
add_subdirectory(daemon)
add_subdirectory(network)
add_subdirectory(securestore)
add_subdirectory(data)

