#include <iostream>
#include <string>
#include <sstream>

int main(){
    std::string s1 = "Hey there";
    std::string s2 = " what number is it? ";
    int num = 23;

    std::stringstream ss;

    ss << s1 << s2 << num;

    std::cout << ss. << std::endl;
    
    return 0;
}