#include "LSEventAction.hh"
#include "LSAnalysisManager.hh"
#include "MyAnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "LSOpticksEventConfigMessenger.hh"
//#include <assert>
#ifdef WITH_G4CXOPTICKS
#include "PLOG.hh"
#include "G4CXOpticks.hh"
#include "SEvt.hh"
#include <cuda_runtime.h>
#endif

#include <ctime>

LSEventAction::LSEventAction()
:
 G4UserEventAction(),
 m_start_t(0),
 m_end_t(0)
{
}

LSEventAction::~LSEventAction()
{;}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void LSEventAction::BeginOfEventAction(const G4Event* evt)
{
    m_start_t = clock();
    G4cout<<"begin of event "<<evt->GetEventID()<<G4endl;
    MyAnalysisManager::GetInstance()->BeginOfEventAction(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSEventAction::EndOfEventAction(const G4Event* event)
{

    m_end_t = clock();
    double run_time_other_physic = static_cast<double>(m_end_t-m_start_t)/CLOCKS_PER_SEC;
    G4cout<<" detsim time of other "<< run_time_other_physic <<" s" <<G4endl;
	
#ifdef WITH_G4CXOPTICKS
    G4int inum_photon = SEvt::GetNumPhotonFromGenstep(0);
    G4int eventid = event->GetEventID();
    //G4CXOpticks* gx = G4CXOpticks::Get();
    LOG(info)<<"gx->simulate()";
    //LOG(info)<< gx->desc();
    //gx->simulate(event->GetEventID(), false);
    std::cout << "SimEventID" << eventid << std::endl;
    G4CXOpticks::Get()->simulate(eventid, false);
    cudaDeviceSynchronize();
    SEvt* sev             = SEvt::Get_EGPU();
    unsigned int num_hits = sev->GetNumHit(0);
    //unsigned int num_hits = SEvt::GetNumHit(0);
    std::cout << "MCEventAction: GetNumPhotonFromGenstep: " << inum_photon << std::endl;
    // std::cout << "MCEventAction: GetNumGenstepFromGenstep: " << inum_genstep << std::endl;
    std::cout << "MCEventAction: NumHits:  " << num_hits << std::endl;
    if(num_hits > 0)
    {
        for(unsigned idx = 0 ; idx < num_hits ; idx++){

                        //U4Hit hit;
                        //U4HitGet::FromEvt(hit, idx );
                        sphoton hit;
                        sev->getHit(hit, idx);
        }
        G4HCtable* hctable = G4SDManager::GetSDMpointer()->GetHCtable();
        for(G4int i = 0; i < hctable->entries(); ++i)
        {
          std::string sdn   = hctable->GetSDname(i);
	  std::size_t found = sdn.find("detectorSD");
          if(found != std::string::npos)
          {
            //          std::cout << "Photondetector: " << sdn << std::endl;
            //PhotonSD* aSD = (PhotonSD*) G4SDManager::GetSDMpointer()->FindSensitiveDetector(sdn);
            //aSD->AddOpticksHits();
          }
        }
      }
   		 //gx->saveEvent();
		 //SEvt::Clear();
		 G4CXOpticks::Get()->reset(eventid);
#endif
    //
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
