#pragma once
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <random>


namespace cg {


int n_alive_neighbors(const std::vector<bool>& data,
                      int i, int j, int n);


void next_generation(std::vector<bool>& result,
                     const std::vector<bool>& data,
                     int n);


std::vector<bool> random_vector(std::mt19937& gen, int n,
                                float ratio_alive);


}  // namespace cg
