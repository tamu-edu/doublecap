#ifndef DCRUNACTION_HH
#define DCRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"

class DCRunAction : public G4UserRunAction {
public:
    DCRunAction();
    virtual ~DCRunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void SumLM(G4double en) { enlm += en; };
    void SumHM(G4double en) { enhm += en; };

private:
    G4Accumulable<G4double> enlm = 0.;
    G4Accumulable<G4double> enhm = 0.;
};

#endif