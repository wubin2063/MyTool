#include <iostream>
#include "FileTool.h"

int main()
{
    FileTool fileTool;
    FileToolErrorCode_E result;

    // 测试文件打开
    result = fileTool.OpenFile("test.txt", std::ios::trunc); // 创建一个新文件
    if (result != E_OK)
    {
        std::cerr << "File open failed with error code: " << result << std::endl;
        return 1;
    }

    // 写入数据到文件
    std::vector<std::string> lines_to_write = {"Line 1", "Line 2", "Line 3"};
    result = fileTool.WriteFile(lines_to_write);
    if (result != E_OK)
    {
        std::cerr << "File write failed with error code: " << result << std::endl;
        return 1;
    }

    // 读取文件内容
    std::vector<std::string> lines_read;
    result = fileTool.ReadFile(lines_read);
    if (result != E_OK)
    {
        std::cerr << "File read failed with error code: " << result << std::endl;
        return 1;
    }

    // 输出读取到的内容
    for (const std::string &line : lines_read)
    {
        std::cout << line << std::endl;
    }

    // 关闭文件
    fileTool.CloseFile();

    return 0;
}
