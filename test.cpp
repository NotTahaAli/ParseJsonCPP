#include <iostream>
#include "Object.h"
using namespace std;

int main() {
    JSONValue a("[[], {}, {\"A\":[1,2,\"Taha\"]}]");
    cout << "a => " << a << endl;
    cout << "a[0] => " << a[0] << endl;
    cout << "a[1] => " << a[1] << endl;
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
    
    return 0;
}