#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "No input file provided." << std::endl;
        exit(1);
    }

    std::string buffer;
    std::ifstream f(argv[1]);
    f.seekg(0, std::ios::end);
    buffer.resize(f.tellg());

    std::cout << f.gcount() <<std::endl;

    return 0;
}
