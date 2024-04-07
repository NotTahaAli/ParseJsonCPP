#include <stdexcept>
#include <string>
#include <iostream>
#include "Helper.h"

#include "Object.h"
using namespace std;

JSONValue::JSONValue(const string valueStr)
{
    string tempString = TrimString(valueStr);
    if (tempString.empty())
    {
        throw invalid_argument("Value String can not be empty");
    }
    if (tempString.compare("true") == 0)
    {
        type = Boolean;
        booleanVal = true;
        return;
    }
    if (tempString.compare("false") == 0)
    {
        type = Boolean;
        booleanVal = false;
        return;
    }
    if (tempString[0] == '"')
    {
        type = String;
        stringVal = new string("");
        bool previousEscaped = false;
        for (int i = 1; tempString[i] != 0; i++)
        {
            if (!previousEscaped && tempString[i] == '\\')
            {
                previousEscaped = true;
                continue;
            }
            if (!previousEscaped && tempString[i] == '"')
            {
                if (i == tempString.size() - 1)
                    continue;
                throw invalid_argument("Value String is a malformed String");
            }
            previousEscaped = false;
            *stringVal += tempString[i];
        }
        return;
    }

    if (isDigit(tempString[0]) || tempString[0] == '.')
    {
        type = Number;
        numberVal = 0;
        int dotPosition = -1;
        for (int i = 0; tempString[i] != 0; i++)
        {
            if (dotPosition == -1 && tempString[i] == '.')
            {
                dotPosition = i;
                continue;
            }
            if (!isDigit(tempString[i]))
            {
                throw invalid_argument("Value String is a malformed Number");
            }
            numberVal *= 10;
            numberVal += tempString[i] - '0';
        }
        if (dotPosition != -1)
        {
            while (++dotPosition < tempString.size())
            {
                numberVal /= 10;
            }
        }
        return;
    }

    type = JSONObject;
    objectVal = new Object(tempString);
}

JSONValue::JSONValue(const JSONValue &rhs)
{
    type = rhs.type;
    if (type == Boolean)
    {
        booleanVal = rhs.booleanVal;
        return;
    }
    if (type == Number)
    {
        numberVal = rhs.numberVal;
        return;
    }
    if (type == String)
    {
        stringVal = new string(*rhs.stringVal);
        return;
    }
    if (type == JSONObject)
    {
        objectVal = new Object(*rhs.objectVal);
    }
}

JSONValue::JSONValue(const double& num) {
    type = Number;
    numberVal = num;
}

JSONValue &JSONValue::operator=(const JSONValue &rhs)
{
    if (this == &rhs)
        return *this;
    type = rhs.type;
    if (type == Boolean)
    {
        booleanVal = rhs.booleanVal;
        return *this;
    }
    if (type == Number)
    {
        numberVal = rhs.numberVal;
        return *this;
    }
    if (type == String)
    {
        delete stringVal;
        stringVal = new string(*rhs.stringVal);
        return *this;
    }
    if (type == JSONObject)
    {
        delete objectVal;
        objectVal = new Object(*rhs.objectVal);
    }
    return *this;
}

JSONValue &JSONValue::setNumber(const double num)
{
    if (type == JSONObject)
    {
        delete objectVal;
    }
    else if (type == String)
    {
        delete stringVal;
    }
    type = Number;
    numberVal = num;
    return *this;
}

JSONValue &JSONValue::setString(const string &str)
{
    if (type == JSONObject)
    {
        delete objectVal;
    }
    else if (type == String)
    {
        delete stringVal;
    }
    type = String;
    stringVal = new string(str);
    return *this;
}

JSONValue &JSONValue::setBoolean(const bool boolean)
{
    if (type == JSONObject)
    {
        delete objectVal;
    }
    else if (type == String)
    {
        delete stringVal;
    }
    type = Boolean;
    numberVal = boolean;
    return *this;
}

JSONValue &JSONValue::operator=(const Object &obj)
{
    if (type == JSONObject)
    {
        delete objectVal;
    }
    else if (type == String)
    {
        delete stringVal;
    }
    type = JSONObject;
    objectVal = new Object(obj);
    return *this;
}

