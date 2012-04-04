include_directories(${QT_INCLUDE_DIR}
                    ${QOAuth_INCLUDE_DIR}
                    ${QCA2_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${LibArchive_INCLUDE_DIRS}
                    ${PROJECT_SOURCE_DIR}/include
                    )

set(qcloud_SRCS
    ibackend.cpp
    oauthbackend.cpp
    oauthwidget.cpp
    reply.cpp
    factory.cpp
    utils.cpp
    isecurestore.cpp
    server.cpp
    client.cpp
    iplugin.cpp
    authdialog.cpp
    authwidget.cpp
    app.cpp
    appmanager.cpp
    packager.cpp
    encrypter.cpp
    info.cpp
    )

set(qcloud_HDRS
    ../include/ibackend.h
    ../include/oauthbackend.h
    ../include/oauthwidget.h
    ../include/reply.h
    ../include/factory.h
    ../include/isecurestore.h
    ../include/server.h
    ../include/client.h
    ../include/authdialog.h
    ../include/authwidget.h
    ../include/appmanager.h
    ../include/packager.h
    ../include/encrypter.h
    )

set(qcloud_MOC_HDRS ${qcloud_HDRS}
    ../include/client_p.h
    ../include/factory_p.h
    ../include/oauthwidget_p.h
    ../include/appmanager_p.h
    )

qt4_wrap_cpp(
    qcloud_MOC_SRCS
    ${qcloud_MOC_HDRS}
)

qt4_wrap_ui(
    qcloud_UI_SRCS
    oauthwidget.ui
    )

qt4_add_dbus_adaptor(qcloud_SRCS
    daemon.xml
    server.h
    QCloud::Server)


set_property(
    SOURCE daemon.xml
    PROPERTY INCLUDE info.h
)

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
                      ${LibArchive_LIBRARIES}
                      ${QCA2_LIBRARIES}
                      )

set_target_properties(qcloud
                      PROPERTIES VERSION 0.1
                      SOVERSION 0
)

install(TARGETS qcloud DESTINATION ${LIB_INSTALL_DIR})
