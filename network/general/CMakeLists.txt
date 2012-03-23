set(network_general_SRCS networkaccessmanager.cpp)

set(network_general_MOC_HDRS
    networkaccessmanager.h
    )

qt4_wrap_cpp(
    network_general_MOC_SRCS
    ${network_general_MOC_HDRS}
)

add_library(qcloud_network_general MODULE ${network_general_SRCS} ${network_general_MOC_SRCS})
target_link_libraries(qcloud_network_general
                      ${QT_QTNETWORK_LIBRARY}
                      qcloud
                      )

install(TARGETS qcloud_network_general DESTINATION ${QCLOUD_PLUGIN_DIR}/network)