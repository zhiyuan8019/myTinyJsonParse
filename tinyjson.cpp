#include "tinyjson.h"
#include <string>
#include <cctype>
#include <stdexcept>


Json::Json() {
    type = tinyjson::JSON_NULL;
    n = 0;
}

void Json::set_type(tinyjson::json_type atype) {
    type = atype;
}

tinyjson::json_type Json::json_get_type() const{
    return type;
}

double Json::json_get_number() const {
    return n;
}

void Json::set_num(double adouble) {
    n = adouble;
}


int Json_Parse::json_parse(const std::string &js_string) {
    str_buff = &js_string;
    str_size = str_buff->length();
    index  = 0;
    set_type(tinyjson::JSON_NULL);
    if(str_size==0)return tinyjson::JSON_PARSE_EXPECT_VALUE;
    int parse_result;
    parse_ws();
    if((parse_result = parse_value())==tinyjson::JSON_PARSE_OK){
        parse_ws();
        if(index<str_size){
            set_type(tinyjson::JSON_NULL);
            return tinyjson::JSON_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    return parse_result;
}

Json_Parse::Json_Parse() {
    index = 0;
    str_buff = nullptr;
    str_size = 0;
}

void Json_Parse::parse_ws() {
    while (index<str_size && (str_buff->at(index) == ' ' || str_buff->at(index) == '\t' || str_buff->at(index) == '\n' || str_buff->at(index) == '\r')){
        ++index;
    }
}

int Json_Parse::parse_value() {
    if(index==str_size)return tinyjson::JSON_PARSE_EXPECT_VALUE;
    switch (str_buff->at(index)) {
        case 'n':  return parse_null();
        case 'f':  return parse_false();
        case 't':  return parse_true();
        default:   return parse_number();
    }
}

int Json_Parse::parse_null() {
    if(index+3>=str_size)return tinyjson::JSON_PARSE_INVALID_VALUE;
    if(str_buff->at(index+1)!='u'||str_buff->at(index+2)!='l'||str_buff->at(index+3)!='l')
    {
        return tinyjson::JSON_PARSE_INVALID_VALUE;
    }
    index+=4;
    set_type(tinyjson::JSON_NULL);
    return tinyjson::JSON_PARSE_OK;
}

int Json_Parse::parse_false() {
    if(index+4>=str_size)return tinyjson::JSON_PARSE_INVALID_VALUE;
    if(str_buff->at(index+1)!='a'||str_buff->at(index+2)!='l'||str_buff->at(index+3)!='s'||str_buff->at(index+4)!='e')
    {
        return tinyjson::JSON_PARSE_INVALID_VALUE;
    }
    index+=5;
    set_type(tinyjson::JSON_FALSE);
    return tinyjson::JSON_PARSE_OK;
}

int Json_Parse::parse_true() {
    if(index+3>=str_size)return tinyjson::JSON_PARSE_INVALID_VALUE;
    if(str_buff->at(index+1)!='r'||str_buff->at(index+2)!='u'||str_buff->at(index+3)!='e')
    {
        return tinyjson::JSON_PARSE_INVALID_VALUE;
    }
    index+=4;
    set_type(tinyjson::JSON_TRUE);
    return tinyjson::JSON_PARSE_OK;
}
/*!
* @brief:数字解析，由于stod()是json数字语法规则的超集，需要先检测
* @param:
* @return:
*/
int Json_Parse::parse_number() {
    std::string num;
    bool isNegOfDex = false;
    size_t left =index;
    if(str_buff->at(index)=='-')++index;
    if(index<str_size&&str_buff->at(index)=='0'){
        ++index;
    }else{
        if(index>=str_size||!isdigit(str_buff->at(index)))return tinyjson::JSON_PARSE_INVALID_VALUE;
        while(index<str_size&&isdigit(str_buff->at(index))) {
            ++index;
        }
    }
    if(index<str_size&&str_buff->at(index)=='.'){
        ++index;
        if(index>=str_size||!isdigit(str_buff->at(index))){
            return tinyjson::JSON_PARSE_INVALID_VALUE;
        }
        while(index<str_size&&isdigit(str_buff->at(index))) {
            ++index;
        }
    }
    if(index<str_size&&(str_buff->at(index)=='e'||str_buff->at(index)=='E')){
        ++index;
        if (index<str_size&&(str_buff->at(index) == '+' || str_buff->at(index) == '-')){
            if(str_buff->at(index) == '-')isNegOfDex = true;
            ++index;
        }
        if(index>=str_size||!isdigit(str_buff->at(index))){
            return tinyjson::JSON_PARSE_INVALID_VALUE;
        }
        while(index<str_size&&isdigit(str_buff->at(index))) {
            ++index;
        }
    }
    num = str_buff->substr(left,index-left+1);
    try {
        double tmp= std::stod(num);
        set_num(tmp);
    } catch (const std::out_of_range& oor) {
        /*!
         * @warning:此处是危险的实现，因为直接使用指数符号为负判断是下溢
         */
        if(isNegOfDex){
            set_type(tinyjson::JSON_NUMBER);
            return tinyjson::JSON_PARSE_OK;
        }
        return tinyjson::JSON_PARSE_NUMBER_TOO_BIG;
    }
    set_type(tinyjson::JSON_NUMBER);
    return tinyjson::JSON_PARSE_OK;

}
