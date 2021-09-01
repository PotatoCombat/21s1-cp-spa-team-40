#CMakeLists for query_processor


add_subdirectory(model)

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query_processor ${srcs} ${headers})

target_link_directories(query_processor PUBLIC model/)

target_link_libraries(query_processor model)
#also transitively gets the "entity" and "relationship" dependency