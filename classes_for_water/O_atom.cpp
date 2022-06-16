//
// Created by danile on 6/3/22.
//

#include "O_atom.h"
#include "H_atom.h"

O_atom::O_atom() {
    H_count = 0;
    std::vector<H_atom*> int_v;
    H_arr = int_v;
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

void O_atom::del_atom(int H_num) {
    H_count--;
    for (int i = 0; i < H_arr.size(); ++i) {
        if((*H_arr[i]).get_name() == H_num){
            H_arr.erase(H_arr.begin() + i);
        }
    }
}

