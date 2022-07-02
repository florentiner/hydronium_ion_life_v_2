//
// Created by danile on 6/3/22.
//

#include "parc_and_analis.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <zlib.h>
#include <cstring>
#include "classes_for_water/H_atom.h"
#include "classes_for_water/O_atom.h"
#include <array>
#include "omp.h"


//returns a vector with arguments between which symbol is located (delimiter)
std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }
    return internal;
}

// if an atom with an input name is in the input vector, then its index is returned, else 0
template <typename T>
int in_vector(std::string name,std::vector<T> H_arr){
    int i = 0;
    for (H_atom* H: H_arr){
        if ((*H).get_name() == stoi(name)){
            return i + 1;
        }
        i++;
    }
    return 0;
}

//return index of nearest O (oxygen) in input vector of Os
int find_nearest_O(H_atom* H, std::vector<O_atom*> o_arr, float* frame_lin){
    float min_dist = float(std::numeric_limits<int>::max());
    int max_i;
    for (int j = 0; j < o_arr.size(); ++j) {
        O_atom* O = o_arr[j];
        float dist = (*H).dist_bt_atoms(*O, frame_lin);
        if (dist < min_dist){
            min_dist = dist;
            max_i = j;
        }
    }
    return max_i;
}

// change input vector. If size of input vector less than input number then add missing quantity to vector and last will be variable order. Else change numberth element to order variable
void glosar(int num, int order, std::vector<int>& input_vect){
    if (num <= input_vect.size()){
        input_vect[num] = order;
    }
    else{
        int end = num - input_vect.size();
        for (int i = 0; i < end; ++i) {
            input_vect.push_back(-1);
        }
        input_vect.push_back(order);
    }
}

//make file with data in input vector
void write_to_file_res(std::vector<int> life_ar, std::string file_name_of_analyzing){
    std::string res_str;
    std::ofstream res_file;
    std::string file_name_write = "res_life.txt";
    res_file.open (file_name_write);
    std::cout << ' ' <<std::endl;
    for(int el:life_ar){
        res_str += std::to_string(el) + ", ";
    }
    res_str.pop_back();
    res_str.pop_back();
    res_file << res_str;
    res_file.close();
}

//return next line of .gz file
std::vector< char > readline( gzFile f ) {
    std::vector< char > v( 256 );
    unsigned pos = 0;
    for ( ;; ) {
        if ( gzgets( f, &v[ pos ], v.size() - pos ) == 0 ) {
            // end-of-file or error
            int err;
            const char *msg = gzerror( f, &err );
            if ( err != Z_OK ) {
                // handle error
            }
            break;
        }
        unsigned read = strlen( &v[ pos ] );
        if ( v[ pos + read - 1 ] == '\n' ) {
            if ( pos + read >= 2 && v[ pos + read - 2 ] == '\r' ) {
                pos = pos + read - 2;
            } else {
                pos = pos + read - 1;
            }
            break;
        }
        if ( read == 0 || pos + read < v.size() - 1 ) {
            pos = read + pos;
            break;
        }
        pos = v.size() - 1;
        v.resize( v.size() * 2 );
    }
    v.resize( pos );
    return v;
}

// struct type of data which will be used to visualize H(hydrogen) moving
struct instruction {
    int jump_index; // this index help for grouping commands on python
    int frame; // frame number where implement instruction
    std::array<int, 4> atom_name; // array of numbers of atoms for which the instruction is executed
};

// make file which will be used to visualize H(hydrogen) moving
void write_to_file_vis(std::vector<instruction> instruction, std::string file_name_of_analyzing){
    std::string res_str;
    std::ofstream res_file;
    std::string file_name_write = "vis_mod.txt";
    res_file.open (file_name_write);
    std::cout << ' ' <<std::endl;
    int i = 0;
    for(struct instruction el: instruction){
        res_str += std::to_string(el.jump_index) + ' ' + std::to_string(el.frame) + ' ';
        for(int numb: el.atom_name){
            (numb != -1) ? res_str += std::to_string(numb) + ',' : std::string();
        }
        res_str.pop_back();
        res_str += "; ";
    }
    res_file << res_str;
    res_file.close();
}

