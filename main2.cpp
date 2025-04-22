#include <iostream>
#include <iomanip>
#include <cmath>
#include "Roue.h"
#include "Arbre.h"
#include "Trans.h"
#include "AlgoGenetique.h"

// Fonction pour afficher les détails d'un train d'engrenages
void afficherDetailsTrain(Trans* train) {
    std::cout << "=== Détails du train d'engrenages ===" << std::endl;
    std::cout << "Vitesse d'entrée: " << train->getNe() << " tr/min" << std::endl;
    std::cout << "Vitesse de sortie: " << train->getNs() << " tr/min" << std::endl;
    std::cout << "Sens de rotation global: " << (train->getSens() > 0 ? "Horaire" : "Anti-horaire") << std::endl;
    std::cout << "Nombre d'arbres: " << train->getNb() << std::endl;
    
    Arbre* arbreCourant = train->getArbres();
    int index = 1;
    
    double masseTotale = 0.0;
    double rho = 7850e-9; // Densité de l'acier (kg/mm³)
    double epaisseur = 20.0; // Épaisseur des roues (mm)
    
    std::cout << std::fixed << std::setprecision(2);
    
    while (arbreCourant != nullptr) {
        Roue* menante = arbreCourant->getMenante();
        Roue* menee = arbreCourant->getMenee();
        
        // Calcul des masses
        double rayonMenante = menante->getD() / 2.0;
        double rayonMenee = menee->getD() / 2.0;
        double volumeMenante = M_PI * rayonMenante * rayonMenante * epaisseur;
        double volumeMenee = M_PI * rayonMenee * rayonMenee * epaisseur;
        double masseMenante = volumeMenante * rho;
        double masseMenee = volumeMenee * rho;
        
        masseTotale += masseMenante + masseMenee;
        
        // Affichage des détails de l'arbre
        std::cout << "\n--- Arbre " << index << " ---" << std::endl;
        std::cout << "Vitesse: " << arbreCourant->getN() << " tr/min" << std::endl;
        std::cout << "Sens: " << (arbreCourant->getSens() > 0 ? "Horaire" : "Anti-horaire") << std::endl;
        
        std::cout << "Roue menante:" << std::endl;
        std::cout << "  Module: " << menante->getM() << " mm" << std::endl;
        std::cout << "  Nombre de dents: " << menante->getZ() << std::endl;
        std::cout << "  Diamètre: " << menante->getD() << " mm" << std::endl;
        std::cout << "  Masse: " << masseMenante * 1000 << " g" << std::endl;
        
        std::cout << "Roue menée:" << std::endl;
        std::cout << "  Module: " << menee->getM() << " mm" << std::endl;
        std::cout << "  Nombre de dents: " << menee->getZ() << std::endl;
        std::cout << "  Diamètre: " << menee->getD() << " mm" << std::endl;
        std::cout << "  Masse: " << masseMenee * 1000 << " g" << std::endl;
        
        // Rapport de transmission local
        double rapportLocal = static_cast<double>(menee->getZ()) / menante->getZ();
        std::cout << "Rapport de transmission local: " << rapportLocal << std::endl;
        
        arbreCourant = arbreCourant->getSuivant();
        index++;
    }
    
    std::cout << "\nMasse totale du train: " << masseTotale * 1000 << " g" << std::endl;
    
    // Calculer le rapport de transmission global
    double rapportGlobal = 1.0;
    arbreCourant = train->getArbres();
    
    while (arbreCourant != nullptr) {
        Roue* menante = arbreCourant->getMenante();
        Roue* menee = arbreCourant->getMenee();
        double rapportLocal = static_cast<double>(menee->getZ()) / menante->getZ();
        rapportGlobal *= rapportLocal;
        arbreCourant = arbreCourant->getSuivant();
    }
    
    std::cout << "Rapport de transmission global: " << rapportGlobal << std::endl;
    std::cout << "Rapport théorique: " << train->getNe() / train->getNs() << std::endl;
}

int main() {
    // Paramètres du problème
    double vitesseEntree = 3000.0;  // tr/min
    double vitesseSortie = 100.0;   // tr/min
    int sensRotation = 1;           // Sens horaire
    
    std::cout << "=========================================" << std::endl;
    std::cout << "Optimisation d'un train d'engrenages" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Paramètres du problème:" << std::endl;
    std::cout << "- Vitesse d'entrée: " << vitesseEntree << " tr/min" << std::endl;
    std::cout << "- Vitesse de sortie: " << vitesseSortie << " tr/min" << std::endl;
    std::cout << "- Rapport de vitesse: " << vitesseEntree / vitesseSortie << std::endl;
    std::cout << "- Sens de rotation: " << (sensRotation > 0 ? "Horaire" : "Anti-horaire") << std::endl;
    std::cout << "=========================================" << std::endl;
    
    // Créer et exécuter l'algorithme génétique
    AlgoGenetique algoGen(vitesseEntree, vitesseSortie, sensRotation);
    Solution meilleureSolution = algoGen.optimiser();
    
    // Générer la transmission correspondant à la meilleure solution
    std::unique_ptr<Trans> trainOptimal = algoGen.genererTransmission();
    
    // Afficher les détails de la solution optimale
    afficherDetailsTrain(trainOptimal.get());
    
    return 0;
}