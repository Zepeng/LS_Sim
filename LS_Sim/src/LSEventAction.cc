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
 m_end_t(0),
 m_opticksMode(0)
{
 
	//LSOpticksEventConfigMessenger* mes = LSOpticksEventConfigMessenger::Get();
	//SetOpticksMode(mes->GetOpticksMode());

	
	m_opticksMode = std::atoi(getenv("LS_OPTICKS_MODE"));

    G4cout << " Opticks Mode "<<m_opticksMode 
			<< G4endl;
}

LSEventAction::~LSEventAction()
{;}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void LSEventAction::SetOpticksMode(int mode){
	m_opticksMode = mode;
}

void LSEventAction::BeginOfEventAction(const G4Event* evt)
{
	
	//LSOpticksEventConfigMessenger* mes = LSOpticksEventConfigMessenger::Get();
	//assert(mes);
	//SetOpticksMode(mes->GetOpticksMode());
	m_start_t = clock();
	
    G4cout<<"begin of event "<<evt->GetEventID()<<G4endl;

	MyAnalysisManager::GetInstance()->SetOpticksMode(m_opticksMode);
    MyAnalysisManager::GetInstance()->BeginOfEventAction(evt);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LSEventAction::EndOfEventAction(const G4Event* event)
{

    m_end_t = clock();
	double run_time_other_physic = static_cast<double>(m_end_t-m_start_t)/CLOCKS_PER_SEC;
	G4cout<<" detsim time of other "<< run_time_other_physic <<" s" <<G4endl;
	
#ifdef WITH_G4CXOPTICKS
	LOG(info)<< "LSEventAction::EndOfEventAction mode = "<<m_opticksMode;
	if( m_opticksMode & 1 ){

   		 G4CXOpticks* gx = G4CXOpticks::Get();
   		 LOG(info)<<"gx->simulate()";
   		 LOG(info)<< gx->desc();
   		 gx->simulate();
   		 cudaDeviceSynchronize();
   		 //gx->saveEvent();
		 //SEvt::Clear();
	}
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
