#include "./Data.h"
#include "./Solution.h"
#include "./ILS.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>
#include <list>

using namespace std;

typedef struct InsertionInfo_t {
  size_t i, j, k;
  double cost;
} InsertionInfo;

void ILS::Construcao(Solution &solution, Data *data, const double alpha) {
  list<int> elements; //= list<int>(data->getDimension());
  for (int i=1; i<data->getDimension(); i++)
    elements.push_back(i+1);

  solution.sequence = vector<int>(data->getDimension()+1);
  solution.sequence[0] = 1;
  int lastSeqElement = 0;
  
  // cout << "Construcao\n";
  // cout << solution.sequence[0] << " ";
  while (!elements.empty()) {
    // std::sort(elements.cbegin(), elements.cend(), [&](const int a, const int b) {
    //   return data->getDistance(solution.sequence[lastSeqElement], a) < data->getDistance(solution.sequence[lastSeqElement], b);
    // });

    elements.sort([&](const int a, const int b) {
      return data->getDistance(solution.sequence[lastSeqElement], a) < data->getDistance(solution.sequence[lastSeqElement], b);
    });

    double kk = alpha * elements.size();
    auto kkk = ceil(kk);
    int kkkk = std::max(kkk, 1.1);
    int k = rand() % kkkk;
    auto it = elements.begin();
    std::advance(it, k);

    lastSeqElement++;
    solution.sequence[lastSeqElement] = *it;

    // cout << (*it) << " ";
    elements.erase(it);
  }

  solution.sequence[data->getDimension()] = -1;
  solution.updateCost(data);
  // cout << solution.sequence[data->getDimension()] << " ";
  // cout << endl;
}
