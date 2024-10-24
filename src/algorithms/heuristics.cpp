#include "heuristics.hpp"
#include "board.hpp"

// simple heuristic which often achieves the 2048-tile
double computeSnakeHeuristicScore(Board state) {

    int n = state.rows();
    Eigen::MatrixXi weight_mat(n, n);

    // construct snake-like weight-matrix
    // TODO: construct nxn weight-matrix
    weight_mat << 2,    4,    8,   16,
             256,  128,  64,   32,
             512, 1024,   2048,  4096,
            65536, 32768,  16384,  8192;

    float utility = 0.0f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            utility += weight_mat(i, j) * state(i, j);
        }
    }

    return utility;
}

double computeMonotonicityHeuristicScore(Board state) {

    int n = state.rows();
    Eigen::Matrix4d weight_mat;

    weight_mat << 
        7, 6, 5, 4,
        6, 5, 4, 3,
        5, 4, 3, 2,
        4, 3, 2, 1;

    float utility = 0.0f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            utility += weight_mat(i, j) * state(i, j);
        }
    }

    return utility;
}

Eigen::MatrixXd buildMonotonictyWeightMatrix(int n) {
    Eigen::MatrixXd matrix(n, n);
    int e = 6;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix(i, j) = pow(4, e-j);
        }
        e--;
    }
    return matrix;
}

static const auto MONO_WEIGHT_MATRIX = buildMonotonictyWeightMatrix(4);

double computeMonotonicityOfVector(Eigen::VectorXi vector) {
    int n = vector.rows();
    double vector_monotonicity = 0.0;
    for (int i = 0; i < n-1; i++) {
            vector_monotonicity += vector(i) - vector(i + 1);
    }
    return vector_monotonicity;
}

double computeMonotonicityScore(Board state) {

    int n = state.rows();
    double monotonicity = 0.0;
    Eigen::VectorXd all_monos = Eigen::VectorXd::Zero(n);

    // for (int i = 0; i < n; i++) {
    //     Eigen::VectorXi row_vec = state.row(i);
    //     Eigen::VectorXi col_vec = state.col(i);
    //     double monotonicity1 = computeMonotonicityOfVector(row_vec);
    //     double monotonicity2 = computeMonotonicityOfVector(col_vec);
    //     double monotonicity3 = computeMonotonicityOfVector(row_vec.reverse());
    //     double monotonicity4 = computeMonotonicityOfVector(col_vec.reverse());
    //     all_monos(0) += monotonicity1 + monotonicity2;
    //     all_monos(1) += monotonicity2 + monotonicity3;
    //     all_monos(2) += monotonicity3 + monotonicity4;
    //     all_monos(3) += monotonicity4 + monotonicity1;
    // }
    auto cpy = MONO_WEIGHT_MATRIX;
    for (int i = 0; i < n; i++) {
        double mono = 0.0;
        for (int j = 0; j < n; j++){
            mono += state(i, j) * cpy(i, j);
        }
        all_monos(i) = mono;
        // cout << endl << "Iteration: " << i << endl << cpy << endl;
        // if (i == 0 || i == 2) cpy = cpy.transpose().rowwise().reverse();
        // if (i == 1) cpy = cpy.colwise().reverse();
    }

    return all_monos.maxCoeff();
}

double computeSmoothnessScore(Board state) {

    int n = state.rows();
    int smoothness = 0;
    int sum = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n-1) smoothness -= abs(state(i, j) - state(i, j+1));
            if (i < n-1) smoothness -= abs(state(i, j) - state(i+1, j));
            sum += state(i, j);
        }
    }

    return smoothness;
}

int last_best = 0;
double evaluateState(Board state) {

    /*  THIS FUNTION EVALUATES A BOARD STATE BASED ON SEVERAL METRICS
    
        A good Board should have the following characteristics:

            1. The max-tile should be in a corner
            2. Tiles should be arranged in a monotonous manner
            3. Many empty cells are preferred -> low risk of getting stuck
            
        Optional:

            4. Prefer Smooth boards -> tile with same (or similar) values arange in adjacent order
            5. Consider merges 
            
        If there is no move doable in the state, apply a penalty

    */

    // static const double MONOTONICITY_WEIGHT = 20.;
    // static const double SMOOTHNESS_WEIGHT = 7.;
    // static const double EMPTY_CELLS_WEIGHT = 50.;

    static const double MONOTONICITY_WEIGHT = 0.5;
    static const double SMOOTHNESS_WEIGHT = 7.;
    static const double EMPTY_CELLS_WEIGHT = 10.;

    int n = state.rows();
    int empty_cells = 0;
    double monotonicity = 0.0;
    double smoothness = 0.0;

    Board state_cpy = state;

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

    // left_to_right+top_to_bottom, right_to_left+top_to_bottom, bottom_to_top+left_to_right, bottom_to_top+right_to_left
    // left_to_right+right_to_left+top_to_bottom, left_to_right+right_to_left+bottom_to_top, left_to_right+right_to_left+top_to_bottom, left_to_right+right_to_left+bottom_to_top
    double max_log_tile = log2(state.maxCoeff());
    int monotinicity = max({left_to_right+bottom_to_top, right_to_left+bottom_to_top, right_to_left+top_to_bottom, left_to_right+top_to_bottom});
    if (monotinicity > last_best) {
        last_best = monotinicity;
    }

    // cout << monotinicity << "  " << smoothness/state.sum() << "  " << empty_cells << "  " << max_log_tile << endl;

    bool is_corner = state(0, 0) == state.maxCoeff() || state(3, 0) == state.maxCoeff() || state(0, 3) == state.maxCoeff() || state(3, 3) == state.maxCoeff();

    // smoothness = sum/smoothness;
    double utility = (
        monotinicity * MONOTONICITY_WEIGHT +
        smoothness / max_log_tile +
        empty_cells * EMPTY_CELLS_WEIGHT
        // max_log_tile * is_corner + 0.5
    );
    // smoothness with empty tiles is decent
    // cout << utility << endl;

    return utility;
}
