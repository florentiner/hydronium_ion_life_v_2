#include <iostream>
#include <vector>
#include <string>
#include "parc_and_analis.h"
#include <thread>
#include <omp.h>


int main(int argc, char *argv[]) {
    bool is_gz; // true if file in .gz format and false if not
    if (argc != 1){
        if (std::string(argv[argc-1]).find(".zip") != std::string::npos) {
            std::cout << "Wrong compression format. Pleas convert to .gz" << std::endl;
            return -1;
        }
        else if (std::string(argv[argc-1]).find(".gz") != std::string::npos) {
            is_gz = true;
        }
        else{
            is_gz = false;
        }

        if (std::string(argv[argc-1]).find("-t") != std::string::npos) {
            std::cout << "Wrong compression format. Pleas convert to .gz" << std::endl;
            return -1;
        }

        std::string path_of_resaults = "./";
        std::size_t num_threads = 1;
        int recrossing_time = 0;
        for (int i = 0; i < argc; i++){
            //Set number of threads (default 1)
            if (std::string(argv[i]).find("-t") != std::string::npos){
                std::string raw_n_of_threads = std::string(argv[i]).substr(3 , std::string(argv[i]).size());
                if(raw_n_of_threads == "all") raw_n_of_threads = std::to_string(std::thread::hardware_concurrency());
                num_threads = std::stoi(raw_n_of_threads);
            }
            //Set directory where will be results
            if (std::string(argv[i]).find("-v") != std::string::npos){
                path_of_resaults = std::string(argv[i]).substr(3 , std::string(argv[i]).size());
            }
            //Set time for recrossing
            if (std::string(argv[i]).find("-n") != std::string::npos){
                recrossing_time = stoi (std::string(argv[i]).substr(3 , std::string(argv[i]).size()));
            }
        }
        omp_set_num_threads(num_threads);
        return hydro_life(argv[argc-1], is_gz, path_of_resaults, recrossing_time);
}
    else{
        std::cout << "Pleas enter input file (text format or .gz. Text format is about 3x faster then .gz)" << std::endl;
        return -1;
    }
}
