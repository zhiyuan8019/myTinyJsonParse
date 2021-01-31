#ifndef TINYJSON_H__
#define TINYJSON_H__

#include <string>

namespace tinyjson{
//json所含类型
typedef enum { JSON_NULL = 0, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } json_type;
//json值
typedef struct {
    json_type type;
}json_value;
//解析状态
//typedef enum { } States;


//解析结果
enum {
    JSON_PARSE_OK = 0,              //OK
    JSON_PARSE_EXPECT_VALUE,        //json空
    JSON_PARSE_INVALID_VALUE,       //json无效
    JSON_PARSE_ROOT_NOT_SINGULAR    //根非单
};
}

class Json{
public:
    Json();
    ~Json() = default;
    void set_type(tinyjson::json_type atype);
    tinyjson::json_type json_get_type();
private:
    tinyjson::json_type type;
};

/*!
 *
 * @param:
 * @return:
 */
class Json_Parse : public Json{
public:
    Json_Parse();
    ~Json_Parse() = default;
    int json_parse(const std::string& js_string);
//  tinyjson::json_type json_get_type();
private:
    void parse_ws();
    int parse_value();
    int parse_null();
    int parse_false();
    int parse_true();
    size_t index;
    const std::string* str_buff;
    size_t str_size;
};

//class Json_Generate : private Json{
//
//};

#endif /*TINYJSON_H__*/