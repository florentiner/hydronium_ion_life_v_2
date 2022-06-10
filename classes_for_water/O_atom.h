//
// Created by danile on 6/3/22.
//

#ifndef HYDRO_ION_LIFE_O_ATOM_H
#define HYDRO_ION_LIFE_O_ATOM_H
#include "Atom.h"
class H_atom;

class O_atom: public Atom, std::vector<std::string> {
protected:
    using Atom::x;
    using Atom::y;
    using Atom::z;
    using Atom::name;

    int H_count = 0;
    std::vector<H_atom*> H_arr;

public:
    O_atom();
    ~O_atom();
    O_atom( std::vector<std::string> re);
    int get_H_count();
    void up_H_count(H_atom* H);
    std::vector<H_atom*> get_H();
    void del_atom(int H_num);
};


#endif //HYDRO_ION_LIFE_O_ATOM_H
