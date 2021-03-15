# myTinyJsonParse

## 本项目是项目[json-tutorial](https://github.com/miloyip/json-tutorial) 的C++"重构"版本

### 简介（Introduction）：
本项目基于c++11编写，能够对Json字符串进行递归下降的解析并能反向生成。使用gtest作为测试框架，包含基本的单元测试

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

### 注意

要运行单元测试，请确保gtest在环境内并确定cmake文件配置正确，或者使用test_old