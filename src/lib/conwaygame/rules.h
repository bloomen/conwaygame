#pragma once
#include <cstddef>
#include <vector>
#include <stdexcept>
#include <random>


namespace cg {


int n_alive_neighbors(const std::vector<bool>& data,
                      std::int64_t i, std::int64_t j, std::int64_t n);


std::vector<bool> next_generation(const std::vector<bool>& data,
                                  std::int64_t n, int n_threads);


std::vector<bool> random_vector(std::mt19937& gen, std::int64_t n,
                                float ratio_alive);


}  // namespace cg
