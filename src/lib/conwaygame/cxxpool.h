/**
 * A portable, header-only thread pool for C++
 * @version 1.0.1
 * @author Christian Blume (chr.blume@gmail.com)
 * @copyright 2015-2016 by Christian Blume
 * cxxpool is released under the MIT license:
 * http://www.opensource.org/licenses/mit-license.php
 */
#pragma once
#include <thread>
#include <future>
#include <stdexcept>
#include <queue>
#include <utility>
#include <functional>
#include <memory>
#include <vector>
#include <chrono>
#include <cstddef>


namespace cxxpool {

/**
 * Waits until all futures contain results
 */
template<typename Iterator>
inline
void wait(Iterator first, Iterator last) {
  for (; first != last; ++first)
    first->wait();
}
/**
 * Waits until all futures contain results with a given timeout duration and
 * returns a container of std::future::status
 */
template<typename Result, typename Iterator, typename Rep, typename Period>
inline
Result wait_for(Iterator first, Iterator last,
                const std::chrono::duration<Rep, Period>& timeout_duration,
                Result result) {
  for (; first != last; ++first)
    result.emplace_back(first->wait_for(timeout_duration));
  return result;
}
/**
 * Waits until all futures contain results with a given timeout duration and
 * returns a vector of std::future::status
 */
template<typename Iterator, typename Rep, typename Period>
inline
std::vector<std::future_status> wait_for(
    Iterator first, Iterator last,
    const std::chrono::duration<Rep, Period>& timeout_duration) {
  return wait_for(first, last, timeout_duration,
                  std::vector<std::future_status>{});
}
/**
 * Waits until all futures contain results with a given timeout time and
 * returns a container of std::future::status
 */
template<typename Result, typename Iterator, typename Clock, typename Duration>
inline
Result wait_until(
     Iterator first, Iterator last,
     const std::chrono::time_point<Clock, Duration>& timeout_time,
     Result result) {
  for (; first != last; ++first)
    result.emplace_back(first->wait_until(timeout_time));
  return result;
}
/**
 * Waits until all futures contain results with a given timeout time and
 * returns a vector of std::future::status
 */
template<typename Iterator, typename Clock, typename Duration>
inline
std::vector<std::future_status> wait_until(
      Iterator first, Iterator last,
      const std::chrono::time_point<Clock, Duration>& timeout_time) {
  return wait_until(first, last, timeout_time,
                    std::vector<std::future_status>{});
}


namespace detail {


template<typename Iterator>
struct future_info {
  typedef typename std::iterator_traits<Iterator>::value_type future_type;
  typedef typename std::result_of<
      decltype(&future_type::get)(future_type)>::type value_type;
  static constexpr bool is_void = std::is_void<value_type>::value;
};


}  // namespace detail

/**
 * Calls get() on all futures
 */
template<typename Iterator, typename = typename std::enable_if<
                            detail::future_info<Iterator>::is_void>::type>
inline
void get(Iterator first, Iterator last) {
  for (; first != last; ++first)
    first->get();
}
/**
 * Calls get() on all futures and stores the results in the returned container
 */
template<typename Result, typename Iterator,
                          typename = typename std::enable_if<
                          !detail::future_info<Iterator>::is_void>::type>
inline
Result get(Iterator first, Iterator last, Result result) {
  for (; first != last; ++first)
    result.emplace_back(first->get());
  return result;
}
/**
 * Calls get() on all futures and stores the results in the returned vector
 */
template<typename Iterator,
         typename = typename std::enable_if<
         !detail::future_info<Iterator>::is_void>::type>
inline
std::vector<typename detail::future_info<Iterator>::value_type>
get(Iterator first, Iterator last) {
  return cxxpool::get(first, last,
         std::vector<typename detail::future_info<Iterator>::value_type>{});
}


namespace detail {


template<typename Index, Index max = std::numeric_limits<Index>::max()>
class infinite_counter {
 public:
  infinite_counter()
  : count_{0}
  {}

  infinite_counter& operator++() {
    if (count_.back() == max)
      count_.push_back(0);
    else
      ++count_.back();
    return *this;
  }

  bool operator>(const infinite_counter& other) const {
    if (count_.size() == other.count_.size()) {
      return count_.back() > other.count_.back();
    } else {
      return count_.size() > other.count_.size();
    }
  }

