#pragma once

#include "ISystem.hpp"

class SpacecraftEquatorialCS : public ISystem {
public:
    /// @param mu geocentric gravitational constant 
    /// @param omega earth angular speed
    /// @param initialPosition initial position of SC
    /// @param initialSpeed initial speed of SC
    SpacecraftEquatorialCS(double mu, double omega, const Vector& initialPosition, const Vector& initialSpeed);

    void f(Vector &state) const;

    Vector getInitialState() const;

    ~SpacecraftEquatorialCS() {};
private:
    double mu;
    double omega;
    Vector initialState;
};