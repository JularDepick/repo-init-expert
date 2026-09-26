#include "./include/CLI11.hpp"
#include <iostream>
#include <string>
using namespace std;

#define VERSION "v0.1.0"

int main(int argc,char* argv[]) {
    CLI::App rie{"repo-init-expert"};
    rie.require_subcommand(0,2);

    bool verbose=false;
    rie.add_flag("-V,--verbose",verbose,"显示详细输出");

    rie.set_help_flag("-h,--help", "显示帮助信息并退出");
    rie.set_version_flag("-v,--version","rie(repo-init-expert) version: " VERSION,"显示版本号并退出");
    rie.usage("rie [OPTIONS] <SUBCOMMANDS>");
    rie.footer("仓库地址: https://github.com/JularDepick/repo-init-expert/");

    auto* init=rie.add_subcommand("init","初始化一个新的仓库");
    init->alias("i");
    init->callback([](){
        cout<<"init failed."<<endl;
    });

    CLI11_PARSE(rie,argc,argv);
    if(argc==1) {
        cout<<rie.help();
        return 0;
    }

    return 0;
} 
