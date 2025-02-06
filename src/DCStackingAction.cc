

#include "DCStackingAction.hh"

#include "G4Track.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


G4ClassificationOfNewTrack DCStackingAction::ClassifyNewTrack(const G4Track *track) {
    G4ParticleDefinition *defn = track->GetDefinition();
    if (defn == G4Gamma::Definition() || defn == G4Electron::Definition() || defn == G4Positron::Definition()) {
        return fKill;
    } else {
        return fUrgent;
    }
}
