cmake_minimum_required(VERSION 3.28)
project(irc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fexperimental-library")
set(CMAKE_EXE_LINKER_FLAGS "")

include_directories(src)

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)


include_directories(
        ${spdlog_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
)


file(GLOB sources CONFIGURE_DEPENDS
        "src/*.cpp"
)

add_executable(irc ${sources})

if(WIN32)
    target_link_libraries(irc PUBLIC
            spdlog::spdlog
            boost::boost
            "libws2_32.a"
    )
else()
    target_link_libraries(irc PUBLIC
            spdlog::spdlog
            boost::boost
    )
endif()

