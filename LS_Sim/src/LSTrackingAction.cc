#include "MyAnalysisManager.hh"
#include "LSTrackingAction.hh"
#include "NormalTrackInfo.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"
#include "G4OpticalPhoton.hh"

LSTrackingAction::LSTrackingAction()
    : G4UserTrackingAction()
{}

LSTrackingAction::~LSTrackingAction()
{}

void LSTrackingAction::PreUserTrackingAction( const G4Track* aTrack )
{

    if (aTrack->GetParentID() == 1) // primary e+-
    {
        if (aTrack->GetParticleDefinition()->GetParticleName() == "e+") {
            //G4cout << aTrack->GetKineticEnergy() << "a ";
        }
        if (aTrack->GetParticleDefinition()->GetParticleName() == "e-") {
            //G4cout << aTrack->GetKineticEnergy() << "b ";
        }
    }

    if(aTrack->GetParentID()==0 and aTrack->GetUserInformation()==0) {
        NormalTrackInfo* anInfo = new NormalTrackInfo(aTrack);
        G4Track* theTrack = (G4Track*)aTrack;
        theTrack->SetUserInformation(anInfo);
    }

    MyAnalysisManager::GetInstance()->PreTrackingAction(aTrack);

}

void LSTrackingAction::PostUserTrackingAction ( const G4Track* aTrack ) 
{
    G4TrackingManager* tm = G4EventManager::GetEventManager() 
                                            -> GetTrackingManager();
    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(secondaries)
    {
        NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());

        if (!info) {
             return;
        }

        size_t nSeco = secondaries->size();
        if(nSeco>0)
        {
            // loop over all secondaries :
            for(size_t i=0;i<nSeco;i++)
            { 
                //NormalTrackInfo* infoNew = new NormalTrackInfo();
                NormalTrackInfo* infoNew = new NormalTrackInfo(info);

                // cerenkov photons tag
                if((*secondaries)[i]->GetCreatorProcess() 
                   and (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()
                   and (*secondaries)[i]->GetCreatorProcess()->GetProcessName() == "Cerenkov" ) {
                    infoNew->setFromCerenkov();
                }

                // reemission tag
                // + parent track is an OP
                // + secondary is also an OP
                // + the creator process is Scintillation
                if (//aTrack->GetDefinition()==G4OpticalPhoton::Definition() and
                    (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()
                    and (*secondaries)[i]->GetCreatorProcess()->GetProcessName() == "Scintillation") {
                    infoNew->setReemission();
                }

                // original photons tag
                if (aTrack->GetDefinition() != G4OpticalPhoton::Definition()
                    and (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()) {
                    infoNew->setOriginalOP();
                }

                (*secondaries)[i] -> SetUserInformation(infoNew);
            }
        }
    } 
    
    // outputs:
    //if (aTrack->GetDefinition() == G4OpticalPhoton::Definition()) {
    //    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation()) ;
    //    G4cout << "PostTrackId = " << aTrack->GetTrackID() << " and parent ID = " << aTrack->GetParentID() 
    //           << " creator process is " << aTrack->GetCreatorProcess()->GetProcessName()
    //           << " is Original Photon ---> " << info->isOriginalOP() 
    //           << " is Cerenkov Photon ---> " << info->isFromCerenkov()
    //           << " is Scintillation Photon ---> " << info->isReemission()
    //           << G4endl;
    //}

    MyAnalysisManager::GetInstance()->PostTrackingAction(aTrack);

}
