// heavily draw on examples/basic/B3
#include "DCRunAction.hh"

#include "DCActionInitialization.hh"

#include "G4RootAnalysisManager.hh"
#include "G4AccumulableManager.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4MTRunManager.hh"


DCRunAction::DCRunAction() {
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();

    accumblmgr->Register(enlm);
    accumblmgr->Register(enhm);
}


void DCRunAction::BeginOfRunAction(const G4Run *run) {

    if (verbosity > 0) {
        G4cout << "Start of BeginOfRunAction" << G4endl;
    }
    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Reset();

    // inform the runManager to save random number seed
    G4MTRunManager::GetRunManager()->SetRandomNumberStore(false);


    auto analysismgr = G4RootAnalysisManager::Instance();
    //analysismgr->SetNtupleMerging(true);

    if (1){//G4Threading::IsMasterThread()) {

        time_t now = time(0);
        tm* localTime = localtime(&now);

        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localTime);

        G4String filename = "simdata_" + G4String(buffer) + ".root";
        analysismgr->OpenFile(filename);

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

    }

    //G4cout << "id = " << id << " out of " << analysismgr->GetNofNtuples() << G4endl;
    //G4cout << "GetNofNtuple = " << analysismgr->GetNofNtuples() << G4endl;
    //G4cout << "finished BeginOfRunAction" << G4endl
    if (verbosity > 0) {
        G4cout << "End of BeginOfRunAction" << G4endl;
    }
}


void DCRunAction::EndOfRunAction(const G4Run *run) {

    if (verbosity > 0) {
        G4cout << "Start of EndOfRunAction" << G4endl;
    }

    G4int nofEvents = run->GetNumberOfEvent();
    if (nofEvents == 0) return;

    G4AccumulableManager *accumblmgr = G4AccumulableManager::Instance();
    accumblmgr->Merge();

    if (1){//G4Threading::IsMasterThread()) {
        auto analysismgr = G4RootAnalysisManager::Instance();
        analysismgr->Write();
        analysismgr->CloseFile();
    }

    //G4cout << "GetNofNtuple = " << analysismgr->GetNofNtuples() << G4endl;
    if (verbosity > 0) {
        G4cout << "End of EndOfRunAction" << G4endl;
    }

}


