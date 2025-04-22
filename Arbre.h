// Arbre.h

#pragma once
#include "Roue.h"

class Arbre{
    private : 
    Roue* menante;
    Roue* menee;
    double N;
    int sens; // 1 si sens horaire, -1 si sens antihoraire
    Arbre* suivant;  // Pointeur vers l'arbre suivant
    public :
    Arbre(Roue * r1, Roue * r2, double,  int);
    ~Arbre(); // Déclaration du destructeur
    void setMenee(Roue * r1);
    void setMenante(Roue * r2);
    Roue* getMenee();
    Roue* getMenante();
    double getN();
    int getSens();
    void setN(double N);
    void setSens(int sens);
    Arbre* getSuivant() { return suivant; }
    void setSuivant(Arbre* suiv) { suivant = suiv; }
    void ajoutArbre(Arbre* a);
    void retraitArbre(Arbre* a);
};
