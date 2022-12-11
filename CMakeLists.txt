cmake_minimum_required(VERSION 3.23)
project(directx)

set(CMAKE_CXX_STANDARD 20)

include_directories(./src)

set (WINDOWS_KIT "C:/Program Files (x86)/Windows Kits/10")
set (WINDOWS_VERSION 10.0.22621.0)
set (WINDOWS_ARCHITECTURE x64)
link_directories("${WINDOWS_KIT}/Lib/${WINDOWS_VERSION}/um/${WINDOWS_ARCHITECTURE}")

find_library(D3D11 d3d11.lib)
find_library(DXGI dxgi.lib)

link_libraries(D3D11 DXGI)

# Build HLSL shaders
add_custom_target(shaders)
set(HLSL_SHADER_FILES src/CubeVertexShader.hlsl src/CubePixelShader.hlsl)
set_source_files_properties(src/CubeVertexShader.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(src/CubePixelShader.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "4_0_level_9_3")
foreach(FILE ${HLSL_SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(type ${FILE} ShaderType)
    get_source_file_property(model ${FILE} ShaderModel)
    add_custom_command(
            TARGET shaders
            COMMAND "${WINDOWS_KIT}/bin/${WINDOWS_VERSION}/${WINDOWS_ARCHITECTURE}/fxc.exe" /nologo /Emain /T${type}_${model} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
    )
endforeach(FILE)

add_executable(${PROJECT_NAME} WIN32
        src/helper.h
        src/targetver.h
        src/framework.h
        src/debug.h
        src/debug.cpp
        src/directx.cpp
        src/directx.h
        src/Application.cpp
        src/Application.h
        src/DeviceResources.cpp
        src/DeviceResources.h
        src/Renderer.cpp
        src/Renderer.h
        src/Timer.cpp
        src/Timer.h
        src/StanfordParser.cpp src/StanfordParser.h src/helper.cpp src/Mesh.cpp src/Mesh.h src/OpenSimplexNoise.cpp src/OpenSimplexNoise.h src/GameModel.cpp src/GameModel.h src/Registry.cpp src/Registry.h)

add_dependencies(${PROJECT_NAME} shaders)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/stdafx.h)


