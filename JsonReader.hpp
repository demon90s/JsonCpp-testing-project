#pragma once

#include "json/json.h"
#include <memory>
#include <string>

class JsonReader
{    
public:
    JsonReader();

    // 解析json文档
    bool Parse(const char *file, std::string &err);

    // 打印格式化后的json文档
    void Print() const;

    // 根据路径获取值，用/分隔
    // 如果要取数组元素，前面加一个@，后面跟下标
    Json::Value GetValueFromPath(const char *path) const;

private:
    Json::CharReaderBuilder m_builder;
    std::unique_ptr<Json::CharReader> m_reader;
    Json::Value m_root;
};