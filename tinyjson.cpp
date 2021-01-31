#include "tinyjson.h"
#include <string>



Json::Json() {
    type = tinyjson::JSON_NULL;
}

void Json::set_type(tinyjson::json_type atype) {
    type = atype;
}

tinyjson::json_type Json::json_get_type() {
    return type;
}

int Json_Parse::json_parse(const std::string &js_string) {
    str_buff = &js_string;
    str_size = str_buff->length();
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
        default:   return tinyjson::JSON_PARSE_INVALID_VALUE;
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
