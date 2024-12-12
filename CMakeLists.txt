cmake_minimum_required(VERSION 3.30)

project("FileTagManager"
        VERSION 0.0.1
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
# staticly linking standart ligraries  
set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CSS_STANDARD_LIBRARIES}")
set(FullBinDir "${CMAKE_CURRENT_SOURCE_DIR}/../bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullBinDir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullBinDir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullBinDir}")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.c)

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/config.json $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
        COMMAND_EXPAND_LISTS
)
set(REQUIRED_PACKAGES "ICU" "SDL2" "SDL2_image" "SDL2_mixer" "SDL2_ttf")
foreach(PATH ${CMAKE_PREFIX_PATH})
        set(LIB_ROOT_PATH ${CMAKE_PREFIX_PATH})
        break()
endforeach()
message(STATUS "LIB_ROOT_PATH - ${LIB_ROOT_PATH}")
foreach(PACKAGE ${REQUIRED_PACKAGES})
        list(APPEND CMAKE_PREFIX_PATH "${LIB_ROOT_PATH}/${PACKAGE}")
        list(APPEND LIB_PATHS "${LIB_ROOT_PATH}/${PACKAGE}")
endforeach()
message(STATUS "CMAKE_PREFIX_PATH - ${CMAKE_PREFIX_PATH}")
find_package(ICU REQUIRED COMPONENTS uc data)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
message(STATUS "ICU_FOUND - ${ICU_FOUND}")
message(STATUS "SDL2_FOUND - ${SDL2_FOUND}")
message(STATUS "SDL2_image_FOUND - ${SDL2_image_FOUND}")
message(STATUS "SDL2_mixer_FOUND - ${SDL2_mixer_FOUND}")
message(STATUS "SDL2_ttf_FOUND - ${SDL2_ttf_FOUND}")
if(ICU_FOUND)
        file(GLOB_RECURSE uc_DLL ${LIB_ROOT_PATH}/ICU/*icuuc*.dll)
        file(GLOB_RECURSE data_DLL ${LIB_ROOT_PATH}/ICU/*icudt*.dll)
        list(APPEND uc_DLL ${data_DLL})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${uc_DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMAND_EXPAND_LISTS
        )
endif()
if(SDL2_FOUND OR SDL2_image_FOUND OR SDL2_mixer_FOUND OR SDL2_ttf_FOUND)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMAND_EXPAND_LISTS
        )
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC Ole32 ${SDL2_LIBRARIES} ICU::uc ICU::data SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
