// heavily draw on examples/basic/B3
#include "DCRunAction.hh"

#include "DCActionInitialization.hh"

#include "G4AccumulableManager.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"


DCRunAction::DCRunAction() {
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();

    accumblmgr->Register(enlm);
    accumblmgr->Register(enhm);
}


void DCRunAction::BeginOfRunAction(const G4Run *run) {
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Reset();

    // inform the runManager to save random number seed
    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}


void DCRunAction::EndOfRunAction(const G4Run *run) {
    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Merge();

}


