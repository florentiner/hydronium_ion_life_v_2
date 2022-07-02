//
// Created by danile on 6/3/22.
//
#include "Atom.h"
#include "H_atom.h"
#include "O_atom.h"

H_atom::H_atom() {
    O_conect = NULL;
}

H_atom::~H_atom() {
    O_conect = NULL;
}

int H_atom::get_O_name() {
    return O_conect->get_name();
}

void H_atom::set_O_conect(O_atom * O) {
    O_conect = O;
}

float H_atom::dist_bt_atoms(O_atom O, float *base) {
    float lenght = 0;
    for (int i = 0; i < 3; ++i) {
        float r = base[i];
        float distance = std::abs(O.coord(i) * r - this->coord(i) * r);
        if(distance < r/2){
            lenght += pow(distance, 2);
        }
        else{
            lenght += pow(r - distance, 2);
        }
    }
    return sqrt(lenght);
}

O_atom *H_atom::get_O_conected() {
    return O_conect;
}


