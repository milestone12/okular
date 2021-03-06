project(okular)

add_definitions(${QT_DEFINITIONS})
include_directories(
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}

   ${QIMAGEBLITZ_INCLUDES}
)

set(okular_SRCS
    okularplugin.cpp
    ${CMAKE_SOURCE_DIR}/ui/guiutils.cpp
    ${CMAKE_SOURCE_DIR}/ui/tocmodel.cpp
    ${CMAKE_SOURCE_DIR}/ui/pagepainter.cpp
    ${CMAKE_SOURCE_DIR}/ui/debug_ui.cpp
    pageitem.cpp
    documentitem.cpp
    thumbnailitem.cpp
    okularsingleton.cpp
    )

kconfig_add_kcfg_files(okular_SRCS ${CMAKE_SOURCE_DIR}/conf/settings.kcfgc )

add_library(okularplugin SHARED ${okular_SRCS})
set_target_properties(okularplugin PROPERTIES COMPILE_DEFINITIONS "okularpart_EXPORTS")
target_link_libraries(okularplugin
        Qt5::Quick
        Qt5::Qml
        Qt5::Core
        Qt5::Xml
        Qt5::Svg
        KF5::Bookmarks
        KF5::IconThemes
        KF5::I18n
        KF5::Service
        okularcore
        )

install(TARGETS okularplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/okular)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/okular)
install(FILES DocumentView.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/okular)
install(FILES private/PageView.qml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/okular/private)

#add_subdirectory(test)
