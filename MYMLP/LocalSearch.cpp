#include "../parser/Data.h"
#include "./Solution.h"
#include "./ILS.h"
#include <vector>

void ILS::BuscaLocal(Solution &solution, Data *data) {

  vector<int> options = {1, 2, 3, 4, 5};
  bool improved = false;

  while (!options.empty()) {
    int n = rand() % options.size();
    switch (options[n]) {
    case 1:
      improved = ILS::bestImprovementSwap(solution, data);
      break;
    case 2:
      improved = ILS::bestImprovement2Opt(solution, data);
      break;
    case 3:
      improved = ILS::bestImprovementOrOpt(solution, data, 1);
      break;
    case 4:
      improved = ILS::bestImprovementOrOpt(solution, data, 2);
      break;
    case 5:
      improved = ILS::bestImprovementOrOpt(solution, data, 3);
      break;
    }

    if (improved) {
      options = {1, 2, 3, 4, 5};
      improved = false;
    } else {
      options.erase(options.begin() + n);
    }
  }
}