#ifndef DCSENSITIVEDETECTOR_HH
#define DCSENSITIVEDETECTOR_HH



#include "DCHit.hh"
#include "G4VSensitiveDetector.hh"


class G4Step;
class G4HCofThisEvent;

class DCSensitiveDetector : public G4VSensitiveDetector {
public:
    DCSensitiveDetector(const G4String& name, const G4String& hitsCollectionName);
    virtual ~DCSensitiveDetector() override = default;

    
    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history) override;
    void EndOfEvent(G4HCofThisEvent* hitCollection) override;

private:
    DCHitsCollection *fHitsCollection = nullptr;
    const G4ParticleDefinition *fGammaPD = nullptr;
};



#endif