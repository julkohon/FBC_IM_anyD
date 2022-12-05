
#ifndef model_hpp
#define model_hpp

#include <assert.h>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string_view>
#include <numeric>

class model
{
public:
    model(int _L, int _Ntrials, int _D)
    {
        this->L = _L;
        this->N = pow(_L, _D);
        D = _D;
        this->N_trials = _Ntrials;
        nbr.reserve(N);
        init();
    }
    void init();
    // void print_nbr(int i){std::cout<<std::get<0>(nbr[i])<<"    "<<std::get<1>(nbr[i])<<"
    // "<<std::get<2>(nbr[i])<<"    "<<std::get<3>(nbr[i])<<std::endl;}
    int get_L() { return L; }
    int get_N() { return N; }
    int get_Ntrials() { return N_trials; }
    int get_S_core_size() {return S_core.size(); }
    int get_S1_size() {return S1.size(); }
    inline int get_nbr(const int i, const int number) const { return nbr[i][number]; }
    inline int get_S_core(const int i) const { return S_core[i]; }
    inline int get_S1(const int i) const { return S1[i]; }
    size_t nbrCountForNode(int node)
    {
        return nbr[node].size();
    }
private:
    int L;
    int N;
    int D;
    int N_trials;
    std::vector<std::vector<int>> nbr;
    std::vector<int> S1;
    std::vector<int> S_core;
};

#endif
