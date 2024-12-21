#ifndef DCSENSITIVEDETECTOR_HH
#define DCSENSITIVEDETECTOR_HH


#include "G4VSensitiveDetector.hh"
#include "G4RootAnalysisManager.hh"


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
    G4RootAnalysisManager *fAnalysisMgr;
};



#endif
