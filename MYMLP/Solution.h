#pragma once
#include "./Data.h"
#include "Subsequence.h"
#include <cstddef>
#include <vector>

class Solution {
public:
  std::vector<int> sequence;
  double cost;

	std::vector<std::vector<Subsequence>> subseq_matrix;

  Solution();
  Solution(size_t n);
  Solution(size_t n, Data *data);

  void updateCost(Data *data);
  double calcCost(const std::vector<int> &v, Data *data);
  void printSolution();
  void printSolution2();
  static void printSolution(std::vector<int> v);
  static void swap(std::vector<int> &v, int i, int j);

  void UpdateAllSubseq(Data *data);
};