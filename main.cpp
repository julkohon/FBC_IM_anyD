//
//  main.cpp
//  Transport
//
//  Created by Shaheen Acheche on 29/09/2017.
//  Copyright © 2017 Shaheen Acheche. All rights reserved.
//

#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <complex>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <tuple>
#include <vector>
#include <stdio.h>
#include <unordered_set>
// #include <boost/filesystem.hpp>
#include "config.hpp"
#include "model.hpp"
#include "tools.hpp"
//#include "timer.h"

using i_vector = std::vector<int>;


int main(int argc, const char *argv[])
{
    const parameters params = get_params();
    model mod(params.L, params.Ntrials, params.D);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, mod.get_N() - 1);
    std::uniform_real_distribution<> dis_real(0, 1);

    d_vector magnetization(mod.get_Ntrials());
    d_vector magnetization1(mod.get_S1_size());
    d_vector magnetization_core(mod.get_S_core_size());
    d_vector magnetisation_noabs(mod.get_Ntrials());
    d_vector energy(mod.get_Ntrials());
    d_vector tNum = linspace(params.Tmin, params.Tmax, params.nT, true);
    d_vector meanMag(tNum.size());
    d_vector meanEne(tNum.size());
    std::unordered_set<int> pocket;
    std::unordered_set<int> cluster;
    i_vector S(mod.get_N());
    d_vector Smean(mod.get_N());
    double beta;
    double p;
    int k, s, che;
    double cutOff;
    
    cutOff =  0.1 * params.Ntrials;
    std::cout<<cutOff<< " " << mod.get_Ntrials()<<" "<< params.Ntrials << std::endl;

    for (size_t i = 0; i < tNum.size(); i++)
    {
        for (int ii =0; ii < mod.get_N(); ii++)
	    {
		Smean[ii] = 0;
	    }
	che = 0;
        std::ofstream txt;
        txt.open("D" + std::to_string(params.D) + "_L" + std::to_string(mod.get_L()) + "_Ntrials"
                 + std::to_string(mod.get_Ntrials()) + "_t" + std::to_string(i) + ".dat");
        std::cout << "Temperature =" << tNum[i] << std::endl;
        beta = 1. / tNum[i];
        p = 1. - std::exp(-2 * beta);
        initLattice(S);
        for (size_t j = 0; j < mod.get_Ntrials(); j++)
        {
            size_t maxPocketSize = 0;
//            Timer timer;
//            timer.restart();

            energy[j] = 0;
            k = dis(gen);
            pocket.insert(k);
            cluster.insert(k);
            while (pocket.size() != 0)
            {
                s = *pocket.cbegin();
                for (int kk = 0; kk < mod.nbrCountForNode(s); kk++)
                {
                    const int nbr = mod.get_nbr(s, kk);
                    if (S[nbr] == S[s])
                    {
                        const std::unordered_set<int>::const_iterator nbrI = cluster.find(nbr);
                        if (nbrI == cluster.cend())
                        {
                            if (dis_real(gen) < p)
                            {
                                pocket.insert(nbr);
                                cluster.insert(nbr);
                            }
                        }
                    }
                }

                if (maxPocketSize < pocket.size())
                    maxPocketSize = pocket.size();

                pocket.erase(s);
            }

            for (std::unordered_set<int>::const_iterator nbrI = cluster.cbegin();
                 nbrI != cluster.cend(); ++nbrI)
            {
                S[*nbrI] = -S[*nbrI];
            }

            for (int a = 0; a < mod.get_N(); a++)
            {
                double nbrEnergy = 0;
                for (int b = 0; b < mod.nbrCountForNode(a); b++)
                {
                    nbrEnergy += S[a] * S[mod.get_nbr(a, b)];
                }
                energy[j] += (nbrEnergy / 2.0);
            }

            cluster.clear();
            for (int boundary = 0; boundary < mod.get_S1_size(); boundary++){
                // for Fixed boundary conditions uncomment next line:
                S[mod.get_S1(boundary)] = 1;
                magnetization1[j] += S[mod.get_S1(boundary)];
            }
            magnetization1[j] = std::abs(magnetization1[j]);
            for (int core = 0; core < mod.get_S_core_size(); core++){
                magnetization_core[j] += S[mod.get_S_core(core)];
            }
            magnetization_core[j] = std::abs(magnetization_core[j]);
            magnetisation_noabs[j]=std::accumulate(S.begin(), S.end(), 0.0);
            magnetization[j] = std::abs(std::accumulate(S.begin(), S.end(), 0.0));
            txt << params.D << "\t" << params.L << std::fixed << std::setprecision(6) << "\t" << tNum[i] << "\t"  << magnetization[j]
                << "\t" << energy[j] << "\t" << magnetization1[j] << "\t" << magnetization_core[j] <<std::endl;
            
            if (j >= cutOff)
            {
                for (int u=0; u<mod.get_N(); u++)
                    if (magnetisation_noabs[j]>=0)
                        {
                            Smean[u]+=S[u];
                        }
                        else
                        {
                            Smean[u]+= - S[u];
                        }
                che++;
            }
            
        }
        std::cout<<che<<std::endl;

        txt.close();
        std::ofstream smtxt;
        smtxt.open("L" + std::to_string(mod.get_L()) + "_Smean_t" + std::to_string(i) +".dat");
        for (int v =0; v < mod.get_N(); v++)
        {
            smtxt << Smean[v] << "\t" << cutOff << std::endl;        
        }
        smtxt.close() ;    
    }

 
    std::cout << "Done !" << std::endl;

    return 0;
}
