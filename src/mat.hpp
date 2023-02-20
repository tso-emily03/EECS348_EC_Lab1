#ifndef mat_hpp
#define mat_hpp

#include <iostream>
#include <cmath>
#include <vector>

namespace Matrix {
    using mat1 = std::vector<int>;
    using mat2 = std::vector<mat1>;

    mat2 zeros(size_t x, size_t y) {
        mat1 mm;
        mm.resize(x, 0);
        mat2 out;
        out.resize(y, mm);
        return out;
    }

    mat2 transpose (mat2 &in) {
        mat2 out = zeros(in.at(0).size(), in.size());
        for (size_t i = 0; i < in.size(); ++i) {
            for (size_t j = 0; j < in.at(i).size(); ++j) {
                out[j][i] = in[i][j];
            }
        }
        return out;
    }

    void print_mat2(const mat2 &in) {
        std::cout<<"{\n";
        for (const auto & vec : in) {
            std::cout<<'{';
            for (const auto & ele : vec)
                std::cout<<ele<<" ";
            std::cout<<"}\n";
        }
        std::cout<<"}\n";
    }

    std::pair<mat2, mat2> split_mat2(const mat2 &in, const size_t size) {
        std::pair<mat2, mat2> data;

        for (size_t i = 0; i < in.size(); ++i) {
            if (i < size)
                data.first.push_back(in.at(i));
            else
                data.second.push_back(in.at(i));
        }
        return data;
    }

};


#endif
