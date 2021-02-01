#include "tinyjson.h"
#include <string>
#include <cctype>
#include <stdexcept>
#include <cstring>

Json::Json() {
    type = tinyjson::JSON_NULL;
    n = 0;
}

Json::~Json() {
    if(type==tinyjson::JSON_STRING)s.std::string::~string();
}

void Json::set_type(tinyjson::json_type atype) {
    if(type ==tinyjson::JSON_STRING){
        s.std::string::~string();
    }
    type = atype;
}

tinyjson::json_type Json::json_get_type() const{
    return type;
}

double Json::json_get_number() const {
    if(type ==tinyjson::JSON_NUMBER){
        return n;
    }else{
        return -1;
    }

}

void Json::set_num(double adouble) {
    if(type == tinyjson::JSON_STRING){
        s.std::string::~string();
    }
    n = adouble;
}

std::string Json::json_get_string() const {
    if(type ==tinyjson::JSON_STRING){
        return s;
    }else {
        return "";
    }
}

const char* Json::json_get_c_string() const {
    char *p = new char[s.length()+1];
    memcpy(p, s.c_str(), s.length() + 1);
    //此处strcpy_s会出现遇字符串中间的\0终止的问题，使用内存级拷贝避免
    //strcpy_s(p,s.length()+1,s.c_str());
    return p;
}

void Json::set_string(std::string astr) {
    new(&s) std::string (astr);
}

bool Json::json_get_boolean() const {
    if(type==tinyjson::JSON_TRUE||type==tinyjson::JSON_TRUE){
        return type==tinyjson::JSON_TRUE;
    }else {
        return false;
    }
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
        case '"':  return parse_string();
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
            set_num(0);
            return tinyjson::JSON_PARSE_OK;
        }
        return tinyjson::JSON_PARSE_NUMBER_TOO_BIG;
    }
    set_type(tinyjson::JSON_NUMBER);
    return tinyjson::JSON_PARSE_OK;

}

int Json_Parse::parse_string() {
    std::string str = "";
    unsigned u;
    //str.push_back(str_buff->at(index));
    ++index;//前双引号
    for(;index<str_size;++index){
        char c = str_buff->at(index);
        switch(c){
            case '\"':
                //str.push_back(str_buff->at(index));
                ++index;
                set_string(str);
                set_type(tinyjson::JSON_STRING);
                return tinyjson::JSON_PARSE_OK;
            case '\\':
                ++index;
                if(index>=str_size)return tinyjson::JSON_PARSE_INVALID_VALUE;
                switch(str_buff->at(index)){
                    case '\"': str.push_back('\"'); break;
                    case '\\': str.push_back('\\'); break;
                    case '/':  str.push_back('/' ); break;
                    case 'b':  str.push_back('\b'); break;
                    case 'f':  str.push_back('\f'); break;
                    case 'n':  str.push_back('\n'); break;
                    case 'r':  str.push_back('\r'); break;
                    case 't':  str.push_back('\t'); break;
                    case 'u':{
                        ++index;
                        if(parse_hex4(u)!=tinyjson::JSON_PARSE_OK){
                            return tinyjson::JSON_PARSE_INVALID_UNICODE_HEX;
                        }
                        if(u>=0xD800&&u<=0xDBFF){
                            ++index;
                            if(index+5>=str_size)return tinyjson::JSON_PARSE_INVALID_UNICODE_SURROGATE;
                            if(str_buff->at(index)!='\\'||str_buff->at(index+1)!='u')return tinyjson::JSON_PARSE_INVALID_UNICODE_SURROGATE;
                            index+=2;
                            unsigned u2=0;
                            if(parse_hex4(u2)!=tinyjson::JSON_PARSE_OK){
                                return tinyjson::JSON_PARSE_INVALID_UNICODE_SURROGATE;
                            }
                            if (u2 < 0xDC00 || u2 > 0xDFFF)return tinyjson::JSON_PARSE_INVALID_UNICODE_SURROGATE;
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }
                        std::string utf = encode_utf8(u);
                        str.append(utf);
                        break;
                    }
                    default:
                        return tinyjson::JSON_PARSE_INVALID_STRING_ESCAPE;
                }
                break;
            default:
                if((unsigned char)c<0x20){
                    return tinyjson::JSON_PARSE_INVALID_STRING_CHAR;
                } else{
                    str.push_back(str_buff->at(index));
                }
        }
    }
    return tinyjson::JSON_PARSE_MISS_QUOTATION_MARK;
}

int Json_Parse::parse_hex4(unsigned int &u) {
    u = 0;
    if(index+3>=str_size)return tinyjson::JSON_PARSE_INVALID_UNICODE_HEX;
    for(int i=0;i<4;++i){
        char ch = str_buff->at(index+i);
        u <<=4;
        if(ch>='0'&&ch<='9') u |= ch-'0';
        else if(ch>='a'&&ch<='f')u|= ch-'a'+10;
        else if(ch>='A'&&ch<='F')u|= ch-'A'+10;
        else{
            return tinyjson::JSON_PARSE_INVALID_UNICODE_HEX;
        }
    }
    index+=3;
    return tinyjson::JSON_PARSE_OK;
}

std::string Json_Parse::encode_utf8(unsigned int u) {
    std::string result;
    if (u <= 0x7F)
        if(u==0)result+='\0';
        else {
            result.push_back(u & 0xFF);
        }
    else if (u <= 0x7FF) {
        result.push_back(0xC0 | ((u >> 6) & 0xFF));
        result.push_back(0x80 | ( u       & 0x3F));
    }
    else if (u <= 0xFFFF) {
        result.push_back(0xE0 | ((u >> 12) & 0xFF));
        result.push_back(0x80 | ((u >>  6) & 0x3F));
        result.push_back(0x80 | ( u        & 0x3F));
    }
    else {
        result.push_back(0xF0 | ((u >> 18) & 0xFF));
        result.push_back(0x80 | ((u >> 12) & 0x3F));
        result.push_back(0x80 | ((u >>  6) & 0x3F));
        result.push_back(0x80 | ( u        & 0x3F));
    }
    return result;
}
