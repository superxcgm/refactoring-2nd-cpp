cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(refactoring_cpp_2nd)



add_executable(main main.cc)
option(USE_JSON "Use json file" OFF)

if(USE_JSON)
    include(FetchContent)
    FetchContent_Declare(nlohmann
            GIT_REPOSITORY https://github.com/nlohmann/json.git
            GIT_TAG        v3.10.5
            )
    FetchContent_MakeAvailable(nlohmann)
    target_link_libraries(main nlohmann)
endif()

#add_subdirectory(src)