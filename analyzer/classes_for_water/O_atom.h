//
// Created by danile on 6/3/22.
//

#ifndef HYDRO_ION_LIFE_O_ATOM_H
#define HYDRO_ION_LIFE_O_ATOM_H
#include "Atom.h"
class H_atom; // prevent mutual using (H_atom ⇌ O_atom)

class O_atom: public Atom, std::vector<std::string> {
protected:
    using Atom::x;
    using Atom::y;
    using Atom::z;
    using Atom::name;

    int H_count = 0; // number of H (hydrogen) that associated with this O (oxygen)
    std::vector<H_atom*> H_arr; // vector of reference to H (hydrogen) that associated with this O (oxygen) (always 2 or 3)

public:
    O_atom(); // default initialization
    ~O_atom(); // distructor
    O_atom(O_atom* O_to_cop);
    O_atom( std::vector<std::string> re): Atom(re) {}; // initializer of O (oxygen) what parse string and get information about atom (use same function in class Atom)
    int get_H_count(); // return number of H (hydrogen) that associated with this O (oxygen)
    std::vector<H_atom*> get_H(); // return vector of reference to H (hydrogen) that associated with this O (oxygen) (always 2 or 3)
    std::array<int, 4> get_O_and_H_name(); // return array of name atoms name ([O, H_1, H_2, H_3])
    void up_H_count(H_atom* H); // add reference to H (hydrogen) that now associated with this O (oxygen)
    void del_atom(int H_num); // drop  reference to H (hydrogen) that now not associated with this O (oxygen)
};

#endif //HYDRO_ION_LIFE_O_ATOM_H
