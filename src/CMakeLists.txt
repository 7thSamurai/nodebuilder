find_package(SDL2 REQUIRED)
find_package(Cairo REQUIRED)

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

target_include_directories(
    nodebuilder
    PRIVATE
    ${CAIRO_INCLUDE_DIRS}
)

target_link_libraries(
    nodebuilder
    PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    ${CAIRO_LIBRARIES}
)
