// AlgoGenetique.h
#ifndef ALGO_GENETIQUE_H
#define ALGO_GENETIQUE_H

#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>
#include <memory>
#include "Roue.h"
#include "Arbre.h"
#include "Trans.h"

// Structure pour stocker une solution (individu)
struct Solution {
    std::vector<double> modules;   // Module de chaque roue
    std::vector<int> dents;        // Nombre de dents de chaque roue
    int nbArbres;                  // Nombre d'arbres dans la solution
    double fitness;                // Score de la solution (masse)
    bool valide;                   // Indique si la solution respecte les contraintes
};

class AlgoGenetique {
private:
    // Paramètres de l'algorithme génétique
    int taillePopulation;
    int nbGenerations;
    double tauxMutation;
    double tauxCroisement;
    
    // Paramètres du problème
    double vitesseEntree;
    double vitesseSortie;
    int sensRotation;
    double epaisseur;              // Épaisseur des roues en mm
    double densite;                // Densité de l'acier en kg/mm³
    
    // Contraintes du problème
    std::vector<double> modulesDisponibles;
    int minDents;
    int maxDents;
    double maxDiametre;
    
    // Population courante
    std::vector<Solution> population;
    
    // Générateur de nombres aléatoires
    std::mt19937 rng;
    
    // Meilleure solution trouvée
    Solution meilleureGlobale;

public:
    // Constructeur
    AlgoGenetique(double vitEntree, double vitSortie, int sens) :
        taillePopulation(100),
        nbGenerations(500),
        tauxMutation(0.2),
        tauxCroisement(0.8),
        vitesseEntree(vitEntree),
        vitesseSortie(vitSortie),
        sensRotation(sens),
        epaisseur(20.0),
        densite(7850e-9),
        minDents(18),
        maxDents(150),
        maxDiametre(15000.0) {
        
        // Initialiser les modules disponibles
        modulesDisponibles = {0.13, 0.5, 0.6, 0.8, 1, 1.25, 1.5, 2, 2.5, 3, 4, 5, 6, 8};
        
        // Initialiser le générateur de nombres aléatoires
        std::random_device rd;
        rng = std::mt19937(rd());
        
        // Initialiser la meilleure solution avec une valeur infinie
        meilleureGlobale.fitness = std::numeric_limits<double>::max();
        meilleureGlobale.valide = false;
    }
    
    // Exécute l'algorithme génétique
    Solution optimiser() {
        // Créer la population initiale
        initialiserPopulation();
        
        // Évaluer la population initiale
        evaluerPopulation();
        
        // Trouver la meilleure solution de la population initiale
        trouverMeilleureSolution();
        
        std::cout << "Démarrage de l'algorithme génétique..." << std::endl;
        std::cout << "Population initiale de " << taillePopulation << " individus" << std::endl;
        
        // Boucle principale de l'algorithme génétique
        for (int generation = 0; generation < nbGenerations; generation++) {
            // Sélectionner les parents pour la reproduction
            std::vector<int> parents = selectionnerParents();
            
            // Créer une nouvelle population par croisement et mutation
            std::vector<Solution> nouvellePopulation;
            
            // Élitisme: conserver les meilleures solutions
            int nbElite = taillePopulation / 10;
            std::vector<Solution> populationTriee = population;
            std::sort(populationTriee.begin(), populationTriee.end(), 
                     [](const Solution& a, const Solution& b) { return a.fitness < b.fitness; });
            
            for (int i = 0; i < nbElite; i++) {
                nouvellePopulation.push_back(populationTriee[i]);
            }
            
            // Compléter la population avec des croisements
            while (nouvellePopulation.size() < taillePopulation) {
                // Sélectionner deux parents
                int idxParent1 = parents[std::uniform_int_distribution<>(0, parents.size() - 1)(rng)];
                int idxParent2 = parents[std::uniform_int_distribution<>(0, parents.size() - 1)(rng)];
                
                // Réaliser le croisement
                if (std::uniform_real_distribution<>(0, 1)(rng) < tauxCroisement) {
                    Solution enfant = croiser(population[idxParent1], population[idxParent2]);
                    
                    // Réaliser la mutation
                    if (std::uniform_real_distribution<>(0, 1)(rng) < tauxMutation) {
                        muter(enfant);
                    }
                    
                    nouvellePopulation.push_back(enfant);
                } else {
                    // Pas de croisement, copier un parent
                    nouvellePopulation.push_back(population[idxParent1]);
                }
            }
            
            // Remplacer l'ancienne population
            population = nouvellePopulation;
            
            // Évaluer la nouvelle population
            evaluerPopulation();
            
            // Trouver la meilleure solution de cette génération
            trouverMeilleureSolution();
            
            if (generation % 50 == 0) {
                std::cout << "Génération " << generation << ", meilleure fitness = " 
                          << meilleureGlobale.fitness << " kg" << std::endl;
            }
        }
        
        std::cout << "Optimisation terminée !" << std::endl;
        std::cout << "Meilleure solution trouvée avec " << meilleureGlobale.nbArbres 
                  << " arbres, masse totale = " << meilleureGlobale.fitness << " kg" << std::endl;
        
        return meilleureGlobale;
    }
    
