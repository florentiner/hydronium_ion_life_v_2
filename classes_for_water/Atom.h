//
// Created by danile on 6/2/22.
//

#ifndef HYDRO_ION_LIFE_ATOM_H
#define HYDRO_ION_LIFE_ATOM_H


#include <string>
#include <cmath>
#include <vector>


class Atom {
protected:
    float x = 0;
    float y = 0;
    float z = 0;
    int name = -1;
public:
    Atom();
    ~Atom() ;
    Atom( std::vector<std::string> re);

    float get_x();
    float get_y();
    float get_z();

    float get_name();


    float coord(int i);


    void set_cord(std::string x,std::string y,std::string z);
};


#endif //HYDRO_ION_LIFE_ATOM_H
