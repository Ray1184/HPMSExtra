# Runtime project
cmake_minimum_required(VERSION 3.15)
project(HPMSExtra)

set(CMAKE_SHARED_LIBRARY_PREFIX)
set(CMAKE_STATIC_LIBRARY_PREFIX)

include_directories(include)

# Third party
include(Shared.cmake)

install_from_repo(https://github.com/Ray1184/HPMSEngine.git HPMSEngine master TRUE /include/)

install_from_repo(https://github.com/assimp/assimp.git ASSIMP master TRUE /include/)

install_from_repo(https://github.com/g-truc/glm.git GLM master TRUE /)

install_from_repo(https://github.com/mtrempoltsev/pods.git PODS master FALSE /include/)

install_from_repo(https://github.com/zeux/pugixml.git PUGIXML master TRUE /src/)

include_directories(${DEP_PATH}/glfw-src/include)
include_directories(${DEP_PATH}/glm-src)
include_directories(${DEP_PATH}/physfs-src/src)

add_executable(HPMSAssimpToHDAT src/HPMSRoomToHDATMain.cpp src/tools/AssimpImporter.cpp)

add_executable(HPMSRoomToHDAT src/HPMSAssimpToHDATMain.cpp src/tools/RoomImporter.cpp)

target_link_libraries(HPMSAssimpToHDAT assimp HPMSEngine)

target_link_libraries(HPMSRoomToHDAT assimp HPMSEngine)