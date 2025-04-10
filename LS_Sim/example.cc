//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: exampleLS.cc 86065 2014-11-07 08:51:15Z gcosmo $
//
/// \file exampleLS.cc
/// \brief Main program of the LS example

#include "LSDetectorConstruction.hh"
#include "LSPhysicsList.hh"
#include "LSActionInitialization.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#ifdef WITH_G4CXOPTICKS
#include "OPTICKS_LOG.hh"
#include "G4CXOpticks.hh"
#include "SEventConfig.hh"
#endif

#include <ctime>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  clock_t start, end;
  start = clock(); 
  // Detect interactive mode (if no arguments) and define UI session
#ifdef WITH_G4CXOPTICKS
  OPTICKS_LOG(argc, argv);
  //SEventConfig::SetRGModeSimulate();
  //SEventConfig::SetStandardFullDebug(); // controls which and dimensions of SEvt arrays 
  //const char * mask = "genstep,photon,hit" ;
  const char * mask = "hit";
  //SEventConfig::SetCompMask(mask);
  SEventConfig::SetMaxGenstep(3000000);
  SEventConfig::SetMaxPhoton(70000000);
  std::cout << SEventConfig::Desc() << std::endl;
  //QRng::DEFAULT_PATH
  

#endif
  G4bool interactive   = false;
  G4String macrofile   = "";
  G4int opticksMode = 0;
  G4UIExecutive* ui    = nullptr;
  for(G4int i = 1; i < argc; i = i + 2)
  {
   if(G4String(argv[i]) == "-m")
    {
      macrofile = G4String(argv[i + 1]);
    }
    else if(G4String(argv[i]) == "-o")
    {
      opticksMode = std::atoi(argv[i + 1]);
    }
  }
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }
  
  //LOG(info) << " LOG(info) == > G4UIExecutive ";

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  
  // Construct the default run manager
  //
//#ifdef G4MULTITHREADED
    //G4MTRunManager* runManager = new G4MTRunManager;
//#else
  G4RunManager* runManager = new G4RunManager;
//#endif

  // Set mandatory initialization classes
  //
  // Detector construction
  LSDetectorConstruction* det = new LSDetectorConstruction();
  runManager->SetUserInitialization(det);

  // Physics list
  //auto physicsList = new QBBC;
  LSPhysicsList* physicsList = new LSPhysicsList();
  //auto physicsList = new FTFP_BERT;
  physicsList->SetVerboseLevel(0);
  physicsList->SetOpticksMode(opticksMode);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new LSActionInitialization());
  
  G4cout << "Finish ActionInitialization" << G4endl;
	

  // Initialize visualization
  //
  // G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  // visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  runManager->Initialize();

  // Process macro or start UI session
  //
  if ( ! ui ) { 
    // batch mode
    G4String command = "/control/execute ";
    //G4String fileName = argv[1];
    UImanager->ApplyCommand(command+macrofile);
  }
  else { 
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
	
  //LOG(info) << " end of  ";

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
  // delete visManager;
  delete runManager;
  end = clock();
  double run_time = static_cast<double>(end-start)/CLOCKS_PER_SEC;
  std::cout<<" run time = "<<run_time 
			<<" s "
			<<std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
