set(securestore_plain_SRCS
    plainstore.cpp
    plugin.cpp
    )

set(securestore_plain_MOC_HDRS
    plainstore.h
    plugin.h
    )

qt4_wrap_cpp(
    securestore_plain_MOC_SRCS
    ${securestore_plain_MOC_HDRS}
)

add_library(qcloud_securestore_plain MODULE ${securestore_plain_SRCS} ${securestore_plain_MOC_SRCS})
target_link_libraries(qcloud_securestore_plain
                      ${QT_QTCORE_LIBRARY}
                      qcloud
                      )

install(TARGETS qcloud_securestore_plain DESTINATION ${QCLOUD_PLUGIN_DIR}/securestore)
