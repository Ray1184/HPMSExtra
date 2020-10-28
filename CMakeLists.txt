# Tools project
cmake_minimum_required(VERSION 3.15)
project(HPMSExtra)

set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_PREFIX)

include_directories(include)

# Third party
include(Shared.cmake)

set(DEP_PATH ${CMAKE_CURRENT_BINARY_DIR}/_deps)
set(CREATE_RUNTIME_LIB TRUE)

install_from_repo(https://github.com/Ray1184/HPMSRuntime.git HPMSRuntime master TRUE /include/)

install_from_repo(https://github.com/g-truc/glm.git GLM master TRUE /)

install_from_repo(https://github.com/mtrempoltsev/pods.git PODS master FALSE /include/)

install_from_repo(https://github.com/zeux/pugixml.git PUGIXML master TRUE /src/)


set(DEP_PATH ${CMAKE_CURRENT_BINARY_DIR}/_deps)

include_directories(${DEP_PATH}/glfw-src/include)
include_directories(${DEP_PATH}/glm-src)
include_directories(${DEP_PATH}/physfs-src/src)
include_directories(${DEP_PATH}/hpmsengine-src/include)
include_directories(${DEP_PATH}/pugixml-src/src)

set(SOURCES_M
        src/tools/LightColladaImporter.cpp
        src/tools/dae/DaeUtils.cpp
        src/tools/dae/LibGeometryParser.cpp
        src/tools/dae/LibMaterialParser.cpp
        src/tools/dae/LibSkeletonParser.cpp
        src/tools/dae/LibAnimParser.cpp
        src/tools/dae/LibSkinningParser.cpp
        src/tools/dae/LibHeaderParser.cpp)

set(SOURCES_R
        src/tools/RoomImporter.cpp
        )

add_executable(HPMS3DModelToHDAT ${SOURCES_M} src/HPMS3DModelToHDATMain.cpp)

add_executable(HPMSRoomToHDAT ${SOURCES_R} src/HPMSRoomToHDATMain.cpp)

target_link_libraries(HPMS3DModelToHDAT pugixml HPMSRuntime)

target_link_libraries(HPMSRoomToHDAT HPMSRuntime)

# Post build
set(HPMSENGINE_DLL HPMSEngine${CMAKE_DEBUG_POSTFIX}.dll)

add_custom_command(TARGET HPMS3DModelToHDAT
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${DEP_PATH}/hpmsengine-build/${HPMSENGINE_DLL} ${CMAKE_CURRENT_BINARY_DIR}/${HPMSENGINE_DLL}
        )
add_custom_command(TARGET HPMSRoomToHDAT
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${DEP_PATH}/hpmsengine-build/${HPMSENGINE_DLL} ${CMAKE_CURRENT_BINARY_DIR}/${HPMSENGINE_DLL}
        )


if (MSYS OR MINGW)
    add_custom_command(TARGET HPMS3DModelToHDAT
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libgcc_s_seh-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libgcc_s_seh-1.dll
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libwinpthread-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/libstdc++-6.dll
            )
    add_custom_command(TARGET HPMSRoomToHDAT
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libgcc_s_seh-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libgcc_s_seh-1.dll
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libwinpthread-1.dll ${CMAKE_CURRENT_BINARY_DIR}/libwinpthread-1.dll
            COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/dist/libstdc++-6.dll ${CMAKE_CURRENT_BINARY_DIR}/libstdc++-6.dll
            )

endif ()