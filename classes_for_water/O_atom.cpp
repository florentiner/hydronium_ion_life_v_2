//
// Created by danile on 6/3/22.
//

#include <array>
#include "O_atom.h"
#include "H_atom.h"

O_atom::O_atom() {
    H_count = 0;
    std::vector<H_atom*> int_v;
    H_arr = int_v;
}


O_atom::O_atom(O_atom *O_to_cop) {
    this->x = (*O_to_cop).x;
    this->y = (*O_to_cop).y;
    this->z = (*O_to_cop).z;
    this->name = (*O_to_cop).name;
    this->H_count = (*O_to_cop).H_count;
    this->H_arr = (*O_to_cop).H_arr;
}

O_atom::~O_atom() {
    H_arr.clear();
}

int O_atom::get_H_count() {
    return H_count;
}

void O_atom::up_H_count(H_atom* H) {
    H_arr.push_back(H);
    H_count+=1;
}

std::vector<H_atom*> O_atom::get_H() {
    return H_arr;
}


std::array<int, 4> O_atom::get_O_and_H_name() {
    std::array<int, 4> atom_arr;
    atom_arr[0] = this->get_name();
    atom_arr[3] = -1;
    int i = 1;
    for (H_atom* H:this->H_arr){
        atom_arr[i]= H->get_name();
        i++;
    }
    return atom_arr;
}

void O_atom::del_atom(int H_num) {
    H_count--;
    for (int i = 0; i < H_arr.size(); ++i) {
        if((*H_arr[i]).get_name() == H_num){
            H_arr.erase(H_arr.begin() + i);
        }
    }
}



