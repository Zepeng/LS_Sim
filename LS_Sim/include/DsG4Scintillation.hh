#ifndef DsG4Scintillation_h
#define DsG4Scintillation_h 1

// Includes

#include "globals.hh"
#include "templates.hh"
#include "Randomize.hh"
#include "G4Poisson.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4Step.hh"
#include "G4VRestDiscreteProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4DynamicParticle.hh"
#include "G4Material.hh" 
#include "G4PhysicsTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4PhysicsOrderedFreeVector.hh"


class DsG4Scintillation : public G4VRestDiscreteProcess
{
    public:
        DsG4Scintillation(const G4String& processName = "Scintillation", G4ProcessType type = fElectromagnetic );
        ~DsG4Scintillation();

    
    public:

        // DsG4Scintillation Process has both PostStepDoIt (for energy 
        // deposition of particles in flight) and AtRestDoIt (for energy
        // given to the medium by particles at rest)

        G4bool IsApplicable(const G4ParticleDefinition& aParticleType);
        // Returns true -> 'is applicable', for any particle type except
        // for an 'opticalphoton' 

	    G4double GetMeanFreePath(const G4Track& aTrack,
				       G4double ,
                                       G4ForceCondition* );
        // Returns infinity; i. e. the process does not limit the step,
        // but sets the 'StronglyForced' condition for the DoIt to be 
        // invoked at every step.

        G4double GetMeanLifeTime(const G4Track& aTrack,
                                 G4ForceCondition* );
        // Returns infinity; i. e. the process does not limit the time,
        // but sets the 'StronglyForced' condition for the DoIt to be
        // invoked at every step.

	    G4VParticleChange* PostStepDoIt(const G4Track& aTrack, 
			                const G4Step&  aStep);
        
        G4VParticleChange* AtRestDoIt (const G4Track& aTrack,
                                       const G4Step& aStep);

        // These are the methods implementing the scintillation process.

    	void SetTrackSecondariesFirst(const G4bool state);
        // If set, the primary particle tracking is interrupted and any
        // produced scintillation photons are tracked next. When all 
        // have been tracked, the tracking of the primary resumes.

        G4bool GetTrackSecondariesFirst() const;
        // Returns the boolean flag for tracking secondaries first.
	

        void SetScintillationYieldFactor(const G4double yieldfactor);
        // Called to set the scintillation photon yield factor, needed when
        // the yield is different for different types of particles. This
        // scales the yield obtained from the G4MaterialPropertiesTable.
        G4double GetScintillationYieldFactor() const;
        // Returns the photon yield factor.


        void SetScintillationExcitationRatio(const G4double excitationratio);
        // Called to set the scintillation exciation ratio, needed when
        // the scintillation level excitation is different for different
        // types of particles. This overwrites the YieldRatio obtained
        // from the G4MaterialPropertiesTable.

        G4double GetScintillationExcitationRatio() const;
        // Returns the scintillation level excitation ratio.

        G4PhysicsTable* GetFastIntegralTable() const;
        // Returns the address of the fast scintillation integral table.

        G4PhysicsTable* GetSlowIntegralTable() const;
        // Returns the address of the slow scintillation integral table.

        G4PhysicsTable* GetReemissionIntegralTable() const;
        // Returns the address of the reemission integral table.

        void DumpPhysicsTable() const;
        // Prints the fast and slow scintillation integral tables.

        // Configuration
        G4double GetPhotonWeight() const { return fPhotonWeight; }
        void SetPhotonWeight(G4double weight) { fPhotonWeight = weight; }
        void SetDoReemission(bool tf = true) { doReemission = tf; }
        bool GetDoReemission() { return doReemission; }
        void SetDoBothProcess(bool tf = true) { doBothProcess = tf; }
        bool GetDoBothProcess() { return doBothProcess; }
        // only do the reemission, no scintillation. 
        // The trick is: only opticalphoton is allowed.
        void SetDoReemissionOnly(bool tf = true) { doReemissionOnly = tf; }
        bool GetDoReemissionOnly() { return doReemissionOnly; }

        // Enable Quenching or not
        void SetDoQuenching(const G4bool enable) { fEnableQuenching = enable; }
        G4bool GetDoQuenching() const { return fEnableQuenching; }

        void SetBirksConstant1(double c1) {birksConstant1 = c1;}
        double GetBirksConstant1() {return birksConstant1;}
        void SetBirksConstant2(double c2) {birksConstant2 = c2;}
        double GetBirksConstant2() {return birksConstant2;}

        void SetSlowerTimeConstant(double st) {slowerTimeConstant = st;}
        double GetSlowerTimeConstant() {return slowerTimeConstant;}

        void SetSlowerRatio(double m_sr) {slowerRatio = m_sr;}
        double GetSlowerRatio() {return slowerRatio;}

        // Gamma, Slower Ratio/Time Constant
        void SetGammaSlowerTimeConstant(double st) { gammaSlowerTime = st;}
        double GetGammaSlowerTimeConstant() {return gammaSlowerTime;}

        void SetGammaSlowerRatio(double m_sr) { gammaSlowerRatio = m_sr;}
        double GetGammaSlowerRatio() {return gammaSlowerRatio;}


