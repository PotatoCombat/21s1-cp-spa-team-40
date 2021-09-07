#CMakeLists for query_processor/model/relationship

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query-processor-relationship STATIC ${srcs} ${headers})
target_link_libraries(query-processor-relationship query-processor-entity)
