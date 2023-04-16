#add_subdirectory(${CMAKE_CURRENT_LIST_DIR} gui)
# target_sources(${TARGET_NAME}
    # PUBLIC ${CMAKE_CURRENT_LIST_DIR}/combobox.c
# )
set(GUI_SOURCES combobox.c
                layoutStack.c 
                linelayout.c 
                persistent.c 
                draw.c 
                toolbuttongroup.c 
                windowlayout.c
                gui.c 
                gridlayout.c 
                combobox.c 
                guiglobals.c 
            )
add_compile_definitions(GUI_RESOURCE_PATH="${CMAKE_CURRENT_LIST_DIR}/icons")
if(SDL)
    list(APPEND GUI_SOURCES backends/x/sdlbackend.c) 
    add_compile_definitions(SDL)
    
    find_package(PkgConfig REQUIRED)
    pkg_search_module(SDLLIB REQUIRED sdl2)
    
    target_include_directories(${TARGET_NAME} PUBLIC ${SDLLIB_INCLUDE_DIRS})
    target_link_libraries(${TARGET_NAME} ${SDLLIB_LIBRARIES})
endif()
if(X)
    list(APPEND GUI_SOURCES 
                backends/x/xbackend.c)
    target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/usr/include/freetype2/)

    find_package(PkgConfig REQUIRED)
    pkg_search_module(MYLIB REQUIRED x11 xrender xft)
endif()

list(TRANSFORM GUI_SOURCES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
target_sources(${TARGET_NAME}
    PUBLIC ${GUI_SOURCES}
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/backends/x)
include(${CMAKE_CURRENT_LIST_DIR}/../../settingsS/settings/settings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../loadImage/loadImage.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../../misc/misc.cmake)


