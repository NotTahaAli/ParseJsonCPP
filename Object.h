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
    JSONValue(const double&);
    std::string getStringValue();
    bool getBooleanValue();
    double getNumberValue();
    ValueType getValueType();
    std::ostream& printObjectValue(std::ostream&);
    int FindKeyIndex(const std::string&);
    JSONValue& operator[](int);
    JSONValue& operator[](const std::string&);
    JSONValue& setNumber(const double);
    JSONValue& setString(const std::string&);
    JSONValue& setBoolean(const bool);
    JSONValue& setObject(const Object&);
    JSONValue& operator=(const Object&);
    JSONValue& pushBack(const JSONValue&);
    JSONValue& setPair(const std::string&, const JSONValue&);
    const std::string* getKeys();
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
    Object(bool);
    Object& operator=(const Object&);
    std::ostream& Print(std::ostream&);
    int FindKeyIndex(const std::string&);
    JSONValue& operator[](int);
    JSONValue& operator[](const std::string&);
    Object& pushBack(const JSONValue&);
    Object& setPair(const std::string&, const JSONValue&);
    const std::string* getKeys();
    ~Object();
};

std::ostream& operator<<(std::ostream&, JSONValue&);
std::ostream& operator<<(std::ostream&, Object&);