# AI generated README

A lightweight C++ implementation of a biological population simulator. It models diploid inheritance, random mutation, and fitness evaluation based on deleterious recessive alleles.

---

## 🛠 Features

* **Diploid Genome Modeling:** Individuals carry two alleles per locus.
* **Mendelian Inheritance:** Offspring inherit one random allele from each parent per locus.
* **Stochastic Mutation:** Configurable mutation rates for transitioning dominant alleles (`A`) to recessive (`a`).
* **Fitness Function:** Calculates survival probability based on the count of homozygous recessive (`aa`) loci using $P(\text{survival}) = (1 - \delta)^n$.

## 🧬 Logic Overview

The simulation utilizes a standard Wright-Fisher approach (under development):

1. **Mutation:** Alleles flip based on `mutation_rate`.
2. **Recombination:** `Individual::reproduce` simulates independent assortment.
3. **Selection:** `fitness(delta)` calculates the biological cost of deleterious mutations.

---

## 🚀 Getting Started

### Prerequisites

* C++11 compatible compiler (GCC).
* Standard Library (headers: `<random>`, `<cmath>`, `<string>`).

### Run

```bash
make
```

---

## Status: In Development

Current code demonstrates single-offspring generation and fitness scoring. The main population loop and generational turnover logic are currently being integrated.