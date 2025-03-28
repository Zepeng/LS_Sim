
/// \file LSDetectorConstruction.cc
/// \brief Implementation of the LSDetectorConstruction class
#include "LSDetectorConstruction.hh"
#include "LSDetectorSD.hh"

#include "G4PhysicalConstants.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSDoseDeposit.hh"

#include "OpticalProperty.icc"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Version.hh"

#ifdef WITH_G4CXOPTICKS
#include "G4CXOpticks.hh"
#include "PLOG.hh"
#include "LSDetectorConstruction_Opticks.hh"
#include "SPath.hh"
#include "SEventConfig.hh"
#include "QRng.hh"
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


LSDetectorConstruction::LSDetectorConstruction()
    : G4VUserDetectorConstruction(),
	fCheckOverlaps(true), 
	air(NULL), 
	water(NULL), 
	LS(NULL), 
	Steel(NULL),
	coeff_abslen(2.862), 
	coeff_rayleigh(0.643), 
	coeff_efficiency(0.5),
#ifdef WITH_G4CXOPTICKS
	m_g4cxopticks(nullptr),
#endif
	m_maxPhoton(-1),
	m_maxGenstep(-1)

{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LSDetectorConstruction::~LSDetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* LSDetectorConstruction::Construct()
{   
    DefineMaterials();

    return DefineVolumes();
}

void LSDetectorConstruction::ModifyOpticalProperty()
{
    for(int i=0; i<497; i++) {
        GdLSABSLength[i] *= coeff_abslen;
    }

    for(int i=0; i<11; i++) {
        GdLSRayLength[i] *= coeff_rayleigh;
    }

    for(int i=0; i<43; i++) {
        fPhCEFFICIENCY_Dynode20inch[i] *= coeff_efficiency;
    }
}

void LSDetectorConstruction::DefineMaterials()
{
    ModifyOpticalProperty();

    // Get nist material manager
    // air construction
    G4NistManager* nist = G4NistManager::Instance();
    air = nist->FindOrBuildMaterial("G4_AIR");
    G4MaterialPropertiesTable* air_mpt = new G4MaterialPropertiesTable();
    air_mpt->AddProperty("RINDEX",    AirEnergy, AirRefIndex,   4);
    air_mpt->AddProperty("ABSLENGTH", AirEnergy, AirAbsLength,  4);
    air->SetMaterialPropertiesTable(air_mpt);


    // Water from Database
    G4Element* H = G4Element::GetElement("Hydrogen", JustWarning);
    if (not H) {
        H = new G4Element("Hydrogen", "H" , 1., 1.01*g/mole);
    }
    G4Element* O  = G4Element::GetElement("Oxygen", JustWarning);
    if (not O) {
        O = new G4Element("Oxygen", "O", 8., 16.00*g/mole); 
    }

    G4double water_density= 1.000*g/cm3;
    water = new G4Material("water", water_density, 2);
    water->AddElement(H,2);
    water->AddElement(O,1);
    G4MaterialPropertiesTable* water_mpt = new G4MaterialPropertiesTable();
    water_mpt->AddProperty("RINDEX", fPP_Water_RIN, fWaterRINDEX, 36);
    water_mpt->AddProperty("ABSLENGTH", fPP_Water_ABS, fWaterABSORPTION,  316);
    water->SetMaterialPropertiesTable(water_mpt);


    // LS from JUNO
    LS = G4Material::GetMaterial("LS", JustWarning);
    G4cout << "Material LS is constructed from the codes !" << G4endl;

    G4Element* TS_C_of_Graphite = G4Element::GetElement("TS_C_of_Graphite", JustWarning);
    if (not TS_C_of_Graphite) { 
        TS_C_of_Graphite = new G4Element("TS_C_of_Graphite", "C_GRAPHITE" , 6., 12.01*g/mole); 
    }

    G4Element* TS_H_of_Water = G4Element::GetElement("TS_H_of_Water", JustWarning);
    if (not TS_H_of_Water) {
        TS_H_of_Water = new G4Element("TS_H_of_Water", "H_WATER" , 1., 1.01*g/mole);
    }
    G4Element* TS_H_of_Polyethylene = G4Element::GetElement("TS_H_of_Polyethylene", JustWarning);
    if (not TS_H_of_Polyethylene) {
        TS_H_of_Polyethylene = new G4Element("TS_H_of_Polyethylene", "H_POLYETHYLENE" , 1., 1.01*g/mole);
    }
    
    G4Element* N  = G4Element::GetElement("Nitrogen", JustWarning);
    if (not N) {
        N = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
    }
    G4Element* S =  G4Element::GetElement("Sulfur", JustWarning);
    if (not S) {
        S = new G4Element("Sulfur", "S", 16., 32.066*g/mole);
    }
    G4Element* C = G4Element::GetElement("Carbon", JustWarning);
    if (not C) { 
        C = new G4Element("Carbon", "C" , 6., 12.01*g/mole); 
    }


    LS  = new G4Material("LS", 0.859*g/cm3, 5);
    LS->AddElement(TS_C_of_Graphite,  0.87924);
    //LS->AddElement(H,  0.1201);
    LS->AddElement(TS_H_of_Water,  0.1201);
    LS->AddElement(O,  0.00034);
    //LS->AddElement(Gd, 0.0010315);
    LS->AddElement(N,  0.00027);
    LS->AddElement(S,  0.00005);
    
    G4MaterialPropertiesTable* LSMPT = new G4MaterialPropertiesTable();
    LSMPT->AddProperty("ABSLENGTH", GdLSABSEnergy, GdLSABSLength, 497);
    LSMPT->AddProperty("RAYLEIGH", GdLSRayEnergy, GdLSRayLength, 11);
    LSMPT->AddProperty("RINDEX", GdLSRefIndexEnergy, GdLSRefIndex, 18) ;
#if G4VERSION_NUMBER >= 1100
    LSMPT->AddProperty("FASTCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275, true);
    LSMPT->AddProperty("SLOWCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275, true);
    LSMPT->AddProperty("REEMISSIONPROB", GdLSReemEnergy, GdLSReem, 28, true);
    LSMPT->AddProperty("SCINTILLATIONYIELD", component, GdLSLY,2, true);
    LSMPT->AddProperty("RESOLUTIONSCALE", component, GdLSResolutionScale,2, true);
    LSMPT->AddProperty("FASTTIMECONSTANT",component,GdLSFastTimeConstant,2, true);
    LSMPT->AddProperty("SLOWTIMECONSTANT",component,GdLSSlowTimeConstant,2, true);
    LSMPT->AddProperty("YIELDRATIO",component,GdLSYieldRatio,2, true);
    LSMPT->AddProperty("GammaFASTTIMECONSTANT", component, GdLSFastTimeConstant,2, true);
    LSMPT->AddProperty("GammaSLOWTIMECONSTANT", component, GdLSSlowTimeConstant,2, true);
    LSMPT->AddProperty("GammaYIELDRATIO", component, GdLSYieldRatio,2, true);
    // add fast/slow time constant for reemission
    LSMPT->AddProperty("ReemissionFASTTIMECONSTANT", component, GdLSReemissionFastTimeConstant,2, true);
    LSMPT->AddProperty("ReemissionSLOWTIMECONSTANT", component, GdLSReemissionSlowTimeConstant,2, true);
    LSMPT->AddProperty("ReemissionYIELDRATIO", component, GdLSReemissionYieldRatio,2, true);
#else
    LSMPT->AddProperty("FASTCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275);
    LSMPT->AddProperty("SLOWCOMPONENT", GdLSComEnergy, GdLSFastComponent, 275);
    LSMPT->AddProperty("REEMISSIONPROB", GdLSReemEnergy, GdLSReem, 28);
    LSMPT->AddProperty("SCINTILLATIONYIELD", component, GdLSLY,2);
    LSMPT->AddProperty("RESOLUTIONSCALE", component, GdLSResolutionScale,2);
    LSMPT->AddProperty("FASTTIMECONSTANT",component,GdLSFastTimeConstant,2);
    LSMPT->AddProperty("SLOWTIMECONSTANT",component,GdLSSlowTimeConstant,2);
    LSMPT->AddProperty("YIELDRATIO",component,GdLSYieldRatio,2);
    LSMPT->AddProperty("GammaFASTTIMECONSTANT", component, GdLSFastTimeConstant,2);
    LSMPT->AddProperty("GammaSLOWTIMECONSTANT", component, GdLSSlowTimeConstant,2);
    LSMPT->AddProperty("GammaYIELDRATIO", component, GdLSYieldRatio,2);
    // add fast/slow time constant for reemission
    LSMPT->AddProperty("ReemissionFASTTIMECONSTANT", component, GdLSReemissionFastTimeConstant,2);
    LSMPT->AddProperty("ReemissionSLOWTIMECONSTANT", component, GdLSReemissionSlowTimeConstant,2);
    LSMPT->AddProperty("ReemissionYIELDRATIO", component, GdLSReemissionYieldRatio,2);
#endif
    LS -> SetMaterialPropertiesTable(LSMPT);
    
    // PMT Materials :
    G4Element* Fe = G4Element::GetElement("Iron", JustWarning);
    if (not Fe) {
        Fe = new G4Element("Iron", "Fe", 26., 55.845*g/mole);
    }
    G4Element* Ni = G4Element::GetElement("Ni", JustWarning);
    if (not Ni) {
        Ni = new G4Element("Ni", "Ni", 28, 58.6934*g/mole);
    }
    G4Element* Cr = G4Element::GetElement("Cr", JustWarning);
    if (not Cr) {
        Cr = new G4Element("Cr", "Cr", 24, 51.9961*g/mole);
    }
    G4Element* Mn = G4Element::GetElement("Mn", JustWarning);
    if (not Mn) {
        Mn = new G4Element("Mn", "Mn", 25, 54.9381*g/mole);
    }
    G4Element* P =  G4Element::GetElement("Phosphorus", JustWarning);
    if (not P) {
        P = new G4Element("Phosphorus", "P", 15, 30.9738*g/mole);
    }
    G4Element* Si = G4Element::GetElement("Silicon", JustWarning);
    if (not Si) {
        Si = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
    }
    
    G4double steel_density = 8.1*g/cm3;
    Steel = new G4Material("Steel", steel_density, 8);
    Steel->AddElement(Fe, 0.70845);
    Steel->AddElement(C, 0.0008);
    Steel->AddElement(Mn, 0.02);
    Steel->AddElement(P, 0.00045);
    Steel->AddElement(S, 0.00030);
    Steel->AddElement(Si, 0.01);
    Steel->AddElement(Cr, 0.18);
    Steel->AddElement(Ni, 0.08);
    G4MaterialPropertiesTable* Steel_mpt = new G4MaterialPropertiesTable();
    G4double SteelEnergy[4] = {1.55*eV, 6.20*eV, 10.33*eV, 15.5*eV};
    G4double SteelAbsLength[4] = {1e-3*mm, 1e-3*mm, 1e-3*mm, 1e-3*mm};
    G4double SteelRefIdx[4] = {1, 1, 1, 1};   // refractive index of Steel... fake
    //Steel_mpt->AddProperty("RINDEX", SteelEnergy, SteelRefIdx, 4);
    Steel_mpt->AddProperty("RINDEX", fPP_Water_RIN, fWaterRINDEX, 36);
    Steel_mpt->AddProperty("ABSLENGTH", SteelEnergy, SteelAbsLength,  4);
    Steel->SetMaterialPropertiesTable(Steel_mpt);
      
    G4double cathode_density = 5. *g/cm3; // true??
    G4Element* K =  G4Element::GetElement("Potassium", JustWarning);
    if (not K) {
        K = new G4Element("Potassium", "K", 19., 39.0983*g/mole);
    }
    Photocathode_mat = new G4Material("photocathode",cathode_density,1);
    Photocathode_mat->AddElement(K, 1);
    
    G4double fPhCEnergy[4] = {1.55*eV, 6.20*eV, 10.33*eV, 15.5*eV};
    G4double fPhCRINDEX[4] = {2.9, 2.9, 2.9, 2.9};
    G4double fPhCKINDEX[4] = {1.6, 1.6, 1.6, 1.6};
    G4double fPhCREFLECTIVITY[4] = {0.0, 0.0, 0.0, 0.0};

	G4double fake_eff[4] = {0.8,0.8,0.8,0.8};   
 
    G4MaterialPropertiesTable* PhotocathodeMPT = new G4MaterialPropertiesTable();
    PhotocathodeMPT->AddProperty("RINDEX", fPhCEnergy, fPhCRINDEX, 4);
    PhotocathodeMPT->AddProperty("KINDEX", fPhCEnergy, fPhCKINDEX, 4, true);
    PhotocathodeMPT->AddProperty("REFLECTIVITY", fPhCEnergy, fPhCREFLECTIVITY, 4);
    PhotocathodeMPT->AddProperty("EFFICIENCY", fPP_PhCQE_Dynode20inch, fPhCEFFICIENCY_Dynode20inch, 43);
	//PhotocathodeMPT->AddProperty("EFFICIENCY", fPhCEnergy, fake_eff, 4);   

    Photocathode_mat -> SetMaterialPropertiesTable(PhotocathodeMPT);

    Photocathode_opsurf = new G4OpticalSurface("Photocathode_opsurf");
    Photocathode_opsurf->SetType(dielectric_metal); // ignored if RINDEX defined
    //Photocathode_opsurf->SetFinish(polished);
	//Photocathode_opsurf->SetModel(unified);
    //steel_surface->SetType(dielectric_metal);
    //steel_surface->SetFinish(ground);
   	//Photocathode_opsurf->SetSigmaAlpha(0.2);

    Photocathode_opsurf->SetMaterialPropertiesTable(G4Material::GetMaterial("photocathode")->GetMaterialPropertiesTable() );
	
	/*
	Vacuum = G4Material::GetMaterial("Vacuum", any_warnings);
    if (Vacuum) {
        G4cout << "Vacuum is constructed from the GDML file" << G4endl;
    } else {
        G4cout << "Vacuum is constructed from the code" << G4endl;
        density     =  1e-3 * kGasThreshold;         //from PhysicalConstants.h
        G4double temperature = STP_Temperature;         //from PhysicalConstants.h
        G4double pressure    = STP_Pressure * density / (1.29e-3*g/cm3);
        Vacuum = new G4Material("Vacuum", density, 1, kStateGas,temperature,pressure);
        Vacuum->AddMaterial(Air, 1.);
    }

	G4double fVacEnergy[2]={1.5*eV,1.55*eV};
	G4double fVacRindex[2]={1.000001,1.000001};
	G4double fVacAbslength[2]={1000000*m, 1000000*m};
	G4MaterialPropertiesTable* VacMPT = new G4MaterialPropertiesTable();
	
    VacMPT->AddProperty("RINDEX", fVacEnergy, fVacRindex, 2);
    VacMPT->AddProperty("ABSLENGTH", fVacEnergy, fVacAbslength,2);
	Vacuum -> SetMaterialPropertiesTable(VacMPT);
	*/
	
	// Pyrex (glass) to implement.
	/*
	G4Material* SiO2 = G4Material::GetMaterial("SiO2", any_warnings);
    if (not SiO2) {
        density = 2.23*g/cm3;
        SiO2 = new G4Material("SiO2", density, 2);
        SiO2->AddElement(Si, 1);
        SiO2->AddElement(O , 2);
    }
    G4Material* B2O2 = G4Material::GetMaterial("B2O2", any_warnings);
    if (not B2O2) {
        density = 2.23*g/cm3;
        B2O2 = new G4Material("B2O2", density, 2);
        B2O2->AddElement(B,  2);
        B2O2->AddElement(O,  2);
    }
    G4Material* Na2O = G4Material::GetMaterial("Na2O", any_warnings);
    if (not Na2O) {
        density = 2.23*g/cm3;
        Na2O = new G4Material("Na2O", density, 2);
        Na2O->AddElement(Na, 2);
        Na2O->AddElement(O,  1);
    }
	Pyrex = G4Material::GetMaterial("Pyrex", any_warnings);
    if (Pyrex) {
        G4cout << "Pyrex is constructed from the GDML file" << G4endl;
    } else {
        G4cout << "Pyrex is constructed from the code" << G4endl;
        density = 2.23*g/cm3;
        Pyrex = new G4Material("Pyrex", density, 3);
        Pyrex->AddMaterial(SiO2, .80);
        Pyrex->AddMaterial(B2O2, .13);
        Pyrex->AddMaterial(Na2O, .07);
	}
	
	G4MaterialPropertiesTable* PyrexMPT = new G4MaterialPropertiesTable();
	G4double fPyrEnergy[]
	*/
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* LSDetectorConstruction::CDConstruction()
{
    
    //G4Box* solidCell  =
    //    new G4Box("cellSolid", 1*cm, 1*cm, 1*cm);
    G4Sphere* solidCell = 
        new G4Sphere("cellSolid", 0*cm, 17700*mm, 0, 2*pi, 0, pi);

    G4ThreeVector zTrans(0, 0, 0);
    G4LogicalVolume* logicCell =                         
        new G4LogicalVolume(
                solidCell,
                LS, 
                // lab,             
                "logicCell");        

    return logicCell;
}

G4LogicalVolume* LSDetectorConstruction::InnerWaterConstruction(){
    G4Sphere* solidInnerWater = new G4Sphere("InnerWaterSolid", 0*mm, 18000*mm, 0*deg, 360*deg, 0, 180*deg);
    G4LogicalVolume* logicInnerWater =  new G4LogicalVolume(solidInnerWater, 
                            				water,"logicInnerWater");
    
    return logicInnerWater;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4LogicalVolume* LSDetectorConstruction::SensDetConstruction()
{
	G4Sphere* solidDet = new G4Sphere("detSolid", 0*mm, 18010*mm, 0*deg, 360*deg, 0, 180*deg);
	G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, 
                            						Steel,
                            						"logicDet");
    return logicDet;
}

G4LogicalVolume* LSDetectorConstruction::VacuumConstruction(){

	G4Sphere* solidVacuum = new G4Sphere("vacuumSolid", 0*mm, 18010*mm, 0*deg, 360*deg, 0, 180*deg);
	G4LogicalVolume* logicVacuum = new G4LogicalVolume(solidVacuum, 
                            							air,
                            							"logicVacuum");
    return logicVacuum;

}

G4LogicalVolume* LSDetectorConstruction::GlassConstruction(){

    G4Sphere* solidGlass = new G4Sphere("glassSolid", 0*mm, 18005*mm, 0*deg, 360*deg, 0, 180*deg);
    G4LogicalVolume* logicGlass = new G4LogicalVolume(solidGlass,
                                                        Steel,
                                                        "logicGlass");
    return logicGlass;

}



G4LogicalVolume* LSDetectorConstruction::PmtConstruction()
{
    G4Tubs* pmttube_solid = new G4Tubs("pmttube_solid", 
                                   0*mm,
                                   254*mm,
                                   150*mm,
                                   0*deg,
                                   360*deg);
    G4LogicalVolume* pmttube_logic = new G4LogicalVolume(pmttube_solid, 
                            								Steel,
                            								"pmttube_logic" );
	return pmttube_logic;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LSDetectorConstruction::DefineVolumes()
{

    G4double worldLength = 40.*m;


    // Visualization attributes
    G4VisAttributes* boxVisAtt = new G4VisAttributes(G4Colour(0, 1, 1));
    //boxVisAtt->SetForceSolid ();
    boxVisAtt->SetForceWireframe();
    G4VisAttributes* detVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 1));
    //detVisAtt->SetForceAuxEdgeVisible();
    detVisAtt->SetForceWireframe();

    //////////////////////////////////////////////////////////////////////////////////////////////


    // Geometry Construction Part
	//World
    
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
    G4Box* worldS
        = new G4Box("world",                                    //its name
                    worldLength/2,worldLength/2,worldLength/2); //its size
    G4LogicalVolume* worldLV
        = new G4LogicalVolume(
                     worldS,   //its solid
                     //air,      //its material
                     water,
                     "World"); //its name
  
    G4VPhysicalVolume* worldPV
     = new G4PVPlacement(
                     0,               // no rotation
                     G4ThreeVector(), // at (0,0,0)
                     worldLV,         // its logical volume
                     "World",         // its name
                     0,               // its mother  volume
                     false,           // no boolean operations
                     0,               // copy number
                     fCheckOverlaps); // checking overlaps 


	/*
    G4LogicalVolume* logicDet = SensDetConstruction();
    G4VPhysicalVolume* physDet =
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,0),
                          logicDet,
                          "physDet", 
                          worldLV,
                          false,
                          0,
                          fCheckOverlaps);
    logicDet -> SetVisAttributes(detVisAtt);
	*/
	//Vacuum
    G4LogicalVolume* logicVac = VacuumConstruction();
    G4VPhysicalVolume* physVac =
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,0),
                          logicVac,
                          "physVac", 
                          worldLV,
                          false,
                          0,
                          fCheckOverlaps);
    //logicVac -> SetVisAttributes(detVisAtt);
	//Glass
	
    G4LogicalVolume* logicGlass = GlassConstruction();
    G4VPhysicalVolume* physGlass =
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,0),
                          logicGlass,
                          "physGlass", 
                          logicVac,
                          false,
                          0,
                          fCheckOverlaps);
    //logicGlass-> SetVisAttributes(detVisAtt);

	//InnerWater	
    G4LogicalVolume* logicInnerWater = InnerWaterConstruction();
    G4VPhysicalVolume* physInnerWater =
        new G4PVPlacement(0,
                          G4ThreeVector(0,0,0),
                          logicInnerWater,
                          "physInnerWater",
                          logicGlass,
                          false,
                          0,
                          fCheckOverlaps);
	
	//LS 
    G4LogicalVolume* logicCell = CDConstruction();
    G4VPhysicalVolume* physCell = 
        new G4PVPlacement(0,                       
                G4ThreeVector(0, 0, 0), 
                logicCell,                
                "physCell",              
                logicInnerWater,              
                false,                   
                0,                       
                fCheckOverlaps);  
    logicCell   ->  SetVisAttributes(boxVisAtt);  // set visualization
    


    // Optical surface :
	G4LogicalBorderSurface* pmtSurface = new G4LogicalBorderSurface("pmtSurface", physInnerWater,physGlass, Photocathode_opsurf);
	//G4LogicalBorderSurface* pmtSurface = new G4LogicalBorderSurface("pmtSurface",physGlass, physVac , Photocathode_opsurf);
	//G4LogicalBorderSurface* pmtSurface_2 = new G4LogicalBorderSurface("pmtSurface",
    //                                            physDet, Photocathode_opsurf);
#ifdef WITH_G4CXOPTICKS
	SEventConfig::SetRGModeSimulate();

	//if( m_maxPhoton > 0 ){
	//	SEventConfig::SetMaxPhoton(m_maxPhoton);		
	//}
	//if( m_maxGenstep > 0 ){
	//	SEventConfig::SetMaxGenstep(m_maxGenstep);
	//}
	
	int Million = SEventConfig::M;
	int max_photon = SEventConfig::MaxPhoton();
	//if( max_photon > 400*Million){
	//	LOG(error) << " max_photon = "<< max_photon
	//			   << " exceed !!! ";
	//	assert(0);
	//}
	/*else if ( max_photon > 100*Million){
		QRng::DEFAULT_PATH = SPath::Resolve("$HOME/.opticks/rngcache/RNG/cuRANDWrapper_400000000_0_0.bin", 0) ;
	}else if( max_photon > 10*Million){
		QRng::DEFAULT_PATH = SPath::Resolve("$HOME/.opticks/rngcache/RNG/cuRANDWrapper_100000000_0_0.bin", 0) ;
	}else if( max_photon > 3*Million){
		QRng::DEFAULT_PATH = SPath::Resolve("$HOME/.opticks/rngcache/RNG/cuRANDWrapper_10000000_0_0.bin", 0) ;
	}else if( max_photon > 1*Million){
		QRng::DEFAULT_PATH = SPath::Resolve("$HOME/.opticks/rngcache/RNG/cuRANDWrapper_3000000_0_0.bin", 0) ;
	}else{
		QRng::DEFAULT_PATH = SPath::Resolve("$HOME/.opticks/rngcache/RNG/cuRANDWrapper_1000000_0_0.bin", 0) ;
	}
	LOG(info)<<" QRng::DEFAULT_PATH " << QRng::DEFAULT_PATH ;
	*/
	m_g4cxopticks = LSDetectorConstruction_Opticks::Setup( worldPV);
	assert(m_g4cxopticks);
#endif
    return worldPV;
}


void LSDetectorConstruction::ConstructSDandField()
{
    G4cout << " ----> Add Sensitive Detector " << G4endl;
    // Sensitive Detector

    auto detectorSD
        =  new LSDetectorSD("detectorSD", "PmtHitsCollection"); //fNofLayers);
    G4SDManager::GetSDMpointer()->AddNewDetector(detectorSD);
    SetSensitiveDetector("logicInnerWater", detectorSD);

}




