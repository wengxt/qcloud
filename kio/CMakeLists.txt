add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} . )

set(kio_cloud_PART_SRCS
   kio_cloud.cpp
   requestwatcher.cpp
   )

kde4_add_plugin(kio_cloud ${kio_cloud_PART_SRCS})

target_link_libraries(kio_cloud ${KDE4_KIO_LIBS} qcloud)

install(TARGETS kio_cloud  DESTINATION ${PLUGIN_INSTALL_DIR})
install( FILES cloud.protocol  DESTINATION  ${SERVICES_INSTALL_DIR} )
