#ifndef MASS_H_
#define MASS_H_

class Mass {
    public:
        double inert;
        double propellant;

        double Sum() {return inert + propellant;}
};

#endif
