#ifndef DCEVENTACTION_HH
#define DCEVENTACTION_HH

#include "G4UserEventAction.hh"

#include "DCRunAction.hh"
#include "DCTrackAction.hh"

class DCRunAction;

class DCEventAction : public G4UserEventAction {
public:
    DCEventAction(DCRunAction*, DCTrackAction*);
    virtual ~DCEventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

private:
    DCRunAction *runAction;
    DCTrackAction *trackAction;

    G4int id_lm = -1;
    G4int id_hm = -1;
};



#endif