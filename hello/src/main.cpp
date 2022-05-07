#include <iostream>

using namespace std;

int editDistance(string str1, string str2);

int main()
{
    cout << editDistance("driving", "diving") << endl;
    cout << editDistance("debate", "irate") << endl;
    cout << editDistance("football", "cookies") << endl;
    return 0;
}

int editDistance(string str1, string str2)
{
    if (str1 == "") {
        return str2.size();
    } else if (str2 == "") {
        return str1.size();
    } else {
        int add = 1 + editDistance(str1, str2.substr(1));
        int del = 1 + editDistance(str1.substr(1), str2);
        int sub = editDistance(str1.substr(1), str2.substr(1));
        if (str1[0] != str2[0]) {
            sub++;
        }
        return min(add, min(del, sub));
    }
}
