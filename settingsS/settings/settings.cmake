target_sources(${TARGET_NAME}
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/getline.c 
    ${CMAKE_CURRENT_LIST_DIR}/settings.c
)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../stb/ stb)
# include(../../stb/stb_ds.cmake)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
