#ifndef TINYJSON_H__
#define TINYJSON_H__

#include <string>
#include <vector>
#include <map>
namespace tinyjson{
//json数据类型
typedef enum { JSON_NULL = 0, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT } json_type;


//解析状态,原项目是手写的递归下降，状态机部分暂时空置
//typedef enum { } States;

//解析结果
enum {
    JSON_PARSE_OK = 0,                      //OK
    JSON_PARSE_EXPECT_VALUE,                //json空
    JSON_PARSE_INVALID_VALUE,               //数据无效
    JSON_PARSE_ROOT_NOT_SINGULAR,           //有效数据后含有其他非ws字符
    JSON_PARSE_NUMBER_TOO_BIG,              //数字过大
    JSON_PARSE_MISS_QUOTATION_MARK,         //引号缺失
    JSON_PARSE_INVALID_STRING_ESCAPE,       //无效转义
    JSON_PARSE_INVALID_STRING_CHAR,         //无效字符
    JSON_PARSE_INVALID_UNICODE_HEX,         //无效unicode数字
    JSON_PARSE_INVALID_UNICODE_SURROGATE,   //无效Unicode代理
    JSON_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,//逗号或右方括号缺失
    JSON_PARSE_MISS_KEY,                    //键缺失
    JSON_PARSE_MISS_COLON,                  //冒号缺失
    JSON_PARSE_MISS_COMMA_OR_CURLY_BRACKET  //逗号或右大括号缺失
};
}


/*!
* @brief:json数据结构
*
*/
class Json{
public:
    Json();

    explicit Json(tinyjson::json_type);

    explicit Json(std::string);

    explicit Json(double);

    explicit Json(std::vector<Json>);

    explicit Json(std::multimap<std::string,Json>);

    Json(const Json&);

    Json &Json::operator =(const Json &);

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
    * @brief:提供给派生类的string设置接口
    * @param:要设置的s
    * @return:void
    */
    void set_string(std::string astr);
    /*!
    * @brief:提供给派生类的array设置接口
    * @param:要设置的a
    * @return:void
    */
    void set_array(std::vector<Json> v);
    /*!
    * @brief:提供给派生类的object设置接口
    * @param:要设置的a
    * @return:void
    */
    void set_object(std::multimap<std::string,Json>);
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
    * @return:this.s.c_str()
    */
    const char* json_get_c_string() const;
    /*!
    * @brief:array查询接口
    * @param:
    * @return:this.a
    */
    std::vector<Json> json_get_array() const;
    /*!
    * @brief:m.size查询接口
    * @param:
    * @return:this.m.size()
    */
    size_t json_get_object_size() const;
    /*!
    * @brief:m查询接口
    * @param:
    * @return:this.m.
    */
    std::multimap<std::string,Json> json_get_object() const;
    /*!
    * @brief:m.key查询接口
    * @param:
    * @return:(this.m.begin()+i)->first
    */
    std::string json_get_object_key(size_t) const;
    /*!
    * @brief:m.key查询接口,c_string接口,为了和测试部分兼容
    * @param:
    * @return:(this.m.begin()+i)->first.c_str()
    */
    const char* json_get_object_key_cstr(size_t) const;
    /*!
    * @brief:value查询接口
    * @param:
    * @return:
    */
    Json json_get_object_value(size_t) const;
    /*!
    * @brief:key-value查询接口
    * @param:
    * @return:
    */
    Json json_get_object_value_by_key(std::string) const;
    /*!
    * @brief:重置接口
    * @param:
    * @return:
    */
    void clear();
private:
    tinyjson::json_type type;
    double n;
    std::string s;
    std::vector<Json> a;
    std::multimap<std::string,Json> m;
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
    int parse_array();
    int parse_object();
    size_t index;
    const std::string* str_buff;
    size_t str_size;
};

//class Json_Generate : private Json{
//
//};

#endif /*TINYJSON_H__*/