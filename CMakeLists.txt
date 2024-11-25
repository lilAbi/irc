cmake_minimum_required(VERSION 3.28)
project(irc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(src)

find_package(spdlog REQUIRED)
find_package(Boost REQUIRED)

include_directories(
        ${spdlog_INCLUDE_DIR}
        ${boost_INCLUDE_DIR}
)


file(GLOB sources CONFIGURE_DEPENDS
        "src/*.cpp"
)

add_executable(irc ${sources})

target_link_libraries(irc PUBLIC spdlog::spdlog boost::boost)

