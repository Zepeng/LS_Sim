#include "DsG4Scintillation.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "globals.hh"


///////////////////////////////////////////////////////////////////

using namespace std;

/////////////////////////
// Class Implementation  
/////////////////////////

//////////////
// Operators
//////////////

// DsG4Scintillation::operator=(const DsG4Scintillation &right)
// {
// }

/////////////////
// Constructors
/////////////////

DsG4Scintillation::DsG4Scintillation(const G4String& processName,
                                     G4ProcessType type)
    : G4VRestDiscreteProcess(processName, type)
    , doReemission(true)
    , doBothProcess(true)
    , doReemissionOnly(false)
    , fEnableQuenching(true)
    , slowerTimeConstant(0) , slowerRatio(0)
    , gammaSlowerTime(0) , gammaSlowerRatio(0)
    , flagDecayTimeFast(true), flagDecayTimeSlow(true)
    , fPhotonWeight(1.0)
    , m_noop(false)
{
    SetProcessSubType(fScintillation);
    fTrackSecondariesFirst = false;

    YieldFactor = 1.0;
    ExcitationRatio = 1.0;

    theFastIntegralTable = NULL;
    theSlowIntegralTable = NULL;
    theReemissionIntegralTable = NULL;


    BuildThePhysicsTable();

    // FORCE reemission only
    doReemissionOnly = true;

}

////////////////
// Destructors
////////////////

DsG4Scintillation::~DsG4Scintillation() 
{
    if (theFastIntegralTable != NULL) {
        theFastIntegralTable->clearAndDestroy();
        delete theFastIntegralTable;
    }
    if (theSlowIntegralTable != NULL) {
        theSlowIntegralTable->clearAndDestroy();
        delete theSlowIntegralTable;
    }
    if (theReemissionIntegralTable != NULL) {
        theReemissionIntegralTable->clearAndDestroy();
        delete theReemissionIntegralTable;
    }
}

////////////
// Methods
////////////

// AtRestDoIt
// ----------
//
G4VParticleChange*
DsG4Scintillation::AtRestDoIt(const G4Track& aTrack, const G4Step& aStep)

// This routine simply calls the equivalent PostStepDoIt since all the
// necessary information resides in aStep.GetTotalEnergyDeposit()

{
    return DsG4Scintillation::PostStepDoIt(aTrack, aStep);
}

// PostStepDoIt
// -------------
//
G4VParticleChange*
DsG4Scintillation::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)

// This routine is called for each tracking step of a charged particle
// in a scintillator. A Poisson/Gauss-distributed number of photons is 
// generated according to the scintillation yield formula, distributed 
// evenly along the track segment and uniformly into 4pi.

