#include "./Solution.h"
#include "./ILS.h"
#include <algorithm>
#include <cstddef>
#include <vector>

typedef struct SwapInfo_t {
  size_t i, j;
  double cost;
} SwapInfo;

bool ILS::bestImprovementSwap(Solution &solution, Data *data) {
  double xb, bz;
  double ay, yc;

  double ac, cb, bd;

  size_t x, y, z, a, b, c, d;
  SwapInfo bestSwap = (SwapInfo){.i = 0, .j = 0, .cost = solution.cost};

  // SWAP de não vizinhos

  // x -> y -> z => x -> b -> z
  // a -> b -> c => a -> y -> c

  for (size_t i = 0; i<solution.subseq_matrix.size(); i++) {
    for (size_t j = 0; j<solution.subseq_matrix[i].size(); j++) {
      cout << i << ";" << j << ";" << solution.subseq_matrix[i][j].first << ";" << solution.subseq_matrix[i][j].last << ";" << solution.subseq_matrix[i][j].C << "\n";
    }
  }

  for (size_t i = 1; i < solution.sequence.size() - 1; i++) {

    x = solution.sequence[i - 1];
    y = solution.sequence[i];
    z = solution.sequence[i + 1];

    for (size_t j = i + 2; j < solution.sequence.size() - 1; j++) {
      a = solution.sequence[j - 1];
      b = solution.sequence[j];
      c = solution.sequence[j + 1];

      xb = data->getDistance(x, b);
      bz = data->getDistance(b, z);

      ay = data->getDistance(a, y);
      yc = data->getDistance(y, c);

      Subsequence sigma0x = solution.subseq_matrix[0][i-1];
      Subsequence sigmabb = solution.subseq_matrix[j][j];
      Subsequence sigmaza = solution.subseq_matrix[i+1][j-1];
      Subsequence sigmayy = solution.subseq_matrix[i][i];
      Subsequence sigmacn = solution.subseq_matrix[j+1][solution.sequence.size() - 1];

      Subsequence sigma;
      sigma.Concatenate(sigma0x, sigmabb, xb);
      sigma.Concatenate(sigma, sigmaza, bz);
      sigma.Concatenate(sigma, sigmayy, ay);
      sigma.Concatenate(sigma, sigmacn, yc);

      //std::cout << "i: " << i << " | j: " << j << " | cost: " << cost << std::endl;

      if (sigma.C < bestSwap.cost) {
        bestSwap.cost = sigma.C;
        bestSwap.i = i;
        bestSwap.j = j;
      };
    }
  }

  // SWAP de vizinhos

  // a -> b -> c -> d => a -> c -> b -> d

  for (size_t i = 1; i < solution.sequence.size() - 2; i++) {

    a = solution.sequence[i - 1];
    b = solution.sequence[i];
    c = solution.sequence[i + 1];
    d = solution.sequence[i + 2];

    ac = data->getDistance(a, c);
    cb = data->getDistance(c, b);
    bd = data->getDistance(b, d);

    Subsequence sigma0a = solution.subseq_matrix[0][i-1];
    Subsequence sigmacc = solution.subseq_matrix[i+1][i+1];
    Subsequence sigmabb = solution.subseq_matrix[i][i];
    Subsequence sigmadn = solution.subseq_matrix[i+2][solution.sequence.size() - 1];

    Subsequence sigma;
    sigma.Concatenate(sigma0a, sigmacc, ac);
    sigma.Concatenate(sigma, sigmabb, cb);
    sigma.Concatenate(sigma, sigmadn, bd);

    //std::cout << "i: " << i << " | j: " << i+1 << " | cost: " << cost << std::endl;

    if (sigma.C < bestSwap.cost) {
      bestSwap.cost = sigma.C;
      bestSwap.i = i;
      bestSwap.j = i + 1;
    };
  }

  if (bestSwap.cost < solution.cost) {
    Solution::swap(solution.sequence, bestSwap.i, bestSwap.j);
    solution.cost = bestSwap.cost;
    solution.UpdateAllSubseq(data);
    return true;
  }

  return false;
}

typedef struct Info2opt {
  size_t i, j;
  double cost;
} Info2opt_t;

