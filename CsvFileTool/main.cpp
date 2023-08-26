#include <vector>
#include <any>
#include <string>
#include <iostream>
#include "CsvParser.h"

using std::vector;
using std::string;
using std::any;

int main()
{
    CsvParser csvParser;

    // 打开CSV文件
    uint32_t result = csvParser.OpenCsvFile("data.csv");
    if (result != 0)
    {
        std::cout << "Failed to open CSV file" << std::endl;
        return 0;
    }

    // 添加一行数据
    std::vector<std::any> rowData = {1, "John", 30};
    result = csvParser.AddRow(rowData);
    if (result != 0)
    {
        std::cout << "Failed to add row" << std::endl;
        return 0;
    }

    // 添加一行数据
    rowData = {1, "ttt", 30};
    result = csvParser.AddRow(rowData);
    if (result != 0)
    {
        std::cout << "Failed to add row" << std::endl;
        return 0;
    }

    // 修改单元格数据
    result = csvParser.AlterCell(0, 1, "Mike");
    if (result != 0)
    {
        std::cout << "Failed to alter cell" << std::endl;
        return 0;
    }

    // 插入一行数据
    rowData = {1, "Mike", 30};
    result = csvParser.InsertRow(1, rowData);
    if (result != 0)
    {
        std::cout << "Failed to insert row" << std::endl;
        return 0;
    }

    // 删除一行数据
    result = csvParser.DeleteRow(0);
    if (result != 0)
    {
        std::cout << "Failed to delete row" << std::endl;
        return 0;
    }

    // 获取行数
    uint32_t rowCount = csvParser.GetRowCount();
    std::cout << "Row count: " << rowCount << std::endl;

    // 获取行内容
    vector<string> strRow = csvParser.GetRowData(0);
    for (auto &str : strRow)
    {
        std::cout << str << " ";
    }
    std::cout << std::endl;

    // 保存CSV文件
    result = csvParser.SaveCsvFile();
    if (result != 0)
    {
        std::cout << "Failed to save CSV file" << std::endl;
        return 0;
    }

    // 关闭CSV文件
    csvParser.CloseCsvFile();

    return 0;
}