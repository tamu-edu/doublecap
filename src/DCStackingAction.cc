

#include "DCStackingAction.hh"

#include "G4Track.hh"


G4ClassificationOfNewTrack DCStackingAction::ClassifyNewTrack(const G4Track *track) {
    G4ClassificationOfNewTrack status = fUrgent;
    const G4ParticleDefinition *part = track->GetParticleDefinition();
    
    return status; // return normal "add-to-top-of-stack" status unless we have a reason to kill
}
