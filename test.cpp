#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tinyjson.h"

using namespace tinyjson;

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
         }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define EXPECT_EQ_DOUBLE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%.17g")
#define EXPECT_EQ_STRING(expect, actual, alength) \
    EXPECT_EQ_BASE(sizeof(expect) - 1 == alength && memcmp(expect, actual, alength) == 0, expect, actual, "%s")
#define EXPECT_TRUE(actual) EXPECT_EQ_BASE((actual) != 0, "true", "false", "%s")
#define EXPECT_FALSE(actual) EXPECT_EQ_BASE((actual) == 0, "false", "true", "%s")
#define EXPECT_EQ_SIZE(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")
#define TEST_NUMBER(expect, json)\
    do {\
        Json_Parse v;\
        EXPECT_EQ_INT(JSON_PARSE_OK,  v.json_parse(json));\
        EXPECT_EQ_INT(JSON_NUMBER, v.json_get_type());\
        EXPECT_EQ_DOUBLE(expect, v.json_get_number());\
    } while(0)

#define TEST_ERROR(error, json)\
    do {\
        Json_Parse v;\
        EXPECT_EQ_INT(error,v.json_parse(json));\
        EXPECT_EQ_INT(JSON_NULL, v.json_get_type());\
    } while(0)

#define TEST_EQUAL(except, json)\
    do {\
        Json_Parse v;\
        EXPECT_EQ_INT(JSON_PARSE_OK,v.json_parse(json));\
        EXPECT_EQ_INT(except, v.json_get_type());\
    } while(0)

#define TEST_STRING(expect, json)\
    do {\
        Json_Parse v;\
        EXPECT_EQ_INT(JSON_PARSE_OK,  v.json_parse(json));\
        EXPECT_EQ_INT(JSON_STRING, v.json_get_type());\
        const char * result = v.json_get_c_string();\
        EXPECT_EQ_STRING(expect,result,v.json_get_string().length());\
        delete [] result;\
    } while(0)

static void test_parse_null() {
    TEST_EQUAL(JSON_NULL,"null");
}
static void test_parse_false() {
    TEST_EQUAL(JSON_FALSE,"false");
}
static void test_parse_true() {
    TEST_EQUAL(JSON_TRUE,"true");
}

static void test_parse_number() {
    TEST_NUMBER(0.0, "0");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
    TEST_NUMBER(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
    TEST_NUMBER( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
    TEST_NUMBER(-4.9406564584124654e-324, "-4.9406564584124654e-324");
    TEST_NUMBER( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
    TEST_NUMBER(-2.2250738585072009e-308, "-2.2250738585072009e-308");
    TEST_NUMBER( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
    TEST_NUMBER(-2.2250738585072014e-308, "-2.2250738585072014e-308");
    TEST_NUMBER( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
    TEST_NUMBER(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}


static void test_parse_expect_value() {
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE,"");
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE," ");
}

static void test_parse_invalid_value() {
    TEST_ERROR(JSON_PARSE_INVALID_VALUE,"nul");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE,"?");
#if  1
    /* invalid number */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+0");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "-");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "+1");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, ".123"); /* at least one digit before '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "1.");   /* at least one digit after '.' */
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "INF");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "inf");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "NAN");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "nan");
#endif
#if 1
    /* too big number */
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "1e309");
    TEST_ERROR(JSON_PARSE_NUMBER_TOO_BIG, "-1e309");
#endif
#if 0
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[1,]");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[\"a\", nul]");
#endif
}

static void test_parse_string() {
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
#if 1
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    TEST_STRING("Hello\0World", "\"Hello\\u0000World\"");
    TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
#endif
}

