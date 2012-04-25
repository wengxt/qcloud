include_directories(${QT_QTCORE_INCLUDE_DIR}
                    ${QOAuth_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QCA2_INCLUDE_DIR}
                    ${QJSON_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/backend/dropbox/
                    )

qt4_wrap_cpp(
    dropbox_test_MOC_SRCS
    ${PROJECT_SOURCE_DIR}/backend/dropbox/dropbox.h
    ${PROJECT_SOURCE_DIR}/backend/dropbox/dropboxrequest.h
)

add_executable(qcloud_dropbox_test
               dropbox_test.cpp
               ${PROJECT_SOURCE_DIR}/backend/dropbox/dropbox.cpp
               ${PROJECT_SOURCE_DIR}/backend/dropbox/dropboxrequest.cpp
               ${dropbox_test_MOC_SRCS}
               )
target_link_libraries(qcloud_dropbox_test
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QJSON_LIBRARY}
                      qcloud
                      )
