#include "Data.h"
#include "ILS.h"
#include <bits/types/time_t.h>
#include <chrono>
#include <iostream>

int main(int argc, char **argv) {
  srand(0);

  auto data = Data(argc, argv[1]);
  data.read();
  // size_t n = data.getDimension();
  data.printMatrixDist();
  srand(time(NULL));

  std::cerr << data.getInstanceName() << std::endl;
  auto start = chrono::high_resolution_clock::now();
  ILS::ILS(&data);
  auto stop = chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << data.getInstanceName();
  printf(" - %lf\n", (double)(duration.count()));

  return 0;
}