//
// Created by danile on 6/3/22.
//

#include "parc_and_analis.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <zlib.h>
#include <cstring>
#include "classes_for_water/H_atom.h"
#include "classes_for_water/O_atom.h"
#include "omp.h"


std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str);
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

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


void write_to_file_res(std::vector<int> life_ar){
    std::string res_str;
    std::ofstream res_file;
    res_file.open ("res_mod.txt");
    std::cout << ' ' <<std::endl;
    for(int el:life_ar){
        res_str += std::to_string(el) + ", ";
    }
    res_str.pop_back();
    res_str.pop_back();
    res_file << res_str;
    res_file.close();
}


std::vector<int> change_glosar(std::vector<int> glosar_O_of_jump_H, int O_nameaecf, int change_index){
    glosar_O_of_jump_H[O_nameaecf] = change_index;
    return glosar_O_of_jump_H;
}


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

int hydro_life(std::string file, bool is_gz){
    std::vector<int> life_ar;
    std::vector<int> frame_time_ar;
    float water_lentg = 0.96;
    int frame_time = 0;
    std::fstream newfile;
    gzFile infile;
    std::string start_frame = "ITEM: BOX BOUNDS pp pp pp";
    std::string end_frame ="ITEM: TIMESTEP";
    std::string start_coord = "ITEM: ATOMS id type xs ys zs";
    std::string tp;
    bool open_frame = false;
    bool first_frame = true;
    bool start_frame_coord = false;
    std::vector<H_atom*> h_arr;
    std::vector<O_atom*> o_arr;
    int open_frame_count = 0;
    std::vector<O_atom*> O_of_jump_H;
    std::vector<int> glosar_O_of_jump_H;
    std::vector<H_atom*> jump_H;
    std::vector<int> nubs_ar;
    std::vector<std::string> lines;
    std::vector<char> char_line;
    float frame_lin[3];
    int life_time = 0;
    int order_count = 0;
    int gloser_O_count = 0;
    bool is_O_of_jump_H_change = false;
    if (is_gz) {
        infile = gzopen(file.c_str(), "rb");
        if (!infile) {
            std::cout << "Unccorect file input" << std::endl;
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
            std::cout << "Unccorect file input" << std::endl;
            return -1;
        }
        else {
            getline(newfile, tp);
        }
    }


    while(!tp.empty()){

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
        else if ((start_frame_coord) and (open_frame_count < 3)){
            std::vector<std::string> re = split(tp, ' ');
            frame_lin[open_frame_count] = std::stof( re[1]);
            open_frame_count++;
        }
        else if ((open_frame_count >= 3) and (tp != start_coord)){
            if (first_frame) {
                std::vector<std::string> re = split(tp, ' ');
                if (re[1] == "1") {
                    h_arr.push_back(new H_atom(re));
                }
                else if (re[1] == "2"){
                    o_arr.push_back(new O_atom(re));
                    glosar(stoi(re[0]), order_count, nubs_ar);
                    order_count++;
                }
            }
            else {
                if (end_frame != tp) {
                    lines.push_back(tp);
                }
            }
        }
        if (end_frame == tp){
            if (first_frame) {
                for (int i = 0; i < h_arr.size(); ++i) {
                    H_atom* H = h_arr[i];
                    int max_ind = find_nearest_O(H, o_arr, frame_lin);
                    O_atom* best_O = o_arr[max_ind];
                    (*H).set_O_conect(best_O);
                    (*best_O).up_H_count(H);
                    if((*best_O).get_H_count() == 3){
                        O_of_jump_H.push_back(best_O);
                        glosar(best_O->get_name(), gloser_O_count, glosar_O_of_jump_H);
                        gloser_O_count++;
                    }
                }
            }
            else{
                #pragma omp parallel for
                for (std::string line: lines) {
                    std::vector<std::string> re = split(line, ' ');
                    if ((re[1] == "1") and (in_vector(re[0], jump_H) )) {
                        H_atom* H = jump_H[in_vector(re[0], jump_H) - 1];
                        (*H).set_cord(re[2], re[3], re[4]);
                    }
                    else if (re[1] == "2"){
                        O_atom* O = o_arr[nubs_ar[stoi(re[0])]];
                        (*O).set_cord(re[2], re[3], re[4]);
                    }
                }
                bool brake_flag = false;
                for (int i = 0; i < jump_H.size(); ++i) {
                    H_atom* H = jump_H[i];
                    O_atom belong_O = *(*H).get_O_conected();
                    float dist_to_prev_belong_O = (*H).dist_bt_atoms(belong_O, frame_lin);
                    #pragma omp parallel for
                    for (int j = 0; j < o_arr.size(); ++j) {
                        O_atom* O = o_arr[j];
                        float dist_to_pos_O = (*H).dist_bt_atoms(*O, frame_lin);
                        if (((dist_to_prev_belong_O - dist_to_pos_O) > water_lentg) and (belong_O.get_name() != O->get_name())){
                            O_atom* prev = (*H).get_O_conected();
                            (*prev).del_atom((*H).get_name());
                            (*H).set_O_conect(O);
                            (*O).up_H_count(H);
                            int index_to_change = (*prev).get_name();
                            int change_index = glosar_O_of_jump_H[index_to_change];
                            glosar_O_of_jump_H[index_to_change] = -1;
                            glosar(O->get_name(), change_index, glosar_O_of_jump_H);
                            O_of_jump_H[change_index] = O;
                            life_ar.push_back(life_time);
                            life_time = 0;
                            is_O_of_jump_H_change = true;
                        }
                    }
                }
            }
            frame_time++;
            life_time++;
            std::fill( std::begin( frame_lin ), std::end( frame_lin ), 0 );
            open_frame_count = 0;
            open_frame = false;
            lines.clear();
            if ((is_O_of_jump_H_change) or (first_frame)) {
                jump_H.clear();
                for (O_atom *O_observ: O_of_jump_H) {
                    std::vector<H_atom *> to_add = O_observ->get_H();
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
    write_to_file_res(life_ar);
    h_arr.clear();
    o_arr.clear();
    return 0;
}