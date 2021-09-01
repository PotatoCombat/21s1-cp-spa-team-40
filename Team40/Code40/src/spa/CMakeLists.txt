file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/common/model/ConstantValue.cpp src/common/model/ConstantValue.h
        src/common/model/expression/Expression.cpp src/common/model/expression/Expression.h
        src/common/model/Factor.cpp src/common/model/Factor.h
        src/common/model/statement/Statement.cpp src/common/model/statement/Statement.h
        src/common/model/term/Term.cpp src/common/model/Term.h
        src/common/model/Variable.cpp src/common/model/Variable.h
        src/common/model/SingleTermExpression.cpp src/common/model/SingleTermExpression.h src/common/model/SumTermsExpression.cpp src/common/model/SumTermsExpression.h src/common/model/SubtractTermsExpression.cpp src/common/model/SubtractTermsExpression.h)
# this makes the headers accessible for other projects which uses spa common
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


