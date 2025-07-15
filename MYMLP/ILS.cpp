#include "./ILS.h"
#include "./Data.h"

#define MAX_ITER 10
#define MAX_ITER_ILS 100

Solution ILS::ILS(Data *data) {
  Solution solution(data->getDimension(), data);
  Solution bestOfAll;
  bestOfAll.cost = solution.cost;
  Solution best;
  std::vector<int> R = std::vector<int>(26);
  for (int i=0; i<26; i++)
    R[i] = i;
  
  vector<double> averages = vector<double>(MAX_ITER);

  int max_iter_ils = std::min(MAX_ITER_ILS, data->getDimension());
    
  std::cout << "Custo inicial: " << solution.cost << std::endl;

  for (int i = 0; i < MAX_ITER; i++) {
    // std::cerr << i << " ";
    int alphaidx = rand()%26;
    ILS::Construcao(solution, data, R[alphaidx]);

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
    double oldCost = best.cost;
    best.updateCost(data);
    averages[i] = best.cost;

    // std::cout << "Solução parcial: " << bestOfAll.cost << std::endl;
  }
  // std::cerr << std::endl;
  double average = accumulate(averages.begin(), averages.end(), 0)/MAX_ITER;

  bestOfAll.printSolution();

  std::cout << "Custo final = " << bestOfAll.cost << std::endl;
  std::cout << "Average sol = " << average << std::endl;
  std::cerr << data->getInstanceName() << ";" << bestOfAll.cost << ";" << average << "\n";

  return bestOfAll;
}
