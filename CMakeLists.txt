cmake_minimum_required(VERSION 3.16)

project(QAI VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

set(COMPONENTS
    component/counter.cpp
    component/counter.h
    component/counter_for_qml.hpp
    component/qt_core_test/q_string_test.h
)

qt_add_executable(appQAI
    main.cpp
    ${COMPONENTS}
)

qt_add_qml_module(appQAI
    URI QAI
    QML_FILES
        Main.qml
)

target_include_directories(appQAI PRIVATE
    ${CMAKE_SOURCE_DIR}/component
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appQAI PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appQAI
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQAI
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appQAI
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
