#pragma once

class Roue{
    private : 
    int Z; // nombre de dents
    double D; // diamètre
    double M; // module
    public :
    Roue(double module, int nbDents);
    Roue(double diametre, double module);
    ~Roue();
    int getZ();
    double getD();
    double getM();
    void setZ(int nbDents);
    void setD(double diametre);
    void setM(double module);
    bool verif(double m, double d, int nbdents);
};