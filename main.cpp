#include <iostream>
#include "Roue.h"
#include "Arbre.h"
#include "Trans.h"

// Instanciation d'une transmission de puissance

int main() {
    // Création de roues pour les arbres avec new (allocation dynamique)
    Roue* r1 = new Roue(1.5, 20);
    Roue* r2 = new Roue(2.5, 1.5);
    Roue* r3 = new Roue(3.0, 30);
    
    // Option 1: Création des arbres directement avec les roues
    // (pas besoin de créer les pointeurs r1, r2, r3 séparément)
    Arbre* a = new Arbre(new Roue(1.5, 20), new Roue(2.5, 1.5), 1.5, 1);
    Arbre* a1 = new Arbre(new Roue(2.5, 1.5), new Roue(1.5, 20), 1.5, -1);
    Arbre* a2 = new Arbre(new Roue(1.5, 20), new Roue(2.5, 1.5), 2.0, 1);
    
    // Chaînage des arbres via le pointeur "suivant"
    a->setSuivant(a1);
    a1->setSuivant(a2);
    
    // Création de la transmission
    Trans t(10.0, 20.0, 1, 3, a);
    
    // Affichage des informations de la transmission
    std::cout << "Transmission:" << std::endl;
    std::cout << "Ne: " << t.getNe() << std::endl;
    std::cout << "Ns: " << t.getNs() << std::endl;
    std::cout << "Sens: " << t.getSens() << std::endl;
    std::cout << "Nb: " << t.getNb() << std::endl;
    
    // Parcours et affichage de la liste d'arbres
    Arbre* courant = t.getArbres();
    int i = 1;
    while(courant != nullptr) {
        std::cout << "Arbre " << i << " : ";
        std::cout << courant->getMenante()->getD() << " " 
                  << courant->getMenee()->getD() << " " 
                  << courant->getN() << " " 
                  << courant->getSens() << std::endl;
        courant = courant->getSuivant();
        i++;
    }
    
    // Test méthode ajout Arbre
    Arbre* a3 = new Arbre(new Roue(2.5, 1.5), new Roue(1.5, 20), 2.0, -1);
    t.ajoutArbre(a3);
    std::cout << "Après ajout d'un arbre:" << std::endl;
    courant = t.getArbres();
    i = 1;
    while(courant != nullptr) {
        std::cout << "Arbre " << i << " : ";
        std::cout << courant->getMenante()->getD() << " " 
                  << courant->getMenee()->getD() << " " 
                  << courant->getN() << " " 
                  << courant->getSens() << std::endl;
        courant = courant->getSuivant();
        i++;
    }



    return 0;
}
