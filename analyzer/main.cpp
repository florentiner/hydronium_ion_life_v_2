#include <iostream>
//#include <vector>
#include <string>
#include "parc_and_analis.h"

int main(int argc, char *argv[]) {

    bool is_gz; // true if file in .gz format and false if not
    if (argc != 1){
        if (std::string(argv[1]).find(".zip") != std::string::npos) {
            std::cout << "Wrong compression format. Pleas convert to .gz" << std::endl;
            return -1;
        }
        else if (std::string(argv[1]).find(".gz") != std::string::npos) {
            is_gz = true;
        }
        else{
            is_gz = false;
        }

        return hydro_life(argv[1], is_gz);
    }
    else{
        int a = 0;
        std::cout << "Pleas enter input file (text format or .gz. Text format is about 3x faster then .gz)" << std::endl;
        return -1;
    }
}
