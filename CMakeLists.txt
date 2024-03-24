cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(ProjectDiamond)

set(CMAKE_CXX_STANDARD 17)

# Diamond Engine:

set(DIAMOND_ENGINE_DIR diamondengine)

add_subdirectory(${DIAMOND_ENGINE_DIR} DiamondEngine)

set(SPRITE_SHEETS_DIR spriteSheets)

set(SOURCES
    ${SRC_DIR}/behaviour/CharacterController2DBehaviour.cpp
    ${SRC_DIR}/builder/CharacterController2DBehaviourBuilder.cpp
    ${SRC_DIR}/config/CharacterController2DBehaviourConfig.cpp
    ${SRC_DIR}/parser/CharacterController2DBehaviourConfigParser.cpp
    ${SRC_DIR}/ProjectDiamond.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

function(collect_files_recursively result directory)
    file(GLOB_RECURSE all_files LIST_DIRECTORIES false "${directory}/*")
    foreach(file ${all_files})
        list(APPEND ${result} ${file})
    endforeach()
    set(${result} ${${result}} PARENT_SCOPE)
endfunction()

collect_files_recursively(GAME_ASSETS ${CONFIG_DIR})
collect_files_recursively(GAME_ASSETS "${DIAMOND_ENGINE_DIR}/${CONFIG_DIR}")
collect_files_recursively(GAME_ASSETS ${SCENES_DIR})
collect_files_recursively(GAME_ASSETS "${DIAMOND_ENGINE_DIR}/${SCENES_DIR}")
collect_files_recursively(GAME_ASSETS ${SHADERS_DIR})
collect_files_recursively(GAME_ASSETS "${DIAMOND_ENGINE_DIR}/${SHADERS_DIR}")

list(APPEND GAME_ASSETS "${AUDIO_DIR}/audioMetadata.xml")
list(APPEND GAME_ASSETS "${SPRITE_SHEETS_DIR}/spriteSheetMetadata.xml")
list(APPEND GAME_ASSETS "${TEXTURES_DIR}/textureMetadata.xml")
list(APPEND GAME_ASSETS "${DIAMOND_ENGINE_DIR}/${TEXTURES_DIR}/textureMetadata.xml")

add_custom_target(GameAssets ALL DEPENDS ${GAME_ASSETS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DIAMOND_ENGINE_DIR}/${INCLUDE_DIR}
    ${INCLUDE_DIR}
    ${INCLUDE_DIR}/behaviour
    ${INCLUDE_DIR}/builder
    ${INCLUDE_DIR}/config
    ${INCLUDE_DIR}/parser)

target_link_libraries(${PROJECT_NAME} PRIVATE DiamondEngine)

add_custom_command(TARGET GameAssets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${GLEW_BINARY_DIR}/glew32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${IRR_KLANG_BINARY_DIR}/ikpMP3.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${IRR_KLANG_BINARY_DIR}/irrKlang.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${CONFIG_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${CONFIG_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${CONFIG_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SPRITE_SHEETS_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${SPRITE_SHEETS_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${AUDIO_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${AUDIO_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${SCENES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SCENES_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SCENES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SCENES_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${SHADERS_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${TEXTURES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TEXTURES_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TEXTURES_DIR}
    VERBATIM)

add_dependencies(${PROJECT_NAME} GameAssets)
