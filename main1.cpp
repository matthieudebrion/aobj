#include <iostream>
#include "Roue.h"
#include "Arbre.h"
#include "Trans.h"

// Fonction pour trouver la meilleure combinaison de nombres de dents
std::pair<int, int> meilleur_rapport(double rapportCible, int minDents, int maxDents) {
    int meilleureZ1 = 0;
    int meilleureZ2 = 0;
    double meilleurEcart = std::numeric_limits<double>::max();
    
    // Parcourir toutes les combinaisons possibles dans les limites
    for (int z1 = minDents; z1 <= maxDents; z1++) {
        // Calculer le nombre de dents théorique pour la roue menée
        double z2Theorique = z1 * rapportCible;
        
        // Tester les nombres de dents entiers autour de cette valeur théorique
        for (int z2 = std::max(minDents, static_cast<int>(z2Theorique - 5)); 
             z2 <= std::min(maxDents, static_cast<int>(z2Theorique + 5)); 
             z2++) {
            
            // Calculer le rapport avec ces nombres de dents
            double rapportActuel = static_cast<double>(z2) / z1;
            
            // Calculer l'écart relatif avec le rapport cible
            double ecart = std::abs(rapportActuel - rapportCible) / rapportCible;
            
            // Si c'est meilleur que ce qu'on a trouvé jusqu'à présent
            if (ecart < meilleurEcart) {
                meilleurEcart = ecart;
                meilleureZ1 = z1;
                meilleureZ2 = z2;
            }
        }
    }
    
    return std::make_pair(meilleureZ1, meilleureZ2);
}

void instancierTrainConnu(Trans& trans, double Ve, double Vs, int sensEntree, int nbArbres) {
    // Calcul du rapport total du train
    double Vtot = Ve / Vs;
    
    // Calcul du rapport pour chaque étage (racine n-ième)
    double Rn = std::pow(Vtot, 1.0 / nbArbres);
    
    // Module fixé à 4 comme spécifié
    double module = 4.0;
    
    // Vitesse courante (commence par la vitesse d'entrée)
    double vitesse = Ve;
    
    // Sens de rotation courant
    int sens = sensEntree;
    
    std::cout << "Création d'un train avec " << nbArbres << " arbres" << std::endl;
    std::cout << "Rapport total: " << Vtot << " (Ve=" << Ve << ", Vs=" << Vs << ")" << std::endl;
    std::cout << "Rapport par étage: " << Rn << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < nbArbres; i++) {
        std::cout << "Arbre #" << (i+1) << ":" << std::endl;
        
        // Utilisation de notre fonction meilleur_rapport
        std::pair<int, int> paireDents = meilleur_rapport(Rn, 18, 150);
        int ZMenante = paireDents.first;
        int ZMenee = paireDents.second;
        
        // Calcul du rapport réel de cet étage
        double rapportReel = static_cast<double>(ZMenee) / ZMenante;
        
        std::cout << "  Module: " << module << std::endl;
        std::cout << "  Dents menante: " << ZMenante << std::endl;
        std::cout << "  Dents menée: " << ZMenee << std::endl;
        std::cout << "  Diamètre menante: " << (module * ZMenante) << " mm" << std::endl;
        std::cout << "  Diamètre menée: " << (module * ZMenee) << " mm" << std::endl;
        std::cout << "  Rapport cible: " << Rn << std::endl;
        std::cout << "  Rapport réel: " << rapportReel << std::endl;
        std::cout << "  Erreur relative: " << (std::abs(rapportReel - Rn) / Rn * 100) << "%" << std::endl;
        std::cout << "  Vitesse d'entrée: " << vitesse << " tr/min" << std::endl;
        
        // Création des roues
        Roue* menante = new Roue(module, ZMenante);
        Roue* menee = new Roue(module, ZMenee);
        
        // Calcul de la vitesse de sortie de cet étage
        double vitesseSortie = vitesse / rapportReel;
        std::cout << "  Vitesse de sortie: " << vitesseSortie << " tr/min" << std::endl;
        std::cout << "  Sens: " << (sens > 0 ? "Horaire" : "Anti-horaire") << std::endl;
        std::cout << std::endl;
        
        // Création de l'arbre avec la bonne signature de constructeur
        Arbre* arbre = new Arbre(menante, menee, vitesse, sens);
        
        // Ajout de l'arbre à la transmission
        trans.ajoutArbre(arbre);
        
        // Mise à jour pour le prochain étage
        vitesse = vitesseSortie;
        sens = -sens; // Inversion du sens de rotation à chaque étage
    }
    
    // Vérification du résultat final
    double rapportObtenu = Ve / vitesse;
    double erreur = std::abs(rapportObtenu - Vtot) / Vtot * 100;
    
    std::cout << "Résumé du train:" << std::endl;
    std::cout << "Rapport demandé: " << Vtot << std::endl;
    std::cout << "Rapport obtenu: " << rapportObtenu << std::endl;
    std::cout << "Erreur globale: " << erreur << "%" << std::endl;
    
    // Vérification du sens de sortie final
    int sensTheorique = sensEntree;
    if (nbArbres % 2 == 1) {
        sensTheorique = -sensEntree; // Inversion si nombre impair d'arbres
    }
    
    std::cout << "Sens d'entrée: " << (sensEntree > 0 ? "Horaire" : "Anti-horaire") << std::endl;
    std::cout << "Sens de sortie: " << (sens > 0 ? "Horaire" : "Anti-horaire") << std::endl;
}

// Fonction de test pour vérifier notre implémentation
void testerInstancierTrainConnu() {
    // Créer de nouvelles transmissions avec les bons paramètres
    // Puisque Trans n'a pas de constructeur par défaut, nous devons fournir tous les paramètres
    
    std::cout << "=== TEST 1: Réduction 10:1 avec 2 arbres ===" << std::endl;
    // Initialiser avec des valeurs par défaut, puis instancier le train
    Trans trans(1000, 100, 1, 2, nullptr); // Ne, Ns, sens, Nb, arbres (nullptr au début)
    instancierTrainConnu(trans, 1000, 100, 1, 2);
    
    std::cout << "\n=== TEST 2: Réduction 50:1 avec 3 arbres ===" << std::endl;
    Trans trans2(2000, 40, 1, 3, nullptr);
    instancierTrainConnu(trans2, 2000, 40, 1, 3);
    
    std::cout << "\n=== TEST 3: Multiplication 1:5 avec 2 arbres ===" << std::endl;
    Trans trans3(500, 2500, -1, 2, nullptr);
    instancierTrainConnu(trans3, 500, 2500, -1, 2);
}

// Fonction main - point d'entrée du programme
int main() {
    std::cout << "Programme de test pour les trains d'engrenages" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    
    // Appel de la fonction de test
    testerInstancierTrainConnu();
    
    return 0;
}
