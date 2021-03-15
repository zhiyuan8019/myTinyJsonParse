#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "tinyjson.h"
#include <cstring>
#include "gtest/gtest.h"

using namespace tinyjson;

#define TEST_EQUAL(except, json_text)\
    do {\
        EXPECT_EQ(JSON_PARSE_OK,v.json_parse(json_text));\
        EXPECT_EQ(except, v.json_get_type());\
    } while(0)

#define TEST_NUMBER(expect, json_text)\
    do {\
        EXPECT_EQ(JSON_PARSE_OK,  v.json_parse(json_text));\
        EXPECT_EQ(JSON_NUMBER, v.json_get_type());\
        EXPECT_DOUBLE_EQ(expect, v.json_get_number());\
    } while(0)

#define TEST_STRING(expect, json_text)\
    do {\
        EXPECT_EQ(JSON_PARSE_OK,  v.json_parse(json_text));\
        EXPECT_EQ(JSON_STRING, v.json_get_type());\
        EXPECT_EQ(expect,v.json_get_string());\
    } while(0)

#define TEST_ERROR(error, json)\
    do {\
        EXPECT_EQ(error,v.json_parse(json));\
        EXPECT_EQ(JSON_NULL, v.json_get_type());\
    } while(0)

class JsonParseTest : public ::testing::Test {
protected:
    void SetUp() override {

    }

    // void TearDown() override {}

    Json_Parse v;
};


TEST_F(JsonParseTest, TestNull) {
    TEST_EQUAL(JSON_NULL,"null");
}

TEST_F(JsonParseTest,TestFalse){
    TEST_EQUAL(JSON_FALSE,"false");
}

TEST_F(JsonParseTest,TestTrue){
    TEST_EQUAL(JSON_TRUE,"true");
}

TEST_F(JsonParseTest,TestNumber){
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

TEST_F(JsonParseTest,TestString){
    TEST_STRING("", "\"\"");
    TEST_STRING("Hello", "\"Hello\"");
#if 1
    TEST_STRING("Hello\nWorld", "\"Hello\\nWorld\"");
    TEST_STRING("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
    //EQ断言读取expect中的'/0'会出现截断，使用STREQ进行cstr测试
    v.json_parse("\"Hello\\u0000World\"");
    EXPECT_STREQ("Hello\0World",v.json_get_c_string());
    TEST_STRING("\x24", "\"\\u0024\"");         /* Dollar sign U+0024 */
    TEST_STRING("\xC2\xA2", "\"\\u00A2\"");     /* Cents sign U+00A2 */
    TEST_STRING("\xE2\x82\xAC", "\"\\u20AC\""); /* Euro sign U+20AC */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\uD834\\uDD1E\"");  /* G clef sign U+1D11E */
    TEST_STRING("\xF0\x9D\x84\x9E", "\"\\ud834\\udd1e\"");  /* G clef sign U+1D11E */
#endif
}

TEST_F(JsonParseTest,TestArray){
#if 1
    v.json_parse("[ null , false , true , 123 , \"abc\" ]");
    auto result = v.json_get_array();
    EXPECT_EQ(JSON_NULL,result[0].json_get_type());
    EXPECT_EQ(JSON_FALSE,result[1].json_get_type());
    EXPECT_EQ(JSON_TRUE,result[2].json_get_type());
    EXPECT_EQ(JSON_NUMBER,result[3].json_get_type());
    EXPECT_EQ(JSON_STRING,result[4].json_get_type());
#endif
#if 1
    v.json_parse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]");
    auto result1 = v.json_get_array();
    EXPECT_EQ(JSON_ARRAY,result1[0].json_get_type());
    EXPECT_EQ(JSON_ARRAY,result1[1].json_get_type());
    EXPECT_EQ(JSON_ARRAY,result1[2].json_get_type());
    EXPECT_EQ(JSON_ARRAY,result1[3].json_get_type());
    auto arr2 = result1[1].json_get_array();
    EXPECT_EQ(JSON_NUMBER,arr2[0].json_get_type());
    auto arr3 = result1[2].json_get_array();
    EXPECT_EQ(JSON_NUMBER,arr3[0].json_get_type());
    EXPECT_EQ(JSON_NUMBER,arr3[1].json_get_type());
    auto arr4 = result1[3].json_get_array();
    EXPECT_EQ(JSON_NUMBER,arr4[0].json_get_type());
    EXPECT_EQ(JSON_NUMBER,arr4[1].json_get_type());
    EXPECT_EQ(JSON_NUMBER,arr4[2].json_get_type());
#endif
}

