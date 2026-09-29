#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "benchmarkImplem.h"
using namespace std;


void sort1(vector<int>& vect) {
    for (int i=0;i<vect.size()-1;i++)
        for (int j=i+1;j<vect.size();j++)
            if (vect[i]>vect[j]) {
                swap(vect[i],vect[j]);
            }


}

void runBenchmark(long long &duration1k, long long &duration10k, long long &duration100k) {
    vector<int> v;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 2000000);
    auto start = std::chrono::high_resolution_clock::now();

    chrono::high_resolution_clock::time_point end1k;
    chrono::high_resolution_clock::time_point end10k;

    for (int repet =0;repet <2000;repet++) {
      //cout<<"iteratie "<<repet<<endl;

        std::uniform_int_distribution<> dist(1, 2000000);

        int randomNumber = dist(gen);
        v.push_back(randomNumber);
        vector <int> vect =v;
        sort1(vect);
        if (repet ==499) {
            end1k = std::chrono::high_resolution_clock::now();
        }
        if (repet==999) {
            end10k = std::chrono::high_resolution_clock::now();
        }

    }
    auto end100k = std::chrono::high_resolution_clock::now();
    duration1k =
        chrono::duration_cast<chrono::milliseconds>(
            end1k - start
        ).count();

   duration10k =
        chrono::duration_cast<chrono::milliseconds>(
            end10k - start
        ).count();

    duration100k =
        chrono::duration_cast<chrono::milliseconds>(
            end100k - start
        ).count();
    //cout<<duration1k<<endl;
    //cout<<duration10k<<endl;
    //cout<<duration100k<<endl;


}