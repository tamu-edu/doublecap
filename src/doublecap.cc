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


int main(int argc, char *argv[]) {
    /*
    usage: ./doublecap macro.mac [args]

    command line arguments

    -f folder
        folder name under output directory to place file. If empty or "default", default depends on simulation mode (0 - "test_data", 1 - "rate_sim", 2 - "capture_sim")

    -i  idx
        ID or index number of filename, appended after timestamp to denote multiple runs started at the same time

    -k kill-gammas
        integer flag to specify whether or not to kill gammas, e-, and e+ (default 0)

    -l Pb-thickness
        thickness (cm) of lead shield between source and detector box (default 20 cm)

    -m  /macro/path
        Path to macro files (all macro files should be given just as titles and this argument should be specified)

    -n  number-of-threads
        specify number of threads for multithreading (default 8)

    -o  /output/dir
        Path to output directory, e.g., for simulation mode zero, output file names will be "/output/dir/test_data/testsim_XXXXXX_XXXXXX_XXXX_tX.root"

    -p PE-thickness
        thickness (cm) of polyethylene between source and detector box (default 1 cm)

    -r  randomize
        integer flag, whether or not to set random state with new random seed (default 1)

    -s  simulation-mode
        set simulation mode (default 1)
        - 0 = test source (0.1 eV neutrons at (0,0,0))
        - 1 = rate simulation (Cf source)
        - 2 = capture simulation (capture source)
        - 3 = neutron gun (particle source with just 2-MeV gammas)
    */
    
    G4String argval;
    G4String filename;
    G4String folder = "default";
    G4String index = "";
    G4bool add_macro_path = false;
    G4String macro_path;
    G4String output_dir = "";
    G4bool KILLGAMMAS = false;
    G4bool RANDOMIZE = true;
    G4int NUMBEROFTHREADS = 8;
    G4int SIMULATIONMODE = 1; 
    G4double leadthickness = 20.*cm; // thickness (height) of lead shield
    G4double pethickness = 1.*cm; // thickness of polyethylene between detector box and lead shield

    if (argc > 2) { // extra command line arguments
        G4int arg_idx = 1;

        while (arg_idx < argc) {

            if (argv[arg_idx][0] == '-') {

                argval = G4String(argv[arg_idx+1]);

                switch (argv[arg_idx][1]) {

                    case 'f': // folder
                        folder = argval;
                        break;

                    case 'i': // idx
                        index = G4String("_") + argval;
                        break;

                    case 'k': // kill-gammas
                        KILLGAMMAS = (argval != "0");
                        break;

                    case 'l': // Pb-thickness
                        leadthickness = atof(argv[arg_idx+1])*cm;
                        break;
                        
                    case 'm': // /macro/path
                        add_macro_path = true;
                        macro_path = argval;
                        break;

                    case 'n': //number-of-threads
                        NUMBEROFTHREADS = atoi(argv[arg_idx+1]);
                        break;

                    case 'o': // /output/dir
                    output_dir = argval + "/";
                        break;

                    case 'p': // PE-thickness
                        pethickness = atof(argv[arg_idx+1])*cm;
                        break;

                    case 'r': // randomize
                        RANDOMIZE = (argval != "0");
                        break;

                    case 's': // simulation-mode
                        SIMULATIONMODE = atoi(argv[arg_idx+1]);
                        break;

                    default:
                        G4cout << "Unrecognized command-line option" << '"' << argv[arg_idx] << '"' << G4endl;
                }
                ++arg_idx;
            } else {
                G4cout << "Unused out-of-phase command line argument: \"" << argv[arg_idx] << '"' << G4endl;
            }
            ++arg_idx;
        }
    }

    if (RANDOMIZE) {
        auto SEED = time(nullptr);
        CLHEP::HepRandom::setTheSeed(SEED);
        G4cout << "RANDOMIZING WITH SEED " << SEED << G4endl;
    } else {
        G4cout << "NOT RANDOMIZING" << G4endl;
    }

    G4cout << "KILLGAMMAS = " << KILLGAMMAS << G4endl;
    G4cout << "NUMBEROFTHREADS = " << NUMBEROFTHREADS << G4endl;
    G4cout << "output_dir: " << output_dir << G4endl;
    G4cout << "folder: " << folder << G4endl;
    G4cout << "index: " << index << G4endl;
    G4cout << "macro_path: " << macro_path << G4endl;
    G4cout << "leadthickness = " << leadthickness/cm << " cm" << G4endl; 
    G4cout << "pethickness = " << pethickness/cm << " cm" << G4endl; 

    G4String title_prefix;
    
    time_t now = time(0);
    tm* localTime = localtime(&now);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", localTime);

    switch (SIMULATIONMODE) {
        case 0:
            if (folder == "default") {folder = "test_data";}
            filename = output_dir + folder + "/testsim_" + G4String(buffer) + index + ".root";
            G4cout << "SIMULATION MODE 0: TEST" << G4endl;
            break;
        case 1:
            if (folder == "default") {folder = "rate_sim";}
            title_prefix = G4String("Lead") + G4String(to_string(leadthickness/cm)) + G4String("_PE") + G4String(to_string(pethickness/cm)) + G4String("_");
            filename = output_dir + folder + "/" + title_prefix + G4String(buffer) + index + ".root";
            G4cout << "SIMULATION MODE 1: RATE SIMULATION" << G4endl;
            break;
        case 2:
            if (folder == "default") {folder = "capture_sim";}
            filename = output_dir + folder + "/simdata_" + G4String(buffer) + index + ".root";
            G4cout << "SIMULATION MODE 2: CAPTURE SIMULATION" << G4endl;
            break;
        case 3:
            // ./doublecap vis100.mac -n 1 -s 3
            if (folder == "default") {folder = "neutron_sim";}
            filename = output_dir + folder + "/neutronHPT_" + G4String(buffer) + index + ".root";
            G4cout << "SIMULATION MODE 3: NEUTRON SIMULATION" << G4endl;
            break;
        default:
            G4cerr << "Error. Unknown simulation mode." << G4endl;
            return 1;
    }
    G4cout << "Writing data to " << filename << G4endl;

    auto *runmgr = G4RunManagerFactory::CreateRunManager();
    runmgr->SetNumberOfThreads(NUMBEROFTHREADS);
    G4PhysListFactory *factory = new G4PhysListFactory;
    G4VisManager* vismgr = new G4VisExecutive;

    // geometry
    G4double lm_face = 1.*cm; // width of lowmass
    G4double lm_thick = 4.*mm; // thickness of lowmass 
    // changed to 4 mm 20250226_114500

    G4double hm_diameter = 7.5*cm; // diameter of highmasses
    G4double hm_thick = 2.5*cm; // thickness of highmasses

    G4double airgap = 0.1*mm; // airgap between detector and surrounding copper
    G4double cu_thick = 0.2*mm; // thickness of copper boxes
    G4double spacing = 1.5*mm; // spacing between silicon detectors

    G4double leadradius = 3.*cm; // radius of cylindrical lead shield
    G4double sourcesize = 1.*mm; // side length of Cf cube

    G4int Z = 98, A = 252; // Cf-252 calibration source

    DCGeometry *geometry = new DCGeometry(lm_face, lm_thick, hm_diameter, hm_thick, cu_thick, spacing, airgap, pethickness, leadthickness, leadradius, sourcesize);

    // z position of center of source block
    G4double sourcez = geometry->GetSourceZ();

    G4cout << "sourcez = " << sourcez/cm << " cm" << G4endl;

    runmgr->SetUserInitialization(factory->GetReferencePhysList("Shielding_HPT"));
    runmgr->SetUserInitialization(new DCInitialization(Z, A, geometry, SIMULATIONMODE, filename, KILLGAMMAS));
    runmgr->SetUserInitialization(geometry);

    runmgr->Initialize();
    vismgr->Initialize();

    // set to have correct gamma spectrum
    G4ParticleHPManager::GetInstance()->SetUseOnlyPhotoEvaporation(true);
    G4ParticleHPManager::GetInstance()->SetDoNotAdjustFinalState(true);

    // set to get all radioactive decays
    G4HadronicParameters::Instance()->SetTimeThresholdForRadioactiveDecay(1.0e+6*year);

    // measure execution time not initialization time
    auto start = std::chrono::steady_clock::now();
    
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if (add_macro_path) {
        G4String command = "/control/macroPath ";
        UImanager->ApplyCommand(command + macro_path);
    }
    if ((argc > 1) && (argv[1][0] != '-')) {
        G4String macro = argv[1];
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    } else { // no macro - interactive mode
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





