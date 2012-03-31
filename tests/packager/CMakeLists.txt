add_executable(packager_test
               packager_test.cpp
               )

target_link_libraries(
    packager_test
    qcloud
)

add_test(packager_test packager_test ${CMAKE_CURRENT_BINARY_DIR}/test.tar.gz ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/COPYING ${PROJECT_SOURCE_DIR}/CMakeLists.txt)