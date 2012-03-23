find_package(PkgConfig REQUIRED)

_pkgconfig_invoke("dbus-1" DBUS SESSION_BUS_SERVICES_DIR "" "--variable=session_bus_services_dir")

set(daemon_SRCS
    daemon.cpp
    main.cpp
    )

set(daemon_MOC_HDRS
    daemon.h
    )

qt4_wrap_cpp(
    daemon_MOC_SRCS
    ${daemon_MOC_HDRS}
)

add_executable(qcloud_daemon ${daemon_SRCS} ${daemon_MOC_SRCS})

target_link_libraries(qcloud_daemon
                      ${QT_QTCORE_LIBRARY}
                      )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/org.qcloud.daemon.service.in ${CMAKE_CURRENT_BINARY_DIR}/org.qcloud.daemon.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.qcloud.daemon.service DESTINATION ${DBUS_SESSION_BUS_SERVICES_DIR})
