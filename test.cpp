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
            main_ret = 1;                                \
         }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null() {
    Json_Parse v;
    EXPECT_EQ_INT(JSON_PARSE_OK, v.json_parse("null"));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());

}
static void test_parse_false() {
    Json_Parse v;
    EXPECT_EQ_INT(JSON_PARSE_OK, v.json_parse("false"));
    EXPECT_EQ_INT(JSON_FALSE, v.json_get_type());

}
static void test_parse_true() {
    Json_Parse v;
    EXPECT_EQ_INT(JSON_PARSE_OK, v.json_parse("true"));
    EXPECT_EQ_INT(JSON_TRUE, v.json_get_type());
}

static void test_parse_expect_value() {
    Json_Parse v;


    EXPECT_EQ_INT(JSON_PARSE_EXPECT_VALUE, v.json_parse(""));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());

    EXPECT_EQ_INT(JSON_PARSE_EXPECT_VALUE, v.json_parse(" "));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());
}

static void test_parse_invalid_value() {
    Json_Parse v;

    EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, v.json_parse("nul"));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());


    EXPECT_EQ_INT(JSON_PARSE_INVALID_VALUE, v.json_parse("?"));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());
}

static void test_parse_root_not_singular() {
    Json_Parse v;

    EXPECT_EQ_INT(JSON_PARSE_ROOT_NOT_SINGULAR, v.json_parse("null x"));
    EXPECT_EQ_INT(JSON_NULL, v.json_get_type());
}

static void test_parse() {
    test_parse_null();
    test_parse_true();
    test_parse_false();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}
