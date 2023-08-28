//
// Created by wubin on 2023/8/26.
//

#ifndef TEST_CSVPARSER_H
#define TEST_CSVPARSER_H
#include <vector>
#include <any>
#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>

class CsvParser{
public:
    CsvParser();
    ~CsvParser();
public:
    uint32_t OpenCsvFile(const std::string &strFilePath);

    void ClearCsvFile();

    uint32_t AddRow(const std::vector<std::any> &vecRowData);

    uint32_t InsertRow(uint32_t nRowIndex, const std::vector<std::any> &vecRowData);

    uint32_t AlterCell(uint32_t nRowIndex, uint32_t nColIndex, const std::any &anyData);

    uint32_t  GetRowCount();

    std::vector<std::string> GetRowData(uint32_t nRowIndex);

    uint32_t DeleteRow(uint32_t nRowIndex);

    uint32_t SaveCsvFile();

    void CloseCsvFile();
private:
    std::fstream fsCsvFile;
    std::string m_strFilePath;
    std::vector<std::vector<std::any>> vecCsvData;
};
#endif //TEST_CSVPARSER_H
