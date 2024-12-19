#ifndef DCTRACKACTION_HH
#define DCTRACKACTION_HH

#include "DCRunAction.hh"
#include "G4Track.hh"
#include "G4UserTrackingAction.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4ParticleTable.hh"

class DCTrackAction : public G4UserTrackingAction {
public:
    DCTrackAction(DCRunAction*);
    virtual ~DCTrackAction() override = default;

    void PreUserTrackingAction(const G4Track*) override;

    void PostUserTrackingAction(const G4Track*) override;
private:
    DCRunAction *fRunAction;
    G4bool fIsCapture;

    static G4int CAPTURECOUNT;

    const G4ParticleDefinition *fGammaPD = 0;
};


#endif