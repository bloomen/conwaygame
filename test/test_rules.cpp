#include <libunittest/all.hpp>
#include <conwaygame/rules.h>


COLLECTION(test_n_alive_neighbors) {

TEST(test_none_alive_with_minimal_board) {
  const std::vector<bool> data = {false, false,
                                  false, false};
  const auto n_alive = cg::n_alive_neighbors(data, 0, 0, 2);
  ASSERT_EQUAL(0, n_alive);
}

TEST(test_all_alive_with_minimal_board) {
  const std::vector<bool> data = {true, true,
                                  true, true};
  const auto n_alive = cg::n_alive_neighbors(data, 0, 0, 2);
  ASSERT_EQUAL(8, n_alive);
}

void make_test_one_alive(int i, int j) {
  const auto n = 4;
  std::vector<bool> data(n*n, false);
  data[i * n + j] = true;
  const auto n_alive = cg::n_alive_neighbors(data, 2, 2, n);
  std::stringstream message;
  message  << "(" << i << ", " << j << ")";
  ASSERT_EQUAL_MSG(1, n_alive, message.str());
}

TEST(test_one_alive) {
  make_test_one_alive(1, 1);
  make_test_one_alive(1, 2);
  make_test_one_alive(1, 3);
  make_test_one_alive(2, 1);
  make_test_one_alive(2, 3);
  make_test_one_alive(3, 1);
  make_test_one_alive(3, 2);
  make_test_one_alive(3, 3);
}

void make_test_one_alive_with_wrapping(int i, int j) {
  const auto n = 5;
  std::vector<bool> data(n*n, false);
  data[i * n + j] = true;
  const auto n_alive = cg::n_alive_neighbors(data, 4, 4, n);
  std::stringstream message;
  message  << "(" << i << ", " << j << ")";
  ASSERT_EQUAL_MSG(1, n_alive, message.str());
}

TEST(test_one_alive_with_wrapping) {
  make_test_one_alive_with_wrapping(3, 3);
  make_test_one_alive_with_wrapping(3, 4);
  make_test_one_alive_with_wrapping(3, 0);
  make_test_one_alive_with_wrapping(4, 3);
  make_test_one_alive_with_wrapping(4, 0);
  make_test_one_alive_with_wrapping(0, 3);
  make_test_one_alive_with_wrapping(0, 4);
  make_test_one_alive_with_wrapping(0, 0);
}

}


COLLECTION(test_next_generation) {

const int n_threads = 4;

TEST(test_with_minimal_dead_board) {
  const std::vector<bool> data = {false, false,
                                  false, false};
  auto result = data;
  cg::next_generation(result, data, 2);
  ASSERT_EQUAL_CONTAINERS(data, result);
}

TEST(test_with_minimal_alive_board) {
  const std::vector<bool> data = {true, true,
                                  true, true};
  auto result = data;
  cg::next_generation(result, data, 2);
  const std::vector<bool> expected = {false, false,
                                      false, false};
  ASSERT_EQUAL_CONTAINERS(expected, result);
}

TEST(test_with_real_board) {
  const std::vector<bool> data = {true, false, true, false, false,
                                  false, true, false, true, false,
                                  false, false, false, true, false,
                                  false, true, true, false, false,
                                  false, true, false, true, false};
  auto result = data;
  cg::next_generation(result, data, 5);
  const std::vector<bool> expected = {true, false, false, true, true,
                                      false, true, false, true, true,
                                      false, true, false, true, false,
                                      false, true, false, true, false,
                                      true, false, false, true, false};
  ASSERT_EQUAL_CONTAINERS(expected, result);
}

}

COLLECTION(test_random_vector) {

TEST(test_with_alive_zero) {
  std::mt19937 gen(1);
  const auto data = cg::random_vector(gen, 10, 0);
  ASSERT_EQUAL(10u, data.size());
  for (auto value : data)
    ASSERT_FALSE(value);
}

TEST(test_with_alive_one) {
  std::mt19937 gen(1);
  const auto data = cg::random_vector(gen, 10, 1);
  ASSERT_EQUAL(10u, data.size());
  for (auto value : data)
    ASSERT_TRUE(value);
}

TEST(test_with_alive_small) {
  std::mt19937 gen(1);
  const auto data = cg::random_vector(gen, 10, 0.1);
  ASSERT_EQUAL(10u, data.size());
  const std::vector<bool> exp = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
  ASSERT_EQUAL_CONTAINERS(exp, data);
}

TEST(test_with_alive_large) {
  std::mt19937 gen(1);
  const auto data = cg::random_vector(gen, 10, 0.9);
  ASSERT_EQUAL(10u, data.size());
  const std::vector<bool> exp = {1, 0, 1, 0, 1, 1, 1, 0, 1, 1};
  ASSERT_EQUAL_CONTAINERS(exp, data);
}

}