 private:
  std::vector<Index> count_;
};


class priority_task {
 public:
  typedef std::uint64_t counter_elem_t;

  priority_task();

  priority_task(std::function<void()> callback, int priority,
                detail::infinite_counter<counter_elem_t> order);

  bool operator<(const priority_task& other) const;

  std::function<void()> callback() const;

 private:
  std::function<void()> callback_;
  int priority_;
  detail::infinite_counter<counter_elem_t> order_;
};


}  // namespace detail

/**
 * A portable, header-only thread pool for C++
 *
 * Constructing the thread pool launches the worker threads while
 * destructing it joins them. The threads will be alive for as long as the
 * thread pool is not destructed.
 *
 * Tasks can be pushed into the pool with and w/o providing a priority >= 0.
 * Not providing a priority is equivalent to providing a priority of 0.
 * Those tasks are processed first that have the highest priority.
 * If priorities are equal those tasks are processed first that were pushed
 * first into the pool (FIFO).
 */
class thread_pool {
 public:
  /**
   * Constructor. The number of threads to launch is determined by calling
   * std::thread::hardware_concurrency()
   * @throws cxxpool::thread_pool_error if hardware concurrency is unavailable
   */
  thread_pool();
  /**
   * Constructor
   * @param n_threads The number of threads to launch. Passing 0 is equivalent
   *  to calling the no-argument constructor
   * @throws cxxpool::thread_pool_error if n_threads < 0
   */
  explicit thread_pool(int n_threads);
  /**
   * Destructor. Joins all threads launched in the constructor. Waits for all
   * running tasks to complete
   */
  ~thread_pool();

  thread_pool(const thread_pool&) = delete;
  thread_pool& operator=(const thread_pool&) = delete;
  thread_pool(thread_pool&&) = delete;
  thread_pool& operator=(thread_pool&&) = delete;

  /**
   * Returns the number of threads launched in the constructor
   */
  int n_threads() const;
  /**
   * Pushes a new task into the thread pool. The task will have a priority of 0
   * @param functor The functor to call
   * @param args The arguments to pass to the functor when calling it
   * @return The future associated to the underlying task
   */
  template<typename Functor, typename... Args>
  auto push(Functor&& functor, Args&&... args)
    -> std::future<decltype(functor(args...))>;
  /**
   * Pushes a new task into the thread pool while providing a priority
   * @param priority A task priority. Higher priorities are processed first
   * @param functor The functor to call
   * @param args The arguments to pass to the functor when calling it
   * @return The future associated to the underlying task
   * @throws cxxpool::thread_pool_error if priority < 0
   */
  template<typename Functor, typename... Args>
  auto push(int priority, Functor&& functor, Args&&... args)
    -> std::future<decltype(functor(args...))>;
  /**
   * Waits until all tasks finished
   */
  void wait() const;
  /**
   * Waits until all tasks finished with a given timeout duration
   * @param timeout_duration The timeout duration
   * @return true if all tasks are finished, false otherwise
   */
  template<typename Rep, typename Period>
  bool wait_for(const std::chrono::duration<Rep, Period>& timeout_duration) const;
  /**
   * Waits until all tasks finished with a given timeout time
   * @param timeout_time The timeout time
   * @return true if all tasks are finished, false otherwise
   */
  template<typename Clock, typename Duration>
  bool wait_until(const std::chrono::time_point<Clock, Duration>& timeout_time) const;

 private:

  void init(int n_threads);

  int hardware_concurrency() const;

  void worker();

