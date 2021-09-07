file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        # common/model/
        src/common/model/ConstantValue.cpp src/common/model/ConstantValue.h
        src/common/model/Factor.cpp src/common/model/Factor.h
        src/common/model/Procedure.cpp src/common/model/Procedure.h
        src/common/model/Variable.cpp src/common/model/Variable.h
        src/common/model/Program.cpp src/common/model/Program.h

        # common/model/condition/
        src/common/model/condition/condition.cpp src/common/model/condition/condition.h
        src/common/model/condition/AndCondition.cpp src/common/model/condition/AndCondition.h
        src/common/model/condition/NotCondition.cpp src/common/model/condition/NotCondition.h
        src/common/model/condition/OrCondition.cpp src/common/model/condition/OrCondition.h
        src/common/model/condition/SingleCondition.cpp src/common/model/condition/SingleCondition.h

        # common/model/expression/
        src/common/model/expression/Expression.cpp src/common/model/expression/Expression.h
        src/common/model/expression/SingleTermExpression.cpp src/common/model/expression/SingleTermExpression.h
        src/common/model/expression/SubtractTermsExpression.cpp src/common/model/expression/SubtractTermsExpression.h
        src/common/model/expression/SumTermsExpression.cpp src/common/model/expression/SumTermsExpression.h

        # common/model/relation/
        src/common/model/relation/Relation.cpp src/common/model/relation/Relation.h
        src/common/model/relation/EqualsRelation.cpp
        src/common/model/relation/LessThanOrEqualsRelation.cpp
        src/common/model/relation/LessThanRelation.cpp
        src/common/model/relation/MoreThanOrEqualsRelation.cpp
        src/common/model/relation/MoreThanRelation.cpp
        src/common/model/relation/NotEqualsRelation.cpp

        # common/model/statement/
        src/common/model/statement/Statement.cpp src/common/model/statement/Statement.h
        src/common/model/statement/AssignStatement.cpp
        src/common/model/statement/CallStatement.cpp
        src/common/model/statement/IfStatement.cpp
        src/common/model/statement/PrintStatement.cpp
        src/common/model/statement/ReadStatement.cpp
        src/common/model/statement/WhileStatement.cpp

        # common/model/term/
        src/common/model/term/Term.cpp src/common/model/term/Term.h
        src/common/model/term/DivideByFactorTerm.cpp
        src/common/model/term/ModuloByFactorTerm.cpp
        src/common/model/term/MultiplyByFactorTerm.cpp
        src/common/model/term/SingleFactorTerm.cpp

        #source_processor/
        src/source_processor/Parser.cpp src/source_processor/Parser.h
        src/source_processor/Line.cpp src/source_processor/Line.h
        )

# this makes the headers accessible for other projects which uses spa common
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


