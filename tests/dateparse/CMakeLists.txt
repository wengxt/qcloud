include_directories(${PROJECT_SOURCE_DIR}/securestore/kwallet/ ${QCA2_INCLUDE_DIRS})

add_executable(dateparse_test
               dateparse_test.cpp
              )

target_link_libraries(
    dateparse_test
    qcloud
)

