cmake_minimum_required(VERSION 3.16)

project(TicTacToe VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RC_COMPILER "C:/msys64/mingw64/bin/windres.exe")
set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc")


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    PasswordTextEdit.cpp
    hoverbutton.cpp
    mainwindow.h
    mainwindow.ui
    PasswordTextEdit.h
    hoverbutton.h
ButtonAnimation.h
ButtonAnimation.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 5)
    qt_add_executable(TicTacToe
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        PasswordTextEdit.h
        PasswordTextEdit.cpp
        hoverbutton.h
        hoverbutton.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/appicon.rc
        ButtonAnimation.cpp
        ButtonAnimation.h
        .gitignore

    )


# Define target properties for Android with Qt 6 as:
#    set_property(TARGET TicTacToe APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(TicTacToe SHARED
            ${PROJECT_SOURCES}

        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(TicTacToe
            ${PROJECT_SOURCES}

        )
    endif()
endif()

target_link_libraries(TicTacToe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(TicTacToe PRIVATE Qt6::Multimedia)
target_link_libraries(TicTacToe PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)
# Define the icon file for the executable


# Add the resource to the executable (Windows-specific)


include_directories(${CMAKE_SOURCE_DIR})
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TicTacToe)
endif()
set_target_properties(TicTacToe PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS TicTacToe
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(TicTacToe)
endif()
