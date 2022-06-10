#include <iostream>
#include "parc_and_analis.h"


//int main(int argc, char *argv[]) {
int main(int argc, char *argv[]) {
    if (argc != 1){
        if (std::string(argv[1]).find(".zip") != std::string::npos) {
            std::cout << "Wrong compression format. Pleas convert to .gz" << std::endl;
            return -1;
        }
        else if (std::string(argv[1]).find(".gz") != std::string::npos) {
            return to_main_par_gz(argv[1]);
        }
        else{
            return to_main_par_test(argv[1]);
        }
    }
    else{
        std::cout << "Pleas enter input file (text format or .gz. Text format is faster)" << std::endl;
        return -1;
    }
}

