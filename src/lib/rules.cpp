#include "conwaygame/rules.h"
#include "conwaygame/expects.h"
#include "conwaygame/cxxpool.h"
#include <algorithm>


namespace cg {

namespace {

int use_or_wrap(int index, int n) {
  if (index < 0)
    return n - 1;
  else if (index > n - 1)
    return 0;
  else
    return index;
}

}

int n_alive_neighbors(const std::vector<bool>& data,
                      int i, int j, int n) {
  const auto prev_i = use_or_wrap(i - 1, n);
  const auto next_i = use_or_wrap(i + 1, n);
  const auto prev_j = use_or_wrap(j - 1, n);
  const auto next_j = use_or_wrap(j + 1, n);

  int n_alive = 0;
  if (data[prev_i * n + prev_j]) ++n_alive;
  if (data[prev_i * n + j]) ++n_alive;
  if (data[prev_i * n + next_j]) ++n_alive;
  if (data[i * n + prev_j]) ++n_alive;
  if (data[i * n + next_j]) ++n_alive;
  if (data[next_i * n + prev_j]) ++n_alive;
  if (data[next_i * n + j]) ++n_alive;
  if (data[next_i * n + next_j]) ++n_alive;
  return n_alive;
}


void next_generation(std::vector<bool>& result,
                     const std::vector<bool>& data,
                     int n) {
  EXPECTS(n > 1);
  EXPECTS(data.size() == static_cast<std::uint64_t>(n * n));
  EXPECTS(result.size() == data.size());

  for (int i=0; i < n; ++i) {
    for (int j=0; j < n; ++j) {
      const auto k = i * n + j;
      const auto n_alive = n_alive_neighbors(data, i, j, n);
      if (data[k] && (n_alive < 2 || n_alive > 3)) {  // alive
        result[k] = false;  // under-population or over-population
      } else if (!data[k] && n_alive == 3) {  // dead
        result[k] = true;  // reproduction
      } else {
        result[k] = data[k];
      }
    }
  }
}


std::vector<bool> random_vector(std::mt19937& gen, int n,
                                float ratio_alive) {
  EXPECTS(ratio_alive >= 0);
  EXPECTS(ratio_alive <= 1);
  std::vector<bool> data(n);
  std::uniform_real_distribution<float> dist(0, 1);
  std::generate(data.begin(), data.end(), [&gen, &dist, ratio_alive]{
    return dist(gen) < ratio_alive;
  });
  return data;
}

}  // namespace cg
