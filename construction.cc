#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
  G4NistManager *nist = G4NistManager::Instance();

  G4Material *worldMat =nist->FindOrBuildMaterial("G4_Galactic");

  G4Material* Silicon = nist->FindOrBuildMaterial("G4_Si");

  
  G4Box *solidWorld = new G4Box("solidWorld",
                                20.0*m,
                                20.0*m,
                                20.0*m);
                                
                                
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld,
                                                      worldMat, 
                                                      "logicWorld");
                                                      
                                                      
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
                                                  G4ThreeVector(0.,0.,0.),
                                                  logicWorld, 
                                                  "physWorld",
                                                  0, 
                                                  false, 
                                                  0, 
                                                  true);
  
  // Defining Logic volume for Silicon sensors
  G4Box *solidDetector = new G4Box("solidDetector", 
                                    100.0/2*mm,
                                    100.0/2*mm,
                                    5.0/2*mm);
                                    
  logicDetector = new G4LogicalVolume(solidDetector,
                                      Silicon, 
                                      "logicDetector");
  
     // Set visualization attributes
    /*G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)); // Blue color
    logicDetector->SetVisAttributes(visAttributes);*/
  //fScoringVolume = logicDetector;
		
	          
                      
       G4VPhysicalVolume *physDetector = new G4PVPlacement(0,
                                                           G4ThreeVector(0,0,0),
                                                           logicDetector,
                                                           "physDetector",
                                                           logicWorld,
                                                           false,
                                                           0,
                                                           true);

  // Set production cuts in energy
  G4ProductionCuts *cuts = new G4ProductionCuts();
  cuts->SetProductionCut(10 * eV, "gamma");    // 10 eV for photons
  cuts->SetProductionCut(10 * eV, "e-");      // 10 eV for electrons
  cuts->SetProductionCut(10 * eV, "e+");      // 10 eV for positrons
  cuts->SetProductionCut(10 * eV, "proton");  // 10 eV for protons (if needed)

  // Apply the cuts to the world volume
  G4Region *region = G4RegionStore::GetInstance()->GetRegion("World");
  if (region)
  {
      region->SetProductionCuts(cuts);
  }
                                                            
  return physWorld;
}

void MyDetectorConstruction :: ConstructSDandField()
{
  MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);
  logicDetector->SetSensitiveDetector(sensDet);
}

