cmake_minimum_required(VERSION 3.11)

project(tinygrammar)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Threads REQUIRED)
pkg_check_modules(epoxy REQUIRED IMPORTED_TARGET epoxy)

set(CMAKE_CXX_STANDARD 17)

add_library(tinygrammar-lib)

target_sources(
    tinygrammar-lib
    PRIVATE
        external/clipper/clipper.cpp
        external/clipper/clipper.hpp
#        external/clipper/glfw/glfw3.h
#        external/clipper/glfw/glfw3native.h
        external/format.cpp
        external/format.h
#        external/glfw/glfw3.h
#        external/glfw/glfw3native.h
        external/imgui/imconfig.h
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui.h
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_internal.h
        external/imgui/stb_rect_pack.h
        external/imgui/stb_textedit.h
        external/imgui/stb_truetype.h
        external/jsonxx.cc
        external/jsonxx.h
        external/nanosvg/nanosvg.h
        external/nanosvg/nanosvgrast.h
        external/nanovg/fontstash.h
        external/nanovg/nanovg.cpp
        external/nanovg/nanovg_gl2.cpp
        external/nanovg/nanovg_gl.h
        external/nanovg/nanovg_gl_utils.h
        external/nanovg/nanovg.h
        external/nanovg/stb_image.h
        external/nanovg/stb_image_write.h
        external/nanovg/stb_truetype.h
        external/stb/stb_perlin.h
        external/tree.hh
        tinygrammar/animator.cpp
        tinygrammar/animator.h
        tinygrammar/animator_matrix.cpp
        tinygrammar/animator_matrix.h
        tinygrammar/clipper_methods.cpp
        tinygrammar/clipper_methods.h
        tinygrammar/common.cpp
        tinygrammar/common.h
        tinygrammar/csg_tree.cpp
        tinygrammar/csg_tree.h
        tinygrammar/draw.cpp
        tinygrammar/draw.h
        tinygrammar/expansion_manager.cpp
        tinygrammar/expansion_manager.h
        tinygrammar/grammar_core.cpp
        tinygrammar/grammar_core.h
        tinygrammar/input_figures.cpp
        tinygrammar/input_figures.h
        tinygrammar/operator.cpp
        tinygrammar/operator.h
        tinygrammar/rule.cpp
        tinygrammar/rule.h
        tinygrammar/shape.cpp
        tinygrammar/shape.h
        tinygrammar/svg.cpp
        tinygrammar/svg.h
        tinygrammar/tangle_utilities.cpp
        tinygrammar/tangle_utilities.h
        tinygrammar/threadpool.h
        tinygrammar/time_manager.cpp
        tinygrammar/time_manager.h
        tinygrammar/ui.h
        tinygrammar/yocto_math.h)
target_include_directories(tinygrammar-lib PUBLIC external)
target_link_libraries(tinygrammar-lib PUBLIC PkgConfig::epoxy OpenGL::GL glfw Threads::Threads)

#add_executable(tinygrammar)
#target_sources(tinygrammar PRIVATE tinygrammar/main.cpp)
#target_link_libraries(tinygrammar PRIVATE tinygrammar-lib)

add_executable(tinygrammar-tag-svg)
target_sources(tinygrammar-tag-svg PRIVATE tinygrammar/main_tag_svg.cpp)
target_link_libraries(tinygrammar-tag-svg PRIVATE tinygrammar-lib)

add_executable(tinygrammar-timeline)
target_sources(tinygrammar-timeline PRIVATE tinygrammar/main_timeline.cpp)
target_link_libraries(tinygrammar-timeline PRIVATE tinygrammar-lib)

add_executable(tinygrammar-time)
target_sources(tinygrammar-time PRIVATE tinygrammar_time/main.cpp)
target_link_libraries(tinygrammar-time PRIVATE tinygrammar-lib)
