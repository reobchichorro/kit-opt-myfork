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

void ILS::Construcao(Solution &solution, Data *data) {
  list<int> elements; //= list<int>(data->getDimension());
  for (int i=1; i<data->getDimension(); i++)
    elements.push_back(i+1);

  solution.sequence = vector<int>(data->getDimension()+1);
  solution.sequence[0] = 1;
  int lastSeqElement = 0;

  double total_dist = 0.0; //Solution::calcCost(path, data);
  
  cout << "Construcao\n";
  cout << solution.sequence[0] << " ";
  while (!elements.empty()) {
    auto it = std::min_element(elements.cbegin(), elements.cend(), [&](const int a, const int b) {
      return data->getDistance(solution.sequence[lastSeqElement], a) < data->getDistance(solution.sequence[lastSeqElement], b);
    });

    lastSeqElement++;
    solution.sequence[lastSeqElement] = *it;

    cout << (*it) << " ";
    elements.erase(it);
  }

  solution.sequence[data->getDimension()] = 1;
  cout << solution.sequence[data->getDimension()] << " ";
  cout << endl;
}

// LEGACY

// typedef struct MinorCost_t {
//   double cost;
//   int j;
//   int k;
//   bool isUpdated = false;
// } MinorCost;

// std::cout << path.size() << std::endl;
// printPath(path);

// return;

// MinorCost minorCost;
// while (!solution.sequence.empty()) {
//   for (int i = 0; i < path.size() - 1; i++) {
//     for (int j = 0; j < solution.sequence.size(); j++) {
//       ik = data->getDistance(path[i], solution.sequence[j]);
//       kj = data->getDistance(solution.sequence[j], path[i + 1]);
//       ij = data->getDistance(path[i], path[i + 1]);
//       cost = ik + kj - ij;

//       if (!minorCost.isUpdated) {
//         minorCost.isUpdated = true;
//         minorCost.cost = cost;
//         minorCost.j = i + 1;
//         minorCost.k = j;
//       } else if (minorCost.cost > cost) {
//         minorCost.cost = cost;
//         minorCost.j = i + 1;
//         minorCost.k = j;
//       }
//     }
//   }

//   minorCost.isUpdated = false;

//   tDist += minorCost.cost;
//   path.insert(path.begin() + minorCost.j, solution.sequence[minorCost.k]);
//   solution.sequence.erase(solution.sequence.begin() + minorCost.k);
// }

// path.pop_back();

// solution.sequence = path;