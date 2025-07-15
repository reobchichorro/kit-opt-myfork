#include "Solution.h"
#include "./Data.h"
#include <cstddef>
#include <numeric>
#include <vector>

Solution::Solution() { // Initializes empty
  this->cost = 0;
  this->sequence = std::vector<int>();
  this->subseq_matrix = std::vector<std::vector<Subsequence>>();
}

Solution::Solution(size_t n) { // Initializes with nodes, but cost set to zero
  this->sequence = std::vector<int>(n);
  this->subseq_matrix = std::vector<std::vector<Subsequence>>(n+1, std::vector<Subsequence>(n+1));

  std::iota(this->sequence.begin(), this->sequence.end(), 1); // @PG
  this->sequence.push_back(-1); //this->sequence[0]);

  this->cost = 0;
}

Solution::Solution(size_t n,
                   Data *data) { // Initializes with nodes and sets initial cost
  this->sequence = std::vector<int>(n);
  this->subseq_matrix = std::vector<std::vector<Subsequence>>(n+1, std::vector<Subsequence>(n+1));
  // double T, C;
  // int W;
  // int first, last;
  std::iota(this->sequence.begin(), this->sequence.end(), 1); // @PG
  this->sequence.push_back(-1); //this->sequence[0]);

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
  double latency = 0.0;
  for (size_t i = 0; i < (path.size() - 1); i++) {
    latency += data->getDistance(path[i], path[i + 1]);
    cost += latency;
  }

  UpdateAllSubseq(data);

  return cost;
}

void Solution::printSolution() {
  for (size_t i = 0; i < this->sequence.size(); i++)
    std::cout << this->sequence[i]
              << (i != this->sequence.size() - 1 ? " -> " : "\n");
}

void Solution::printSolution(std::vector<int> v) {
  for (size_t i = 0; i < v.size(); i++)
    std::cout << v[i] << (i != v.size() - 1 ? " -> " : "\n");
}

void Solution::swap(std::vector<int> &v, int i, int j) {
  int aux = v[i];
  v[i] = v[j];
  v[j] = aux;
}

void Solution::UpdateAllSubseq(Data *data) {
  int n = data->getDimension()+1;
  // subsequencias de um unico no
  for (int i = 0; i < n; i++)
  {
    subseq_matrix[i][i].W = (i > 0);
    subseq_matrix[i][i].C = 0;
    subseq_matrix[i][i].T = 0;
    subseq_matrix[i][i].first = this->sequence[i];
    subseq_matrix[i][i].last = this->sequence[i];
  }

  
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
      subseq_matrix[i][j].Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j], data->getDistance(subseq_matrix[i][j-1].last, subseq_matrix[j][j].first));
  
  for (int i = n - 1; i >= 0; i--) {
    for (int j = i - 1; j > 0; j--)
      subseq_matrix[i][j].Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j], data->getDistance(subseq_matrix[i][j+1].last, subseq_matrix[j][j].first));
    if (i>0)
      subseq_matrix[i][0].Concatenate(subseq_matrix[i][1], subseq_matrix[n-1][n-1], data->getDistance(subseq_matrix[i][1].last, subseq_matrix[n-1][n-1].first));
  }

}
