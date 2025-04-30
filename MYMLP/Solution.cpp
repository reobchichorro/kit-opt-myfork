#include "Solution.h"
#include "./Data.h"
#include <cstddef>
#include <numeric>
#include <vector>

Solution::Solution() { // Initializes empty
  this->cost = 0;
  this->sequence = std::vector<int>();
}

Solution::Solution(size_t n) { // Initializes with nodes, but cost set to zero
  this->sequence = std::vector<int>(n);

  std::iota(this->sequence.begin(), this->sequence.end(), 1); // @PG
  this->sequence.push_back(this->sequence[0]);

  this->cost = 0;
}

Solution::Solution(size_t n,
                   Data *data) { // Initializes with nodes and sets initial cost
  this->sequence = std::vector<int>(n);

  std::iota(this->sequence.begin(), this->sequence.end(), 1); // @PG
  this->sequence.push_back(this->sequence[0]);

  this->updateCost(data);
}

void Solution::updateCost(Data *data) {

  if (this->sequence.size() < 2) {
    this->cost = 0;
    return;
  }

  this->cost = Solution::calcCost(this->sequence, data);
}

double Solution::calcCost(const std::vector<int> &path, Data *data) {

  if (path.size() < 2)
    return 0;

  double cost = 0;
  for (int i = 0; i < (path.size() - 1); i++)
    cost += data->getDistance(path[i], path[i + 1]);
  return cost;
}

void Solution::printSolution() {
  for (int i = 0; i < this->sequence.size(); i++)
    std::cout << this->sequence[i]
              << (i != this->sequence.size() - 1 ? " -> " : "\n");
}

void Solution::printSolution(std::vector<int> v) {
  for (int i = 0; i < v.size(); i++)
    std::cout << v[i] << (i != v.size() - 1 ? " -> " : "\n");
}

void Solution::swap(std::vector<int> &v, int i, int j) {
  int aux = v[i];
  v[i] = v[j];
  v[j] = aux;
}
