// Trans.cpp

#include "Trans.h"
#include "Arbre.h"
#include "Roue.h"


Trans::Trans(double Ne, double Ns, int sens, int Nb, Arbre* arbres){
    this->Ne = Ne;
    this->Ns = Ns;
    this->sens = sens;
    this->Nb = Nb;
    this->arbres = arbres;
}

// permet de détruire une instance de la classe Trans ainsi que ses composants (c'est-à-dire tous les arbres et les roues !)
Trans::~Trans(){
    Arbre* courant = arbres;
    Arbre* suivant;
    while(courant != nullptr){
        suivant = courant->getSuivant();
        delete courant;
        courant = suivant;
    }
}


double Trans::getNe(){
    return Ne;
}

double Trans::getNs(){
    return Ns;
}

int Trans::getSens(){
    return sens;
}

int Trans::getNb(){
    return Nb;
}


void Trans::setNe(double Ne){
    this->Ne = Ne;
}

void Trans::setNs(double Ns){
    this->Ns = Ns;    
}

void Trans::setSens(int sens){
    this->sens = sens;
}

void Trans::setNb(int Nb){
    this->Nb = Nb;
}


// choix si on veut ajouter un arbre à la fin ou au début de la liste
// ici on ajoute à la fin
// ajout d'un maillon dans la liste chainée des arbres
void Trans::ajoutArbre(Arbre* nouveau){
    if(nouveau == nullptr)
        return;
        
    nouveau->setSuivant(nullptr); // S'assurer que le nouvel arbre termine la liste

    if(arbres == nullptr) {
        arbres = nouveau;
    } else {
        Arbre* courant = arbres;
        while(courant->getSuivant() != nullptr) {
            courant = courant->getSuivant();
        }
        courant->setSuivant(nouveau);
    }
}


void Trans::retraitArbre(int rang) {

    if(rang < 1) {
        return;
    }
    if(rang == 1) {
        Arbre* temp = arbres;
        arbres = arbres->getSuivant();
        delete temp;
        return;
    }

    Arbre* courant = arbres;
    Arbre* precedent = nullptr;
    int i = 1;
    while(courant != nullptr && i < rang) {
        precedent = courant;
        courant = courant->getSuivant();
        i++;
    }
    // Si la position demandée existe
    if(courant != nullptr) {
        precedent->setSuivant(courant->getSuivant());
        delete courant;
    }
}
