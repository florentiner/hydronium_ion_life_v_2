//
// Created by danile on 6/3/22.
//
#ifndef HYDRO_ION_LIFE_H_ATOM_H
#define HYDRO_ION_LIFE_H_ATOM_H
#include "Atom.h"
class O_atom;

class H_atom: public Atom, private std::vector<std::string> {
private:
    using Atom::x;
    using Atom::y;
    using Atom::z;
    using Atom::name;

    O_atom* O_conect = NULL;

public:
    H_atom();
    ~H_atom();
    H_atom(std::vector<std::string> re);
    int get_O_name();
    O_atom* get_O_conected();
    void set_O_conect(O_atom* O);
    float dist_bt_atoms(O_atom O, float base[3]);
};


#endif //HYDRO_ION_LIFE_H_ATOM_H
