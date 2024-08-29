cmake_minimum_required(VERSION 3.8)
project(Pseudo3DGame)

set(CMAKE_CXX_STANDARD 17)

# DEBUG in RELEASE
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG")

add_compile_options("$<$<CONFIG:RELEASE>:/Zi>")
add_link_options("$<$<CONFIG:RELEASE>:/DEBUG /OPT:REF /OPT:ICF>")


file(GLOB_RECURSE FILES "Implementation/*.cpp" "Implementation/*.h")

add_executable(Pseudo3DGame ${FILES} main.cpp)

#set_target_properties(Pseudo3DGame PROPERTIES WIN32_EXECUTABLE TRUE)

if(APPLE OR UNIX)
    SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

    include_directories(/usr/local/include)
else()
    SET(SFML_DIR "D:/Documents/Projects/repos/Libraries/SFML-2.6.1/lib/cmake/SFML")
    SET(SFML_STATIC_LIBRARIES TRUE)

    find_package(Threads REQUIRED)
    target_link_libraries(Pseudo3DGame Threads::Threads)

endif()

find_package(SFML 2.6.1 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Pseudo3DGame sfml-audio sfml-network sfml-graphics sfml-window sfml-system)
endif()

