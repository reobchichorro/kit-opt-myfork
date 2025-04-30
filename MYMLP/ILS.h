#pragma once
#include "./Data.h"
#include "./Solution.h"
#include "Subsequence.h"
#include "ILS.h"

namespace ILS {

Solution ILS(Data *data);

void Construcao(Solution &solution, Data *data);

void BuscaLocal(Solution &solution, Data *data);

bool bestImprovementSwap(Solution &solution, Data *data);

bool bestImprovement2Opt(Solution &solution, Data *data);

bool bestImprovementOrOpt(Solution &solution, Data *data, int n);

Solution Pertubacao(Solution solution, Data *data);

void UpdateAllSubseq(Data* data, Solution *s, vector<vector<Subsequence>>& subseq_matrix);

}; // namespace ILS
