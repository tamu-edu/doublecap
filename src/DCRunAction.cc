// heavily draw on examples/basic/B3
#include "DCRunAction.hh"

#include "DCActionInitialization.hh"

#include "G4RootAnalysisManager.hh"
#include "G4AccumulableManager.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"


DCRunAction::DCRunAction(G4String fname) : filename(fname) {
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();

    accumblmgr->Register(enlm);
    accumblmgr->Register(enhm);

    auto analysismgr = G4RootAnalysisManager::Instance();
    /*analysismgr->SetVerboseLevel(4);
    analysismgr->SetNtupleRowWise(true, true);
    analysismgr->SetNtupleMerging(true);
    analysismgr->SetBasketSize(32000);
    analysismgr->SetBasketEntries(4000);*/

    G4int id = analysismgr->CreateNtuple("tree", "tree");
    analysismgr->CreateNtupleIColumn("EventNum");
    analysismgr->CreateNtupleIColumn("TrkNum");
    analysismgr->CreateNtupleDColumn("Edep");
    analysismgr->CreateNtupleSColumn("VolName");
    analysismgr->CreateNtupleSColumn("PName");
    analysismgr->CreateNtupleSColumn("ProcName");
    analysismgr->CreateNtupleSColumn("ParentVol");
    analysismgr->CreateNtupleIColumn("IsCapture");
    analysismgr->FinishNtuple();

    if (verbosity > 0) {
        G4cout << "DCRunAction:" << G4endl;
        G4cout << "    ROOT filename = " << filename << G4endl;
        G4cout << "    Outputting Ntuple id = " << id << G4endl;
    }
}


void DCRunAction::BeginOfRunAction(const G4Run *run) {

    if (verbosity > 1) {
        G4cout << "Start of BeginOfRunAction" << G4endl;
    }
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Reset();

    // inform the runManager to save random number seed
    G4MTRunManager::GetRunManager()->SetRandomNumberStore(false);

    auto analysismgr = G4RootAnalysisManager::Instance();
    analysismgr->OpenFile(filename);

    if (verbosity > 1) {
        G4cout << "End of BeginOfRunAction" << G4endl;
    }
}


void DCRunAction::EndOfRunAction(const G4Run *run) {

    if (verbosity > 1) {
        G4cout << "Start of EndOfRunAction" << G4endl;
    }

    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Merge();


    auto analysismgr = G4RootAnalysisManager::Instance();
    analysismgr->Write();

    //if (G4Threading::IsMasterThread()) {
    analysismgr->CloseFile();
    

    //G4cout << "GetNofNtuple = " << analysismgr->GetNofNtuples() << G4endl;
    if (verbosity > 1) {
        G4cout << "End of EndOfRunAction" << G4endl;
    }

}