    // Génère une transmission à partir de la meilleure solution
    std::unique_ptr<Trans> genererTransmission() {
        if (!meilleureGlobale.valide) {
            std::cout << "Aucune solution valide trouvée !" << std::endl;
            return std::unique_ptr<Trans>(new Trans(vitesseEntree, vitesseSortie, sensRotation, 0, nullptr));
        }
        
        // Nombre de roues = 2 * nombre d'arbres
        int nbRoues = meilleureGlobale.nbArbres * 2;
        
        // Créer les arbres
        Arbre* premierArbre = nullptr;
        Arbre* arbrePrecedent = nullptr;
        
        double vitesseCourante = vitesseEntree;
        int sensCourant = sensRotation;
        
        for (int i = 0; i < meilleureGlobale.nbArbres; i++) {
            double moduleRoueMenante = meilleureGlobale.modules[i * 2];
            double moduleRoueMenee = meilleureGlobale.modules[i * 2 + 1];
            
            int dentsRoueMenante = meilleureGlobale.dents[i * 2];
            int dentsRoueMenee = meilleureGlobale.dents[i * 2 + 1];
            
            Roue* roueMenante = new Roue(moduleRoueMenante, dentsRoueMenante);
            Roue* roueMenee = new Roue(moduleRoueMenee, dentsRoueMenee);
            
            // Calculer la vitesse de sortie de cet arbre
            double rapport = static_cast<double>(dentsRoueMenee) / dentsRoueMenante;
            double vitesseSortieArbre = vitesseCourante / rapport;
            
            // Utiliser le constructeur spécifié
            Arbre* nouvelArbre = new Arbre(roueMenante, roueMenee, vitesseCourante, sensCourant);
            
            if (premierArbre == nullptr) {
                premierArbre = nouvelArbre;
            } else {
                arbrePrecedent->setSuivant(nouvelArbre);
            }
            
            arbrePrecedent = nouvelArbre;
            
            // Mettre à jour pour l'arbre suivant
            vitesseCourante = vitesseSortieArbre;
            sensCourant = -sensCourant; // Inverser le sens à chaque étage
        }
        
        // S'assurer que le dernier arbre n'a pas de suivant
        if (arbrePrecedent != nullptr) {
            arbrePrecedent->setSuivant(nullptr);
        }
        
        // Utiliser le constructeur spécifié pour Trans
        return std::unique_ptr<Trans>(new Trans(vitesseEntree, vitesseSortie, sensRotation, meilleureGlobale.nbArbres, premierArbre));
    }

private:
    // Initialise la population avec des solutions aléatoires
    void initialiserPopulation() {
        population.clear();
        
        // Générer une population de solutions aléatoires
        for (int i = 0; i < taillePopulation; i++) {
            // Déterminer un nombre d'arbres aléatoire (entre 1 et 5 par exemple)
            int nbArbres = std::uniform_int_distribution<>(1, 5)(rng);
            
            Solution solution;
            solution.nbArbres = nbArbres;
            solution.valide = false;
            
            // Générer des modules aléatoires parmi les disponibles
            for (int j = 0; j < nbArbres * 2; j++) {
                int idxModule = std::uniform_int_distribution<>(0, modulesDisponibles.size() - 1)(rng);
                solution.modules.push_back(modulesDisponibles[idxModule]);
            }
            
            // Générer des nombres de dents aléatoires dans la plage autorisée
            for (int j = 0; j < nbArbres * 2; j++) {
                int nbDents = std::uniform_int_distribution<>(minDents, maxDents)(rng);
                solution.dents.push_back(nbDents);
            }
            
            population.push_back(solution);
        }
    }
    
