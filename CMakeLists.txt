cmake_minimum_required(VERSION 3.23)
project(directx)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

set (WINDOWS_KIT "C:/Program Files (x86)/Windows Kits/10")
link_directories("${WINDOWS_KIT}/Lib/10.0.22000.0/um/x64")

find_library(D3D11 d3d11.lib)
find_library(DXGI dxgi.lib)

link_libraries(D3D11 DXGI)

# Build HLSL shaders
add_custom_target(shaders)
set(HLSL_SHADER_FILES CubeVertexShader.hlsl CubePixelShader.hlsl)
set_source_files_properties(CubeVertexShader.hlsl PROPERTIES ShaderType "vs")
set_source_files_properties(CubePixelShader.hlsl PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "4_0_level_9_3")
foreach(FILE ${HLSL_SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(type ${FILE} ShaderType)
    get_source_file_property(model ${FILE} ShaderModel)
    add_custom_command(
            TARGET shaders
            COMMAND "${WINDOWS_KIT}/bin/10.0.22000.0/x64/fxc.exe" /nologo /Emain /T${type}_${model} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
            MAIN_DEPENDENCY ${FILE}
            COMMENT "HLSL ${FILE}"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM
    )
endforeach(FILE)

add_executable(${PROJECT_NAME} WIN32
        helper.h
        targetver.h
        framework.h
        Application.cpp
        Application.h
        directx.cpp
        directx.h
        DeviceResources.cpp
        DeviceResources.h
        Renderer.cpp
        Renderer.h
        )
add_dependencies(${PROJECT_NAME} shaders)