  bool done_;
  std::vector<std::thread> threads_;
  std::priority_queue<detail::priority_task> tasks_;
  detail::infinite_counter<typename detail::priority_task::counter_elem_t>
    task_counter_;
  std::atomic<std::uint64_t> task_balance_;
  std::condition_variable task_cond_var_;
  std::mutex task_mutex_;
  mutable std::condition_variable wait_cond_var_;
  mutable std::mutex wait_mutex_;
};


class thread_pool_error : public std::runtime_error {
 public:
  explicit thread_pool_error(const std::string& message)
  : std::runtime_error{message}
  {}
};


inline
thread_pool::thread_pool()
: done_{false}, threads_{}, tasks_{}, task_counter_{}, task_balance_{},
  task_cond_var_{}, task_mutex_{}, wait_cond_var_{}, wait_mutex_{}
{
  init(0);
}

inline
thread_pool::thread_pool(int n_threads)
: done_{false}, threads_{}, tasks_{}, task_counter_{}, task_balance_{},
  task_cond_var_{}, task_mutex_{}, wait_cond_var_{}, wait_mutex_{}
{
  init(n_threads);
}

inline
thread_pool::~thread_pool() {
  {
    std::lock_guard<std::mutex> lock{task_mutex_};
    done_ = true;
  }
  task_cond_var_.notify_all();
  for (auto& thread : threads_)
    thread.join();
}

inline
int thread_pool::n_threads() const {
  return threads_.size();
}

template<typename Functor, typename... Args>
inline
auto thread_pool::push(Functor&& functor, Args&&... args)
  -> std::future<decltype(functor(args...))> {
  return push(0, std::forward<Functor>(functor), std::forward<Args>(args)...);
}

template<typename Functor, typename... Args>
inline
auto thread_pool::push(int priority, Functor&& functor, Args&&... args)
  -> std::future<decltype(functor(args...))> {
  if (priority < 0)
    throw thread_pool_error{"priority smaller than zero: " +
                            std::to_string(priority)};
  typedef decltype(functor(args...)) result_type;
  auto pack_task = std::make_shared<std::packaged_task<result_type()>>(
    std::bind(std::forward<Functor>(functor), std::forward<Args>(args)...));
  auto future = pack_task->get_future();
  {
      std::lock_guard<std::mutex> lock{task_mutex_};
      if (done_)
        throw thread_pool_error{"push called while pool is shutting down"};
      ++task_counter_; ++task_balance_;
      tasks_.emplace([pack_task]{ (*pack_task)(); }, priority, task_counter_);
  }
  task_cond_var_.notify_one();
  return future;
}

inline
void thread_pool::wait() const {
  std::unique_lock<std::mutex> lock{wait_mutex_};
  wait_cond_var_.wait(lock, [this]{ return task_balance_ == 0; });
}

template<typename Rep, typename Period>
inline
bool thread_pool::wait_for(
    const std::chrono::duration<Rep, Period>& timeout_duration) const {
  std::unique_lock<std::mutex> lock{wait_mutex_};
  return wait_cond_var_.wait_for(lock, timeout_duration,
                                 [this]{ return task_balance_ == 0; });
}

template<typename Clock, typename Duration>
inline
bool thread_pool::wait_until(
    const std::chrono::time_point<Clock, Duration>& timeout_time) const {
  std::unique_lock<std::mutex> lock{wait_mutex_};
  return wait_cond_var_.wait_until(lock, timeout_time,
                                   [this]{ return task_balance_ == 0; });
}

inline
void thread_pool::init(int n_threads) {
  if (n_threads <= 0)
    n_threads = hardware_concurrency();
  threads_ = std::vector<std::thread>(n_threads);
  for (auto& thread : threads_)
    thread = std::thread{&thread_pool::worker, this};
}

inline
int thread_pool::hardware_concurrency() const {
  const auto n_threads = std::thread::hardware_concurrency();
  if (n_threads == 0)
    throw thread_pool_error{
    "got zero from std::thread::hardware_concurrency()"};
  return static_cast<int>(n_threads);
}

inline
void thread_pool::worker() {
  for (;;) {
    detail::priority_task task;
    {
      std::unique_lock<std::mutex> lock{task_mutex_};
      task_cond_var_.wait(lock, [this]{ return done_ || !tasks_.empty(); });
      if (done_ && tasks_.empty())
          break;
      task = tasks_.top();
      tasks_.pop();
    }
    task.callback()();
    --task_balance_;
    wait_cond_var_.notify_all();
  }
}


namespace detail {


inline
priority_task::priority_task()
: callback_{}, priority_{}, order_{}
{}

inline
priority_task::priority_task(std::function<void()> callback, int priority,
                             detail::infinite_counter<counter_elem_t> order)
: callback_{std::move(callback)}, priority_(priority),
  order_{std::move(order)}
{}

inline
bool priority_task::operator<(const priority_task& other) const {
  if (priority_ == other.priority_) {
    return order_ > other.order_;
  } else {
    return priority_ < other.priority_;
  }
}

inline
std::function<void()> priority_task::callback() const {
  return callback_;
}


}  // namespace detail


}  // namespace cxxpool