{
    aParticleChange.Initialize(aTrack);

    if (m_noop) {               // do nothing, bail
        aParticleChange.SetNumberOfSecondaries(0);
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }


    G4String pname="";
    G4ThreeVector vertpos;
    G4double vertenergy=0.0;
    G4double reem_d=0.0;
    G4bool flagReemission= false;
    //DsPhotonTrackInfo* reemittedTI=0;
    if (aTrack.GetDefinition() == G4OpticalPhoton::OpticalPhoton()) {
        G4Track *track=aStep.GetTrack();
        //G4CompositeTrackInfo* composite=dynamic_cast<G4CompositeTrackInfo*>(track->GetUserInformation());
        //reemittedTI = composite?dynamic_cast<DsPhotonTrackInfo*>( composite->GetPhotonTrackInfo() ):0;
        
        const G4VProcess* process = track->GetCreatorProcess();
        if(process) pname = process->GetProcessName();

        if(doBothProcess) {
            flagReemission= doReemission
                && aTrack.GetTrackStatus() == fStopAndKill
                && aStep.GetPostStepPoint()->GetStepStatus() != fGeomBoundary;     
        }
        else{
            flagReemission= doReemission
                && aTrack.GetTrackStatus() == fStopAndKill
                && aStep.GetPostStepPoint()->GetStepStatus() != fGeomBoundary
                && pname=="Cerenkov";
        }
        if (!flagReemission) {
            return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        }
    }

    G4double TotalEnergyDeposit = aStep.GetTotalEnergyDeposit();
    if (TotalEnergyDeposit <= 0.0 && !flagReemission) {
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();
    const G4String aParticleName = aParticle->GetDefinition()->GetParticleName();
    const G4Material* aMaterial = aTrack.GetMaterial();

    G4MaterialPropertiesTable* aMaterialPropertiesTable =
        aMaterial->GetMaterialPropertiesTable();

    //aMaterialPropertiesTable-> DumpTable();

    if (!aMaterialPropertiesTable)
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);

    G4String FastTimeConstant = "FASTTIMECONSTANT";
    G4String SlowTimeConstant = "SLOWTIMECONSTANT";
    G4String strYieldRatio = "YIELDRATIO";

    // reset the slower time constant and ratio
    slowerTimeConstant = 0.0;
    slowerRatio = 0.0;
    
    if (aParticleName == "opticalphoton") {
      FastTimeConstant = "ReemissionFASTTIMECONSTANT";
      SlowTimeConstant = "ReemissionSLOWTIMECONSTANT";
      strYieldRatio = "ReemissionYIELDRATIO";
    }
    else if(aParticleName == "gamma" || aParticleName == "e+" || aParticleName == "e-") {
      FastTimeConstant = "GammaFASTTIMECONSTANT";
      SlowTimeConstant = "GammaSLOWTIMECONSTANT";
      strYieldRatio = "GammaYIELDRATIO";
      slowerTimeConstant = gammaSlowerTime;
      slowerRatio = gammaSlowerRatio;
    }
    //else if(aParticleName == "alpha") {
    //  FastTimeConstant = "AlphaFASTTIMECONSTANT";
    //  SlowTimeConstant = "AlphaSLOWTIMECONSTANT";
    //  strYieldRatio = "AlphaYIELDRATIO";
    //  slowerTimeConstant = alphaSlowerTime;
    //  slowerRatio = alphaSlowerRatio;
    //}
    //else {
    //  FastTimeConstant = "NeutronFASTTIMECONSTANT";
    //  SlowTimeConstant = "NeutronSLOWTIMECONSTANT";
    //  strYieldRatio = "NeutronYIELDRATIO";
    //  slowerTimeConstant = neutronSlowerTime;
    //  slowerRatio = neutronSlowerRatio;
    //}

    const G4MaterialPropertyVector* Fast_Intensity = 
        aMaterialPropertiesTable->GetProperty("FASTCOMPONENT"); 
    const G4MaterialPropertyVector* Slow_Intensity =
        aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");
    const G4MaterialPropertyVector* Reemission_Prob =
        aMaterialPropertiesTable->GetProperty("REEMISSIONPROB");
    if (!Fast_Intensity && !Slow_Intensity )
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);

    G4int nscnt = 1;
    if (Fast_Intensity && Slow_Intensity) nscnt = 2;
    G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
    G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();

    G4ThreeVector x0 = pPreStepPoint->GetPosition();
    G4ThreeVector p0 = aStep.GetDeltaPosition().unit();
    G4double      t0 = pPreStepPoint->GetGlobalTime();

    //Replace NumPhotons by NumTracks
    G4int NumTracks=0;
    G4double weight=1.0;
    if (flagReemission) {   
        
        if ( Reemission_Prob == 0)
            return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        G4double p_reemission=
            Reemission_Prob->Value(aTrack.GetKineticEnergy());
        if (G4UniformRand() >= p_reemission)
            return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
        NumTracks= 1;
        weight= aTrack.GetWeight();
    }
    else {
        //////////////////////////////////// Birks' law ////////////////////////
        // J.B.Birks. The theory and practice of Scintillation Counting. 
        // Pergamon Press, 1964.      
        // For particles with energy much smaller than minimum ionization 
        // energy, the scintillation response is non-linear because of quenching  
        // effect. The light output is reduced by a parametric factor: 
        // 1/(1 + birk1*delta + birk2* delta^2). 
        // Delta is the energy loss per unit mass thickness. birk1 and birk2 
        // were measured for several organic scintillators.         
        // Here we use birk1 = 0.0125*g/cm2/MeV and ignore birk2.               
        // R.L.Craun and D.L.Smith. Nucl. Inst. and Meth., 80:239-244, 1970.   
        // Liang Zhan  01/27/2006 
        // /////////////////////////////////////////////////////////////////////
        
        
        G4double ScintillationYield = 0;
        {// Yield.  Material must have this or we lack raisins dayetras
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty("SCINTILLATIONYIELD");
            if (!ptable) {
                G4cout << "ConstProperty: failed to get SCINTILLATIONYIELD"
                       << G4endl;
                return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
            }
            ScintillationYield = ptable->Value(0);
        }

        G4double ResolutionScale    = 1;
        {// Resolution Scale
            const G4MaterialPropertyVector* ptable =
                aMaterialPropertiesTable->GetProperty("RESOLUTIONSCALE");
            if (ptable)
                ResolutionScale = ptable->Value(0);
        }

        G4double dE = TotalEnergyDeposit;
        G4double dx = aStep.GetStepLength();
        G4double dE_dx = dE/dx;
        if(aTrack.GetDefinition() == G4Gamma::Gamma() && dE > 0)
        { 
          G4LossTableManager* manager = G4LossTableManager::Instance();
          dE_dx = dE/manager->GetRange(G4Electron::Electron(), dE, aTrack.GetMaterialCutsCouple());
          //G4cout<<"gamma dE_dx = "<<dE_dx/(MeV/mm)<<"MeV/mm"<<G4endl;
        }
        
        G4double delta = dE_dx/aMaterial->GetDensity();//get scintillator density 
        //G4double birk1 = 0.0125*g/cm2/MeV;
        G4double birk1 = birksConstant1;
        if(abs(aParticle->GetCharge())>1.5)//for particle charge greater than 1.
            birk1 = 0.57*birk1;
        
        G4double birk2 = 0;
        //birk2 = (0.0031*g/MeV/cm2)*(0.0031*g/MeV/cm2);
        birk2 = birksConstant2;
        
        G4double QuenchedTotalEnergyDeposit = TotalEnergyDeposit;
        // if quenching is enabled, apply the birks law
        if (fEnableQuenching) {
            QuenchedTotalEnergyDeposit
            = TotalEnergyDeposit/(1+birk1*delta+birk2*delta*delta);
        }

        ScintillationYield = YieldFactor * ScintillationYield; 

        G4double MeanNumberOfPhotons= ScintillationYield * QuenchedTotalEnergyDeposit;
   
        // Implemented the fast simulation method from GLG4Scint
        // Jianglai 09-05-2006
        
        // randomize number of TRACKS (not photons)
        // this gets statistics right for number of PE after applying
        // boolean random choice to final absorbed track (change from
        // old method of applying binomial random choice to final absorbed
        // track, which did want poissonian number of photons divided
        // as evenly as possible into tracks)
        // Note for weight=1, there's no difference between tracks and photons.
        G4double MeanNumberOfTracks= MeanNumberOfPhotons/fPhotonWeight; 
        if (MeanNumberOfTracks > 10.) {
            G4double sigma = ResolutionScale * sqrt(MeanNumberOfTracks);
            NumTracks = G4int(G4RandGauss::shoot(MeanNumberOfTracks,sigma)+0.5);
        }
        else {
            NumTracks = G4int(G4Poisson(MeanNumberOfTracks));
        }
    }
    weight*=fPhotonWeight;
    if (NumTracks <= 0) {
        // return unchanged particle and no secondaries 
        aParticleChange.SetNumberOfSecondaries(0);
        return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
    }

    ////////////////////////////////////////////////////////////////

    aParticleChange.SetNumberOfSecondaries(NumTracks);

    if (fTrackSecondariesFirst) {
        if (!flagReemission) 
            if (aTrack.GetTrackStatus() == fAlive )
                aParticleChange.ProposeTrackStatus(fSuspend);
    }
        
    ////////////////////////////////////////////////////////////////

    G4int materialIndex = aMaterial->GetIndex();

    G4PhysicsOrderedFreeVector* ReemissionIntegral = NULL;
    ReemissionIntegral =
        (G4PhysicsOrderedFreeVector*)((*theReemissionIntegralTable)(materialIndex));

    // Retrieve the Scintillation Integral for this material  
    // new G4PhysicsOrderedFreeVector allocated to hold CII's

    G4int Num = NumTracks; //# tracks is now the loop control
        
    G4double fastTimeConstant = 0.0;
    if (flagDecayTimeFast) { // Fast Time Constant
        const G4MaterialPropertyVector* ptable =
        aMaterialPropertiesTable->GetProperty(FastTimeConstant.c_str());
        if (!ptable) ptable = aMaterialPropertiesTable->GetProperty("FASTTIMECONSTANT");
        if (ptable) {
            fastTimeConstant = ptable->Value(0);
        }
    }

    G4double slowTimeConstant = 0.0;
    if (flagDecayTimeSlow) { // Slow Time Constant
        const G4MaterialPropertyVector* ptable =
        aMaterialPropertiesTable->GetProperty(SlowTimeConstant.c_str());
        if(!ptable) ptable = aMaterialPropertiesTable->GetProperty("SLOWTIMECONSTANT");
        if (ptable){
          slowTimeConstant = ptable->Value(0);
        }
    }

    G4double YieldRatio = 0.0;
    { // Slow Time Constant
        const G4MaterialPropertyVector* ptable =
            aMaterialPropertiesTable->GetProperty(strYieldRatio.c_str());
        if(!ptable) ptable = aMaterialPropertiesTable->GetProperty("YIELDRATIO");
        if (ptable) {
            YieldRatio = ptable->Value(0);
        }
    }


    //loop over fast/slow scintillations
    for (G4int scnt = 1; scnt <= nscnt; scnt++) {

        G4double ScintillationTime = 0.*ns;
        G4PhysicsOrderedFreeVector* ScintillationIntegral = NULL;

        if (scnt == 1) {//fast
            if (nscnt == 1) {
                if(Fast_Intensity){
                    ScintillationTime   = fastTimeConstant;
                    ScintillationIntegral =
                        (G4PhysicsOrderedFreeVector*)((*theFastIntegralTable)(materialIndex));
                }
                if(Slow_Intensity){
                    ScintillationTime   = slowTimeConstant;
                    ScintillationIntegral =
                        (G4PhysicsOrderedFreeVector*)((*theSlowIntegralTable)(materialIndex));
                }
            }
            else {
                if ( ExcitationRatio == 1.0 ) {
                  Num = G4int( 0.5 +  (min(YieldRatio,1.0) * NumTracks) );  // round off, not truncation
                }
                else {
                  Num = G4int( 0.5 +  (min(ExcitationRatio,1.0) * NumTracks));
                }
                ScintillationTime   = fastTimeConstant;
                ScintillationIntegral =
                    (G4PhysicsOrderedFreeVector*)((*theFastIntegralTable)(materialIndex));
            }
        }
        else {//slow
            Num = NumTracks - Num;
            ScintillationTime   =   slowTimeConstant;
            ScintillationIntegral =
                (G4PhysicsOrderedFreeVector*)((*theSlowIntegralTable)(materialIndex));
        }

        if (!ScintillationIntegral) continue;
        
        // Max Scintillation Integral

        for (G4int i = 0; i < Num; i++) { //Num is # of 2ndary tracks now
            // Determine photon energy

        if(scnt == 2) {
            ScintillationTime = slowTimeConstant;
            if(flagDecayTimeSlow && G4UniformRand() < slowerRatio && (!flagReemission)) ScintillationTime = slowerTimeConstant;
        }

            G4double sampledEnergy;
            if ( !flagReemission ) {
                // normal scintillation
                G4double CIIvalue = G4UniformRand()*
                    ScintillationIntegral->GetMaxValue();
                sampledEnergy=
                    ScintillationIntegral->GetEnergy(CIIvalue);

            }
            else {
                // reemission, the sample method need modification
                G4double CIIvalue = G4UniformRand()*
                    ScintillationIntegral->GetMaxValue();
                if (CIIvalue == 0.0) {
                    // return unchanged particle and no secondaries  
                    aParticleChange.SetNumberOfSecondaries(0);
                    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
                }
                sampledEnergy=
                    ScintillationIntegral->GetEnergy(CIIvalue);
            }

            // Generate random photon direction

            G4double cost = 1. - 2.*G4UniformRand();
            G4double sint = sqrt((1.-cost)*(1.+cost));

            G4double phi = twopi*G4UniformRand();
            G4double sinp = sin(phi);
            G4double cosp = cos(phi);

            G4double px = sint*cosp;
            G4double py = sint*sinp;
            G4double pz = cost;

            // Create photon momentum direction vector 

            G4ParticleMomentum photonMomentum(px, py, pz);

            // Determine polarization of new photon 

            G4double sx = cost*cosp;
            G4double sy = cost*sinp; 
            G4double sz = -sint;

            G4ThreeVector photonPolarization(sx, sy, sz);

            G4ThreeVector perp = photonMomentum.cross(photonPolarization);

            phi = twopi*G4UniformRand();
            sinp = sin(phi);
            cosp = cos(phi);

            photonPolarization = cosp * photonPolarization + sinp * perp;

            photonPolarization = photonPolarization.unit();

            // Generate a new photon:

            G4DynamicParticle* aScintillationPhoton =
                new G4DynamicParticle(G4OpticalPhoton::OpticalPhoton(), 
                                      photonMomentum);
            aScintillationPhoton->SetPolarization
                (photonPolarization.x(),
                 photonPolarization.y(),
                 photonPolarization.z());

            aScintillationPhoton->SetKineticEnergy(sampledEnergy);

            // Generate new G4Track object:

            G4double rand=0;
            G4ThreeVector aSecondaryPosition;
            G4double deltaTime;
            if (flagReemission) {
                deltaTime= pPostStepPoint->GetGlobalTime() - t0 
                           -ScintillationTime * log( G4UniformRand() );
                aSecondaryPosition= pPostStepPoint->GetPosition();
                vertpos = aTrack.GetVertexPosition();
                vertenergy = aTrack.GetKineticEnergy();
                reem_d = 
                    sqrt( pow( aSecondaryPosition.x()-vertpos.x(), 2)
                          + pow( aSecondaryPosition.y()-vertpos.y(), 2)
                          + pow( aSecondaryPosition.z()-vertpos.z(), 2) );
            }
            else {
                if (aParticle->GetDefinition()->GetPDGCharge() != 0) 
                    {
                        rand = G4UniformRand();
                    }
                else
                    {
                        rand = 1.0;
                    }

                G4double delta = rand * aStep.GetStepLength();
                deltaTime = delta /
                    ((pPreStepPoint->GetVelocity()+
                      pPostStepPoint->GetVelocity())/2.);

                deltaTime = deltaTime - 
                    ScintillationTime * log( G4UniformRand() );

                aSecondaryPosition =
                    x0 + rand * aStep.GetDeltaPosition();
            }
            G4double aSecondaryTime = t0 + deltaTime;
            G4Track* aSecondaryTrack = 
                new G4Track(aScintillationPhoton,aSecondaryTime,aSecondaryPosition);

                
            aSecondaryTrack->SetWeight( weight );
            aSecondaryTrack->SetTouchableHandle(aStep.GetPreStepPoint()->GetTouchableHandle());
            // aSecondaryTrack->SetTouchableHandle((G4VTouchable*)0);//this is wrong
                
            aSecondaryTrack->SetParentID(aTrack.GetTrackID());
                
            // add the secondary to the ParticleChange object
            aParticleChange.SetSecondaryWeightByProcess( true ); // recommended
            aParticleChange.AddSecondary(aSecondaryTrack);
                
            aSecondaryTrack->SetWeight( weight );
        }
    } // end loop over fast/slow scints

    return G4VRestDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

