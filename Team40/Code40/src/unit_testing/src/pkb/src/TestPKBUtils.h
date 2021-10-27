#include <set>
#include <string>
#include <vector>

#include "catch.hpp"

using namespace std;

struct TestPKBUtils {
public:
    // Underscore == Wildcard
    inline static string WILDCARD_VARNAME = "_";

    // Empty List
    inline static vector<string> WILDCARD_EXPRLIST;

    // Standard
    inline static string EXPRESSION_1 = "v + x * y + z * t";
    inline static vector<string> INFIX_1 = TestUtils::tokenizePattern(EXPRESSION_1);
    inline static string EXACT_1 = "v x y*+ z t*+";
    inline static set<string> PARTIAL_1 = {
        "v",
        "x",
        "y",
        "x y*",
        "v x y*+",
        "z",
        "t",
        "z t*",
        "v x y*+ z t*+",
    };

    // Prioritise brackets
    inline static string EXPRESSION_2 = "( ( x + 10 ) * ( y - z ) / 5 )";
    inline static vector<string> INFIX_2 = TestUtils::tokenizePattern(EXPRESSION_2);
    inline static string EXACT_2 = "x 10+ y z-* 5/";
    inline static set<string> PARTIAL_2 = {
        "x",
        "10",
        "x 10+",
        "y",
        "z",
        "y z-",
        "x 10+ y z-*",
        "5",
        "x 10+ y z-* 5/",
    };

    // Nested from right
    inline static string EXPRESSION_3 = "( a + ( b - ( c * ( d / ( e % ( f ) ) ) ) ) )";
    inline static vector<string> INFIX_3 = TestUtils::tokenizePattern(EXPRESSION_3);
    inline static string EXACT_3 = "a b c d e f%/*-+";
    inline static set<string> PARTIAL_3 = {
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "e f%",
        "d e f%/",
        "c d e f%/*",
        "b c d e f%/*-",
        "a b c d e f%/*-+",
    };

    // Nested from left
    inline static string EXPRESSION_4 = "( ( ( ( ( ( a ) % b ) * c ) / d ) + e ) - f )";
    inline static vector<string> INFIX_4 = TestUtils::tokenizePattern(EXPRESSION_4);
    inline static string EXACT_4 = "a b% c* d/ e+ f-";
    inline static set<string> PARTIAL_4 = {
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "a b%",
        "a b% c*",
        "a b% c* d/",
        "a b% c* d/ e+",
        "a b% c* d/ e+ f-",
    };

    // INFIX: ( ( x + 10 ) * ( y - z ) / 5 )
    // POSTFIX: "x 10+ y z-* 5/"
    inline static vector<string> QUERIES_2 {
        "x",
        "10",
        "x + 10",
        "y",
        "z",
        "y - z",
        "( x + 10 ) * ( y - z )",
        "5",
        "( x + 10 ) * ( y - z ) / 5",
        "( x )",
        "( 10 )",
        "( x + 10 )",
        "( y )",
        "( z )",
        "( y - z )",
        "( ( x + 10 ) * ( y - z ) )",
        "( 5 )",
        "( ( x + 10 ) * ( y - z ) / 5 )",
        "( x ) + 10",
        "x + ( 10 )",
        "( y ) - z",
        "y - ( z )",
        "( ( x ) + 10 ) * ( y - ( z ) )",
        "( x + ( 10 ) ) * ( ( y ) - z )",
        "( ( x + 10 ) ) * ( y - z )",
        "( x + 10 ) * ( ( y - z ) )",
        "( ( x + 10 ) ) * ( ( y - z ) )",
        "( ( x + 10 ) ) * ( y - z ) / 5",
        "( x + 10 ) * ( ( y - z ) ) / 5",
        "( ( x + 10 ) ) * ( ( y - z ) ) / 5",
    };
};
