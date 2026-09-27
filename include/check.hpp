#include "./verifyInPath.hpp"

bool checkToolEnv() {
    bool res=true;
    cout<<"正在检查工具环境...\n"<<endl;
    res&=verifyInPath("git");
    res&=!system("@git --version");
    return res;
}
