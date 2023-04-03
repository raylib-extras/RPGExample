cmake_minimum_required(VERSION 3.16)
project(rpg_game)

set(CMAKE_CXX_STANDARD 17)
set(RAYLIB_VERSION 4.2.0)

find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
        message(INFO ${raylib_BINARY_DIR})
    endif()
endif()

add_subdirectory(libs/pugixml)

add_executable(
        rpg_game_client
        client/main.cpp
        client/audio.cpp
        client/combat.cpp
        client/game.cpp
        client/game_hud.cpp
        client/items.cpp
        client/loading.cpp
        client/map.cpp
        client/main.cpp
        client/monsters.cpp
        client/pause.cpp
        client/pause.cpp
        client/screens.cpp
        client/sprites.cpp
        client/tile_map_drawing.cpp
        client/tile_map_io.cpp
        client/treasure.cpp
)
target_include_directories(rpg_game_client PUBLIC client/include)
target_link_libraries(rpg_game_client pugixml raylib)

if (APPLE)
    target_link_libraries(rpg_game_client "-framework IOKit")
    target_link_libraries(rpg_game_client "-framework Cocoa")
    target_link_libraries(rpg_game_client "-framework OpenGL")
endif()