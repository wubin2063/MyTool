#include <vector>
#include <any>
#include <string>
#include <fstream>
#include <iostream>
#include <typeinfo>

using std::vector;
using std::string;
using std::any;

class CsvParser{
public:
    CsvParser() {}
    ~CsvParser()
    {
        CloseCsvFile();
    }
public:
    uint32_t OpenCsvFile(const string &strFilePath)
    {
        CloseCsvFile();

        fsCsvFile.open(strFilePath, std::ios::in);
        if (!fsCsvFile.is_open()) {
            // 文件不存在，创建空文件
            fsCsvFile.open(strFilePath, std::ios::out);
            fsCsvFile.close();

            // 重新打开文件
            fsCsvFile.open(strFilePath, std::ios::in);
            if (!fsCsvFile.is_open()) {
                return 0x00000001; // 打开文件失败
            }
        }

        // 读取文件内容
        string line;
        while (std::getline(fsCsvFile, line)) {
            vector<any> row;
            size_t startPos = 0;
            size_t commaPos = line.find(',');
            while (commaPos != string::npos) {
                string cellData = line.substr(startPos, commaPos - startPos);
                row.push_back(any(cellData));
                startPos = commaPos + 1;
                commaPos = line.find(',', startPos);
            }
            string cellData = line.substr(startPos);
            row.push_back(any(cellData));
            vecCsvData.push_back(row);
        }
        this->strFilePath  = strFilePath;
        return 0;
    }

    void ClearCsvFile()
    {
        vecCsvData.clear();
    }

    uint32_t AddRow(const vector<any> &vecRowData)
    {
        if (vecRowData.empty()) {
            return 0x00000002; // 文件内容错误
        }

        vecCsvData.push_back(vecRowData);
        return 0;
    }

    uint32_t InsertRow(uint32_t nRowIndex, const vector<any> &vecRowData)
    {
        if (vecRowData.empty()) {
            return 0x00000002; // 文件内容错误
        }

        if (nRowIndex > vecCsvData.size()) {
            return 0x00000003; // 插入位置错误
        }

        vecCsvData.insert(vecCsvData.begin() + nRowIndex, vecRowData);
        return 0;
    }

    uint32_t AlterCell(uint32_t nRowIndex, uint32_t nColIndex, const any &anyData)
    {
        if (nRowIndex >= vecCsvData.size() || nColIndex >= vecCsvData[nRowIndex].size()) {
            return 0x00000003; // 修改位置错误
        }

        vecCsvData[nRowIndex][nColIndex] = anyData;
        return 0;
    }

    uint32_t DeleteRow(uint32_t nRowIndex)
    {
        if (nRowIndex >= vecCsvData.size()) {
            return 0x00000003; // 删除位置错误
        }

        vecCsvData.erase(vecCsvData.begin() + nRowIndex);
        return 0;
    }

    uint32_t SaveCsvFile()
    {
        if (!fsCsvFile.is_open()) {
            return 0x00000001; // 打开文件失败
        }

        fsCsvFile.close();
        fsCsvFile.open(strFilePath, std::ios::out |  std::ios::trunc);
        if (!fsCsvFile.is_open()) {
            return 0x00000003; // 这里还会失败 为什么？
        }

        for (const vector<any>& row : vecCsvData) {
            for (size_t i = 0; i < row.size(); i++) {
                const std::type_info& valueType = row[i].type();
                if (valueType == typeid(string)) {
                    fsCsvFile << std::any_cast<string>(row[i]);
                } else if (valueType == typeid(int)) {
                    fsCsvFile << std::any_cast<int>(row[i]);
                } else if (valueType == typeid(uint32_t)) {
                    fsCsvFile << std::any_cast<uint32_t>(row[i]);
                } else if (valueType == typeid(double)) {
                    fsCsvFile << std::any_cast<double>(row[i]);
                } else if (valueType == typeid(float)) {
                    fsCsvFile << std::any_cast<float>(row[i]);
                }
                else if (valueType == typeid(const char *)) {
                    fsCsvFile << std::any_cast<const char *>(row[i]);
                }
                else {
                    std::cout << valueType.name() << std::endl;
                    std::cout << typeid(string).name() << std::endl;
                    std::cout << typeid(row[i]).name() << std::endl;
                    return 0x00000004; // 其他类型错误
                }
                if (i < row.size() - 1) {
                    fsCsvFile << ",";
                }
            }
            fsCsvFile << std::endl;
        }

        return 0;
    }

    void CloseCsvFile()
    {
        if (fsCsvFile.is_open()) {
            fsCsvFile.close();
        }
    }
private:
    std::fstream fsCsvFile;
    string strFilePath;
    vector<vector<any>> vecCsvData;
};

int main()
{
    CsvParser csvFile;
    uint32_t uiRet = csvFile.OpenCsvFile("test.csv");
    if (uiRet != 0) {
        std::cout << "OpenCsvFile failed, error code: " << uiRet << std::endl;
        return 0;
    }
    csvFile.ClearCsvFile();
    string str1 = "1";
    string str2 = "2";
    string str3 = "3";
    uiRet = csvFile.AddRow({str1,2,8.2});
    if (uiRet != 0) {
        std::cout << "AddRow failed, error code: " << uiRet << std::endl;
        return 0;
    }
    uiRet = csvFile.AddRow(vector<any>{any("4"), any("5"), any("6")});
    if (uiRet != 0) {
        std::cout << "AddRow failed, error code: " << uiRet << std::endl;
        return 0;
    }
    uiRet = csvFile.AddRow(vector<any>{any("7"), any("8"), any("9")});
    if (uiRet != 0) {
        std::cout << "AddRow failed, error code: " << uiRet << std::endl;
        return 0;
    }
    uiRet = csvFile.SaveCsvFile();
    if (uiRet != 0) {
        std::cout << "SaveCsvFile failed, error code: " << uiRet << std::endl;
        return 0;
    }
    csvFile.CloseCsvFile();
    return 0;
}