        void SetFlagDecayTimeFast(bool flag) { flagDecayTimeFast = flag; }
        bool GetFlagDecayTimeFast() { return flagDecayTimeFast; }
        void SetFlagDecayTimeSlow(bool flag) { flagDecayTimeSlow = flag; }
        bool GetFlagDecayTimeSlow() { return flagDecayTimeSlow; }

        // Don't actually do anything.  This is needed, as apposed to
        // simply not including the scintilation process, because
        // w/out this process no optical photons make it into the
        // photocathode (???) 
        void SetNoOp(bool tf = true) { m_noop = tf; }

public:
        // interface for OP simulator. Reuse part of code.
        G4PhysicsTable* getSlowIntegralTable();
        G4PhysicsTable* getFastIntegralTable();
        G4PhysicsTable* getReemissionIntegralTable();
        
private:

        void BuildThePhysicsTable();
        // It builds either the fast or slow scintillation integral table; 
        // or both. 

        ///////////////////////
        // Class Data Members
        ///////////////////////

protected:

        G4PhysicsTable* theSlowIntegralTable;
        G4PhysicsTable* theFastIntegralTable;
        G4PhysicsTable* theReemissionIntegralTable;

        // on/off flag for absorbed opticalphoton reemission
        G4bool doReemission;
        // choose only reemission of Cerenkov or both of Cerenkov and Scintillaton;
        G4bool doBothProcess;

        // on/off flag for absorbed opticalphoton reemission,
        // but only do such job.
        G4bool doReemissionOnly;

        // on/off quenching
        G4bool fEnableQuenching;

        // Birks constant C1 and C2
        double  birksConstant1;
        double  birksConstant2;

        double  slowerTimeConstant;
        double  slowerRatio;

        double gammaSlowerTime;
        double gammaSlowerRatio;

        // add flags to enable or disable decay time (fast, slow and so on)
        // -- lintao <lintao@ihep.ac.cn>
        // -- 2016.01.02
        bool flagDecayTimeFast;
        bool flagDecayTimeSlow;

    private:

	    G4bool fTrackSecondariesFirst;

        G4double YieldFactor;
        G4double ExcitationRatio;
  
        //mean number of true photons per secondary track in GLG4Scint
        G4double fPhotonWeight;
        bool m_noop;




};

//
////////////////////
// Inline methods
////////////////////

inline 
G4bool DsG4Scintillation::IsApplicable(const G4ParticleDefinition& aParticleType)
{
        if (aParticleType.GetParticleName() == "opticalphoton"){
           return true;
        } else if (doReemissionOnly) {
           // if only do reemission, don't allow other particles to call it.
           return false;
        } else {
           return true;
        }
}

inline 
void DsG4Scintillation::SetTrackSecondariesFirst(const G4bool state) 
{ 
	fTrackSecondariesFirst = state;
}

inline
G4bool DsG4Scintillation::GetTrackSecondariesFirst() const
{
        return fTrackSecondariesFirst;
}

inline
void DsG4Scintillation::SetScintillationYieldFactor(const G4double yieldfactor)
{
        YieldFactor = yieldfactor;
}


inline
G4double DsG4Scintillation::GetScintillationYieldFactor() const
{
        return YieldFactor;
}



inline
void DsG4Scintillation::SetScintillationExcitationRatio(const G4double excitationratio)
{
        ExcitationRatio = excitationratio;
}

inline
G4double DsG4Scintillation::GetScintillationExcitationRatio() const
{
        return ExcitationRatio;
}

inline
G4PhysicsTable* DsG4Scintillation::GetSlowIntegralTable() const
{
        return theSlowIntegralTable;
}

inline
G4PhysicsTable* DsG4Scintillation::GetFastIntegralTable() const
{
        return theFastIntegralTable;
}

inline
G4PhysicsTable* DsG4Scintillation::GetReemissionIntegralTable() const
{
 	return theReemissionIntegralTable;
}

inline
void DsG4Scintillation::DumpPhysicsTable() const
{
        if (theFastIntegralTable) {
           G4int PhysicsTableSize = theFastIntegralTable->entries();
           G4PhysicsOrderedFreeVector *v;

           for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
           {
        	v = (G4PhysicsOrderedFreeVector*)(*theFastIntegralTable)[i];
        	v->DumpValues();
           }
         }

        if (theSlowIntegralTable) {
           G4int PhysicsTableSize = theSlowIntegralTable->entries();
           G4PhysicsOrderedFreeVector *v;

           for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
           {
                v = (G4PhysicsOrderedFreeVector*)(*theSlowIntegralTable)[i];
                v->DumpValues();
           }
         }

        if (theReemissionIntegralTable) {
           G4int PhysicsTableSize = theReemissionIntegralTable->entries();
           G4PhysicsOrderedFreeVector *v;

           for (G4int i = 0 ; i < PhysicsTableSize ; i++ )
           {
                v = (G4PhysicsOrderedFreeVector*)(*theReemissionIntegralTable)[i];
                v->DumpValues();
           }
         }
}


#endif







