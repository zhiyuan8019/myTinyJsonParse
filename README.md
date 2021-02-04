# myTinyJsonParse

## 本项目是项目[json-tutorial](https://github.com/miloyip/json-tutorial) 的C++"重构"版本

### 简介（Introduction）：
本项目基于c++11编写，能够对Json字符串进行递归下降的解析并能反向生成。包含基本的测试框架。

### 调用：

```c++
#include "tinyjson.h"
#include <string>
#include <iostream>

std::string json_text = "{"
                            "\"title\": \"Design Patterns\","
                            "\"subtitle\": \"Elements of Reusable Object-Oriented Software\","
                            "\"author\": [\"Erich Gamma\",\"Richard Helm\",\"Ralph Johnson\",\"John Vlissides\"],"
                            "\"year\": 2009,"
                            "\"weight\": 1.8,"
                            "\"hardcover\": true,"
                            "\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"},"
                            "\"website\": null}";
Json_Parse js;
//解析
js.json_parse(json_text);
//生成
std::string result = js.json_generater();
std::cout<< result <<std::endl;
```
