// Trans.h
#pragma once
class Arbre;
class Roue;

class Trans{
    private : 
    double Ne; //vitesse d'entrée
    double Ns; // vitesse de sortie
    int sens; // 1 si sens horaire, -1 si sens trigo
    int Nb; // nombre de arbres
    Arbre* arbres;  // Pointeur vers le premier arbre de la liste
    public :
    Trans(double Ne, double Ns, int sens, int Nb, Arbre* arbres);
    ~Trans();
    double getNe();
    double getNs();
    int getSens();
    int getNb();
    Arbre* getArbres() { return arbres; }
    void setArbres(Arbre* a) { arbres = a;}
    void setNe(double Ne);
    void setNs(double Ns);
    void setSens(int sens);
    void setNb(int Nb);
    void ajoutArbre(Arbre* a);
    void retraitArbre(int);
};




