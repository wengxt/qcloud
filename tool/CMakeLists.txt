include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(tool_SRCS
    accountdialog.cpp
    mainwindow.cpp
    main.cpp
    )

set(tool_MOC_HDRS
    accountdialog.h
    mainwindow.h
    )

qt4_wrap_cpp(
    tool_MOC_SRCS
    ${tool_MOC_HDRS}
)

qt4_wrap_ui(
    tool_UI_SRCS
    accountdialog.ui
    tool.ui
    )

add_executable(qcloud_tool ${tool_SRCS} ${tool_MOC_SRCS} ${tool_UI_SRCS})

target_link_libraries(qcloud_tool
                      qcloud
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      )

