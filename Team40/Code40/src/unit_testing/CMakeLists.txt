file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} src/common/models/TestConstantValue.cpp src/common/models/TestVariable.cpp src/common/models/term/TestSingleFactorTerm.cpp src/common/models/term/TestMultiplyByFactorTerm.cpp)

target_link_libraries(unit_testing spa)

