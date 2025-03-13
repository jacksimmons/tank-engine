set(APPLICATION_DIR "../Application/$<CONFIG>")
project("Engine") #DLL
    file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
        "src/*.hpp"
        "src/*.cpp"
    )
    add_library(${PROJECT_NAME} SHARED
        ${PROJECT_SOURCES}
    )
    source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES
        ${PROJECT_SOURCES}
    )
    target_compile_features(${PROJECT_NAME} PRIVATE
        "cxx_std_20"
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        "../include"
        "../include/glm"
        "include"
        "src"
    )
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        "src/tepch.hpp"
    )
    # Provide root directory to the application
    get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        TANK_DLL
        GLM_ENABLE_EXPERIMENTAL
        FMT_UNICODE=0
        ROOT_DIRECTORY="${ROOT_DIR}"
    )
    # Copy all required DLL/LIBs into the output dir
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        lib $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    # Copy Engine and its deps into Application
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}> "${APPLICATION_DIR}"
    )