#include "gradient_descent.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>

static std::string expect_token(std::istringstream &line_stream, const std::string &keyword) {
    std::string tok;
    if (!(line_stream >> tok) || tok != keyword)
        throw std::runtime_error("Malformed Gradient Descent input: expected keyword '" + keyword + "'");
    return tok;
}

GDInput read_gd_input(const std::string &path) {
    std::ifstream fin(path);
    if (!fin.is_open())
        throw std::runtime_error("Could not open input file: " + path);

    GDInput in;
    std::string line;
    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing DEGREE line");
    {
        std::istringstream ls(line);
        expect_token(ls, "DEGREE");
        if (!(ls >> in.degree))
            throw std::runtime_error("Malformed Gradient Descent input: missing degree value");
    }
    if (in.degree < 0)
        throw std::runtime_error("Invalid Gradient Descent input: degree must be non-negative");

    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing COEFFICIENTS line");
    {
        std::istringstream ls(line);
        expect_token(ls, "COEFFICIENTS");
        double c;
        while (ls >> c) in.coeffs.push_back(c);
    }
    if (static_cast<int>(in.coeffs.size()) != in.degree + 1)
        throw std::runtime_error("Invalid Gradient Descent input: coefficient count must equal degree + 1");
    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing INITIAL_X line");{
        std::istringstream ls(line);
        expect_token(ls, "INITIAL_X");
        if (!(ls >> in.x0))
            throw std::runtime_error("Malformed Gradient Descent input: missing initial x value");
    }
    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing LEARNING_RATE line");{
        std::istringstream ls(line);
        expect_token(ls, "LEARNING_RATE");
        if (!(ls >> in.learning_rate))
            throw std::runtime_error("Malformed Gradient Descent input: missing learning rate value");
    }
    if (!(in.learning_rate > 0.0))
        throw std::runtime_error("Invalid Gradient Descent input: learning rate must be positive");
    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing TOLERANCE line");{
        std::istringstream ls(line);
        expect_token(ls, "TOLERANCE");
        if (!(ls >> in.tolerance))
            throw std::runtime_error("Malformed Gradient Descent input: missing tolerance value");
    }
    if (!(in.tolerance > 0.0))
        throw std::runtime_error("Invalid Gradient Descent input: tolerance must be positive");
    if (!std::getline(fin, line))
        throw std::runtime_error("Malformed Gradient Descent input: missing MAX_ITERATIONS line");{
        std::istringstream ls(line);
        expect_token(ls, "MAX_ITERATIONS");
        if (!(ls >> in.max_iterations))
            throw std::runtime_error("Malformed Gradient Descent input: missing max iterations value");
    }
    if (in.max_iterations <= 0)
        throw std::runtime_error("Invalid Gradient Descent input: max iterations must be positive");

    return in;
}

double poly_eval(const std::vector<double> &coeffs, double x) {
    double result = 0.0;
    for (int i = static_cast<int>(coeffs.size()) - 1; i >= 0; --i) {
        result = result * x + coeffs[i];
    }
    return result;
}

double poly_derivative_eval(const std::vector<double> &coeffs, double x) {
    int d = static_cast<int>(coeffs.size()) - 1;
    double result = 0.0;
    for (int i = d; i >= 1; --i) {
        result = result * x + static_cast<double>(i) * coeffs[i];
    }
    return result;
}

GDResult run_gradient_descent(const GDInput &in) {
    GDResult r;
    r.degree = in.degree;

    double x = in.x0;
    long long iter = 0;
    double grad = poly_derivative_eval(in.coeffs, x);

    while (std::fabs(grad) > in.tolerance && iter < in.max_iterations) {
        x = x - in.learning_rate * grad;
        grad = poly_derivative_eval(in.coeffs, x);
        ++iter;
    }

    r.final_x = x;
    r.final_fx = poly_eval(in.coeffs, x);
    r.iterations = iter;
    r.converged = std::fabs(grad) <= in.tolerance;
    return r;
}
