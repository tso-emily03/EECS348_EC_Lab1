/**
 * @file main.cpp
 * @author Emily Tso (tsoemily@ku.edu)
 * @brief Extra Credit Lab 1 for EECS 348
 * @version 0.1
 * @date 2023-02-19
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <functional>

#include "mat.hpp"

#define INPUT_FILE "input.txt"

class Programmer {
public:

	/**
	 * @brief Construct a new Programmer object
	 * 
	 * @param id Id of the Object
	 * @param taken True when object is taken
	 */
	Programmer (int id, bool taken=false) : id(id), taken(taken) {}

	int id;	// programmer id
	bool taken;	// check to see if programmer is taken
	int res;

	int get_pref(int i) {
		return prefs.at(i);	// return prefs[i].
	}

	std::vector<int> prefs;

	friend std::ostream &operator<<(std::ostream &os, const Programmer &programmer) {
		os<<programmer.id<<": ";
		for (const auto & ele : programmer.prefs) {
			os<<ele<<" ";
		}
		return os;
	}
};

using Department = Programmer;

static std::vector<Programmer> programmers;
static std::vector<Department> departments;

void read_from_file(std::string filename);

Programmer &find(int id, std::vector<Programmer> &prgs) {
	for (auto & programmer : prgs)
		if (programmer.id == id)
			return programmer;
	return prgs.back();
}

Programmer &get_top(Programmer &a, std::vector<Programmer> &b) {
	for (auto p : a.prefs) {
		auto t = !find(p, b).taken;
		if (t)
			return find(p, b);
	}
	throw std::runtime_error ("oops...");

}

Programmer &get_top_programmer(Department &dept) {
	return get_top(dept, programmers);
}

bool greater_top(Programmer &p, Department &a, Department &b) {
	int loc_a = 0, loc_b = 0;
	for (; loc_a < a.prefs.size(); ++loc_a)
		if (find(a.get_pref(loc_a), programmers).id == p.id)
			break;
	for (; loc_b < b.prefs.size(); ++loc_b)
		if (find(b.get_pref(loc_b), programmers).id == p.id)
			break;
	return loc_a < loc_b;
}

int main () {
	read_from_file (INPUT_FILE);
	int i;
	for (auto & department : departments) {
		i = 0;
		START_OVER:
		auto &top = find(department.get_pref(i), programmers);

		if (top.taken) {
			i++;
			goto START_OVER;
		}

		for (size_t j = 0; j < top.prefs.size(); ++j) {
			auto &dept = find(top.prefs[j], departments);
			if (dept.taken) 
				continue;

			bool overwrite = false;
			auto &d_top = get_top_programmer(dept);
			int d_top_id = d_top.id;

			overwrite = dept.id == department.id;
			if (overwrite) {
				d_top_id = top.id;
				goto I2_SKIP;
			}
			if (d_top_id == top.id) {
				if(greater_top(top, department, dept)) {
					I2_SKIP:
					top.taken = true;
					department.res = top.id;
					goto CONT;
				}
				++i;
				goto START_OVER;
			}
		}
		CONT:
		int dept_id = department.id;
		int dept_re = department.res;
		department.taken = true;
		std::printf("Department #%i took Programmer #%i\n", dept_id, dept_re);
	}

	return 0;
}

void read_from_file(std::string filename) {
	std::fstream file {filename, std::ios_base::in};

	Matrix::mat2 mat;


	auto f = [](std::string x)->Matrix::mat1 {
		Matrix::mat1 mat;
		std::string y;
		for (char c : x) {
			switch (c) {
				case ' ':
					mat.push_back(std::stoi(y));
					y.clear();
				default:
					y += c;
			};
		}
		if (y.length() > 0)
			mat.push_back(std::stoi(y));
		return mat;
	};

	std::string x;
	while (std::getline(file, x))
		mat.push_back(f(x));


	auto [high, low] = Matrix::split_mat2(mat, mat.size()/2);

	high = Matrix::transpose(high);
	low  = Matrix::transpose(low);

	for (size_t i = 0; i < high.size(); ++i) {
		departments.push_back(Department(static_cast<int>(i+1)));
		departments.at(i).prefs = high.at(i);
	}

	for (size_t i = 0; i < low.size(); ++i) {
		programmers.push_back(Programmer(static_cast<int>(i+1)));
		programmers.at(i).prefs = low.at(i);
	}

	file.close();
}
