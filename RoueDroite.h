#ifndef ROUE_DROITE_H
#define ROUE_DROITE_H

#include "Roue.h"

class RoueDroite : public Roue {
public:
    RoueDroite(double module, int nbDents) : Roue(module, nbDents) {}
    RoueDroite(int diametre, double module) : Roue(diametre, module) {}
};

#endif // ROUE_DROITE_H