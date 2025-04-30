#include "./Data.h"
#include "./Solution.h"
#include "./ILS.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>

using namespace std;

typedef struct InsertionInfo_t {
  size_t i, j, k;
  double cost;
} InsertionInfo;

void ILS::Construcao(Solution &solution, Data *data) {
  vector<int> path;

  path.push_back(solution.sequence[0]);
  path.push_back(solution.sequence[0]);

  solution.sequence.erase(solution.sequence.begin());
  solution.sequence.pop_back();

  double total_dist = Solution::calcCost(path, data);
  double alpha;
  double cost;
  size_t selection;

  size_t ik;
  size_t kj;
  size_t ij;

  std::vector<InsertionInfo> vcost;

  while (!solution.sequence.empty()) {
    for (size_t i = 0; i < path.size() - 1; i++) {
      // i = i
      // j = i + 1
      // k = k
      for (size_t k = 0; k < solution.sequence.size(); k++) {
        ik = data->getDistance(path[i], solution.sequence[k]);
        kj = data->getDistance(solution.sequence[k], path[i + 1]);
        ij = data->getDistance(path[i], path[i + 1]);
        cost = ik + kj - ij;

        vcost.push_back(
            (InsertionInfo){.i = i, .j = i + 1, .k = k, .cost = cost});
      }
    }

    std::sort(vcost.begin(), vcost.end(),
              [](const InsertionInfo &a, const InsertionInfo &b) {
                return a.cost < b.cost;
              });

    // bias towards begin
    alpha = (double)rand() / RAND_MAX;
    selection = rand() % ((size_t)ceil(alpha * vcost.size()));

    total_dist += vcost[selection].cost;
    path.insert(path.begin() + vcost[selection].j,
                solution.sequence[vcost[selection].k]);
    solution.sequence.erase(solution.sequence.begin() + vcost[selection].k);

    vcost.clear();
  }

  if (total_dist < solution.cost) {
    solution.sequence = path;
    solution.cost = total_dist;
  } else {
    solution = Solution(data->getDimension(), data);
  }
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