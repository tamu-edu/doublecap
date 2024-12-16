#ifndef DCTRACKACTION_HH
#define DCTRACKACTION_HH


#include "G4UserTrackAction.hh"


class DCTrackAction : public G4UserTrackAction {
public:
    DCTrackAction();
    virtual ~DCTrackAction() override = default;
};


#endif