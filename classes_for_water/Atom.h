//
// Created by danile on 6/2/22.
//

#ifndef HYDRO_ION_LIFE_ATOM_H
#define HYDRO_ION_LIFE_ATOM_H

/*
 It is a base class Atom which contains functions common to both classes (for example set_cord set update coordinates for booth classes).
 Сlass O_atom (oxygen) and H_atom (hydrogen) are inherited from Atom class.
*/

#include <string>
#include <cmath>
#include <vector>

class Atom {
protected:
    float x = 0; // x coordinate
    float y = 0; // y coordinate
    float z = 0; // z coordinate
    int name = -1; // atom number in file
public:
    Atom(); // default initialization
    ~Atom() ; // distructor
    Atom( std::vector<std::string> re); // initializer what parse string and get information about atom

    float get_name(); // return atom number
    float coord(int i); // return x or y or z depends of number (0->x, 1->y, 2->z)
    void set_cord(std::string x,std::string y,std::string z); // update atom coordinates
};

#endif //HYDRO_ION_LIFE_ATOM_H
