#include "JsonReader.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{
    class FileReader
    {
    public:
        FileReader() : m_length(0), m_data(0) {}
        ~FileReader()
        {
            if (m_data) free(m_data);
        }
        FileReader(const FileReader&) = delete;
        FileReader& operator=(const FileReader&) = delete;

        bool Load(const char *filename)
        {
            FILE *fp = fopen(filename, "rb");
            if (!fp) return false;

            // Get the file size, so we can pre-allocate the memory. HUGE speed impact.
            long length = 0;
            fseek(fp, 0, SEEK_END);
            length = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            m_data = (char*)malloc(length + 1);
            memset(m_data, 0, length + 1);
            fread(m_data, length, 1, fp);

            m_length = length;

            fclose(fp);

            return true;
        }

        char *Data()
        {
            return m_data;
        }

        const char *Data() const
        {
            return m_data;
        }

        long Length() const
        {
            return m_length;
        }

    private:
        long m_length;
        char *m_data;
    };

    std::vector<std::string> SplitString(const std::string &str, const char *delim)
    {
        std::vector<std::string> result;
        size_t delim_len = strlen(delim);
        std::string::size_type pos1 = 0;
        std::string::size_type pos2 = str.find(delim);

        while (pos2 != std::string::npos)
        {
            result.push_back(std::string(str, pos1, pos2 - pos1));

            pos1 = pos2 + delim_len;
            pos2 = str.find(delim, pos1);
        }

        if (pos1 != str.length())
        {
            result.push_back(std::string(str, pos1));
        }

        return result;
    }
}

JsonReader::JsonReader() :
    m_reader(m_builder.newCharReader())
{

}

bool JsonReader::Parse(const char *file, std::string &err)
{
    FileReader freader;
    if (!freader.Load(file))
    {
        err = "read file fail";
        return false;
    }

    return m_reader->parse(freader.Data(), freader.Data() + freader.Length(), &m_root, &err);
}

void JsonReader::Print() const
{
    std::cout << m_root << std::endl;
}

Json::Value JsonReader::GetValueFromPath(const char *path) const
{
    std::vector<std::string> path_vec = SplitString(path, "/");

    Json::Value v = m_root;
    for (const auto &node : path_vec)
    {
        if (node[0] == '@')
        {
            // node is array index
            // example: @2
            std::string index_str = node.substr(1);
            int index = std::stoi(index_str);
            v = v[index];
        }
        else
        {
            // node is key
            v = v[node];
        }
    }

    return v;
}
