#include "IniReader.h"
#include <iostream>

int main() {
    // 使用INIReader从config.ini文件中读取配置信息
    INIReader reader("config.ini");
    if (reader.ParseError() < 0) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    // 输出所有的节名
    std::cout << "Sections:" << std::endl;
    const std::set<std::string>& sections = reader.Sections();
    for (const std::string& section : sections) {
        std::cout << "[" << section << "]" << std::endl;
    }

    // 从指定节中获取配置项并输出
    std::cout << "Value from section 'Network': Port = " << reader.GetInteger("Network", "Port", 0) << std::endl;
    std::cout << "Value from section 'Display': Width = " << reader.GetInteger("Display", "Width", 800) << std::endl;
    std::cout << "Value from section 'Database': Username = " << reader.Get("Database", "Username", "default_user") << std::endl;
    std::cout << "Value from section 'Window': IsResizable = " << std::boolalpha << reader.GetBoolean("Window", "IsResizable", false) << std::endl;

    return 0;
}
