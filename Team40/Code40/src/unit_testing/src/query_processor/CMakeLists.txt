file(GLOB catch_main "${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp")
file(GLOB srcs 
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/model/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/relationship_handler/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_util/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/test_util/*.h"
)

target_sources(unit_testing PRIVATE ${srcs})

# Automatically create pkb_testing, which contains only pkb unit tests.
add_executable(query_processor_testing ${catch_main} ${srcs})

# Ensures that unit_testing runs with spa library.
target_link_libraries(query_processor_testing query_processor)
