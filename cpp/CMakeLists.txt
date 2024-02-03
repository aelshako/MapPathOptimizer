cmake_minimum_required(VERSION 3.18)
project(cpp_test)
include(GoogleTest)
enable_testing()

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_library(
    osm_routing
    SHARED
    osm_routing.hpp
    utils.hpp
    osm_file_parser.hpp
    geojson_generator.hpp
    osm_routing.cpp
    utils.cpp
    osm_file_parser.cpp
    geojson_generator.cpp
    main.cpp
)

add_executable(
  osm
  main.cpp
)

add_executable(
  osm_routing_test
  osm_routing_test.cpp
)

target_link_libraries(
  osm
  osm_routing
)

 target_link_libraries(
   osm_routing_test
   GTest::gtest_main
 )

target_link_libraries(
  osm_routing_test
  osm_routing
)

gtest_discover_tests(osm_routing_test)

