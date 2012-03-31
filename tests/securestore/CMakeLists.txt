include_directories(${PROJECT_SOURCE_DIR}/securestore/kwallet/
                    )
qt4_wrap_cpp(
    kwallet_test_MOC_SRCS
    securestorewidget.h
    ${PROJECT_SOURCE_DIR}/securestore/kwallet/kwallet.h
)

add_executable(kwallet_test
               kwallet_test.cpp
               securestorewidget.cpp
               ${PROJECT_SOURCE_DIR}/securestore/kwallet/kwallet.cpp
               ${kwallet_test_MOC_SRCS}
               )

target_link_libraries(
    kwallet_test
    ${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KDEUI_LIBS}
    qcloud
)
