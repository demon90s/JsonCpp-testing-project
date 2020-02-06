#include <json/json.h>
#include <iostream>
#include <string>
#include <memory>
#include <cassert>
#include "JsonReader.hpp"

void write_example()
{
	Json::Value json_obj;
	json_obj["name"] = Json::Value("diwen");
	json_obj["age"] = Json::Value(29);

	Json::Value json_root;
	json_root["key_string"] = Json::Value("value_string");
	json_root["key_number"] = Json::Value(12345);
	json_root["key_boolean"] = Json::Value(false);
	json_root["key_double"] = Json::Value(3.14);
	json_root["key_obj"] = Json::Value(json_obj);
	json_root["key_array"].append("array_string");
	json_root["key_array"].append(1234);

	Json::StreamWriterBuilder builder;
	Json::StreamWriter *writer = builder.newStreamWriter();
	writer->write(json_root, &std::cout);
	delete writer;
}

void read_example()
{
	std::string str_test = R"(
{
	// useless comment
	"id": 1,
	"name": "diwen",
	"array": [1234, "hello"],

	"skill":{ "lang": "c++", "book": "linux" }
}
	)";

	Json::CharReaderBuilder builder;
	std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

	Json::Value root;
	std::string err;
	if (!reader->parse(&str_test[0], &str_test.back(), &root, &err))
	{
		std::cerr << "Parse error: " << err;
		return;
	}

	std::cout << root << std::endl;	// 输出json文件内容(经过了格式化)

	assert(root["id"].isInt() 
		&& root["name"].isString()
		&& root["array"].isArray()
		&& root["skill"].isObject());

	std::cout << "id: " << root["id"].asInt() << std::endl;
	std::cout << "name: " << root["name"].asString() << std::endl;

	// access array
	std::cout << "array: ";
	const Json::Value arr = root["array"];
	for (int i = 0; i < static_cast<int>(arr.size()); i++)
	{
		std::cout << arr[i].asString() << " ";
	}
	std::cout << std::endl;

	// access object
	std::cout << "skill: ";
	std::cout << "lang: ";
	std::cout << root["skill"]["lang"].asString() << ", ";
	std::cout << "book: ";
	std::cout << root["skill"]["book"].asString() << "\n";
}

void test_JsonReader()
{
	JsonReader reader;

	std::string err;
	bool ret = reader.Parse("testjson.json", err);
	if (!ret)
	{
		std::cout << err << std::endl;
		return;
	}

	reader.Print();

	auto v = reader.GetValueFromPath("skill/lang");
	std::cout << v.asString() << std::endl;

	v = reader.GetValueFromPath("array/@1");
	std::cout << v.asString() << std::endl;

	v = reader.GetValueFromPath("array/@0");
	std::cout << v.asInt() << std::endl;

	v = reader.GetValueFromPath("array/@2/number/@0");
	std::cout << v.asInt() << std::endl;
}

int main()
{
	//write_example();

	//read_example();

	test_JsonReader();

	return 0;
}