    // Évalue la fitness de toutes les solutions de la population
    void evaluerPopulation() {
        for (auto& solution : population) {
            evaluerSolution(solution);
        }
    }
    
    // Évalue une solution individuelle
    void evaluerSolution(Solution& solution) {
        // Vérifier si la solution respecte la contrainte de rapport de vitesse
        double rapportGlobal = vitesseEntree / vitesseSortie;
        double rapportCalcule = 1.0;
        
        for (int i = 0; i < solution.nbArbres; i++) {
            int idxMenante = i * 2;
            int idxMenee = i * 2 + 1;
            
            double rapport = static_cast<double>(solution.dents[idxMenee]) / solution.dents[idxMenante];
            rapportCalcule *= rapport;
        }
        
        // Tolérance de 1% sur le rapport de vitesse
        double tolerance = 0.01;
        solution.valide = std::abs(rapportCalcule / rapportGlobal - 1.0) <= tolerance;
        
        if (solution.valide) {
            // Calculer la masse totale des roues
            double masseTotale = 0.0;
            
            for (int i = 0; i < solution.nbArbres * 2; i++) {
                double module = solution.modules[i];
                int dents = solution.dents[i];
                
                // Diamètre primitif = module * nombre de dents
                double diametre = module * dents;
                
                // Vérifier que le diamètre est dans la plage autorisée
                if (diametre > maxDiametre) {
                    solution.valide = false;
                    solution.fitness = std::numeric_limits<double>::max();
                    return;
                }
                
                // Calculer la masse de la roue (volume * densité)
                double rayon = diametre / 2.0;
                double volume = M_PI * rayon * rayon * epaisseur;
                double masse = volume * densite;
                
                masseTotale += masse;
            }
            
            solution.fitness = masseTotale;
        } else {
            solution.fitness = std::numeric_limits<double>::max();
        }
    }
    
    // Trouve la meilleure solution de la population courante
    void trouverMeilleureSolution() {
        for (const auto& solution : population) {
            if (solution.valide && solution.fitness < meilleureGlobale.fitness) {
                meilleureGlobale = solution;
            }
        }
    }
    
    // Sélectionne les parents pour la reproduction (sélection par tournoi)
    std::vector<int> selectionnerParents() {
        std::vector<int> parents;
        
        // Taille du tournoi
        int tailleTournoi = 3;
        
        for (int i = 0; i < taillePopulation; i++) {
            // Sélectionner des individus aléatoires pour le tournoi
            std::vector<int> tournoi;
            for (int j = 0; j < tailleTournoi; j++) {
                int idx = std::uniform_int_distribution<>(0, taillePopulation - 1)(rng);
                tournoi.push_back(idx);
            }
            
            // Trouver le meilleur du tournoi
            int idxMeilleur = tournoi[0];
            for (int j = 1; j < tailleTournoi; j++) {
                if (population[tournoi[j]].fitness < population[idxMeilleur].fitness) {
                    idxMeilleur = tournoi[j];
                }
            }
            
            parents.push_back(idxMeilleur);
        }
        
        return parents;
    }
    
