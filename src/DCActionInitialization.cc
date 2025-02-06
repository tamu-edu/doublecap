// DCActionInitialization.cc - source code for DCActionInitialization.hh

#include "DCUtilities.hh"
#include "DCActionInitialization.hh"
#include "DCPrimaryGenerator.hh"
#include "DCRunAction.hh"
#include "DCTrackAction.hh"
#include "DCStackingAction.hh"

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4IonTable.hh"
#include "G4ParticleTable.hh"


DCInitialization::DCInitialization(G4int Z, G4int A, DCGeometry *geom, G4int mode, G4String fname, G4bool killGammas) : 
    G4VUserActionInitialization(), 
    Z(Z), A(A), mode(mode), filename(fname),
    geometry(geom), fKillGammas(killGammas)
{;}

void DCInitialization::BuildForMaster() const {
    SetUserAction(new DCRunAction(filename));
}


void DCInitialization::Build() const {
    DCRunAction *runAction = new DCRunAction(filename);
    SetUserAction(runAction);
    SetUserAction(new DCTrackAction);

    if (fKillGammas) SetUserAction(new DCStackingAction);

    switch (mode) {
        case 0:
            SetUserAction(new TestGenerator);
            break;
        case 1:
            SetUserAction(new DCPrimaryGenerator(Z, A, geometry));
            break;
        case 2:
            SetUserAction(new CaptureGenerator(geometry));
            break;
        default:
            G4cerr << "Simulation mode not recognized. This will probably not go well." << G4endl;
    }
}

