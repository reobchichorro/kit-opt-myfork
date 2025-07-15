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

  // for (size_t i = 0; i<solution.subseq_matrix.size(); i++) {
  //   for (size_t j = 0; j<solution.subseq_matrix[i].size(); j++) {
  //     cout << i << ";" << j << ";" << solution.subseq_matrix[i][j].first << ";" << solution.subseq_matrix[i][j].last << ";" << solution.subseq_matrix[i][j].C << "\n";
  //   }
  // }

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
  OrOptInfo bestOrOp = (OrOptInfo){.i = 0, .j = 0, .cost = solution.cost};

  for (int i = 0; i < solution.sequence.size() - 1; i++) {
    bool initIn = i==0 || i+n > solution.sequence.size()-1;
    for (int jj = 0; jj < solution.sequence.size() - 1 - n; jj++) {
      int j = (i+jj+n)%(solution.sequence.size() - 1);

      if (initIn) {
        if (i==0) {
          a = solution.sequence[0];
          b = solution.sequence[i + n - 1];
          
          c = solution.sequence[i + n];
          d = solution.sequence[j];
          e = solution.sequence[j + 1];
          f = solution.sequence[solution.sequence.size() - 2];

          eb = data->getDistance(b, e);
          cf = data->getDistance(f, c);
          ad = data->getDistance(d, a);

          Subsequence sigma0b = solution.subseq_matrix[0][i+n-1];
          Subsequence sigmaef = solution.subseq_matrix[j+1][solution.sequence.size() - 2];
          Subsequence sigmacd = solution.subseq_matrix[i+n][j];
          Subsequence sigmann = solution.subseq_matrix[solution.sequence.size() - 1][solution.sequence.size() - 1];
      
          Subsequence sigma;
          sigma.Concatenate(sigma0b, sigmaef, eb);
          sigma.Concatenate(sigma, sigmacd, cf);
          sigma.Concatenate(sigma, sigmann, ad);
          if (sigma.C < bestOrOp.cost) {
            bestOrOp.cost = sigma.C;
            bestOrOp.i = i;
            bestOrOp.j = j;
          }
        }
        else {
          int s_1 = solution.sequence.size() - 1;
          a = solution.sequence[i];
          b = solution.sequence[(i + n - 1)%s_1];
          
          c = solution.sequence[(i + n)%s_1];
          d = solution.sequence[j];
          e = solution.sequence[j + 1];
          f = solution.sequence[i - 1];

          eb = data->getDistance(b, e);
          cf = data->getDistance(f, c);
          ad = data->getDistance(d, a);

          Subsequence sigma0b = solution.subseq_matrix[0][(i+n-1)%s_1];
          Subsequence sigmaef = solution.subseq_matrix[j+1][i-1];
          Subsequence sigmacd = solution.subseq_matrix[(i+n)%s_1][j];
          Subsequence sigmaan = solution.subseq_matrix[i][solution.sequence.size() - 1];
      
          Subsequence sigma;
          sigma.Concatenate(sigma0b, sigmaef, eb);
          sigma.Concatenate(sigma, sigmacd, cf);
          sigma.Concatenate(sigma, sigmaan, ad);
          if (sigma.C < bestOrOp.cost) {
            bestOrOp.cost = sigma.C;
            bestOrOp.i = i;
            bestOrOp.j = j;
          }
        }
      }
      else if (i < j) {
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
      else {
        a = solution.sequence[i - 1];
        b = solution.sequence[i];
        c = solution.sequence[i + n - 1];
        d = solution.sequence[i + n];

        e = solution.sequence[j];
        f = solution.sequence[j + 1];

        eb = data->getDistance(e, b);
        cf = data->getDistance(c, f);
        ad = data->getDistance(a, d);

        Subsequence sigma0e = solution.subseq_matrix[0][j];
        Subsequence sigmabc = solution.subseq_matrix[i][i+n-1];
        Subsequence sigmafa = solution.subseq_matrix[j+1][i-1];
        Subsequence sigmadn = solution.subseq_matrix[i+n][solution.sequence.size() - 1];
    
        Subsequence sigma;
        sigma.Concatenate(sigma0e, sigmabc, eb);
        sigma.Concatenate(sigma, sigmafa, cf);
        sigma.Concatenate(sigma, sigmadn, ad);
        if (sigma.C < bestOrOp.cost) {
          bestOrOp.cost = sigma.C;
          bestOrOp.i = i;
          bestOrOp.j = j;
        }
      }
    }
  }

  if (bestOrOp.cost < solution.cost) {
    // if (bestOrOp.i > bestOrOp.j) {
    //   for (size_t i = 0; i < n; i++) {
    //     aux = solution.sequence[bestOrOp.i + i];
    //     solution.sequence.erase(solution.sequence.begin() + bestOrOp.i + i);
    //     solution.sequence.insert(solution.sequence.begin() + bestOrOp.j + i + 1, aux);
    //     solution.UpdateAllSubseq(data);
    //   }
    // } else {
    //   for (size_t i = 0; i < n; i++) {+
    //     aux = solution.sequence[bestOrOp.i];
    //     solution.sequence.erase(solution.sequence.begin() + bestOrOp.i);
    //     solution.sequence.insert(solution.sequence.begin() + bestOrOp.j, aux);
    //     solution.UpdateAllSubseq(data);
    //   }
    // }
    
    // if (j >= i && j < i + n)
    //   continue;
    
    // cerr << bestOrOp.i << "," << bestOrOp.j << " - " << n << "\n";
    solution.sequence.pop_back();
    // for (int k = 0; k < solution.sequence.size(); k++)
    //   cerr << solution.sequence[k] << " ";
    // cerr << "\n";

    std::rotate(solution.sequence.begin(), solution.sequence.begin() + bestOrOp.i, solution.sequence.end());
    // for (int k = 0; k < solution.sequence.size(); k++)
    //   cerr << solution.sequence[k] << " ";
    // cerr << "\n";

    if (bestOrOp.i < bestOrOp.j)
      std::rotate(solution.sequence.begin() + n, solution.sequence.begin() + bestOrOp.j-bestOrOp.i+1, solution.sequence.begin()+solution.sequence.size());
    else
      std::rotate(solution.sequence.begin() + n, solution.sequence.begin() + solution.sequence.size()+bestOrOp.j-bestOrOp.i+1, solution.sequence.end());
    // for (int k = 0; k < solution.sequence.size(); k++)
    //   cerr << solution.sequence[k] << " ";
    // cerr << "\n";
    
    auto it = std::find(solution.sequence.begin(), solution.sequence.end(), 1);
    std::rotate(solution.sequence.begin(), it, solution.sequence.end());
    // for (int k = 0; k < solution.sequence.size(); k++)
    //   cerr << solution.sequence[k] << " ";
    // cerr << "\n";
    solution.sequence.push_back(-1);

    solution.UpdateAllSubseq(data);
    solution.cost = bestOrOp.cost;
    
    // solution.updateCost(data);
    // if (bestOrOp.cost != solution.cost)
    //   cerr << bestOrOp.cost << " " << solution.cost << "\n";

    return true;
  }

  return false;
}