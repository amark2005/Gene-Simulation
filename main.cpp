#include <cstddef>
#include <cstdint>
#include<iostream>
#include<string>
#include<random>
#include<cmath>
using std::cout;
using std::string;
constexpr int32_t POP_SIZE=500;
constexpr int32_t LOCI_COUNT=900;
constexpr int32_t GENERATION=1000;
static int32_t randi(const int32_t min, const int32_t max) {
  static std::mt19937 rng(std::random_device{}());
  return std::uniform_int_distribution<int32_t>(min, max)(rng);
}
static double randif(const double min, const double max) {
  static std::mt19937 rng(std::random_device{}());
  return std::uniform_real_distribution<double>(min, max)(rng);
}
struct Individual{
  string genome;
  //string locus;
  //string allele;
  double mutation_rate;

  Individual(int loci,double mr){
    mutation_rate=mr;
    for(int i=0;i<loci;i++){
      genome+="AA"; // init of gene
    }
  }

  void mutate(){
    for(char &g:genome){
      if(g=='A'&&randif(0.0,1.0)<mutation_rate) g='a';
    }
  }
  static Individual reproduce(const Individual &p1,const Individual &p2){        //Using Mendelian Inheritance
    int loci=p1.genome.size()/2;
    Individual child(loci,p1.mutation_rate);
    child.genome.clear();
    for(int i=0;i<loci;i++){
      char a1=p1.genome[i*2+randi(0,1)];
      char a2=p2.genome[i*2+randi(0,1)];

      child.genome.push_back(a1);
      child.genome.push_back(a2);

    }
    return child;
  }

  double fitness(double delta){
    int bad=0;
    for(size_t i=0;i<genome.size();i+=2){
      if(genome[i]=='a'&& genome[i+1]=='a') bad++;
    }
    return pow(1-delta,bad);
  }
};


int main(){
  
  Individual p1(900,0.05);
  Individual p2(900,0.05);

  p1.mutate();
  p2.mutate();

  Individual child = Individual::reproduce(p1,p2);

  cout<<"Parent1: "<<p1.genome<<"\n";
  cout<<"Parent2: "<<p2.genome<<"\n";
  cout<<"Child:   "<<child.genome<<"\n";

  cout<<"Fitness: "<<child.fitness(0.02)<<"\n";

}