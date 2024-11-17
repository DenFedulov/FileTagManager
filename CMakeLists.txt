cmake_minimum_required(VERSION 3.30)

project("FileTagManager"
        VERSION 0.0.1
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
# staticly linking standart ligraries  
set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CSS_STANDARD_LIBRARIES} -Wl,-Bstatic -lstdc++ -lpthread") 
set(FullBinDir "${CMAKE_CURRENT_SOURCE_DIR}/../bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullBinDir}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullBinDir}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullBinDir}")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${FullBinDir}/")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/config.json" DESTINATION "${FullBinDir}/")

add_executable(${PROJECT_NAME} main.cpp ${SOURCE_FILES})
add_subdirectory("H:/Projects/C++_projects/libraries/SDL" "${FullBinDir}/SDL")
add_subdirectory("H:/Projects/C++_projects/libraries/SDL_image" "${FullBinDir}/SDL_image")
add_subdirectory("H:/Projects/C++_projects/libraries/SDL_mixer" "${FullBinDir}/SDL_mixer")
add_subdirectory("H:/Projects/C++_projects/libraries/SDL_ttf" "${FullBinDir}/SDL_ttf")
target_link_libraries(${PROJECT_NAME} PUBLIC SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
