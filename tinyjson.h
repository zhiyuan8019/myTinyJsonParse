#ifndef TINYJSON_H__
#define TINYJSON_H__

#include <string>

namespace tinyjson{
//json所含类型
typedef enum { JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } json_type;
//json值
typedef struct {
    json_type type;
}json_value;
//解析状态
enum {
    JSON_PARSE_OK = 0,              //OK
    JSON_PARSE_EXPECT_VALUE,        //json空
    JSON_PARSE_INVALID_VALUE,       //json无效
    JSON_PARSE_ROOT_NOT_SINGULAR    //根非单
};
//json解析 ，返回状态
//re：状态；para：解析值，json文本
int json_parse(json_value* v, const std::string& json);
//获取json_value的json_type
//re：json_type；para：json_value
json_type json_get_type(const json_value* v);
}


class Json{

};

class Json_Parse : private Json{

};

class Json_Generate : private Json{

};

#endif /*TINYJSON_H__*/