#include "LSRunAction.hh"
#include "LSAnalysisManager.hh"
#include "MyAnalysisManager.hh"
#include "ContinuousSpectrumManager.hh"
#include "Randomize.hh"
#include <ctime>

#ifdef WITH_G4CXOPTICKS
#include "OPTICKS_LOG.hh"
#include "SEventConfig.hh"
#include "G4CXOpticks.hh"
#  include "G4TransportationManager.hh"
#endif


LSRunAction::LSRunAction()
: G4UserRunAction(), fSaveRndm(0), fAutoSeed(false)
{
}


LSRunAction::~LSRunAction()
{;}

void LSRunAction::BeginOfRunAction(const G4Run*)
{
    G4cout << "Begin of One Run" << G4endl;



    //G4CXOpticks gx ;
    //gx.setGeometry();

    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    analysis->book();

    if (ContinuousSpectrumManager::GetInstance()->GetActivate()) {
        ContinuousSpectrumManager::GetInstance()->LoadContinuousSpectrum();
    }

    MyAnalysisManager::GetInstance()->BeginOfRunAction();

    if (fAutoSeed) {
     // automatic (time-based) random seeds for each run
     G4cout << "*******************" << G4endl;
     G4cout << "*** AUTOSEED ON ***" << G4endl;
     G4cout << "*******************" << G4endl;
     long seeds[2];
     time_t systime = time(NULL);
     seeds[0] = (long) systime;
     seeds[1] = (long) (systime*G4UniformRand());
     G4Random::setTheSeeds(seeds);
     G4Random::showEngineStatus();
  } else {
     G4Random::showEngineStatus();
  }

  if (fSaveRndm > 0)
  {
     std::ostringstream os;
     os<<"beginOfRun_"<<fSaveRndm<<".rndm";
     G4Random::saveEngineStatus(os.str().c_str());
  }
  #ifdef WITH_G4CXOPTICKS
  if(LSOpticksEventConfigMessenger::GetInstance()->GetOpticksMode())
  {
      G4cout << "\n\n###[ RunAction::BeginOfRunAction\n\n" << G4endl;
      G4VPhysicalVolume* world = G4TransportationManager::GetTransportationManager()
                                   ->GetNavigatorForTracking()
                                   ->GetWorldVolume();
      assert(world);
      bool standardize_geant4_materials = false;  // required for alignment
      G4CXOpticks* g4ok                 = G4CXOpticks::Get();
      // g4ok->setGeometry(world, standardize_geant4_materials);
      // hjw            const std::vector<G4PVPlacement*>& sensor_placements =
      // hjw                    g4ok->getSensorPlacements();
      // hjw            G4cout << "sensor_placements.size():  " << sensor_placements.size()
      // hjw        << G4endl;
      /*
            for (unsigned i = 0; i < sensor_placements.size(); i++) {
                float efficiency_1 = 0.5f;
                float efficiency_2 = 1.0f;
                int sensor_cat = -1; // -1:means no angular info
                int sensor_identifier =
                        0xc0ffee + i; // mockup a detector specific identifier
                unsigned sensorIndex = 1 + i; // 1-based
                g4ok->setSensorData(sensorIndex, efficiency_1, efficiency_2, sensor_cat,
                        sensor_identifier);
            }
            G4cout << "\n\n###] RunAction::BeginOfRunAction G4CXOpticks.setGeometry\n\n"
                    << G4endl;
            geo_initialized = true;
      */
  }
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSRunAction::EndOfRunAction(const G4Run* )
{
    #ifdef WITH_G4CXOPTICKS
  if(LSOpticksEventConfigMessenger::GetInstance()->GetOpticksMode())
  {
    bool fDebug = true;
    if(fDebug)
    {
      G4cout << "\n\n###[ RunAction::EndOfRunAction G4CXOpticks.Finalize\n\n" << G4endl;
    }
    G4CXOpticks::Finalize();
    if(fDebug)
    {
      G4cout << "\n\n###] RunAction::EndOfRunAction G4CXOpticks.Finalize\n\n" << G4endl;
    }
  }
#endif
    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    analysis->finish();

    MyAnalysisManager::GetInstance()->EndOfRunAction();

    if (fSaveRndm == 1) {
     G4Random::showEngineStatus();
     std::ostringstream os;
     os<<"endOfRun_"<<fSaveRndm<<".rndm";
     G4Random::saveEngineStatus(os.str().c_str());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


