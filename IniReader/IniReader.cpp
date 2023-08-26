//
// Created by 28153 on 2023/8/26.
//

#include "IniReader.h"
#include <fstream>
#include <algorithm>
#include <cctype>

// 构造函数，接受INI文件名并解析它
INIReader::INIReader(const std::string& filename) {
    _error = 0;
    std::ifstream file(filename);

    if (!file.is_open()) {
        _error = -1;
        return;
    }

    std::string line;
    std::string current_section;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            if (line[0] == '[' && line[line.size() - 1] == ']') {
                current_section = line.substr(1, line.size() - 2);
                _sections.insert(current_section);
            } else {
                size_t pos = line.find('=');
                if (pos != std::string::npos) {
                    std::string name = line.substr(0, pos);
                    std::string value = line.substr(pos + 1);
                    _values[MakeKey(current_section, name)] = value;
                }
            }
        }
    }
}

// 获取解析错误的状态码
int INIReader::ParseError() const {
    return _error;
}

// 获取INI文件中所有的节名
const std::set<std::string>& INIReader::Sections() const {
    return _sections;
}

// 获取指定节和键的字符串值，如果未找到则返回默认值
std::string INIReader::Get(const std::string& section, const std::string& name, const std::string& default_value) const {
    std::string key = MakeKey(section, name);
    return _values.count(key) ? _values.at(key) : default_value;
}

// 获取指定节和键的整数值，如果未找到或不是有效整数则返回默认值
long INIReader::GetInteger(const std::string& section, const std::string& name, long default_value) const {
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

// 获取指定节和键的浮点数值，如果未找到或不是有效浮点数则返回默认值
double INIReader::GetReal(const std::string& section, const std::string& name, double default_value) const {
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

// 获取指定节和键的单精度浮点数值，如果未找到或不是有效浮点数则返回默认值
float INIReader::GetFloat(const std::string& section, const std::string& name, float default_value) const {
    std::string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    float n = strtof(value, &end);
    return end > value ? n : default_value;
}

// 获取指定节和键的布尔值，如果未找到或不是有效布尔值则返回默认值
bool INIReader::GetBoolean(const std::string& section, const std::string& name, bool default_value) const {
    std::string valstr = Get(section, name, "");
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

// 生成键名以便在_map中查找
std::string INIReader::MakeKey(const std::string& section, const std::string& name) {
    std::string key = section + "=" + name;
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

// 解析INI文件时的回调函数
int INIReader::ValueHandler(void* user, const char* section, const char* name, const char* value) {
    auto* reader = (INIReader*)user;
    std::string key = MakeKey(section, name);
    if (reader->_values.count(key) > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;
    reader->_sections.insert(section);
    return 1;
}
