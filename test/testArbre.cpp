#include <iostream>
#include "Arbre.h"
#include "Roue.h"
int main(){
  // Création de roues pour les arbres
  Roue r1(1.5, 20);
  Roue r2(2.5, 1.5);
  // Création d'un arbre
  Arbre a(&r1, &r2, 1.5, 1);
  std::cout << "Création d'un arbre" << std::endl;
  std::cout << "Sens de l'arbre (1)" << a.getSens() << std::endl;
  std::cout << "Vitesse de rotation (1.5)" << a.getN() << std::endl;

  std::cout << "Nombre de dent Roue menante de l'arbre " << a.getMenante()->getZ() << std::endl;
  std::cout << "Nombre de dent Roue menee de l'arbre " << a.getMenee()->getZ() <<std::endl;
  return 0;


}
