include_directories(${QT_QTCORE_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/lib
                    ${QOAuth_INCLUDE_DIR}
                    ${QCA2_INCLUDE_DIR}
                    ${QT_QTGUI_INCLUDE_DIR}
                    ${QT_QTWEBKIT_INCLUDE_DIR}
                    ${QT_QTNETWORK_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    )

set(qcloud_SRCS
    ibackend.cpp
    oauthbackend.cpp
    authorizewidget.cpp
    reply.cpp
    factory.cpp
    utils.cpp
    )

set(qcloud_MOC_HDRS
    ibackend.h
    oauthbackend.h
    authorizewidget.h
    reply.h
    factory.h
    )

qt4_wrap_cpp(
    qcloud_MOC_SRCS
    ${qcloud_MOC_HDRS}
)

qt4_wrap_ui(
    qcloud_UI_SRCS
    authorizewidget.ui
    )

add_library(qcloud SHARED ${qcloud_SRCS} ${qcloud_MOC_SRCS} ${qcloud_UI_SRCS})
target_link_libraries(qcloud
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTWEBKIT_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QTOAUTH_LIBRARY}
                      )