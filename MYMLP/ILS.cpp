#include "./ILS.h"
#include "./Data.h"

#define MAX_ITER 10
#define MAX_ITER_ILS 100

Solution ILS::ILS(Data *data) {
  Solution solution(data->getDimension(), data);
  Solution bestOfAll;
  bestOfAll.cost = solution.cost;
  Solution best;

  int max_iter_ils = std::min(MAX_ITER_ILS, data->getDimension());

  std::cout << "Custo inicial: " << solution.cost << std::endl;

  for (int i = 0; i < MAX_ITER; i++) {
    std::cerr << i << " ";
    ILS::Construcao(solution, data, 0.1);

    best = solution;

    for (int iterIls = 0; iterIls < max_iter_ils; iterIls++) {
      ILS::BuscaLocal(solution, data);
      if (solution.cost < best.cost) {
        best = solution;
        iterIls = 0;
      }

      solution = ILS::Pertubacao(best, data);
    }
    if (best.cost < bestOfAll.cost) {
      bestOfAll = best;
    }

    // std::cout << "Solução parcial: " << bestOfAll.cost << std::endl;
  }
  std::cerr << std::endl;

  bestOfAll.printSolution();

  std::cout << "Custo final = " << bestOfAll.cost << std::endl;

  return bestOfAll;
}
