#include "./Data.h"
#include "./Solution.h"
#include "./ILS.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <vector>

struct slice {
  size_t index;
  size_t size;
};

Solution ILS::Pertubacao(Solution solution, Data *data) {

  solution.sequence.pop_back();

  struct slice sliceA;
  struct slice sliceB;

  if (solution.sequence.size() <= 30)
    sliceA.size = 2;
  else
    sliceA.size = ceil(rand() % (solution.sequence.size() / 10 - 2)) + 2;

  sliceA.index = rand() % (solution.sequence.size() - sliceA.size) + 1;

  std::rotate(solution.sequence.begin(),
              solution.sequence.begin() + sliceA.index,
              solution.sequence.end());

  if (solution.sequence.size() <= 30)
    sliceB.size = 2;
  else
    sliceB.size =
        ceil(rand() % ((solution.sequence.size() - sliceA.size) / 10 - 2)) + 2;

  sliceB.index =
      rand() % (solution.sequence.size() - sliceA.size - sliceB.size + 1) +
      sliceA.size;

  std::rotate(solution.sequence.begin(),
              solution.sequence.begin() + sliceB.index,
              solution.sequence.begin() + sliceB.index + sliceB.size);

  std::rotate(solution.sequence.begin() + sliceB.size,
              solution.sequence.begin() + sliceB.size + sliceA.size,
              solution.sequence.begin() + sliceB.index + sliceB.size);

  auto it = std::find(solution.sequence.begin(), solution.sequence.end(), 1);

  std::rotate(solution.sequence.begin(),
              it,
              solution.sequence.end());

  solution.sequence.push_back(-1);

  solution.cost = solution.calcCost(solution.sequence, data);

  return solution;
}

// LEGACY

// vector<int> solutionCpy(solution);

// int indexA = randInt(0, solutionCpy.size());
// int sliceSizeA =
//     magnitude * (solutionCpy.size() - randInt(indexA, solutionCpy.size())) /
//         10 +
//     1;

// int sliceA[sliceSizeA];

// for (int i = 0; i < sliceSizeA; i++) {
//   sliceA[i] = solutionCpy[indexA];
//   solutionCpy.erase(solutionCpy.begin() + indexA);
// }

// int indexB = randInt(0, solutionCpy.size());
// int sliceSizeB;

// if (indexB < indexA) {
//   sliceSizeB = magnitude * (indexA - randInt(indexB, indexA)) / 10 + 1;
// } else {
//   sliceSizeB =
//       magnitude * (solutionCpy.size() - randInt(indexB, solutionCpy.size()))
//       /
//           10 +
//       1;
// }

// int sliceB[sliceSizeB];

// for (int i = 0; i < sliceSizeB; i++) {
//   sliceB[i] = solutionCpy[indexB];
//   solutionCpy.erase(solutionCpy.begin() + indexB);
// }

// for (int i = 0; i < solution.size(); i++) {
//   solutionCpy.push_back(-1);
// }

// for (int i = 0; i < sliceSizeA; i++) {
//   solutionCpy.insert(solutionCpy.begin() + indexB, sliceA[i]);
// }

// for (int i = 0; i < sliceSizeB; i++) {
//   solutionCpy.insert(solutionCpy.begin() + indexA, sliceB[i]);
// }

// for (int i = solutionCpy.size(); i >= 0; i--) {
//   if (solutionCpy[i] == -1) {
//     solutionCpy.erase(solutionCpy.begin() + i);
//   }
// }

// BuscaLocal(solutionCpy, data);

// if (calcCost(solutionCpy, data) < calcCost(solution, data)) {
//   solution = solutionCpy;
//   return true;
// }

// return false;