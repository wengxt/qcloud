include_directories(${QT_INCLUDE_DIR}
                    ${QOAuth_INCLUDE_DIR}
                    ${QCA2_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    )

set(qcloud_SRCS
    ibackend.cpp
    oauthbackend.cpp
    authorizewidget.cpp
    reply.cpp
    factory.cpp
    utils.cpp
    isecurestore.cpp
    isecurestorewidget.cpp
    server.cpp
    client.cpp
    )

set(qcloud_HDRS
    ibackend.h
    oauthbackend.h
    authorizewidget.h
    reply.h
    factory.h
    isecurestore.h
    isecurestorewidget.h
    server.h
    client.h
    )

set(qcloud_MOC_HDRS ${qcloud_HDRS}
    client_p.h
    factory_p.h
    )

qt4_wrap_cpp(
    qcloud_MOC_SRCS
    ${qcloud_MOC_HDRS}
)

qt4_wrap_ui(
    qcloud_UI_SRCS
    authorizewidget.ui
    )

qt4_add_dbus_adaptor(qcloud_SRCS
    daemon.xml
    server.h
    QCloud::Server)

qt4_add_dbus_interface(qcloud_SRCS
    daemon.xml
    org.qcloud.Daemon
)

add_library(qcloud SHARED ${qcloud_SRCS} ${qcloud_MOC_SRCS} ${qcloud_UI_SRCS})
target_link_libraries(qcloud
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTWEBKIT_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ${QT_QTDBUS_LIBRARY}
                      ${QTOAUTH_LIBRARY}
                      )