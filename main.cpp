#include <cstdint>
#include <iostream>
#include <random>
#include <cmath>
#include <thread>
#include <cstdio>
#include <fstream>

using std::cout;

namespace simconfig{
    const int32_t POP_SIZE    = 500;
    const int32_t LOCI_COUNT  = 100;
    const int32_t GENERATION  = 5000;
    const double  DELTA       = 0.3;
    const int32_t INBRED_POOL = 10;
}

thread_local std::mt19937 rng(std::random_device{}());

static int32_t randi(const int32_t min, const int32_t max){
    return std::uniform_int_distribution<int32_t>(min, max)(rng);
}
static double randif(const double min, const double max){
    return std::uniform_real_distribution<double>(min, max)(rng);
}

struct Individual{
    char   genome[simconfig::LOCI_COUNT * 2];
    double mutation_rate;

    Individual(){ mutation_rate = 0.0; }

    Individual(int32_t loci, double mr, double rec_freq){
        mutation_rate = mr;
        for(int32_t i = 0; i < loci * 2; i++)
            genome[i] = (randif(0.0, 1.0) < rec_freq) ? 'a' : 'A';
    }

    void mutate(){
        for(int32_t i = 0; i < simconfig::LOCI_COUNT * 2; i++)
            if(randif(0.0, 1.0) < mutation_rate)
                genome[i] = (genome[i] == 'A') ? 'a' : 'A';
    }

    static Individual reproduce(const Individual &p1, const Individual &p2){
        Individual child;
        child.mutation_rate = p1.mutation_rate;
        for(int32_t i = 0; i < simconfig::LOCI_COUNT; i++){
            child.genome[i * 2]     = p1.genome[i * 2 + randi(0, 1)];
            child.genome[i * 2 + 1] = p2.genome[i * 2 + randi(0, 1)];
        }
        return child;
    }

    double fitness(double delta) const{
        int32_t bad = 0;
        for(int32_t i = 0; i < simconfig::LOCI_COUNT * 2; i += 2)
            if(genome[i] == 'a' && genome[i + 1] == 'a') ++bad;
        return pow(1.0 - delta, bad);
    }
};

namespace Matingstrat{
    enum Type{ OUTBRED, INBRED, MIXBRED };
}

struct Population{
    Individual       members[simconfig::POP_SIZE];
    Individual       ext_pool[simconfig::POP_SIZE];
    Individual       next[simconfig::POP_SIZE];
    int32_t          gen;
    Matingstrat::Type strat;

    void initpopu(double mr){
        gen = 0;
        for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
            members[i]  = Individual(simconfig::LOCI_COUNT, mr, 0.3);
            ext_pool[i] = Individual(simconfig::LOCI_COUNT, mr, 0.3);
        }
    }

    void mutate(){
        for(int32_t i = 0; i < simconfig::POP_SIZE; i++)
            members[i].mutate();
    }

    void nextgen(){
        switch(strat){
            case Matingstrat::INBRED:{
                for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
                    int32_t a = randi(0, simconfig::INBRED_POOL - 1);
                    int32_t b = randi(0, simconfig::INBRED_POOL - 2);
                    if(b >= a) ++b;
                    next[i] = Individual::reproduce(members[a], members[b]);
                }
                break;
            }
            case Matingstrat::OUTBRED:{
                for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
                    int32_t a = randi(0, simconfig::POP_SIZE - 1);
                    int32_t b = randi(0, simconfig::POP_SIZE - 1);
                    next[i] = Individual::reproduce(members[a], ext_pool[b]);
                }
                for(int32_t i = 0; i < simconfig::POP_SIZE; ++i)
                    ext_pool[i] = members[i];
                break;
            }
            case Matingstrat::MIXBRED:{
                if((gen / 50) % 2 == 0){
                    for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
                        int32_t a = randi(0, simconfig::POP_SIZE - 1);
                        int32_t b = randi(0, simconfig::POP_SIZE - 1);
                        next[i] = Individual::reproduce(members[a], ext_pool[b]);
                    }
                    for(int32_t i = 0; i < simconfig::POP_SIZE; ++i)
                        ext_pool[i] = members[i];
                } else {
                    for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
                        int32_t a = randi(0, simconfig::POP_SIZE - 1);
                        int32_t b = randi(0, simconfig::POP_SIZE - 2);
                        if(b >= a) ++b;
                        next[i] = Individual::reproduce(members[a], members[b]);
                    }
                }
                break;
            }
        }
        for(int32_t i = 0; i < simconfig::POP_SIZE; ++i) members[i] = next[i];
        ++gen;
    }

    double avgfitness(double delta) const{
        double sum = 0.0;
        for(int32_t i = 0; i < simconfig::POP_SIZE; ++i)
            sum += members[i].fitness(delta);
        return sum / simconfig::POP_SIZE;
    }

    int32_t fixedloci() const{
        int32_t fixed = 0;
        for(int32_t l = 0; l < simconfig::LOCI_COUNT; ++l){
            bool all_aa = true;
            for(int32_t i = 0; i < simconfig::POP_SIZE; ++i){
                if(!(members[i].genome[l*2] == 'a' && members[i].genome[l*2+1] == 'a'))
                    { all_aa = false; break; }
            }
            if(all_aa) ++fixed;
        }
        return fixed;
    }
};

void runpop(Population* pop, const char* label, char* outbuf, int32_t bufsize){
    int32_t pos = 0;
    for(int32_t g = 1; g <= simconfig::GENERATION; ++g){
        pop->mutate();
        pop->nextgen();
        if(g % 100 == 0){
            double  f  = pop->avgfitness(simconfig::DELTA);
            int32_t fl = pop->fixedloci();
            pos += snprintf(outbuf + pos, bufsize - pos,
                "%s,%d,%.6f,%d\n", label, g, f, fl);
        }
    }
}

int main(){
    Population inbred, outbred, mixed;

    inbred.strat  = Matingstrat::INBRED;
    outbred.strat = Matingstrat::OUTBRED;
    mixed.strat   = Matingstrat::MIXBRED;

    inbred.initpopu(0.005);
    outbred.initpopu(0.005);
    mixed.initpopu(0.005);

    static char buf1[8192], buf2[8192], buf3[8192];

    std::thread t1(runpop, &inbred,  "INBRED",  buf1, 8192);
    std::thread t2(runpop, &outbred, "OUTBRED", buf2, 8192);
    std::thread t3(runpop, &mixed,   "MIXED",   buf3, 8192);

    t1.join();
    t2.join();
    t3.join();

    std::ofstream csv("results.csv");
    csv << "label,gen,avgfitness,fixedloci\n";
    csv << buf1 << buf2 << buf3;
    csv.close();

    cout << "written to results.csv\n";
}