JSONValue &JSONValue::setObject(const Object &obj)
{
    return (*this = obj);
}

JSONValue::~JSONValue()
{
    if (type == String)
    {
        delete stringVal;
        return;
    }
    if (type == JSONObject)
    {
        delete objectVal;
        return;
    }
}

std::string JSONValue::getStringValue()
{
    if (type != String)
    {
        throw invalid_argument("Value is not a string");
    }
    return *stringVal;
}

bool JSONValue::getBooleanValue()
{
    if (type != Boolean)
    {
        throw invalid_argument("Value is not a boolean");
    }
    return booleanVal;
}

double JSONValue::getNumberValue()
{
    if (type != Number)
    {
        throw invalid_argument("Value is not a number");
    }
    return numberVal;
}

ValueType JSONValue::getValueType()
{
    return type;
}

ostream &JSONValue::printObjectValue(ostream &out)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    objectVal->Print(out);
    return out;
}

JSONValue &JSONValue::operator[](int index)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    return objectVal->operator[](index);
}

int JSONValue::FindKeyIndex(const string &key)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    return objectVal->FindKeyIndex(key);
}

JSONValue &JSONValue::operator[](const string &key)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    return objectVal->operator[](key);
}

Object::~Object()
{
    for (int i = 0; i < size; i++)
    {
        delete values[i];
    }
    delete[] keys;
    delete[] values;
}

