#pragma once

#include <iostream>
#include "Eigen/Dense"
#include <random>


using namespace std;

using Board = Eigen::MatrixXi;
using Cells = vector<pair<int, int>>;

enum Move { Left, Right, Up, Down };

extern random_device rd;
extern mt19937 rng;
