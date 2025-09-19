cmake_minimum_required(VERSION 3.5)

project(sw)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (WIN32)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG -DWIN32")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -DWIN32")
endif (WIN32)

# find_package(SDL2_mixer REQUIRED)
find_package(SDL2 REQUIRED)
find_package(imgui REQUIRED)
find_package(stb REQUIRED)

set(
        SRC
        SoftwarePhysicsPlayground.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/rasteriser/timer.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/rasteriser/rasteriser.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdlrenderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends/imgui_impl_sdl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/particle/Generator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/particle/Particle.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/physics/marshal.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/physics/proxy.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/physics/proxy_properties_soa.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/physics/solver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/delegate/delegate.cpp
)

add_executable(
        ${PROJECT_NAME}
        ${SRC}
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        ${SDL2_LIBRARIES}
        ${_sdl2mixer_dll}
)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${IMGUI_INCLUDE_DIRS}
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_compile_features(
        ${PROJECT_NAME}
        PUBLIC
        cxx_std_23
)

# copy sdl libs to binary locaton
add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND cp ${SDL2_BINDIR}/SDL2.dll ${CMAKE_CURRENT_BINARY_DIR} 
)
