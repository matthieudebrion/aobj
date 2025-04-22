// Arbre.cpp

#include "Arbre.h"
#include "Roue.h"

Arbre::Arbre(Roue* menante, Roue* menee, double N, int sens){
    this->menante = menante;
    this->menee = menee;
    this->N = N;
    this->sens = sens;
    this->suivant=nullptr;
}

double Arbre::getN(){
    return N;
}

int Arbre::getSens(){
    return sens;
}

Roue* Arbre::getMenante(){
    return menante;
}

Roue* Arbre::getMenee(){
    return menee;
}


void Arbre::setMenee(Roue* menee){
    this->menee = menee;
}

void Arbre::setMenante(Roue* menante){
    this->menante = menante;
}

void Arbre::setN(double N){
    this->N = N;
}

void Arbre::setSens(int sens){
    this->sens = sens;
}
