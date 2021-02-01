#ifndef TINYJSON_H__
#define TINYJSON_H__

#include <string>

namespace tinyjson{
//json数据类型
typedef enum { JSON_NULL = 0, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } json_type;


//解析状态,原项目是手写的递归下降，状态机部分暂时空置
//typedef enum { } States;

//解析结果
enum {
    JSON_PARSE_OK = 0,                  //OK
    JSON_PARSE_EXPECT_VALUE,            //json空
    JSON_PARSE_INVALID_VALUE,           //数据无效
    JSON_PARSE_ROOT_NOT_SINGULAR,       //有效数据后含有其他非ws字符
    JSON_PARSE_NUMBER_TOO_BIG,          //数字过大
    JSON_PARSE_MISS_QUOTATION_MARK,     //引号缺失
    JSON_PARSE_INVALID_STRING_ESCAPE,   //无效转义
    JSON_PARSE_INVALID_STRING_CHAR,     //无效字符
    JSON_PARSE_INVALID_UNICODE_HEX,     //无效unicode数字
    JSON_PARSE_INVALID_UNICODE_SURROGATE//无效Unicode代理
};
}


/*!
* @brief:json数据结构
*
*/
class Json{
public:
    Json();
    ~Json();
    /*!
    * @brief:提供给派生类的设置接口
    * @param:要设置的type
    * @return:void
    */
    void set_type(tinyjson::json_type atype);
    /*!
    * @brief:提供给派生类的数字设置接口
    * @param:要设置的n
    * @return:void
    */
    void set_num(double adouble);
    /*!
    * @brief:提供给派生类的数字设置接口
    * @param:要设置的n
    * @return:void
    */
    void set_string(std::string astr);
    /*!
    * @brief:类型查询接口
    * @param:
    * @return:this.type
    */
    tinyjson::json_type json_get_type() const;
    /*!
    * @brief:boolean查询接口
    * @param:
    * @return:this.type
    */
    bool json_get_boolean() const;
    /*!
    * @brief:数字查询接口
    * @param:
    * @return:this.n
    */
    double  json_get_number() const;
    /*!
    * @brief:string查询接口
    * @param:
    * @return:this.s
    */
    std::string json_get_string() const;
    /*!
    * @brief:c_string查询接口,为了和测试部分兼容
    * @param:
    * @return:this.s
    */
    const char* json_get_c_string() const;
private:
    tinyjson::json_type type;
    union {
        double n;
        std::string s;
    };

};

/*!
* @brief:解释器类
*
*/
class Json_Parse : public Json{
public:
    Json_Parse();
    ~Json_Parse() = default;
    /*!
    * @brief:调用接口，接受一个string进行解析
    * @param:要解析的字符串
    * @return:解析结果
    */
    int json_parse(const std::string& js_string);

private:
    void parse_ws();
    int parse_value();
    int parse_null();
    int parse_false();
    int parse_true();
    int parse_number();
    int parse_string();
    int parse_hex4(unsigned &u);
    std::string encode_utf8(unsigned u);
    size_t index;
    const std::string* str_buff;
    size_t str_size;
};

//class Json_Generate : private Json{
//
//};

#endif /*TINYJSON_H__*/