#pragma once

#include "globals.hpp"

double computeSnakeHeuristicScore(Board state);
double computeMonotonicityScore(Board state);
double computeSmoothnessScore(Board state);
double evaluateState(Board state);