static void test_parse_object() {
    Json_Parse v;
    EXPECT_EQ(JSON_PARSE_OK, v.json_parse(" { } "));
    EXPECT_EQ(JSON_OBJECT, v.json_get_type());
    v.clear();
    EXPECT_EQ(JSON_PARSE_OK, v.json_parse(
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
    EXPECT_EQ(JSON_OBJECT, v.json_get_type());
    EXPECT_EQ(7, v.json_get_object_size());
    EXPECT_EQ(JSON_NULL,   v.json_get_object_value_by_key("n").json_get_type());
    EXPECT_EQ(JSON_FALSE,  v.json_get_object_value_by_key("f").json_get_type());
    EXPECT_EQ(JSON_TRUE,   v.json_get_object_value_by_key("t").json_get_type());
    EXPECT_EQ(JSON_NUMBER, v.json_get_object_value_by_key("i").json_get_type());
    EXPECT_DOUBLE_EQ(123.0, v.json_get_object_value_by_key("i").json_get_number());
    EXPECT_EQ(JSON_STRING, v.json_get_object_value_by_key("s").json_get_type());
    EXPECT_EQ("abc", v.json_get_object_value_by_key("s").json_get_string());
    EXPECT_EQ(JSON_ARRAY, v.json_get_object_value_by_key("a").json_get_type());
    EXPECT_EQ(JSON_OBJECT, v.json_get_object_value_by_key("o").json_get_type());
}

TEST_F(JsonParseTest,TestInvalidValue){
    //Expect Value
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE,"");
    TEST_ERROR(JSON_PARSE_EXPECT_VALUE," ");
    //Invalid
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
#if 1
    //Invalid array
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[1,]");
    TEST_ERROR(JSON_PARSE_INVALID_VALUE, "[\"a\", nul]");
#endif
}

TEST_F(JsonParseTest,TestRootNotSingular){
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR,"null x");

#if 1
    /* invalid number */
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0123"); /* after zero should be '.' , 'E' , 'e' or nothing */
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x0");
    TEST_ERROR(JSON_PARSE_ROOT_NOT_SINGULAR, "0x123");
#endif
}

TEST_F(JsonParseTest,TestMissQuotationMark){
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"");
    TEST_ERROR(JSON_PARSE_MISS_QUOTATION_MARK, "\"abc");
}

TEST_F(JsonParseTest,TestInvalidSrtingEscape){
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\v\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\'\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\0\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_ESCAPE, "\"\\x12\"");
}

TEST_F(JsonParseTest,TestInvalidStringChar){
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x01\"");
    TEST_ERROR(JSON_PARSE_INVALID_STRING_CHAR, "\"\x1F\"");
}

TEST_F(JsonParseTest,TestInvalidUnicodeHex){
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

TEST_F(JsonParseTest,TestInvalidUnicodeSurrogate){
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\\\\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uDBFF\"");
    TEST_ERROR(JSON_PARSE_INVALID_UNICODE_SURROGATE, "\"\\uD800\\uE000\"");
}

TEST_F(JsonParseTest,TestMissCommaOrSquareBracket){
#if 1
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1}");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[1 2");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET, "[[]");
#endif
}

TEST_F(JsonParseTest,TestMissKey){
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{1:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{true:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{false:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{null:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{[]:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{{}:1,");
    TEST_ERROR(JSON_PARSE_MISS_KEY, "{\"a\":1,");
}

TEST_F(JsonParseTest,TestMissColon){
    TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\"}");
    TEST_ERROR(JSON_PARSE_MISS_COLON, "{\"a\",\"b\"}");
}

TEST_F(JsonParseTest,TestMissCommaOrCurlyBracket){
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1]");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":1 \"b\"");
    TEST_ERROR(JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET, "{\"a\":{}");
}

TEST(JsonDumpTest,TestGenerate){
    Json_Parse js;
    std::string json_text = "{"
                            "\"title\": \"Design Patterns\","
                            "\"subtitle\": \"Elements of Reusable Object-Oriented Software\","
                            "\"author\": [\"Erich Gamma\",\"Richard Helm\",\"Ralph Johnson\",\"John Vlissides\"],"
                            "\"year\": 2009,"
                            "\"weight\": 1.8,"
                            "\"hardcover\": true,"
                            "\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"},"
                            "\"website\": null}";
    EXPECT_EQ(JSON_PARSE_OK,js.json_parse(json_text));
    EXPECT_EQ(JSON_OBJECT,js.json_get_type());
    std::string result = js.json_generater();
    printf("%s\n",result.c_str());
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}