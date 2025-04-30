class Subsequence {
public:
    Subsequence();
    void Concatenate(Subsequence &sigma_1, Subsequence &sigma_2, double temp);
    
    double T, C;
    int W;
    int first, last;
};