Object::Object(const string JSONString)
{
    size = 0;
    keys = nullptr;
    values = nullptr;

    string tempString = TrimString(JSONString);
    if (tempString.empty())
    {
        throw invalid_argument("JSON String can not be empty");
    }

    if (tempString[0] == '[')
    {
        hasKeys = false;
        if (tempString[tempString.size() - 1] != ']')
        {
            throw invalid_argument("Malformed JSON String");
        }
        if (tempString.size() == 2)
            return;
        int valStart = 1;
        bool inString = false;
        bool isEscaped = false;
        int arrayDepth = 0;
        int objectDepth = 0;
        for (int i = 1; i < tempString.size() - 1; i++)
        {
            if (!inString && tempString[i] == '[')
            {
                arrayDepth++;
                continue;
            }
            if (!inString && tempString[i] == '{')
            {
                objectDepth++;
                continue;
            }
            if (!inString && tempString[i] == ']')
            {
                arrayDepth--;
                if (arrayDepth < 0)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                continue;
            }
            if (!inString && tempString[i] == '}')
            {
                objectDepth--;
                if (objectDepth < 0)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                continue;
            }
            if (tempString[i] == '\\')
            {
                if (!inString)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                isEscaped = true;
            }
            if (tempString[i] == '"' && !isEscaped)
            {
                inString = !inString;
                continue;
            }
            if (!inString && tempString[i] == ',' && arrayDepth == 0 && objectDepth == 0)
            {
                if (i < valStart + 1)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                JSONValue tempVal = tempString.substr(valStart, i - valStart);
                JSONValue **newArr = new JSONValue *[size + 1];
                for (int i = 0; i < size; i++)
                {
                    newArr[i] = values[i];
                }
                delete[] values;
                newArr[size++] = new JSONValue(tempVal);
                values = newArr;
                valStart = i + 1;
            }
            isEscaped = false;
        }
        if (tempString.size() - 1 < valStart + 1)
        {
            throw invalid_argument("Malformed JSON String");
        }
        string trimmed = TrimString(tempString.substr(valStart, tempString.size() - 1 - valStart));
        if (trimmed.empty())
            return;
        JSONValue tempVal = trimmed;
        JSONValue **newArr = new JSONValue *[size + 1];
        for (int i = 0; i < size; i++)
        {
            newArr[i] = values[i];
        }
        delete[] values;
        newArr[size++] = new JSONValue(tempVal);
        values = newArr;
        return;
    }

    if (tempString[0] == '{')
    {
        hasKeys = true;
        if (tempString[tempString.size() - 1] != '}')
        {
            throw invalid_argument("Malformed JSON String");
        }
        if (tempString.size() == 2)
            return;
        int valStart = 1;
        bool inString = false;
        bool isEscaped = false;
        int arrayDepth = 0;
        int objectDepth = 0;
        for (int i = 1; i < tempString.size() - 1; i++)
        {
            if (!inString && tempString[i] == '[')
            {
                arrayDepth++;
                continue;
            }
            if (!inString && tempString[i] == '{')
            {
                objectDepth++;
                continue;
            }
            if (!inString && tempString[i] == ']')
            {
                arrayDepth--;
                if (arrayDepth < 0)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                continue;
            }
            if (!inString && tempString[i] == '}')
            {
                objectDepth--;
                if (objectDepth < 0)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                continue;
            }
            if (tempString[i] == '\\')
            {
                if (!inString)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                isEscaped = true;
            }
            if (tempString[i] == '"' && !isEscaped)
            {
                inString = !inString;
                continue;
            }
            if (!inString && tempString[i] == ',' && arrayDepth == 0 && objectDepth == 0)
            {
                if (i <= valStart + 1)
                {
                    throw invalid_argument("Malformed JSON String");
                }
                string *splitVals = SplitFirst(tempString.substr(valStart, i - valStart));
                try
                {
                    JSONValue tempKey = splitVals[0];
                    if (tempKey.getValueType() != String)
                    {
                        throw invalid_argument("Malformed JSON String");
                    }
                    JSONValue tempVal = splitVals[1];
                    string *newStringArray = new string[size + 1];
                    for (int i = 0; i < size; i++)
                    {
                        newStringArray[i] = keys[i];
                    }
                    delete[] keys;
                    newStringArray[size] = tempKey.getStringValue();
                    keys = newStringArray;
                    JSONValue **newArr = new JSONValue *[size + 1];
                    for (int i = 0; i < size; i++)
                    {
                        newArr[i] = values[i];
                    }
                    delete[] values;
                    newArr[size++] = new JSONValue(tempVal);
                    values = newArr;
                    valStart = i + 1;
                    delete[] splitVals;
                    splitVals = nullptr;
                }
                catch (const std::exception &e)
                {
                    if (splitVals)
                        delete[] splitVals;
                    throw;
                }
            }
            isEscaped = false;
        }
        if (tempString.size() - 1 < valStart + 1)
        {
            throw invalid_argument("Malformed JSON String");
        }
        string trimmed = TrimString(tempString.substr(valStart, tempString.size() - 1 - valStart));
        if (trimmed.empty())
            return;
        string *splitVals = SplitFirst(trimmed);
        try
        {
            JSONValue tempKey = splitVals[0];
            if (tempKey.getValueType() != String)
            {
                throw invalid_argument("Malformed JSON String");
            }
            JSONValue tempVal = splitVals[1];

            string *newStringArray = new string[size + 1];
            for (int i = 0; i < size; i++)
            {
                newStringArray[i] = keys[i];
            }
            delete[] keys;
            newStringArray[size] = tempKey.getStringValue();
            keys = newStringArray;
            JSONValue **newArr = new JSONValue *[size + 1];
            for (int i = 0; i < size; i++)
            {
                newArr[i] = values[i];
            }
            delete[] values;
            newArr[size++] = new JSONValue(tempVal);
            values = newArr;
            delete[] splitVals;
            splitVals = nullptr;
        }
        catch (const std::exception &e)
        {
            if (splitVals)
                delete[] splitVals;
            throw;
        }
        return;
    }

    throw invalid_argument("Malformed JSON String");
}

Object::Object(const Object &rhs)
{
    size = rhs.size;
    hasKeys = rhs.hasKeys;
    if (size > 0)
    {
        if (hasKeys)
        {
            keys = new string[size];
            for (int i = 0; i < size; i++)
            {
                keys[i] = rhs.keys[i];
            }
        }
        else
        {
            keys = nullptr;
        }
        values = new JSONValue *[size];
        for (int i = 0; i < size; i++)
        {
            values[i] = new JSONValue(*(rhs.values[i]));
        }
    }
    else
    {
        keys = nullptr;
        values = nullptr;
    }
}

Object::Object(bool _hasKeys)
{
    size = 0;
    hasKeys = _hasKeys;
    keys = nullptr;
    values = nullptr;
}

