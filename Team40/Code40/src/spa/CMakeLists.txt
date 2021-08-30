file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/lib/Model/Term.cpp
        src/lib/Model/Term.cpp src/lib/Model/ConstantValue.cpp
        src/lib/Model/Term.cpp src/lib/Model/ConstantValue.h
        src/lib/Model/Term.cpp src/lib/Model/Expression.h
        src/lib/Model/Term.cpp src/lib/Model/Factor.cpp
        src/lib/Model/Term.cpp src/lib/Model/Factor.h
        src/lib/Model/Term.cpp src/lib/Model/PrintStatement.cpp
        src/lib/Model/Term.cpp src/lib/Model/Statement.cpp
        src/lib/Model/Term.cpp src/lib/Model/Statement.h
        src/lib/Model/Term.cpp src/lib/Model/Term.cpp
        src/lib/Model/Term.cpp src/lib/Model/Term.h
        src/lib/Model/Term.cpp src/lib/Model/Variable.cpp
        src/lib/Model/Term.cpp src/lib/Model/Variable.h
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


