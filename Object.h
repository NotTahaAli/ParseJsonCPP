#pragma once
#include <string>

enum ValueType {
    Number,
    Boolean,
    String,
    JSONObject
};

class Object;

class JSONValue
{
private:
    ValueType type;
    double numberVal;
    bool booleanVal;
    std::string* stringVal;
    Object* objectVal;
public:
    JSONValue(const JSONValue&);
    JSONValue& operator=(const JSONValue&);
    JSONValue(const std::string);
    std::string getStringValue();
    bool getBooleanValue();
    double getNumberValue();
    ValueType getValueType();
    std::ostream& printObjectValue(std::ostream&);
    int FindKeyIndex(const std::string&);
    JSONValue& operator[](int);
    JSONValue& operator[](const std::string&);
    ~JSONValue();
};

class Object
{
private:
    int size;
    bool hasKeys;
    std::string *keys;
    JSONValue **values;

public:
    Object(const std::string JSONString);
    Object(const Object& rhs);
    Object& operator=(const Object&);
    std::ostream& Print(std::ostream&);
    int FindKeyIndex(const std::string&);
    JSONValue& operator[](int);
    JSONValue& operator[](const std::string&);
    ~Object();
};

std::ostream& operator<<(std::ostream&, JSONValue&);
std::ostream& operator<<(std::ostream&, Object&);