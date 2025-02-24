cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(ProjectDiamond)

set(CMAKE_CXX_STANDARD 17)

# Diamond Engine:

set(DIAMOND_ENGINE_DIR diamondengine)

add_subdirectory(${DIAMOND_ENGINE_DIR} DiamondEngine)

set(INCLUDE_DIR include)
set(LIB_DIR lib)
set(SRC_DIR src)
set(AUDIO_DIR audio)
set(COMPONENT_DIR component)
set(CONFIG_DIR config)
set(PARSER_DIR parser)
set(SPRITE_SHEETS_DIR spriteSheets)
set(SCENES_DIR scenes)
set(TEXTURES_DIR textures)
set(SHADERS_DIR shaders)

set(SOURCES
    "${SRC_DIR}/${COMPONENT_DIR}/BoxCharacter2D.cpp"
    "${SRC_DIR}/${COMPONENT_DIR}/JumpResetBox.cpp"
    "${SRC_DIR}/${COMPONENT_DIR}/LevelDoor.cpp"
    "${SRC_DIR}/${COMPONENT_DIR}/ScrollingBackground.cpp"
    "${SRC_DIR}/${CONFIG_DIR}/BoxCharacter2DConfig.cpp"
    "${SRC_DIR}/${CONFIG_DIR}/JumpResetBoxConfig.cpp"
    "${SRC_DIR}/${CONFIG_DIR}/LevelDoorConfig.cpp"
    "${SRC_DIR}/${CONFIG_DIR}/ScrollingBackgroundConfig.cpp"
    "${SRC_DIR}/${PARSER_DIR}/BoxCharacter2DConfigParser.cpp"
    "${SRC_DIR}/${PARSER_DIR}/JumpResetBoxConfigParser.cpp"
    "${SRC_DIR}/${PARSER_DIR}/LevelDoorConfigParser.cpp"
    "${SRC_DIR}/${PARSER_DIR}/ScrollingBackgroundConfigParser.cpp"
    "${SRC_DIR}/ProjectDiamond.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

function(collect_files_recursively result directory)
    file(GLOB_RECURSE all_files LIST_DIRECTORIES false "${directory}/*")
    foreach(file ${all_files})
        list(APPEND ${result} ${file})
    endforeach()
    set(${result} ${${result}} PARENT_SCOPE)
endfunction()

collect_files_recursively(GAME_ASSETS ${CONFIG_DIR})
collect_files_recursively(GAME_ASSETS ${SCENES_DIR})
collect_files_recursively(GAME_ASSETS ${SHADERS_DIR})
collect_files_recursively(GAME_ASSETS "${DIAMOND_ENGINE_DIR}/${SHADERS_DIR}")

list(APPEND GAME_ASSETS "${AUDIO_DIR}/audioMetadata.xml")
list(APPEND GAME_ASSETS "${SPRITE_SHEETS_DIR}/spriteSheetMetadata.xml")
list(APPEND GAME_ASSETS "${TEXTURES_DIR}/textureMetadata.xml")

add_custom_target(GameAssets ALL DEPENDS ${GAME_ASSETS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${DIAMOND_ENGINE_DIR}/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/${CONFIG_DIR}/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/audio/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/game/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/graphics/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/input/${INCLUDE_DIR}
    ${DIAMOND_ENGINE_DIR}/${LIB_DIR}/${PARSER_DIR}/${INCLUDE_DIR}
    ${INCLUDE_DIR}
    "${INCLUDE_DIR}/component"
    "${INCLUDE_DIR}/config"
    "${INCLUDE_DIR}/parser")

target_link_libraries(${PROJECT_NAME} PRIVATE DiamondEngine)

add_custom_command(TARGET GameAssets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${GLEW_BINARY_DIR}/glew32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${IRR_KLANG_BINARY_DIR}/ikpMP3.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${IRR_KLANG_BINARY_DIR}/irrKlang.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${CONFIG_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${CONFIG_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SPRITE_SHEETS_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${SPRITE_SHEETS_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${AUDIO_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${AUDIO_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SCENES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SCENES_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${DIAMOND_ENGINE_DIR}/${SHADERS_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${SHADERS_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${SHADERS_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/${TEXTURES_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>/${TEXTURES_DIR}
    VERBATIM)

add_dependencies(${PROJECT_NAME} GameAssets)
