#include <regex>
#include <string>
#include <iostream>
using namespace std;

int main() {
    // 是否匹配整个序列，第一个参数为被匹配的str，第二个参数为regex对象
    //cout << regex_match("123", regex("[0-9]+")) << endl;

    // regex搜索
    //string str = "Get / HTTP/1.1\r\na: 1\r\nb: 1\r\n";
    //regex re("(\\w+) ([\./\\w]+) ([\./\\w]+)\r\n(.+ .+\r\n)+");
  string str = "ababab";
    regex re("((ab)+)");
    smatch sm;   // 存放string结果的容器
    regex_match(str, sm, re);
    for(int i = 0; i < sm.size(); ++i)
        cout << sm[i] << endl;
    cout << endl;

    // regex搜索多次
    //str = "!!!123!!!12333!!!890!!!";
    //re = regex("[0-9]+");
    //while(regex_search(str, sm, re)) {
        //for(int i = 0; i < sm.size(); ++i)
            //cout << sm[i] << " ";
        //cout << endl;
        //str = sm.suffix().str();
    //}
    return 0;
}
