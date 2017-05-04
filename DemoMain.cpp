#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <future>
#include <thread>
#include <memory>

using std::string;
using std::vector;
using std::thread;
using std::unique_ptr;
using std::uniform_int_distribution;
using std::minstd_rand;
using namespace std::chrono;

int main()
{
  auto tm1 = steady_clock::now();
  vector<thread> workers;
  const int numThreads = std::thread::hardware_concurrency();
  for (int tid = 0; tid < numThreads; ++tid) {
    workers.emplace_back([tid]() {
      minstd_rand rnd(tid);
      uniform_int_distribution<int> sizeDist(1, 1500);
      for (int it = 0; it < 10; ++it)
      {
        vector<unique_ptr<char[]>> vec;
        for (int i = 0; i < 50000; ++i)
        {
          vec.emplace_back(std::make_unique<char[]>(sizeDist(rnd)));
          vec.back()[0] = 1;
        }
      }
    });
  }
  for (thread& t : workers) {
    t.join();
  }
  auto tm2 = steady_clock::now();
  std::cout << "Done in " << std::to_string((tm2 - tm1) / milliseconds(1)) << "ms\n";
  return 0;
}
