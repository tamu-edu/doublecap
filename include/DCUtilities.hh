// DCUtilities: constants and functions of use
#ifndef DCUTILITIES_HH
#define DCUTILITIES_HH


#include <cmath>
#include <vector>
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

typedef std::vector<G4double> doubles;

// function declarations
G4ThreeVector RandomDirection();

G4double trap(const doubles, const doubles);


inline double interp(double x, double x1, double y1, double x2, double y2) {
    if (x1 == x2) {
        std::cerr << "Warning: interp() tried to interpolate between points on vertical line. Returning mean of endpoints." << std::endl << '\t' << x1 << " == " << x2 << std::endl;
        return (y1 + y2)/2;
    }
    return y1 + (x - x1)*(y2 - y1)/(x2 - x1);
}


#endif // DCUTILITIES_HH
