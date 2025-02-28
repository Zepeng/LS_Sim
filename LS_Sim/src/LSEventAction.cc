#include "LSEventAction.hh"
#include "LSAnalysisManager.hh"
#include "MyAnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "LSOpticksEventConfigMessenger.hh"
#include "LSDetectorSD.hh"
//#include <assert>
#ifdef WITH_G4CXOPTICKS
#include "PLOG.hh"
#include "G4CXOpticks.hh"
#include "SEvt.hh"
#include "NP.hh"
#include <cuda_runtime.h>
#endif

#include <ctime>

LSEventAction::LSEventAction()
:
 G4UserEventAction(),
 m_start_t(0),
 m_end_t(0),
 m_opticksMode(0)
{
 
}

LSEventAction::~LSEventAction()
{;}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void LSEventAction::SetOpticksMode(int mode){
	//m_opticksMode = mode;
}

void LSEventAction::BeginOfEventAction(const G4Event* evt)
{
	
	//LSOpticksEventConfigMessenger* mes = LSOpticksEventConfigMessenger::Get();
	//assert(mes);
	//SetOpticksMode(mes->GetOpticksMode());
	m_start_t = clock();
	
    G4cout<<"begin of event "<<evt->GetEventID()<<G4endl;

	//MyAnalysisManager::GetInstance()->SetOpticksMode(m_opticksMode);
    MyAnalysisManager::GetInstance()->BeginOfEventAction(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSEventAction::EndOfEventAction(const G4Event* event)
{
    m_end_t = clock();
    double run_time_other_physic = static_cast<double>(m_end_t-m_start_t)/CLOCKS_PER_SEC;
    G4cout<<" detsim time of other "<< run_time_other_physic <<" s" <<G4endl;
    #ifdef WITH_G4CXOPTICKS
    if(LSOpticksEventConfigMessenger::GetInstance()->GetOpticksMode())
    {
	bool fDebug = true;
	G4int inum_photon = SEvt::GetNumPhotonFromGenstep(0);
	if(fDebug){
      // G4int inum_photon          = SEvt::GetNumPhotonFromGenstep(0);
      G4int inum_genstep         = SEvt::GetNumGenstepFromGenstep(0);
      G4int num_PhotonCollected  = SEvt::GetNumPhotonCollected(0);
      G4int num_PhotonGenstepMax = SEvt::GetNumPhotonGenstepMax(0);
      G4int num_Hit              = SEvt::GetNumHit(0);
      std::cout << "------------------------------------------------------------------"
                << std::endl;
      std::cout << "MCEventAction: GetNumPhotonFromGenstep: " << inum_photon << std::endl;
      std::cout << "MCEventAction: GetNumGenstepFromGenstep: " << inum_genstep << std::endl;
      std::cout << "MCEventAction: GetNumPhotonCollected:  " << num_PhotonCollected << std::endl;
      std::cout << "MCEventAction: GetNumPhotonGenstepMax: " << num_PhotonGenstepMax << std::endl;
      std::cout << "MCEventAction: GetNumHit:            " << num_Hit << std::endl;
      std::cout << "------------------------------------------------------------------"
                << std::endl;
    }

    // cudaDeviceSynchronize();
    //         G4AutoLock lock(&opticks_mutex);
    //  G4RunManager* rm     = G4RunManager::GetRunManager();
    //  const G4Event* event = rm->GetCurrentEvent();
    // G4int eventid = event->GetEventID();
    if(inum_photon > 0)
    {
      G4int eventid = event->GetEventID();
      if(fDebug)
      {
        std::cout << "MCEventAction: Launch Opticks: " << std::endl;
      }
      G4CXOpticks::Get()->simulate(eventid, false);
      cudaDeviceSynchronize();
      unsigned int num_hits = SEvt::GetNumHit(0);
      if(fDebug)
      {
        std::cout << "MCEventAction: GetNumPhotonFromGenstep: " << inum_photon << std::endl;
        // std::cout << "MCEventAction: GetNumGenstepFromGenstep: " << inum_genstep << std::endl;
        std::cout << "MCEventAction: NumHits:  " << num_hits << std::endl;
      }
      if(num_hits > 0)
      {
        G4HCtable* hctable = G4SDManager::GetSDMpointer()->GetHCtable();
        for(G4int i = 0; i < hctable->entries(); ++i)
        {
          std::string sdn   = hctable->GetSDname(i);
          std::size_t found = sdn.find("detectorSD");
          if(found != std::string::npos)
          {
                      std::cout << "detectorSD: " << sdn << std::endl;
            LSDetectorSD* aSD = (LSDetectorSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(sdn);
            aSD->AddOpticksHits();
          }
        }
      }
      G4CXOpticks::Get()->reset(eventid);
    }
  }
#endif  //  WITH_G4CXOPTICKS
    
    m_end_t = clock();
    double run_time_wo_io = static_cast<double>(m_end_t-m_start_t)/CLOCKS_PER_SEC;
    G4cout<<" without io to detsim.root "<< run_time_wo_io <<" s" <<G4endl;
    LSAnalysisManager* analysis = LSAnalysisManager::getInstance();
    G4int evtId = event->GetEventID();
    analysis -> analyseEventID(evtId);

    analysis -> analyseAddNtupleRow();

    MyAnalysisManager::GetInstance()->EndOfEventAction(event);
    m_end_t = clock();
		
  	double run_time = static_cast<double>(m_end_t-m_start_t)/CLOCKS_PER_SEC;

    G4cout << "end of event "<<event->GetEventID()
			<< " time = "<<run_time
			<< " s" <<G4endl;
}
