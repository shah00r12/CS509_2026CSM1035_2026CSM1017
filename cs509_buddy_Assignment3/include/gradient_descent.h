#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

#include <vector>
#include <string>
struct GDInput {
    int degree = 0;
    std::vector<double> coeffs; 
    double x0 = 0.0;
    double learning_rate = 0.0;
    double tolerance = 0.0;
    long long max_iterations = 0;
};

struct GDResult {
    int degree = 0;
    double final_x = 0.0;
    double final_fx = 0.0;
    long long iterations = 0;
    bool converged = false;
};
GDInput read_gd_input(const std::string &path);
double poly_eval(const std::vector<double> &coeffs, double x);
double poly_derivative_eval(const std::vector<double> &coeffs, double x);
GDResult run_gradient_descent(const GDInput &in);

#endif
