// DCActionInitialization.cc - source code for DCActionInitialization.hh

#include "DCUtilities.hh"
#include "DCActionInitialization.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCEventAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCInitialization::DCInitialization(G4int Z, G4int A, G4double sourcex, G4double sourcey, G4double sourcez, G4bool test) : 
    G4VUserActionInitialization(), 
    Z(Z), A(A), test(test),
    source_position(G4ThreeVector(sourcex, sourcey, sourcez)) 
{;}

void DCInitialization::Build() const {
    DCRunAction *runAction = new DCRunAction();
    SetUserAction(runAction);
    SetUserAction(new DCEventAction(runAction));

    if (test) {
        SetUserAction(new TestGenerator());
    } else {
        SetUserAction(new DCPrimaryGenerator(Z, A, source_position));
    }
}

