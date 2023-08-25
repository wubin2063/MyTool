

#include <iostream>
#include <regex>

int main() {
    std::string str = "digit";

  
    std::regex pattern(R"(^(?!digit$|digital$).*$)");


    if (std::regex_match(str, pattern)) {
        std::cout << "?????" << std::endl;
    } else {
        std::cout << "??????" << std::endl;
    }

    return 0;
}