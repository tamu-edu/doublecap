

#include "DCStackingAction.hh"

#include "G4Track.hh"


G4ClassificationOfNewTrack DCStackingAction::ClassifyNewTrack(const G4Track *track) {

    return fUrgent; // return normal "add-to-top-of-stack" status unless we have a reason to kill
}
