#include "./Solution.h"
#include "./ILS.h"
#include <algorithm>
#include <cstddef>
#include <vector>

typedef struct SwapInfo_t {
  size_t i, j;
  double delta;
} SwapInfo;

bool ILS::bestImprovementSwap(Solution &solution, Data *data) {

  double delta;
  double xy, yz;
  double ab, bc;

  double xb, bz;
  double ay, yc;

  double cd;
  double ac, cb, bd;

  size_t x, y, z, a, b, c, d;
  SwapInfo bestSwap = (SwapInfo){.i = 0, .j = 0, .delta = 0};

  // SWAP de não vizinhos

  // x -> y -> z => x -> b -> z
  // a -> b -> c => a -> y -> c

  for (size_t i = 1; i < solution.sequence.size() - 1; i++) {

    x = solution.sequence[i - 1];
    y = solution.sequence[i];
    z = solution.sequence[i + 1];

    xy = data->getDistance(x, y);
    yz = data->getDistance(y, z);

    for (size_t j = i + 2; j < solution.sequence.size() - 1; j++) {
      a = solution.sequence[j - 1];
      b = solution.sequence[j];
      c = solution.sequence[j + 1];

      ab = data->getDistance(a, b);
      bc = data->getDistance(b, c);

      xb = data->getDistance(x, b);
      bz = data->getDistance(b, z);

      ay = data->getDistance(a, y);
      yc = data->getDistance(y, c);

      delta = xb + bz + ay + yc - (xy + yz + ab + bc);

      //std::cout << "i: " << i << " | j: " << j << " | delta: " << delta << std::endl;

      if (delta < bestSwap.delta) {
        bestSwap.delta = delta;
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

    ab = data->getDistance(a, b);
    bc = data->getDistance(b, c);
    cd = data->getDistance(c, d);

    ac = data->getDistance(a, c);
    cb = data->getDistance(c, b);
    bd = data->getDistance(b, d);

    delta = ac + cb + bd - (ab + bc + cd);

    //std::cout << "i: " << i << " | j: " << i+1 << " | delta: " << delta << std::endl;

    if (delta < bestSwap.delta) {
      bestSwap.delta = delta;
      bestSwap.i = i;
      bestSwap.j = i + 1;
    };
  }

  if (bestSwap.delta < 0) {
    Solution::swap(solution.sequence, bestSwap.i, bestSwap.j);
    solution.cost += bestSwap.delta;
    return true;
  }

  return false;
}

typedef struct Info2opt {
  size_t i, j;
  double delta;
} Info2opt_t;

bool ILS::bestImprovement2Opt(Solution &solution, Data *data) {

  size_t a, b, c, d;
  double ab, cd, ac, bd;

  double delta;
  Info2opt best2opt = (Info2opt){
      .i = 0,
      .j = 0,
      .delta = 0,
  };

  for (size_t i = 0; i < solution.sequence.size(); i++) {
    for (size_t j = i + 2; j < solution.sequence.size() - 1; j++) {
      a = solution.sequence[i];
      b = solution.sequence[i + 1];
      c = solution.sequence[j];
      d = solution.sequence[j + 1];

      ab = data->getDistance(a, b);
      cd = data->getDistance(c, d);

      ac = data->getDistance(a, c);
      bd = data->getDistance(b, d);

      delta = ac + bd - (ab + cd);

      if (delta < best2opt.delta) {
        best2opt.delta = delta;
        best2opt.i = i + 1;
        best2opt.j = j + 1;
      }
    }
  }

  if (best2opt.delta < 0) {

    solution.cost += best2opt.delta;

    reverse(solution.sequence.begin() + best2opt.i,
            solution.sequence.begin() + best2opt.j);
    return true;
  }

  return false;
}

typedef struct OrOptInfo {
  size_t i, j;
  double delta;
} OrOptInfo_t;

bool ILS::bestImprovementOrOpt(Solution &solution, Data *data, int n) {
  size_t a, b, c, d, e, f, aux;
  double ab, cd, ad, eb, cf, ef;
  double delta;
  OrOptInfo bestOrOp = (OrOptInfo){.i = 0, .j = 0, .delta = 0};

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

      ab = data->getDistance(a, b);
      cd = data->getDistance(c, d);
      ad = data->getDistance(a, d);

      eb = data->getDistance(e, b);
      cf = data->getDistance(c, f);
      ef = data->getDistance(e, f);

      delta = ad + eb + cf - (ab + cd + ef);

      if (delta < bestOrOp.delta) {
        bestOrOp.delta = delta;
        bestOrOp.i = i;
        bestOrOp.j = j;
      }
    }
  }

  if (bestOrOp.delta < 0) {
    if (bestOrOp.i > bestOrOp.j) {
      for (int i = 0; i < n; i++) {
        aux = solution.sequence[bestOrOp.i + i];
        solution.sequence.erase(solution.sequence.begin() + bestOrOp.i + i);
        solution.sequence.insert(solution.sequence.begin() + bestOrOp.j + i + 1,
                                 aux);
      }
    } else {
      for (int i = 0; i < n; i++) {
        aux = solution.sequence[bestOrOp.i];
        solution.sequence.erase(solution.sequence.begin() + bestOrOp.i);
        solution.sequence.insert(solution.sequence.begin() + bestOrOp.j, aux);
      }
    }

    solution.cost += bestOrOp.delta;

    return true;
  }

  return false;
}