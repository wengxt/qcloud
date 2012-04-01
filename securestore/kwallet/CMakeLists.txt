set(securestore_kwallet_SRCS kwalletstore.cpp plugin.cpp )

set(securestore_kwallet_MOC_HDRS
    kwalletstore.h
    plugin.h
    )

qt4_wrap_cpp(
    securestore_kwallet_SRCS
    ${securestore_kwallet_MOC_HDRS}
)

add_library(qcloud_securestore_kwallet MODULE ${securestore_kwallet_SRCS} ${securestore_kwallet_MOC_SRCS})
target_link_libraries(qcloud_securestore_kwallet
                        ${QT_QTCORE_LIBRARY}
                        ${KDE4_KDECORE_LIBS}
                        ${KDE4_KPARTS_LIBS}
                        ${KDE4_KDEUI_LIBS}
                        qcloud )
install(TARGETS qcloud_securestore_kwallet DESTINATION ${QCLOUD_PLUGIN_DIR}/securestore)