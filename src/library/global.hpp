#pragma once

#include <iostream>
#include "eigen3/Eigen/Dense"
#include <random>

using namespace std;

using Board = Eigen::MatrixXi;
enum Move { Left, Right, Up, Down };

random_device rd;
mt19937 rng(rd());

