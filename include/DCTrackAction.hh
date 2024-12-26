#ifndef DCTRACKACTION_HH
#define DCTRACKACTION_HH

#include "G4Track.hh"
#include "G4UserTrackingAction.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ParticleTable.hh"

class DCTrackAction : public G4UserTrackingAction {
public:
    DCTrackAction();
    virtual ~DCTrackAction() override = default;

    void PreUserTrackingAction(const G4Track*) override;

    void PostUserTrackingAction(const G4Track*) override;
};


#endif