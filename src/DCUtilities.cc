// source file for DCUtilities.hh

#include "DCUtilities.hh"


G4ThreeVector RandomDirection() {

  G4double cosTheta = 2*G4UniformRand() - 1., phi = CLHEP::twopi*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi),
           uy = sinTheta*std::sin(phi),
           uz = cosTheta;

    return G4ThreeVector(ux, uy, uz);
}



// trapezoidal integration of (x,y) data
G4double trap(const doubles &x, const doubles &y) {
    size_t n = x.size();
    if (n != y.size()) {
        throw std::invalid_argument("Invalid arguments to function trap(). " 
        "Vectors must have the same length.");
    }
    G4double s = 0;
    for (size_t i = 1; i < n; ++i) {
        s += (y[i] + y[i-1])*(x[i] - x[i-1])/2.;
    }
    return s;
}


