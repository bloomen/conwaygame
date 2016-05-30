#include "conwaygame/rules.h"
#include "conwaygame/expects.h"
#include "conwaygame/cxxpool.h"


namespace cg {


int n_alive_neighbors(const std::vector<bool>& data,
                      std::int64_t i, std::int64_t j, std::int64_t n) {
  EXPECTS(n > 1);
  EXPECTS(data.size() == static_cast<std::uint64_t>(n * n));
  EXPECTS(i >= 0 && i < n);
  EXPECTS(j >= 0 && j < n);

  auto use_or_wrap = [n](std::int64_t index) {
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
  std::vector<bool> result(data.size());
  cxxpool::thread_pool pool{n_threads};
  for (std::int64_t i=0; i < n; ++i) {
    for (std::int64_t j=0; j < n; ++j) {
      pool.push([&data, &result, i, j, n] {
        const auto k = i * n + j;
        const auto n_alive = n_alive_neighbors(data, i, j, n);
        if (data[k] && (n_alive < 2 || n_alive > 3)) {  // alive
          result[k] = false;  // under-population or over-population
        } else if (!data[k] && n_alive == 3) {  // dead
          result[k] = true;  // reproduction
        } else {
          result[k] = data[k];
        }
      });
    }
  }
  return result;
}


}  // namespace cg
