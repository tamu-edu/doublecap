#ifndef DCRUNACTION_HH
#define DCRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"

class DCRunAction : public G4UserRunAction {
public:
    DCRunAction(G4String);
    virtual ~DCRunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

private:
    G4int verbosity = 0;
    G4String filename;
};

#endif