// BuildThePhysicsTable for the scintillation process
// --------------------------------------------------
//

void DsG4Scintillation::BuildThePhysicsTable()
{
    if (theFastIntegralTable && theSlowIntegralTable && theReemissionIntegralTable) return;

    const G4MaterialTable* theMaterialTable = 
        G4Material::GetMaterialTable();
    G4int numOfMaterials = G4Material::GetNumberOfMaterials();

    // create new physics table
    if(!theFastIntegralTable)theFastIntegralTable = new G4PhysicsTable(numOfMaterials);
    if(!theSlowIntegralTable)theSlowIntegralTable = new G4PhysicsTable(numOfMaterials);
    if(!theReemissionIntegralTable)theReemissionIntegralTable
                                       = new G4PhysicsTable(numOfMaterials);
    // loop for materials

    for (G4int i=0 ; i < numOfMaterials; i++) {
        G4PhysicsOrderedFreeVector* aPhysicsOrderedFreeVector =
            new G4PhysicsOrderedFreeVector();
        G4PhysicsOrderedFreeVector* bPhysicsOrderedFreeVector =
            new G4PhysicsOrderedFreeVector();
        G4PhysicsOrderedFreeVector* cPhysicsOrderedFreeVector =
            new G4PhysicsOrderedFreeVector();

        // Retrieve vector of scintillation wavelength intensity for
        // the material from the material's optical properties table.

        G4Material* aMaterial = (*theMaterialTable)[i];

        G4MaterialPropertiesTable* aMaterialPropertiesTable =
            aMaterial->GetMaterialPropertiesTable();

        if (aMaterialPropertiesTable) {

            G4MaterialPropertyVector* theFastLightVector = 
                aMaterialPropertiesTable->GetProperty("FASTCOMPONENT");

            if (theFastLightVector) {
                // Retrieve the first intensity point in vector
                // of (photon energy, intensity) pairs 

                G4double currentIN = (*theFastLightVector)[0];

                if (currentIN >= 0.0) {

                    // Create first (photon energy, Scintillation 
                    // Integral pair  

                    G4double currentPM = theFastLightVector->
                        Energy(0);

                    G4double currentCII = 0.0;

                    aPhysicsOrderedFreeVector->
                        InsertValues(currentPM , currentCII);

                    // Set previous values to current ones prior to loop

                    G4double prevPM  = currentPM;
                    G4double prevCII = currentCII;
                    G4double prevIN  = currentIN;

                    // loop over all (photon energy, intensity)
                    // pairs stored for this material  

                    for(size_t ii = 1;
                              ii < theFastLightVector->GetVectorLength();
                              ++ii) 
                    {
                        currentPM = theFastLightVector->Energy(ii);

                        currentIN= (*theFastLightVector)[ii];

                        currentCII = 0.5 * (prevIN + currentIN);

                        currentCII = prevCII +
                            (currentPM - prevPM) * currentCII;

                        aPhysicsOrderedFreeVector->
                            InsertValues(currentPM, currentCII);

                        prevPM  = currentPM;
                        prevCII = currentCII;
                        prevIN  = currentIN;
                    }

                }
            }

            G4MaterialPropertyVector* theSlowLightVector =
                aMaterialPropertiesTable->GetProperty("SLOWCOMPONENT");

            if (theSlowLightVector) {

                // Retrieve the first intensity point in vector
                // of (photon energy, intensity) pairs

                G4double currentIN = (*theSlowLightVector)[0];

                if (currentIN >= 0.0) {

                    // Create first (photon energy, Scintillation
                    // Integral pair

                    G4double currentPM = theSlowLightVector->Energy(0);

                    G4double currentCII = 0.0;

                    bPhysicsOrderedFreeVector->
                        InsertValues(currentPM , currentCII);

                    // Set previous values to current ones prior to loop

                    G4double prevPM  = currentPM;
                    G4double prevCII = currentCII;
                    G4double prevIN  = currentIN;

                    // loop over all (photon energy, intensity)
                    // pairs stored for this material

                    for (size_t ii = 1;
                         ii < theSlowLightVector->GetVectorLength();
                         ++ii)
                    {
                        currentPM = theSlowLightVector->Energy(ii);

                        currentIN = (*theSlowLightVector)[ii];

                        currentCII = 0.5 * (prevIN + currentIN);

                        currentCII = prevCII +
                            (currentPM - prevPM) * currentCII;

                        bPhysicsOrderedFreeVector->
                            InsertValues(currentPM, currentCII);

                        prevPM  = currentPM;
                        prevCII = currentCII;
                        prevIN  = currentIN;
                    }

                }
            }

            G4MaterialPropertyVector* theReemissionVector =
                aMaterialPropertiesTable->GetProperty("REEMISSIONPROB");

            if (theReemissionVector) {

                // Retrieve the first intensity point in vector
                // of (photon energy, intensity) pairs

              G4double currentIN = (*theReemissionVector)[0];

                if (currentIN >= 0.0) {

                    // Create first (photon energy, Scintillation
                    // Integral pair

                    G4double currentPM = theReemissionVector->Energy(0);

                    G4double currentCII = 0.0;

                    cPhysicsOrderedFreeVector->
                        InsertValues(currentPM , currentCII);

                    // Set previous values to current ones prior to loop

                    G4double prevPM  = currentPM;
                    G4double prevCII = currentCII;
                    G4double prevIN  = currentIN;

                    // loop over all (photon energy, intensity)
                    // pairs stored for this material

                    for (size_t ii = 1;
                         ii < theReemissionVector->GetVectorLength();
                         ++ii)
                    {

                        currentPM = theReemissionVector->Energy(ii);

                        currentIN = (*theReemissionVector)[ii];

                        currentCII = 0.5 * (prevIN + currentIN);

                        currentCII = prevCII +
                            (currentPM - prevPM) * currentCII;

                        cPhysicsOrderedFreeVector->
                            InsertValues(currentPM, currentCII);

                        prevPM  = currentPM;
                        prevCII = currentCII;
                        prevIN  = currentIN;
                    }

                }
            }

        }

        // The scintillation integral(s) for a given material
        // will be inserted in the table(s) according to the
        // position of the material in the material table.

        theFastIntegralTable->insertAt(i,aPhysicsOrderedFreeVector);
        theSlowIntegralTable->insertAt(i,bPhysicsOrderedFreeVector);
        theReemissionIntegralTable->insertAt(i,cPhysicsOrderedFreeVector);
    }
}

// GetMeanFreePath
// ---------------
//

G4double DsG4Scintillation::GetMeanFreePath(const G4Track&,
                                            G4double ,
                                            G4ForceCondition* condition)
{
    *condition = StronglyForced;

    return DBL_MAX;

}

// GetMeanLifeTime
// ---------------
//

G4double DsG4Scintillation::GetMeanLifeTime(const G4Track&,
                                            G4ForceCondition* condition)
{
    *condition = Forced;

    return DBL_MAX;

}

// OP simulator
G4PhysicsTable* DsG4Scintillation::getSlowIntegralTable() {
    return theSlowIntegralTable;
}
G4PhysicsTable* DsG4Scintillation::getFastIntegralTable() {
    return theFastIntegralTable;
}
G4PhysicsTable* DsG4Scintillation::getReemissionIntegralTable() {
    return theReemissionIntegralTable;
}
