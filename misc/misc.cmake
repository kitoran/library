target_sources(${TARGET_NAME}
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/newFile.c 
    ${CMAKE_CURRENT_LIST_DIR}/timeAFunction.c
)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
