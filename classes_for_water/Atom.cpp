//
// Created by danile on 6/2/22.
//

#include "Atom.h"

Atom::Atom() {
    float x = 0;
    float y = 0;
    float z = 0;
    int name = -1;
}

Atom::~Atom() {
}

Atom::Atom(std::vector<std::string> re) {
    this->name = stoi(re[0]);
    this->x =stof(re[2]);
    this->y = stof(re[3]);
    this->z = stof(re[4]);
}

Atom::Atom( std::string* re){
    this->name = stoi(re[0]);
    this->x =stof(re[2]);
    this->y = stof(re[3]);
    this->z = stof(re[4]);
}


int Atom::get_name() {
    return name;
}

float Atom::coord(int i) {
    if (i == 0){
        return x;
    }
    else if (i == 1){
        return y;
    }
    else{
        return z;
    }

}

void Atom::set_cord(std::string x, std::string y, std::string z) {
    this->x = stof(x);
    this->y = stof(y);
    this->z = stof(z);
}



