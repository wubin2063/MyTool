//
// Created by wubin on 2023/8/26.
//
#include "CsvParser.h"

using std::string;
using std::vector;
using std::fstream;
using std::any;

CsvParser::CsvParser() = default;

CsvParser::~CsvParser()
{
    CloseCsvFile();
}

uint32_t CsvParser::OpenCsvFile(const string &strFilePath)
{
    CloseCsvFile();

    fsCsvFile.open(strFilePath, std::ios::in);
    if (!fsCsvFile.is_open())
    {
        // 文件不存在，创建空文件
        fsCsvFile.open(strFilePath, std::ios::out);
        fsCsvFile.close();

        // 重新打开文件
        fsCsvFile.open(strFilePath, std::ios::in);
        if (!fsCsvFile.is_open())
        {
            return 0x00000001; // 打开文件失败
        }
    }

    // 读取文件内容
    string line;
    while (std::getline(fsCsvFile, line))
    {
        vector<any> row;
        size_t startPos = 0;
        size_t commaPos = line.find(',');
        while (commaPos != string::npos)
        {
            string cellData = line.substr(startPos, commaPos - startPos);
            row.emplace_back(cellData);
            startPos = commaPos + 1;
            commaPos = line.find(',', startPos);
        }
        string cellData = line.substr(startPos);
        row.emplace_back(cellData);
        vecCsvData.push_back(row);
    }
    this->m_strFilePath = strFilePath;
    return 0;
}

void CsvParser::ClearCsvFile()
{
    vecCsvData.clear();
}

uint32_t CsvParser::AddRow(const vector<any> &vecRowData)
{
    if (vecRowData.empty())
    {
        return 0x00000002; // 文件内容错误
    }

    vecCsvData.push_back(vecRowData);
    return 0;
}

uint32_t CsvParser::InsertRow(uint32_t nRowIndex, const vector<any> &vecRowData)
{
    if (vecRowData.empty())
    {
        return 0x00000002; // 文件内容错误
    }

    if (nRowIndex > vecCsvData.size())
    {
        return 0x00000003; // 插入位置错误
    }

    vecCsvData.insert(vecCsvData.begin() + nRowIndex, vecRowData);
    return 0;
}

uint32_t CsvParser::AlterCell(uint32_t nRowIndex, uint32_t nColIndex, const any &anyData)
{
    if (nRowIndex >= vecCsvData.size() || nColIndex >= vecCsvData[nRowIndex].size())
    {
        return 0x00000003; // 修改位置错误
    }

    vecCsvData[nRowIndex][nColIndex] = anyData;
    return 0;
}

uint32_t  CsvParser::GetRowCount()
{
    return vecCsvData.size();
}

std::vector<std::string> CsvParser::GetRowData(uint32_t nRowIndex)
{
    if (nRowIndex >= vecCsvData.size())
    {
        return {};
    }

    std::vector<std::string> rowData;
    for (const auto& cell : vecCsvData[nRowIndex])
    {
        std::string cellData;
        try
        {
            if (cell.type() == typeid(std::string))
            {
                cellData = std::any_cast<std::string>(cell);
            }
            else if (cell.type() == typeid(int))
            {
                cellData = std::to_string(std::any_cast<int>(cell));
            }
            else if (cell.type() == typeid(uint32_t))
            {
                cellData = std::to_string(std::any_cast<uint32_t>(cell));
            }
            else if (cell.type() == typeid(double))
            {
                cellData = std::to_string(std::any_cast<double>(cell));
            }
            else if (cell.type() == typeid(float))
            {
                cellData = std::to_string(std::any_cast<float>(cell));
            }
            else if (cell.type() == typeid(const char*))
            {
                cellData = std::any_cast<const char*>(cell);
            }
            else
            {
                cellData = "Unknown Type";
            }
        }
        catch (const std::bad_any_cast& e)
        {
            cellData = "Bad Any Cast";
        }
        rowData.push_back(cellData);
    }
    return rowData;
}


uint32_t CsvParser::DeleteRow(uint32_t nRowIndex)
{
    if (nRowIndex >= vecCsvData.size())
    {
        return 0x00000003; // 删除位置错误
    }

    vecCsvData.erase(vecCsvData.begin() + nRowIndex);
    return 0;
}

uint32_t CsvParser::SaveCsvFile()
{
    if (!fsCsvFile.is_open())
    {
        return 0x00000001; // 打开文件失败
    }

    fsCsvFile.close();
    fsCsvFile.open(m_strFilePath, std::ios::out | std::ios::trunc);
    if (!fsCsvFile.is_open())
    {
        return 0x00000003; // 这里还会失败 为什么？
    }

    for (const vector<any> &row: vecCsvData)
    {
        for (size_t i = 0; i < row.size(); i++)
        {
            const std::type_info &valueType = row[i].type();
            if (valueType == typeid(string))
            {
                fsCsvFile << std::any_cast<string>(row[i]);
            }
            else if (valueType == typeid(int))
            {
                fsCsvFile << std::any_cast<int>(row[i]);
            }
            else if (valueType == typeid(uint32_t))
            {
                fsCsvFile << std::any_cast<uint32_t>(row[i]);
            }
            else if (valueType == typeid(double))
            {
                fsCsvFile << std::any_cast<double>(row[i]);
            }
            else if (valueType == typeid(float))
            {
                fsCsvFile << std::any_cast<float>(row[i]);
            }
            else if (valueType == typeid(const char *))
            {
                fsCsvFile << std::any_cast<const char *>(row[i]);
            }
            else
            {
                return 0x00000004; // 其他类型错误
            }
            if (i < row.size() - 1)
            {
                fsCsvFile << ",";
            }
        }
        fsCsvFile << std::endl;
    }

    return 0;
}

void CsvParser::CloseCsvFile()
{
    if (fsCsvFile.is_open())
    {
        fsCsvFile.close();
    }
}
