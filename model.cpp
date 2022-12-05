#include "model.hpp"

static int power(int N,int P){ return (P==0)? 1: N*power(N,P-1); }
static int mod(int a, int b){return a %b;}

void model::init()
{   
    
    for (unsigned int a = 0; a < power(L,D); a++){
            for (unsigned int j = 1; j<D+1;j++){

                if (mod(a,power(L,j))<power(L,j-1)) {
                S1.push_back(a);
                break;
                }
                else if (mod(a,power(L,j)) >= power(L,j-1)*(L-1)) {
                S1.push_back(a);
                break;
                }

            }
    }
    std::vector<int> N_vec(power(L,D));
    std::iota(N_vec.begin(), N_vec.end(), 0);
    std::set_difference(N_vec.begin(), N_vec.end(), S1.begin(), S1.end(), std::inserter(S_core, S_core.begin()));
    std::vector<std::vector<int>> neigh1(power(L,D-1));
    int u=0;
    for (unsigned int k = 0; k < power(L,D-1);k++){
        for (unsigned int p = 0; p<L;p++){
            neigh1[k].push_back(u);
            u++;
        }
    }
    nbr.resize(N);
    u = 0;
    for (unsigned int a = 0; a < power(L,D-1); a++){
        for (unsigned int b = 0; b<L;b++){
            for (unsigned int j = 0; j<D;j++){
                unsigned int ne2 = neigh1[a][b]+power(L,j); 
                unsigned int ne1 = neigh1[a][b]-power(L,j);                 
            if (j==0){
                if (b==0){ 
                nbr[u].push_back(ne2);

                }
                else if (b==L-1){
                nbr[u].push_back(ne1);

                }
                else {
                nbr[u].push_back(ne1);
                nbr[u].push_back(ne2);

                }
            }
            else {
                if (mod(a,power(L,j))<power(L,j-1)) {
                nbr[u].push_back(ne2);

                }
                else if (mod(a,power (L,j)) >= power(L,j-1)*(L-1)) {
                nbr[u].push_back(ne1);

                }
                else {
                nbr[u].push_back(ne1);
                nbr[u].push_back(ne2);

                }
            }
            }
        u++;
        }
    }
    
    }



