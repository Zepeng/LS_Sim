
// $Id: LSDetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file LSDetectorConstruction.hh
/// \brief Definition of the LSDetectorConstruction class

#ifndef LSDetectorConstruction_h
#define LSDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4GDMLParser.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;


#ifdef WITH_G4CXOPTICKS
class G4CXOpticks;
#endif

class LSOpticksEventConfigMessenger;

/// Detector construction class to define materials and geometry.

class LSDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        LSDetectorConstruction();
        virtual ~LSDetectorConstruction();

        virtual G4VPhysicalVolume* Construct();
        virtual void ConstructSDandField();

        void DefineMaterials();
        G4VPhysicalVolume* DefineVolumes();

        void ModifyOpticalProperty();
		//void SetOpticksMode(int mode);


    private:
        G4LogicalVolume* CDConstruction();
        G4LogicalVolume* SensDetConstruction();
        G4LogicalVolume* PmtConstruction();
	G4LogicalVolume* InnerWaterConstruction();
	G4LogicalVolume* VacuumConstruction();
	G4LogicalVolume* GlassConstruction();

	G4GDMLParser parser;
	// Reading and Writing Settings
	G4String fReadFile;
	G4String fWriteFile;
	G4String fStepFile;
	G4int writingChoice;
        G4bool fCheckOverlaps;

        G4Material* air;
        G4Material* water;
        G4Material* LS;
        G4Material* Steel;
        G4Material* Photocathode_mat;
		
		
        G4OpticalSurface* m_mirror_opsurf;
        G4OpticalSurface* Photocathode_opsurf;
#ifdef WITH_G4CXOPTICKS
		G4CXOpticks* m_g4cxopticks;
#endif
		
		LSOpticksEventConfigMessenger * m_lsOpticksEvtMes; 
		int m_opticksMode;
		int m_maxPhoton;
		int m_maxGenstep;
	public:
		void SetOpticksMaxGenstep(int max_genstep){ m_maxGenstep =  max_genstep;}
		void SetOpticksMaxPhoton(int max_photon){ m_maxPhoton = max_photon;}

	private:
        G4double coeff_abslen;
        G4double coeff_rayleigh;
        G4double coeff_efficiency;

		
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

