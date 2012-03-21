include_directories(${PROJECT_SOURCE_DIR}/lib)

set(network_kde_SRCS kdenetworkaccessmanager.cpp)

set(network_kde_MOC_HDRS
    kdenetworkaccessmanager.h
    )

qt4_wrap_cpp(
    network_kde_MOC_SRCS
    ${network_kde_MOC_HDRS}
)

add_library(qcloud_network_kde MODULE ${network_kde_SRCS} ${network_kde_MOC_SRCS})
target_link_libraries(qcloud_network_kde
                      ${KDE4_KIO_LIBS}
                      qcloud
                      )
                      
install(TARGETS qcloud_network_kde DESTINATION ${QCLOUD_PLUGIN_DIR}/network)