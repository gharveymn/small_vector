//=======================================================================
// Copyright (c) 2014 Baptiste Wicht
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#include <chrono>

#include "graphs.hpp"
#include "demangle.hpp"

// chrono typedefs

using std::chrono::milliseconds;
using std::chrono::microseconds;

using Clock = std::chrono::high_resolution_clock;

// Number of repetitions of each test

static const std::size_t REPEAT = 7;

// variadic policy runner

template <class Container>
inline void
run (Container&, std::size_t)
{
  //End of recursion
}

template <template <class> class Test,
  template <class> class ...Rest, class Container>
inline void
run (Container& container, std::size_t size)
{
  Test<Container>::run (container, size);
  run<Rest...> (container, size);
}

// benchmarking procedure

template <typename Container,
          typename DurationUnit,
  template <class> class CreatePolicy,
  template <class> class ...TestPolicy,
  typename Iter>
inline
void
bench (const std::string& type, const Iter first, const Iter last)
{
  // create an element to copy so the temporary creation
  // and initialization will not be accounted in a benchmark
  std::for_each (first, last, [&](std::size_t size) {
    std::size_t duration = 0;

    for (std::size_t i = 0 ; i < REPEAT ; ++i)
    {
      auto container = CreatePolicy<Container>::make (size);

      Clock::time_point t0 = Clock::now ();

      run<TestPolicy...> (container, size);

      Clock::time_point t1 = Clock::now ();
      duration += static_cast<std::size_t> (std::chrono::duration_cast<DurationUnit> (t1 - t0).count ());
    }

    graphs::new_result (type, std::to_string (size), duration / REPEAT);
  });

  CreatePolicy<Container>::clean ();
}

template <template <class> class Benchmark>
inline
void
bench_types ()
{
  //Recursion end
}

template <template <class> class Benchmark, typename T, typename ...Types>
inline
void
bench_types ()
{
  Benchmark<T>::run ();
  bench_types<Benchmark, Types...> ();
}

inline
bool
is_tag (int c)
{
  return std::isalnum (c) || c == '_';
}

inline
std::string
tag (std::string name)
{
  std::replace_if (begin (name), end (name), [] (char c) { return !is_tag (c); }, '_');
  std::string res;
  res.swap (name);
  return res;
}

template <typename T>
inline
void
new_graph (const std::string& testName, const std::string& unit)
{
  std::string title (testName + " - " + demangle (typeid (T).name ()));
  graphs::new_graph (tag (title), title, unit);
}
