#pragma once

#include "globals.hpp"


double evaluateState(Board state) {

    /*  
        A preferable board should have the following characteristics:

        1. Smoothness - Arrange tiles with same/similar values adjacent
        2. Monotonicity - Tiles should be arranged in ascending/descending order along rows and columns
        3. Empty Cells - A board should have as many empty cells as possible
    */

    static const double MONOTONICITY_WEIGHT = 0.5;
    double SMOOTHNESS_WEIGHT = log2(state.maxCoeff());
    static const double EMPTY_CELLS_WEIGHT = 10.;

    int n = state.rows();
    double smoothness = 0.0;
    int empty_cells = 0;

    // monotonicity values
    int left_to_right = 0; // decreasing from left to right
    int right_to_left = 0; // decreasing from right to left
    int top_to_bottom = 0; // decreasing from up to down
    int bottom_to_top = 0; // decreasing from down to up

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (state(i, j) == 0) empty_cells += 1;
            if (j < n-1) {
                smoothness -= abs(state(i, j) - state(i, j+1));
                if (state(i, j) > state(i, j+1)) left_to_right += 1;
                else if (state(i, j) < state(i, j+1)) right_to_left += 1;
                else {
                    left_to_right += 1;
                    right_to_left += 1;
                }
            }
            if (i < n-1) {
                smoothness -= abs(state(i, j) - state(i+1, j));
                if (state(i, j) > state(i+1, j)) top_to_bottom += 1;
                else if (state(i, j) < state(i+1, j)) bottom_to_top += 1;
                else {
                    top_to_bottom += 1;
                    bottom_to_top += 1;
                }
            }
        }
    }

    int monotinicity = max({
        left_to_right + bottom_to_top, 
        right_to_left + bottom_to_top, 
        right_to_left + top_to_bottom, 
        left_to_right + top_to_bottom
    });
    double utility = (
        monotinicity * MONOTONICITY_WEIGHT +
        (smoothness / SMOOTHNESS_WEIGHT) +
        empty_cells * EMPTY_CELLS_WEIGHT
    );

    return utility;
}
