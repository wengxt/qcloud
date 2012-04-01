include_directories(${PROJECT_SOURCE_DIR}/securestore/kwallet/)

qt4_wrap_cpp(
    encrypter_test_MOC_SRCS
    ${PROJECT_SOURCE_DIR}/securestore/kwallet/kwalletstore.h
    ${PROJECT_SOURCE_DIR}/include/encrypter.h
)

add_executable(encrypter_test
               encrypter_test.cpp
               ${PROJECT_SOURCE_DIR}/lib/encrypter.cpp
               ${PROJECT_SOURCE_DIR}/securestore/kwallet/kwalletstore.cpp
               ${encrypter_test_MOC_SRCS}
              )

target_link_libraries(
    encrypter_test
    ${QT_QTCORE_LIBRARY}
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KDEUI_LIBS}
    qcloud
)
