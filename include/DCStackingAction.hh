#ifndef DCSTACKINGACTION_HH
#define DCSTACKINGACTION_HH


#include "G4UserStackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"

class DCStackingAction : public G4UserStackingAction {
public:
    DCStackingAction() = default;
    virtual ~DCStackingAction() override = default;

    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*) override;
};


#endif