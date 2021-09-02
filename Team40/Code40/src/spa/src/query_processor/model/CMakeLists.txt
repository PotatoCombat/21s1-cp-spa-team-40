#CMakeLists for query_processor/model

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query-processor-model ${srcs} ${headers})

add_subdirectory(entity)
add_subdirectory(relationship)

target_link_directories(query-processor-model PUBLIC entity/ relationship/)

target_link_libraries(query-processor-model query-processor-relationship)
#also transitively gets the "entity" dependency
