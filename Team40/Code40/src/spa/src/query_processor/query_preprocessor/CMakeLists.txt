#CMakeLists for query_processor/query_preprocessor

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(query-preprocessor ${srcs} ${headers})

add_subdirectory(pql_model)

target_include_directories(query-preprocessor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

target_link_libraries(query-preprocessor pql-model)
