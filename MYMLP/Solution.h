#pragma once
#include "./Data.h"
#include <cstddef>
#include <vector>

class Solution {
public:
  std::vector<int> sequence;
  double cost;

  Solution();
  Solution(size_t n);
  Solution(size_t n, Data *data);

  void updateCost(Data *data);
  static double calcCost(const std::vector<int> &v, Data *data);
  void printSolution();
  static void printSolution(std::vector<int> v);
  static void swap(std::vector<int> &v, int i, int j);
};