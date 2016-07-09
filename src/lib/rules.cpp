#include "conwaygame/rules.h"
#include "conwaygame/expects.h"
#include "conwaygame/cxxpool.h"
#include <algorithm>


namespace cg {


int n_alive_neighbors(const std::vector<bool>& data,
                      std::int64_t i, std::int64_t j, std::int64_t n) {
  EXPECTS(n > 1);
  EXPECTS(data.size() == static_cast<std::uint64_t>(n * n));
  EXPECTS(i >= 0 && i < n);
  EXPECTS(j >= 0 && j < n);

  auto use_or_wrap = [n](std::int64_t index) -> std::int64_t {
    if (index < 0)
      return n - 1;
    else if (index > n - 1)
      return 0L;
    else
      return index;
  };
  const auto prev_i = use_or_wrap(i - 1);
  const auto next_i = use_or_wrap(i + 1);
  const auto prev_j = use_or_wrap(j - 1);
  const auto next_j = use_or_wrap(j + 1);

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


std::vector<bool> next_generation(const std::vector<bool>& data,
                                  std::int64_t n, int n_threads) {
  EXPECTS(n > 1);
  EXPECTS(data.size() == static_cast<std::uint64_t>(n * n));
  EXPECTS(n_threads >= 0);
  cxxpool::thread_pool pool{n_threads};
  std::vector<std::future<bool>> futures;
  for (std::int64_t i=0; i < n; ++i) {
    for (std::int64_t j=0; j < n; ++j) {
      futures.emplace_back(pool.push([&data, i, j, n]() -> bool {
        const auto k = i * n + j;
        const auto n_alive = n_alive_neighbors(data, i, j, n);
        if (data[k] && (n_alive < 2 || n_alive > 3)) {  // alive
          return false;  // under-population or over-population
        } else if (!data[k] && n_alive == 3) {  // dead
          return true;  // reproduction
        } else {
          return data[k];
        }
      }));
    }
  }
  return cxxpool::get(futures.begin(), futures.end());
}


std::vector<bool> random_vector(std::mt19937& gen, std::int64_t n,
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
