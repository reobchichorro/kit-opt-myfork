#include "./ILS.h"
#include "./Data.h"

#define MAX_ITER 5
#define MAX_ITER_ILS 1000

Solution ILS::ILS(Data *data) {
  Solution solution(data->getDimension(), data);
  Solution bestOfAll;
  bestOfAll.cost = solution.cost;
  Solution best;

  std::cout << "Custo inicial: " << solution.cost << std::endl;

  for (int i = 0; i < MAX_ITER; i++) {
    ILS::Construcao(solution, data);

    best = solution;

    for (int iterIls = 0; iterIls < MAX_ITER_ILS; iterIls++) {
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

  bestOfAll.printSolution();

  std::cout << "Custo final = " << bestOfAll.cost << std::endl;

  return bestOfAll;
}

void ILS::UpdateAllSubseq(Data *data, Solution *s, vector<vector<Subsequence>>& subseq_matrix) {
    int n = s->sequence.size();
    
    for (int i = 0; i < n; i++) {
        int v = s->sequence[i];
        subseq_matrix[i][i].W = (i > 0);
        subseq_matrix[i][i].C = 0;
        subseq_matrix[i][i].T = 0;
        subseq_matrix[i][i].first = s->sequence[i];
        subseq_matrix[i][i].last = s->sequence[i];
    }

    for (int i = 0; i < n; i++)
      for (int j = i + 1; j < n; j++)
        subseq_matrix[i][j].Concatenate(subseq_matrix[i][j-1], subseq_matrix[j][j],  data->getDistance(subseq_matrix[i][j-1].last+1, subseq_matrix[j][j].first+1));
    
    for (int i = n - 1; i >= 0; i--)
      for (int j = i - 1; j >= 0; j--)
        subseq_matrix[i][j].Concatenate(subseq_matrix[i][j+1], subseq_matrix[j][j],  data->getDistance(subseq_matrix[i][j-1].last+1, subseq_matrix[j][j].first+1));

}
