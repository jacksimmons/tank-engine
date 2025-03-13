set(MONO_ROOT "../vendor/mono")
set(SCRIPTCORE_ROOT "Resources/Scripts/$<CONFIG>")
set(ENGINE_ROOT "../Engine/$<CONFIG>")
get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

project("Application") #EXE
    file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "src/*.hpp"
        "src/*.cpp"
        "include/imgui/imgui.cpp"
        "include/imgui/imgui_demo.cpp"
        "include/imgui/imgui_draw.cpp"
        "include/imgui/imgui_tables.cpp"
        "include/imgui/imgui_widgets.cpp"
        "include/imgui/backends/imgui_impl_glfw.cpp"
        "include/imgui/backends/imgui_impl_opengl3.cpp"
        "include/glad/glad.cpp"
    )
    add_executable(${PROJECT_NAME} "src/editor.cpp"
        ${PROJECT_SOURCES}
    )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
        ${PROJECT_SOURCES}
    )
    find_package(OpenGL REQUIRED)
    target_compile_features(${PROJECT_NAME} PRIVATE "cxx_std_20")
    target_include_directories(${PROJECT_NAME} PRIVATE
        "../include"
        "../include/glm"
        "../Engine/src"
        "src"
        "include"
        "include/imgui"
        "include/imgui/backends"
        "${MONO_ROOT}/include"
    )
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        "../Engine/src/tepch.hpp"
    )
    target_link_directories(${PROJECT_NAME} PRIVATE
        "lib"
        "${MONO_ROOT}/lib/$<CONFIG>"
        "${SCRIPTCORE_ROOT}"
    )
    target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        "Engine"
        "glfw3"
        "glfw3dll"
        "assimp-vc143-mt"
        "mono-2.0-sgen"
        "Tank-ScriptCore"
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLM_ENABLE_EXPERIMENTAL
        FMT_UNICODE=0
        ROOT_DIRECTORY="${ROOT_DIR}/"
        APP_DIRECTORY="${ROOT_DIR}/${PROJECT_NAME}/"
        SCRIPTCORE_DIRECTORY="${SCRIPTCORE_ROOT}/"
    )
    # Copy ScriptCore and its deps into Application
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SCRIPTCORE_ROOT}" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    # Copy all deps into the output dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "lib/$<CONFIG>" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
