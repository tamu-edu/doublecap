// do the thing


#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "G4RunManagerFactory.hh"
#include "G4NistManager.hh"
#include "G4PhysListFactory.hh"
#include "G4ParticleHPManager.hh"

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

#include "G4TScoreNtupleWriter.hh"
#include "G4AnalysisManager.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4ElementVector.hh"

#include "G4Neutron.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4HadronicProcess.hh"
#include "G4HadronicParameters.hh"

#include "G4ios.hh"

#include "DCUtilities.hh"
#include "DCGeometry.hh"
#include "DCActionInitialization.hh"
#include "DCSensitiveDetector.hh"


const G4bool RANDOMIZE = false; // set random seed for RNG
const G4bool USETESTSOURCE = false; // test neutron source inside highmass1 detector
const G4int NUMBEROFTHREADS = 4; // number of threads for MT mode

int main(int argc, char *argv[]) {


    auto start = std::chrono::steady_clock::now();

    if (RANDOMIZE) {
        auto SEED = time(nullptr);
        CLHEP::HepRandom::setTheSeed(SEED);
        G4cout << "RANDOMIZING WITH SEED " << SEED << G4endl;
    } else {
        G4cout << "NOT RANDOMIZING" << G4endl;
    }

    auto *runmgr = G4RunManagerFactory::CreateRunManager();
    runmgr->SetNumberOfThreads(NUMBEROFTHREADS);
    G4PhysListFactory *factory = new G4PhysListFactory;
    G4VisManager* vismgr = new G4VisExecutive;

    runmgr->SetUserInitialization(factory->GetReferencePhysList("Shielding_HPT"));

    // geometry
    G4double lm_face = 1.*cm; // width of lowmass
    G4double lm_thick = 1.*mm; // thickness of lowmass

    G4double hm_diameter = 7.5*cm; // diameter of highmasses
    G4double hm_thick = 2.5*cm; // thickness of highmasses

    G4double airgap = 0.1*mm; // airgap between detector and surrounding copper
    G4double cu_thick = 0.2*mm; // thickness of copper boxes
    G4double spacing = 1.5*mm; // spacing between silicon detectors

    // spacing between copper layers
    G4double cuspace1 = 16.*cm;
    G4double cuspace2 = 9.*cm;

    // thickness of copper layer
    G4double platethickness = 6.*mm;

    G4double hang_distance = 6.5*mm; // distance below 4k stage center of source box hangs


    // position of center of source box
    G4double sourcex = 0; 
    G4double sourcey = 0;
    G4double sourcez = - (lm_thick/2. + spacing + hm_thick + airgap + cu_thick) + cuspace1 + cuspace2 + platethickness - hang_distance;


    G4int Z = 98, A = 252; // Cf-252 calibration source
    //G4int Z = 0, A = 0; // generic neutron source

    DCGeometry *geometry = new DCGeometry(lm_face, lm_thick, hm_diameter, hm_thick, cu_thick, spacing, airgap, sourcex, sourcey, sourcez, cuspace1, cuspace2, platethickness);

    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localTime);

    G4String filename = "simdata_" + G4String(buffer) + ".root";


    runmgr->SetUserInitialization(new DCInitialization(Z, A, sourcex, sourcey, sourcez, USETESTSOURCE, filename));
    runmgr->SetUserInitialization(geometry);

    // scoring ntuple writer for scorers
    G4TScoreNtupleWriter<G4AnalysisManager> scoreNtupleWriter;
    //scoreNtupleWriter.SetVerboseLevel(1);
    scoreNtupleWriter.SetNtupleMerging(true); // only for multithreaded mode

    runmgr->Initialize();
    vismgr->Initialize();

    // set to have correct gamma spectrum
    G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation(true);
    G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState(true);

    // set to get all radioactive decays
    G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay(1.0e+6*year);


    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if (argc > 1) {
        G4String macro = argv[1];
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    } else { // no macro - return error
        //G4cout << "No macro supplied. Executing test_macro.mac" << G4endl;
        //UImanager->ApplyCommand("/control/execute test_macro.mac");
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);
        ui->SessionStart();
        delete ui;
    }

    delete runmgr;
    delete vismgr;

    auto diff = std::chrono::steady_clock::now() - start;

    G4cout << "Elapsed time: " << std::chrono::duration<double, std::milli>(diff).count()/1000. << " seconds" << G4endl;

    return 0;
}





