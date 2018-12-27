cmake_minimum_required( VERSION 3.5 )

project( tabuada )
set(PROJECT_DESCRIPTION "Scalable, high performance, experimental web server")

#list( APPEND CMAKE_CXX_FLAGS "-O0 -std=c++11 ${CMAKE_CXX_FLAGS} -ggdb")
list( APPEND CMAKE_CXX_FLAGS "-O3 -std=c++17 ${CMAKE_CXX_FLAGS} -ggdb")

add_subdirectory(src)

add_executable(tabuada
  tabuada.c)

target_link_libraries(tabuada m)
target_link_libraries(tabuada gp)
target_link_libraries(tabuada fitexternal)
target_link_libraries(tabuada fitmult)
target_link_libraries(tabuada simplemachine)