// make instruction object
instruction add_instruction_to_vector(O_atom O, int frame_time, int jump_index) {
    struct instruction inst = {jump_index, frame_time, O.get_O_and_H_name()};
    return inst;
}

//main function that calculate vector of hydronium lifetime
int hydro_life(std::string file, bool is_gz){
    std::vector<int> life_ar; // vector with life time of hydronium
    std::vector<instruction> arr_instruction_to_atom_visual; // vector of instruction with content
    float water_length = 0.9584; // distance between H(hydrogen) and O(oxygen) in water molecule
    int frame_time = 0; //counter of frame
    int life_time = 0; //counter of hydronium life time
    int make_glossary_for_O = 0; // use for make glossary for O(oxygen)
    int change_O_glossary = 0; // use to change glossary for O(oxygen)
    int open_frame_count = 0; // use to parse information about frame boundaries
    std::fstream newfile;
    gzFile infile;
    std::string start_frame = "ITEM: BOX BOUNDS pp pp pp"; // line from which start frame
    std::string end_frame ="ITEM: TIMESTEP"; // line from which frame end
    std::string start_coord = "ITEM: ATOMS id type xs ys zs"; // line from which start information about atoms coordinates
    float frame_lin[3]; // array with information about frame boundaries
    std::vector<H_atom*> h_arr; // vector with all H(hydrogen) in experiment which updated as needed
    std::vector<O_atom*> o_arr; // vector with all O(oxygen) in experiment which updated every frame
    std::vector<O_atom*> O_of_jump_H; // vector with O(oxygen) what include in hydronium
    std::vector<int> glossary_O_of_jump_H; // glossary (index is atom number, value index in O_of_jump_H) of O(oxygen) what include in hydronium
    std::vector<H_atom*> jump_H; // vector with H(hydrogen) in hydronium which need to be updated every frame
    std::vector<int> glossary_of_O; // glossary (index is atom number, value index in o_arr) of O(oxygen)
    std::vector<char> char_line; // raw information from file
    std::string tp; // read string from file
    std::vector<std::string> lines; // vector with all lines in frame that include atoms data
    bool first_frame = true; // indicator is the frame first
    bool start_frame_coord = false; // indicator is the frame first
    bool is_O_of_jump_H_change = false; //indicator that coordinates are being read

    //open file and check if file has opened. If type of file .gz then one algorithm else another.
    if (is_gz) {
        infile = gzopen(file.c_str(), "rb");
        if (!infile) {
            std::cout << "Incorrect file input" << std::endl;
            return -1;
        }
        else{
            char_line = readline(infile);
            tp = std::string (char_line.begin(), char_line.end());
        }
    }
    else{
        newfile.open(file,std::ios::in);
        if (!newfile.is_open()) {
            std::cout << "Incorrect file input" << std::endl;
            return -1;
        }
        else {
            getline(newfile, tp);
        }
    }

    // cycle to read, parse and calculate needing data
    while(!tp.empty()){

        //reading for different type of file
        if(is_gz){
            char_line = readline(infile);
            tp = std::string (char_line.begin(), char_line.end());
        }
        else {
            getline(newfile, tp);
        }

        if(start_frame == tp){
            start_frame_coord = true;
        }
        //parse information for boundaries in frame
        else if ((start_frame_coord) and (open_frame_count < 3)){
            std::vector<std::string> re = split(tp, ' ');
            frame_lin[open_frame_count] = std::stof( re[1]);
            open_frame_count++;
        }
        //parse information about atoms
        else if ((open_frame_count >= 3) and (tp != start_coord)){
            //parse information to create of h_arr and o_arr
            if (first_frame) {
                std::vector<std::string> re = split(tp, ' ');
                if (re[1] == "1") {
                    h_arr.push_back(new H_atom(re));
                }
                else if (re[1] == "2"){
                    o_arr.push_back(new O_atom(re));
                    glosar(stoi(re[0]), make_glossary_for_O, glossary_of_O);
                    make_glossary_for_O++;
                }
            }
            else (end_frame != tp) ? lines.push_back(tp) : void(); // filling vector with all information about atoms
        }
        if (end_frame == tp){
            // First step. Find out which atoms are in the hydronym.
            if (first_frame) {
                for (int i = 0; i < h_arr.size(); ++i) {
                    H_atom* H = h_arr[i];
                    int max_ind = find_nearest_O(H, o_arr, frame_lin);
                    O_atom* best_O = o_arr[max_ind];
                    (*H).set_O_conect(best_O);
                    (*best_O).up_H_count(H);
                    if((*best_O).get_H_count() == 3){
                        O_of_jump_H.push_back(best_O);
                        glosar(best_O->get_name(), change_O_glossary, glossary_O_of_jump_H);
                        instruction show_instruction = add_instruction_to_vector( best_O, frame_time, change_O_glossary);
                        arr_instruction_to_atom_visual.push_back(show_instruction);
                        change_O_glossary++;
                    }
                }
            }
            else{
                // parse data from lines in parrarel mode
                #pragma omp parallel for
                for (std::string line: lines) {
                    std::vector<std::string> re = split(line, ' ');
                    if ((re[1] == "1") and (in_vector(re[0], jump_H) )) {
                        H_atom* H = jump_H[in_vector(re[0], jump_H) - 1];
                        (*H).set_cord(re[2], re[3], re[4]);
                    }
                    else if (re[1] == "2"){
                        O_atom* O = o_arr[glossary_of_O[stoi(re[0])]];
                        (*O).set_cord(re[2], re[3], re[4]);
                    }
                }
                // calculates distance between H(hydrogen) that was in hydronym in previous frame.
                for (int i = 0; i < jump_H.size(); ++i) {
                    H_atom* H = jump_H[i];
                    O_atom belong_O = *(*H).get_O_conected();
                    float dist_to_prev_belong_O = (*H).dist_bt_atoms(belong_O, frame_lin);
                    #pragma omp parallel for
                    for (int j = 0; j < o_arr.size(); ++j) {
                        O_atom* O = o_arr[j];
                        float dist_to_pos_O = (*H).dist_bt_atoms(*O, frame_lin);
                        //If difference between H(hydrogen) and O(oxygen) from the previous frame is less than the difference with the nearest O from the current frame, then it is assumed that H has changed the orbital.
                        if (((dist_to_prev_belong_O - dist_to_pos_O) > water_length) and (belong_O.get_name() != O->get_name())){
                            O_atom* prev = (*H).get_O_conected();
                            (*prev).del_atom((*H).get_name());
                            (*H).set_O_conect(O);
                            (*O).up_H_count(H);
                            int index_to_change = (*prev).get_name();
                            int change_index = glossary_O_of_jump_H[index_to_change];
                            glossary_O_of_jump_H[index_to_change] = -1;
                            glosar(O->get_name(), change_index, glossary_O_of_jump_H);
                            O_of_jump_H[change_index] = O;
                            life_ar.push_back(life_time);
                            life_time = 0;
                            is_O_of_jump_H_change = true;
                            instruction show_instruction = add_instruction_to_vector(O, frame_time, change_index);
                            arr_instruction_to_atom_visual.push_back(show_instruction);
                        }
                    }
                }
            }
            frame_time++;
            life_time++;
            std::fill( std::begin( frame_lin ), std::end( frame_lin ), 0 );
            open_frame_count = 0;
            lines.clear();
            // If H(hydrogen) change orbital, then change O(oxygen) and H, which need to be controlled.
            if ((is_O_of_jump_H_change) or (first_frame)) {
                jump_H.clear();
                for (O_atom *O_observe: O_of_jump_H) {
                    std::vector<H_atom *> to_add = O_observe->get_H();
                    jump_H.insert(jump_H.end(), std::begin(to_add), std::end(to_add));
                }
            }
            is_O_of_jump_H_change = false;
            start_frame_coord = false;
            first_frame = false;
        }
    }

    is_gz ? (void)gzclose(infile) : newfile.close(); //close the file object.
    std::cout << frame_time << std::endl;
    write_to_file_res(life_ar, file);
    write_to_file_vis(arr_instruction_to_atom_visual, file);
    h_arr.clear();
    o_arr.clear();
    return 0;
}