// DCActionInitialization.cc - source code for DCActionInitialization.hh

#include "DCUtilities.hh"
#include "DCActionInitialization.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCTrackAction.hh"
#include "DCEventAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCInitialization::DCInitialization(G4int Z, G4int A, G4double sourcex, G4double sourcey, G4double sourcez, G4bool test, G4String fname) : 
    G4VUserActionInitialization(), 
    Z(Z), A(A), test(test), filename(fname),
    source_position(G4ThreeVector(sourcex, sourcey, sourcez)) 
{;}

void DCInitialization::BuildForMaster() const {
    SetUserAction(new DCRunAction(filename));
}


void DCInitialization::Build() const {
    DCRunAction *runAction = new DCRunAction(filename);
    SetUserAction(runAction);
    SetUserAction(new DCTrackAction);
    SetUserAction(new DCEventAction(runAction));

    if (test) {
        SetUserAction(new TestGenerator());
    } else {
        SetUserAction(new DCPrimaryGenerator(Z, A, source_position));
    }
}

