include_directories(${QT_QTCORE_INCLUDE_DIR}
                    ${QOAuth_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QCA2_INCLUDE_DIR}
                    )

set(dropbox_SRCS dropbox.cpp)

set(dropbox_MOC_HDRS
    dropbox.h
    )

qt4_wrap_cpp(
    dropbox_MOC_SRCS
    ${dropbox_MOC_HDRS}
)

add_library(qcloud_dropbox MODULE ${dropbox_SRCS} ${dropbox_MOC_SRCS})
target_link_libraries(qcloud_dropbox
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      qcloud
                      )

install(TARGETS qcloud_dropbox DESTINATION ${QCLOUD_PLUGIN_DIR}/backend)

add_executable(qcloud_dropbox_test dropbox_test.cpp ${dropbox_SRCS} ${dropbox_MOC_SRCS} )
target_link_libraries(qcloud_dropbox_test
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      qcloud
                      )