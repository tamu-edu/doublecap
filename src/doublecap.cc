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
const G4int NUMBEROFTHREADS = 1; // number of threads for MT mode

// simulation mode:
// 0 = test source (0.1 eV neutrons at (0,0,0))
// 1 = rate simulation (Cf source)
// 2 = capture simulation (capture source)
const G4int SIMULATIONMODE = 0; 


int main(int argc, char *argv[]) {

    auto start = std::chrono::steady_clock::now();

    auto *runmgr = G4RunManagerFactory::CreateRunManager();
    runmgr->SetNumberOfThreads(NUMBEROFTHREADS);
    G4PhysListFactory *factory = new G4PhysListFactory;
    G4VisManager* vismgr = new G4VisExecutive;


    if (RANDOMIZE) {
        auto SEED = time(nullptr);
        CLHEP::HepRandom::setTheSeed(SEED);
        G4cout << "RANDOMIZING WITH SEED " << SEED << G4endl;
    } else {
        G4cout << "NOT RANDOMIZING" << G4endl;
    }

    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localTime);

    G4String filename;
    
    switch (SIMULATIONMODE) {
        case 0:
            filename = "test_data/testsim_" + G4String(buffer) + ".root";
            G4cout << "SIMULATION MODE 0: TEST" << G4endl;
            break;
        case 1:
            filename = "capture_sim/simtest_" + G4String(buffer) + ".root";
            G4cout << "SIMULATION MODE 1: RATE SIMULATION" << G4endl;
            break;
        case 2:
            filename = "capture_sim/simdata_" + G4String(buffer) + ".root";
            G4cout << "SIMULATION MODE 2: CAPTURE SIMULATION" << G4endl;
            break;
        default:
            G4cerr << "Error. Unknown simulation mode." << G4endl;
            return 1;
    }
    G4cout << "Writing data to " << filename << G4endl;

    // geometry
    G4double lm_face = 1.*cm; // width of lowmass
    G4double lm_thick = 1.*mm; // thickness of lowmass

    G4double hm_diameter = 7.5*cm; // diameter of highmasses
    G4double hm_thick = 2.5*cm; // thickness of highmasses

    G4double airgap = 0.1*mm; // airgap between detector and surrounding copper
    G4double cu_thick = 0.2*mm; // thickness of copper boxes
    G4double spacing = 1.5*mm; // spacing between silicon detectors

    G4double pethickness = 5.*cm; // thickness of polyethylene between detector box and lead shield
    G4double leadthickness = 10.*cm; // thickness (height) of lead shield
    G4double leadradius = 3.*cm; // radius of cylindrical lead shield
    G4double sourcesize = 1.*mm; // side length of Cf cube

    G4int Z = 98, A = 252; // Cf-252 calibration source
    //G4int Z = 0, A = 0; // generic neutron source

    DCGeometry *geometry = new DCGeometry(lm_face, lm_thick, hm_diameter, hm_thick, cu_thick, spacing, airgap, pethickness, leadthickness, leadradius, sourcesize);

    // z position of center of source block
    G4double sourcez = geometry->GetSourceZ();

    G4cout << "sourcez = " << sourcez/cm << " cm" << G4endl;

    runmgr->SetUserInitialization(factory->GetReferencePhysList("Shielding_HP"));
    runmgr->SetUserInitialization(new DCInitialization(Z, A, geometry, SIMULATIONMODE, filename));
    runmgr->SetUserInitialization(geometry);

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





