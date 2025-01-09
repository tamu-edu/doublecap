// DCActionInitialization.cc - source code for DCActionInitialization.hh

#include "DCUtilities.hh"
#include "DCActionInitialization.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCTrackAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCInitialization::DCInitialization(G4int Z, G4int A, G4double sourcez, G4double sourcesize, G4int mode, G4String fname) : 
    G4VUserActionInitialization(), 
    Z(Z), A(A), mode(mode), filename(fname),
    sourcez(sourcez),
    sourcesize(sourcesize)
{;}

void DCInitialization::BuildForMaster() const {
    SetUserAction(new DCRunAction(filename));
}


void DCInitialization::Build() const {
    DCRunAction *runAction = new DCRunAction(filename);
    SetUserAction(runAction);
    SetUserAction(new DCTrackAction);

    switch (mode) {
        case 0:
            SetUserAction(new TestGenerator);
            break;
        case 1:
            SetUserAction(new DCPrimaryGenerator(Z, A, sourcez, sourcesize));
            break;
        default:
            G4cerr << "Simulation mode not recognized. This will probably not go well." << G4endl;
    }
}