static void test_parse_array() {
#if 1
    Json_Parse v;
    v.json_parse("[ null , false , true , 123 , \"abc\" ]");
    auto result = v.json_get_array();
    EXPECT_EQ_INT(JSON_NULL,result[0].json_get_type());
    EXPECT_EQ_INT(JSON_FALSE,result[1].json_get_type());
    EXPECT_EQ_INT(JSON_TRUE,result[2].json_get_type());
    EXPECT_EQ_INT(JSON_NUMBER,result[3].json_get_type());
    EXPECT_EQ_INT(JSON_STRING,result[4].json_get_type());
#endif
#if 1
    Json_Parse v1;
    v1.json_parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]");
    auto result1 = v1.json_get_array();
    EXPECT_EQ_INT(JSON_ARRAY,result1[0].json_get_type());
    EXPECT_EQ_INT(JSON_ARRAY,result1[1].json_get_type());
    EXPECT_EQ_INT(JSON_ARRAY,result1[2].json_get_type());
    EXPECT_EQ_INT(JSON_ARRAY,result1[3].json_get_type());
    auto arr2 = result1[1].json_get_array();
    EXPECT_EQ_INT(JSON_NUMBER,arr2[0].json_get_type());
    auto arr3 = result1[2].json_get_array();
    EXPECT_EQ_INT(JSON_NUMBER,arr3[0].json_get_type());
    EXPECT_EQ_INT(JSON_NUMBER,arr3[1].json_get_type());
    auto arr4 = result1[3].json_get_array();
    EXPECT_EQ_INT(JSON_NUMBER,arr4[0].json_get_type());
    EXPECT_EQ_INT(JSON_NUMBER,arr4[1].json_get_type());
    EXPECT_EQ_INT(JSON_NUMBER,arr4[2].json_get_type());
#endif
}

static void test_parse_object() {
    Json_Parse v;
    EXPECT_EQ_INT(JSON_PARSE_OK, v.json_parse(" { } "));
    EXPECT_EQ_INT(JSON_OBJECT, v.json_get_type());
    v.clear();
    EXPECT_EQ_INT(JSON_PARSE_OK, v.json_parse(
                                            " { "
                                            "\"n\" : null , "
                                            "\"f\" : false , "
                                            "\"t\" : true , "
                                            "\"i\" : 123 , "
                                            "\"s\" : \"abc\", "
                                            "\"a\" : [ 1, 2, 3 ],"
                                            "\"o\" : { \"1\" : 1, \"2\" : 2, \"3\" : 3 }"
                                            " } "
    ));
    EXPECT_EQ_INT(JSON_OBJECT, v.json_get_type());
    EXPECT_EQ_SIZE(7, v.json_get_object_size());
    EXPECT_EQ_INT(JSON_NULL,   v.json_get_object_value_by_key("n").json_get_type());
    EXPECT_EQ_INT(JSON_FALSE,  v.json_get_object_value_by_key("f").json_get_type());
    EXPECT_EQ_INT(JSON_TRUE,   v.json_get_object_value_by_key("t").json_get_type());
    EXPECT_EQ_INT(JSON_NUMBER, v.json_get_object_value_by_key("i").json_get_type());
    EXPECT_EQ_DOUBLE(123.0, v.json_get_object_value_by_key("i").json_get_number());
    EXPECT_EQ_INT(JSON_STRING, v.json_get_object_value_by_key("s").json_get_type());
    EXPECT_EQ_STRING("abc", v.json_get_object_value_by_key("s").json_get_c_string(), v.json_get_object_value_by_key("s").json_get_string().length());
    EXPECT_EQ_INT(JSON_ARRAY, v.json_get_object_value_by_key("a").json_get_type());
    EXPECT_EQ_INT(JSON_OBJECT, v.json_get_object_value_by_key("o").json_get_type());
}



static void test_parse_root_not_singular() {
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR,"null x");

#if 1
    /* invalid number */
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
#endif
}

static void test_parse_missing_quotation_mark() {
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"abc");
}

static void test_parse_invalid_string_escape() {
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\v\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

static void test_parse_invalid_string_char() {
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}

static void test_parse_invalid_unicode_hex() {
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u01\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u012\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u/000\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\uG000\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0/00\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u0G00\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u00/0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u00G0\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u000/\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u000G\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_HEX, "\"\\u 123\"");
}

static void test_parse_invalid_unicode_surrogate() {
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uE000\"");
}

static void test_parse_miss_comma_or_square_bracket() {
#if 1
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
#endif
}

static void test_parse_miss_key() {
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{1:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{true:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{false:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{null:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{[]:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{{}:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{\"a\":1,");
}

static void test_parse_miss_colon() {
    TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\"}");
    TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\",\"b\"}");
}

static void test_parse_miss_comma_or_curly_bracket() {
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1]");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1 \"b\"");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":{}");
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_number();
    test_parse_string();
    test_parse_array();
    test_parse_object();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
    test_parse_missing_quotation_mark();
    test_parse_invalid_string_escape();
    test_parse_invalid_string_char();
    test_parse_invalid_unicode_hex();
    test_parse_invalid_unicode_surrogate();
    test_parse_miss_comma_or_square_bracket();
    test_parse_miss_key();
    test_parse_miss_colon();
    test_parse_miss_comma_or_curly_bracket();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
