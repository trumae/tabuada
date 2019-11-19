#include <iostream>
#include <vector>






using Individuo = std::vector<int>;
void print(Individuo &ind) {
   std::cout << "< ";
   for(auto d : ind) 
      std::cout << d << " ";
   std::cout << ">";
}
struct Populacao {
  double txcross;
  double txmut;
  std::vector<Individuo> data;
};
