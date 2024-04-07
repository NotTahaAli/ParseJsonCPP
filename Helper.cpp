#include <string>
#include "Helper.h"

using namespace std;

string TrimString(const string &str)
{
    int initialSpaces, endingPos;
    for (initialSpaces = 0; initialSpaces < str.size() && str[initialSpaces] == ' '; initialSpaces++)
        ;
    for (endingPos = str.size() - 1; endingPos >= 0 && str[endingPos] == ' '; endingPos--)
        ;
    return str.substr(initialSpaces, endingPos - initialSpaces + 1);
}

bool isDigit(const char c)
{
    return (c >= '0' && c <= '9');
}

string EscapeString(const string &str)
{
    string newString = "";
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '"' || str[i] == '\\')
        {
            newString += '\\';
        }
        newString += str[i];
    }
    return newString;
}

std::string *SplitFirst(const std::string &str, const char delim)
{
    string* strings = new string[2];
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == delim) {
            strings[0] = str.substr(0,i);
            strings[1] = str.substr(i+1);
            return strings;
        }
    }
    strings[0] = str;
    strings[1] = "";
    return strings;
}