bool ILS::bestImprovement2Opt(Solution &solution, Data *data) {

  size_t a, b, c, d;
  double ac, bd;

  Info2opt best2opt = (Info2opt){
      .i = 0,
      .j = 0,
      .cost = solution.cost,
  };

  for (size_t i = 0; i < solution.sequence.size(); i++) {
    for (size_t j = i + 2; j < solution.sequence.size() - 1; j++) {
      a = solution.sequence[i];
      b = solution.sequence[i + 1];
      c = solution.sequence[j];
      d = solution.sequence[j + 1];

      ac = data->getDistance(a, c);
      bd = data->getDistance(b, d);

      Subsequence sigma1;
      sigma1.Concatenate(solution.subseq_matrix[0][i], solution.subseq_matrix[j][i+1], ac);
      Subsequence sigma2;
      sigma2.Concatenate(sigma1, solution.subseq_matrix[j+1][solution.sequence.size() - 1], bd);

      if (sigma2.C < best2opt.cost) {
        best2opt.cost = sigma2.C;
        best2opt.i = i + 1;
        best2opt.j = j + 1;
      }
    }
  }

  if (best2opt.cost < solution.cost) {
    solution.cost = best2opt.cost;
    reverse(solution.sequence.begin() + best2opt.i,
            solution.sequence.begin() + best2opt.j);
    solution.UpdateAllSubseq(data);
    return true;
  }

  return false;
}

typedef struct OrOptInfo {
  size_t i, j;
  double cost;
} OrOptInfo_t;

bool ILS::bestImprovementOrOpt(Solution &solution, Data *data, size_t n) {
  size_t a, b, c, d, e, f, aux;
  double ad, eb, cf;
  OrOptInfo bestOrOp = (OrOptInfo){.i = 0, .j = 0, .cost = 0};

  if (solution.sequence.size() <= n) {
    return true;
  }

  for (size_t i = 1; i < solution.sequence.size() - n; i++) {
    for (size_t j = 1; j < solution.sequence.size() - 1; j++) {

      if (j >= i - 1 && j < i + n)
        continue;

      // a -> [b -> c] -> d => a -> d
      // e -> f => e -> [b -> c] -> d

      a = solution.sequence[i - 1];
      b = solution.sequence[i];
      c = solution.sequence[i + n - 1];
      d = solution.sequence[i + n];

      e = solution.sequence[j];
      f = solution.sequence[j + 1];

      ad = data->getDistance(a, d);
      eb = data->getDistance(e, b);
      cf = data->getDistance(c, f);

      Subsequence sigma0a = solution.subseq_matrix[0][i-1];
      Subsequence sigmade = solution.subseq_matrix[i+n][j];
      Subsequence sigmabc = solution.subseq_matrix[i][i+n-1];
      Subsequence sigmafn = solution.subseq_matrix[j+1][solution.sequence.size() - 1];
  
      Subsequence sigma;
      sigma.Concatenate(sigma0a, sigmade, ad);
      sigma.Concatenate(sigma, sigmabc, eb);
      sigma.Concatenate(sigma, sigmafn, cf);

      if (sigma.C < bestOrOp.cost) {
        bestOrOp.cost = sigma.C;
        bestOrOp.i = i;
        bestOrOp.j = j;
      }
    }
  }

  if (bestOrOp.cost < solution.cost) {
    if (bestOrOp.i > bestOrOp.j) {
      for (size_t i = 0; i < n; i++) {
        aux = solution.sequence[bestOrOp.i + i];
        solution.sequence.erase(solution.sequence.begin() + bestOrOp.i + i);
        solution.sequence.insert(solution.sequence.begin() + bestOrOp.j + i + 1, aux);
        solution.UpdateAllSubseq(data);
      }
    } else {
      for (size_t i = 0; i < n; i++) {
        aux = solution.sequence[bestOrOp.i];
        solution.sequence.erase(solution.sequence.begin() + bestOrOp.i);
        solution.sequence.insert(solution.sequence.begin() + bestOrOp.j, aux);
        solution.UpdateAllSubseq(data);
      }
    }

    solution.cost = bestOrOp.cost;

    return true;
  }

  return false;
}