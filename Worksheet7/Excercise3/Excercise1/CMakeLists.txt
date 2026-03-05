cmake_minimum_required(VERSION 3.19)
project(Excercise1 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)
find_package(VTK REQUIRED)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

qt_standard_project_setup()

qt_add_executable(Excercise1
    WIN32 MACOSX_BUNDLE
    main.cpp
    vrbasesetup.cpp
    vrbasesetup.h
    vrbasesetup.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    optiondialog.h
    optiondialog.cpp
    optiondialog.ui
)

qt_add_translations(
    TARGETS Excercise1
    TS_FILES Excercise1_en_150.ts
)

target_link_libraries(Excercise1
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        ${VTK_LIBRARIES}
)

include(GNUInstallDirs)

install(TARGETS Excercise1
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Excercise1
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

add_custom_target( VRBindings )
add_custom_command( TARGET VRBindings PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
            copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/ )
