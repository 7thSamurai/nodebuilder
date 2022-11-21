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

if(WIN32)
    add_custom_command(TARGET nodebuilder POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_RUNTIME_LIBRARY}
            ${CAIRO_RUNTIME_LIBRARY}
            $<TARGET_FILE_DIR:nodebuilder>)
endif()