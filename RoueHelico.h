#ifndef ROUE_HELICO_H
#define ROUE_HELICO_H

#include "Roue.h"

class RoueHelico : public Roue {
private:
    double angleHelice; // angle d'hélice

public:
    RoueHelico(double module, int nbDents, double angle = 0)
        : Roue(module, nbDents), angleHelice(angle) {}
    
    RoueHelico(int diametre, double module, double angle = 0)
        : Roue(diametre, module), angleHelice(angle) {}
    
    double getAngleHelice() const { return angleHelice; }
    void setAngleHelice(double angle) { angleHelice = angle; }
};

#endif // ROUE_HELICO_H