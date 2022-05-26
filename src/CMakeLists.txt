find_package(SDL2 REQUIRED)

add_executable(
    nodebuilder
    blockmap.cpp
    bsp.cpp
    main.cpp
    map.cpp
    node.cpp
    renderer.cpp
    splitter.cpp
    wad.cpp
)

target_link_libraries(
    nodebuilder
    PRIVATE
    SDL2::SDL2
)
