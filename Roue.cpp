#include "Roue.h"

Roue::Roue(double module, int nbDents) {
    M = module;
    Z = nbDents;
    D = M * Z;
}

Roue::Roue(double diametre, double module) {
    D = diametre;
    M = module;
    Z = D / M;
}
Roue::~Roue(){
    // Destructeur vide
}

int Roue::getZ(){
    return Z;
}

double Roue::getD(){
    return D;
}

double Roue::getM(){
    return M;
}

void Roue::setZ(int nbDents){
    Z = nbDents;
}

void Roue::setD(double diametre){
    D = diametre;
}

void Roue::setM(double module){
    M = module;
}

bool Roue::verif(double m, double d, int nbDents){
    return d == m*nbDents;
}

