#include "Subsequence.h"

Subsequence::Subsequence() {

}

void Subsequence::Concatenate(Subsequence &sigma_1, Subsequence &sigma_2, double temp) {
    W = sigma_1.W + sigma_2.W;
    T = sigma_1.T + temp + sigma_2.T;
    C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
    first = sigma_1.first;
    last = sigma_2.last;
}