    // Réalise un croisement entre deux solutions
    Solution croiser(const Solution& parent1, const Solution& parent2) {
        Solution enfant;
        
        // Décider du nombre d'arbres (prendre celui d'un des parents)
        if (std::uniform_int_distribution<>(0, 1)(rng) == 0) {
            enfant.nbArbres = parent1.nbArbres;
        } else {
            enfant.nbArbres = parent2.nbArbres;
        }
        
        // Croisement des modules et dents
        int taille = enfant.nbArbres * 2;
        
        // Point de croisement aléatoire
        int pointCroisement = std::uniform_int_distribution<>(1, taille - 1)(rng);
        
        for (int i = 0; i < taille; i++) {
            if (i < pointCroisement) {
                // Prendre les gènes du parent 1 (si disponibles)
                if (i < parent1.modules.size()) {
                    enfant.modules.push_back(parent1.modules[i]);
                    enfant.dents.push_back(parent1.dents[i]);
                } else {
                    // Générer aléatoirement
                    int idxModule = std::uniform_int_distribution<>(0, modulesDisponibles.size() - 1)(rng);
                    enfant.modules.push_back(modulesDisponibles[idxModule]);
                    enfant.dents.push_back(std::uniform_int_distribution<>(minDents, maxDents)(rng));
                }
            } else {
                // Prendre les gènes du parent 2 (si disponibles)
                if (i < parent2.modules.size()) {
                    enfant.modules.push_back(parent2.modules[i]);
                    enfant.dents.push_back(parent2.dents[i]);
                } else {
                    // Générer aléatoirement
                    int idxModule = std::uniform_int_distribution<>(0, modulesDisponibles.size() - 1)(rng);
                    enfant.modules.push_back(modulesDisponibles[idxModule]);
                    enfant.dents.push_back(std::uniform_int_distribution<>(minDents, maxDents)(rng));
                }
            }
        }
        
        return enfant;
    }
    
    // Applique une mutation à une solution
    void muter(Solution& solution) {
        // Choisir aléatoirement s'il faut modifier le nombre d'arbres
        if (std::uniform_real_distribution<>(0, 1)(rng) < 0.1) {
            // Modifier le nombre d'arbres (+1 ou -1)
            int delta = std::uniform_int_distribution<>(0, 1)(rng) * 2 - 1; // -1 ou +1
            solution.nbArbres = std::max(1, std::min(5, solution.nbArbres + delta));
            
            // Ajuster la taille des vecteurs modules et dents
            int nouvelleTaille = solution.nbArbres * 2;
            
            if (nouvelleTaille > solution.modules.size()) {
                // Ajouter des modules et dents aléatoires
                while (solution.modules.size() < nouvelleTaille) {
                    int idxModule = std::uniform_int_distribution<>(0, modulesDisponibles.size() - 1)(rng);
                    solution.modules.push_back(modulesDisponibles[idxModule]);
                    solution.dents.push_back(std::uniform_int_distribution<>(minDents, maxDents)(rng));
                }
            } else if (nouvelleTaille < solution.modules.size()) {
                // Réduire les vecteurs
                solution.modules.resize(nouvelleTaille);
                solution.dents.resize(nouvelleTaille);
            }
        }
        
        // Mutation des modules (changer un module aléatoirement)
        for (int i = 0; i < solution.modules.size(); i++) {
            if (std::uniform_real_distribution<>(0, 1)(rng) < 0.1) {
                int idxModule = std::uniform_int_distribution<>(0, modulesDisponibles.size() - 1)(rng);
                solution.modules[i] = modulesDisponibles[idxModule];
            }
        }
        
        // Mutation des dents (ajouter ou retirer quelques dents)
        for (int i = 0; i < solution.dents.size(); i++) {
            if (std::uniform_real_distribution<>(0, 1)(rng) < 0.1) {
                // Modifier le nombre de dents (+/-5 dents maximum)
                int delta = std::uniform_int_distribution<>(-5, 5)(rng);
                solution.dents[i] = std::max(minDents, std::min(maxDents, solution.dents[i] + delta));
            }
        }
    }
};

#endif // ALGO_GENETIQUE_H