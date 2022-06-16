//
// Created by danile on 6/3/22.
//
#ifndef HYDRO_ION_LIFE_H_ATOM_H
#define HYDRO_ION_LIFE_H_ATOM_H

/*
 This class describes a hydrogen atom
*/

#include "Atom.h"
class O_atom; // prevent mutual using (O_atom ⇌ H_atom)

class H_atom: public Atom, private std::vector<std::string> {
private:
    using Atom::x;
    using Atom::y;
    using Atom::z;
    using Atom::name;

    O_atom* O_conect = NULL; // reference to the O (oxygen) that H (hydrogen) is associated with

public:
    H_atom(); // default initialization
    ~H_atom(); // distructor
    H_atom(std::vector<std::string> re): Atom(re) {};  // initializer of H (hydrogen) what parse string and get information about atom (use same function in class Atom)
    int get_O_name(); // return number of O (oxygen) that H (hydrogen) is associated with
    O_atom* get_O_conected(); // return reference to O (oxygen) that H (hydrogen) is associated with
    void set_O_conect(O_atom* O); // change reference to another O (oxygen) that now H (hydrogen) is associated with
    float dist_bt_atoms(O_atom O, float base[3]); //returns the distance between H (hydrogen) and O (oxygen), taking into account that if an atom crosses the boundaries, its coordinates "move" to the other end (for more information, see presentation)
};

#endif //HYDRO_ION_LIFE_H_ATOM_H
