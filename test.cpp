#include <iostream>
#include "Object.h"
using namespace std;

int main() {
    JSONValue a("[[], null, {\"A\":[1,2,\"Taha\"]}]");
    cout << "a => " << a << endl;
    cout << "a[0] => " << a[0] << endl;
    cout << "a[1] => " << a[1] << endl;
    a[2].setPair("A", JSONValue("[1,2,3]"));
    cout << "a[2] => " << a[2] << endl;
    cout << "a[2][\"A\"] => " << a[2]["A"] << endl;
    cout << "a[2][\"A\"][2] => " << a[2]["A"][2] << endl;
    
    cout << "a[3] => ";
    try
    {
        cout << a[3] << endl;
    }
    catch(const std::exception& e)
    {
        cout << "Error Occured: " << e.what() << endl;
    }

    cout << "a[0][\"B\"] => ";
    try
    {
        cout << a[0]["B"] << endl;
    }
    catch(const std::exception& e)
    {
        cout << "Error Occured: " << e.what() << endl;
    }

    cout << "a[2][\"B\"] => ";
    try
    {
        cout << a[2]["B"] << endl;
    }
    catch(const std::exception& e)
    {
        cout << "Error Occured: " << e.what() << endl;
    }

    cout << endl << endl << "After a[1].setObject(Object(true).setPair(\"Value\", JSONValue()));" << endl;
    a[1].setObject(Object(true).setPair("Value", JSONValue()));
    cout << "a => " << a << endl;

    cout << endl << endl << "After a.pushBack(JSONValue().setString(\"Hello World\"));" << endl;
    a.pushBack(JSONValue().setString("Hello World"));
    cout << "a => " << a << endl;

    cout << endl << endl << "After a[2].setPair(\"Enabled\", JSONValue().setBoolean(false));" << endl;
    a[2].setPair("Enabled", JSONValue().setBoolean(false));
    cout << "a => " << a << endl;
    
    cout << endl << endl << "After a[2].setPair(\"Enabled\", JSONValue().setBoolean(true));" << endl;
    a[2].setPair("Enabled", JSONValue().setBoolean(true));
    cout << "a => " << a << endl;

    cout << endl << endl << "After a[2][\"Enabled\"].setString(\"Not Enabled\");" << endl;
    a[2]["Enabled"].setString("Not Enabled");
    cout << "a => " << a << endl;

    return 0;
}