Object &Object::operator=(const Object &rhs)
{
    if (this == &rhs)
        return *this;
    size = rhs.size;
    hasKeys = rhs.hasKeys;
    delete[] keys;
    for (int i = 0; i < size; i++)
    {
        delete values[i];
    }
    delete values;
    if (size > 0)
    {
        if (hasKeys)
        {
            keys = new string[size];
            for (int i = 0; i < size; i++)
            {
                keys[i] = rhs.keys[i];
            }
        }
        else
        {
            keys = nullptr;
        }
        values = new JSONValue *[size];
        for (int i = 0; i < size; i++)
        {
            values[i] = new JSONValue(*(rhs.values[i]));
        }
    }
    else
    {
        keys = nullptr;
        values = nullptr;
    }
    return *this;
}

JSONValue &Object::operator[](int index)
{
    if (index < 0 || index >= size)
        throw invalid_argument("Index out of range");
    return *values[index];
}

int Object::FindKeyIndex(const string &key)
{
    if (!hasKeys)
    {
        throw invalid_argument("Object is an array.");
    }
    for (int i = 0; i < size; i++)
    {
        if (keys[i].compare(key) == 0)
        {
            return i;
        }
    }
    return -1;
}

JSONValue &Object::operator[](const string &key)
{
    int index = FindKeyIndex(key);
    if (index == -1)
    {
        throw invalid_argument("Key Not Found in object");
    }
    return *values[index];
}

std::ostream &operator<<(std::ostream &out, JSONValue &val)
{
    ValueType type = val.getValueType();
    if (type == Number)
    {
        out << val.getNumberValue();
        return out;
    }
    if (type == Boolean)
    {
        out << val.getBooleanValue();
        return out;
    }
    if (type == String)
    {
        out << '"' << EscapeString(val.getStringValue()) << '"';
        return out;
    }
    return val.printObjectValue(out);
}

std::ostream &Object::Print(std::ostream &out)
{
    out << ((hasKeys) ? "{" : "[");
    for (int i = 0; i < size; i++)
    {
        if (i != 0)
            out << ", ";
        if (hasKeys)
        {
            out << '"' << EscapeString(keys[i]) << '"' << ": ";
        }
        out << *values[i];
    }
    out << ((hasKeys) ? "}" : "]");
    return out;
}

std::ostream &operator<<(std::ostream &out, Object &val)
{
    return val.Print(out);
}

JSONValue &JSONValue::pushBack(const JSONValue &val)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    objectVal->pushBack(val);
    return *this;
}
JSONValue &JSONValue::setPair(const string &key, const JSONValue &val)
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    objectVal->setPair(key, val);
    return *this;
}
const string *JSONValue::getKeys()
{
    if (type != JSONObject)
    {
        throw invalid_argument("Value is not an object");
    }
    return objectVal->getKeys();
}

Object &Object::pushBack(const JSONValue &val)
{
    if (hasKeys)
    {
        throw invalid_argument("Object is not an array");
    }
    JSONValue **newValArr = new JSONValue *[size + 1];
    for (int i = 0; i < size; i++)
    {
        newValArr[i] = values[i];
    }
    delete[] values;
    values = newValArr;
    values[size] = new JSONValue(val);
    return *this;
    return *this;
}

Object &Object::setPair(const string &key, const JSONValue &val)
{
    if (!hasKeys)
    {
        throw invalid_argument("Object is an array");
    }
    int index = FindKeyIndex(key);
    if (index != -1)
    {
        delete values[index];
        values[index] = new JSONValue(val);
        return *this;
    }
    string *newKeyArr = new string[size + 1];
    JSONValue **newValArr = new JSONValue *[size + 1];
    for (int i = 0; i < size; i++)
    {
        newKeyArr[i] = keys[i];
        newValArr[i] = values[i];
    }
    delete[] keys;
    delete[] values;
    newValArr[size] = new JSONValue(val);
    newKeyArr[size++] = key;
    values = newValArr;
    keys = newKeyArr;
    return *this;
}

const string *Object::getKeys()
{
    if (!hasKeys)
    {
        throw invalid_argument("Object is an array");
    }
    return keys;
}