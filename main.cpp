#include <cstdint>
#include <iostream>
#include <string>
#include <random>
#include <cmath>

using std::cout;
using std::string;

namespace simconfig{
    const int32_t POP_SIZE = 500;
    const int32_t LOCI_COUNT = 100;
    const int32_t GENERATION = 1000;
}

static std::mt19937 rng(std::random_device{}());

static int32_t randi(const int32_t min,const int32_t max){
    return std::uniform_int_distribution<int32_t>(min,max)(rng);
}

static double randif(const double min,const double max){
    return std::uniform_real_distribution<double>(min,max)(rng);
}

struct Individual{
    char genome[simconfig::LOCI_COUNT*2];
    double mutation_rate;

    Individual(){ mutation_rate = 0.0; }

    Individual(int32_t loci,double mr){
        mutation_rate = mr;
        for(int32_t i=0;i<loci;i++)
            genome[i]='A';
    }

    void mutate(){
        for(int32_t i=0;i<simconfig::LOCI_COUNT*2;i++){
            if(genome[i]=='A' && randif(0.0,1.0) < mutation_rate)
                genome[i]='a';
        }
    }

    static Individual reproduce(const Individual &p1,const Individual &p2){
        int32_t loci = simconfig::LOCI_COUNT;
        Individual child;
        child.mutation_rate = p1.mutation_rate;
        for(int32_t i=0;i<loci;i++){
            char a1 = p1.genome[i*2 + randi(0,1)];
            char a2 = p2.genome[i*2 + randi(0,1)];
            child.genome[i*2]=a1;
            child.genome[i*2+1]=a2;
        }

        return child;
    }

    double fitness(double delta){
        int32_t bad = 0;
        for(int32_t i=0;i<(int32_t)simconfig::LOCI_COUNT*2;i+=2){
            if(genome[i]=='a' && genome[i+1]=='a')
                bad++;
        }
        return pow(1-delta,bad);
    }
};
struct Population{
  void initpopu(double mr);
  void mutate();



};


int32_t main(){

    Individual p1(simconfig::LOCI_COUNT,0.05);
    Individual p2(simconfig::LOCI_COUNT,0.05);

    p1.mutate();
    p2.mutate();

    Individual child = Individual::reproduce(p1,p2);

    cout<<"Parent1: "<<p1.genome<<"\n";
    cout<<"Parent2: "<<p2.genome<<"\n";
    cout<<"Child:   "<<child.genome<<"\n";
    cout<<"Fitness: "<<child.fitness(0.02)<<"\n";

}