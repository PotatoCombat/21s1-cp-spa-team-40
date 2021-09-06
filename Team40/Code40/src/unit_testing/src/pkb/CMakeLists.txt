file(GLOB main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(pkb_testing ${main} ${srcs})

target_link_libraries(pkb_testing spa)

