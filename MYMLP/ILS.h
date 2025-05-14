#pragma once
#include "./Data.h"
#include "./Solution.h"
#include "ILS.h"

namespace ILS {

Solution ILS(Data *data);

void Construcao(Solution &solution, Data *data, const double alpha);

void BuscaLocal(Solution &solution, Data *data);

bool bestImprovementSwap(Solution &solution, Data *data);

bool bestImprovement2Opt(Solution &solution, Data *data);

bool bestImprovementOrOpt(Solution &solution, Data *data, size_t n);

Solution Pertubacao(Solution solution, Data *data);
};
