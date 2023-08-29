#ifndef FILETOOL_FILETOOL_H
#define FILETOOL_FILETOOL_H

#include <fstream>
#include <string>
#include <vector>

// 定义文件操作错误码的枚举
typedef enum
{
    E_OK = 0,               // 操作成功
    E_FILE_OPEN_FAIL,       // 文件打开失败
    E_FILE_READ_FAIL,       // 文件读取失败
    E_FILE_WRITE_FAIL,      // 文件写入失败
    E_FILE_NOT_EXIST,        // 文件不存在
    E_FILE_NOT_OPEN,         // 文件未打开
} FileToolErrorCode_E;

class FileTool
{
public:
    // 打开文件：如果文件存在则打开，如果文件不存在则创建，打开方式为trunc或append
    FileToolErrorCode_E OpenFile(const std::string strFileName, std::ios::openmode mode = std::ios::app)
    {
        // 打开文件
        m_file.open(strFileName.c_str(), mode | std::ios::out);

        // 检查文件是否成功打开
        if (!m_file.is_open())
        {
            return E_FILE_OPEN_FAIL; // 打开失败
        }
        m_strFileName = strFileName; // 保存文件名
        return E_OK; // 打开成功
    }

    // 读取文件,剔除每行的分隔符
    FileToolErrorCode_E ReadFile(std::vector<std::string> &vecLine, const std::string &strSeparator = "\n")
    {
        std::string line;
        // 检查文件是否打开
        if (!m_file.is_open())
        {
            return E_FILE_NOT_OPEN; // 文件未打开
        }
        m_file.close();
        m_file.open(m_strFileName.c_str(), std::ios::in);
        // 清空输出向量
        vecLine.clear();

        // 逐行读取文件
        while (std::getline(m_file, line))
        {
            // 剔除行末的分隔符
            if (!strSeparator.empty() && line.length() >= strSeparator.length() &&
                line.compare(line.length() - strSeparator.length(), strSeparator.length(), strSeparator) == 0)
            {
                line.erase(line.length() - strSeparator.length(), strSeparator.length());
            }

            // 如果行不为空，加入向量
            if (!line.empty())
            {
                vecLine.push_back(line);
            }
        }

        return E_OK; // 读取成功
    }

    // 向文件写入一行数据
    template <typename ValueType>
    FileToolErrorCode_E WriteFile(const std::vector<ValueType> &vecValue, const std::string &strSeparator = "\n")
    {
        if (!m_file.is_open())
        {
            return E_FILE_NOT_OPEN; // 文件未打开
        }
        m_file.close();
        m_file.open(m_strFileName.c_str(), std::ios::out | m_mode);
        for (const auto &value : vecValue)
        {
            m_file << value << strSeparator;
            if (m_file.fail())
            {
                return E_FILE_WRITE_FAIL;
            }
        }
        return E_OK;
    }

    // 关闭文件
    void CloseFile()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

public:
    FileTool() {} // 构造函数
    ~FileTool()
    {
        // 在析构函数中关闭文件
        CloseFile();
    }

private:
    std::string m_strFileName; // 文件名
    std::fstream m_file; // 文件流对象
    std::ios::openmode m_mode; // 打开模式
};

#endif // FILETOOL_FILETOOL_H
