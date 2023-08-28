//
// Created by 28153 on 2023/8/26.
//

#ifndef INIREADER_H
#define INIREADER_H

#include <map>
#include <set>
#include <string>

class INIReader {
public:
    // 构造函数，接受INI文件名并解析它
    INIReader(const std::string& filename);

    // 获取解析错误的状态码
    int ParseError() const;

    // 获取INI文件中所有的节名
    const std::set<std::string>& Sections() const;

    // 获取指定节和键的字符串值，如果未找到则返回默认值
    std::string Get(const std::string& section, const std::string& name, const std::string& default_value) const;

    // 获取指定节和键的整数值，如果未找到或不是有效整数则返回默认值
    long GetInteger(const std::string& section, const std::string& name, long default_value) const;

    // 获取指定节和键的浮点数值，如果未找到或不是有效浮点数则返回默认值
    double GetReal(const std::string& section, const std::string& name, double default_value) const;

    // 获取指定节和键的单精度浮点数值，如果未找到或不是有效浮点数则返回默认值
    float GetFloat(const std::string& section, const std::string& name, float default_value) const;

    // 获取指定节和键的布尔值，如果未找到或不是有效布尔值则返回默认值
    bool GetBoolean(const std::string& section, const std::string& name, bool default_value) const;

private:
    int _error; // 解析错误状态码
    std::map<std::string, std::string> _values; // 存储键值对的容器
    std::set<std::string> _sections; // 存储节名的容器
    // 生成键名以便在_map中查找
    static std::string MakeKey(const std::string& section, const std::string& name);
    // 解析INI文件时的回调函数
    static int ValueHandler(void* user, const char* section, const char* name, const char* value);
};

#endif // INIREADER_H

