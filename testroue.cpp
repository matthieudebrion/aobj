#include <iostream>
#include "Roue.h"

int main(){
  Roue r1(1.5,3);
  std::cout << "Création d'une roue avec m=1.5 et Z=3" << std::endl;
  std::cout << "Nombre de dents: " << r1.getZ() << std::endl; 
  std::cout << "de module : " << r1.getM() << std::endl;
  std::cout << "Et de diamètre : " << std::endl;
  std::cout << r1.getD() << std::endl;
  return 0;

}
