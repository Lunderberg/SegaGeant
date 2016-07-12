#include "Doppler.hh"

//Doppler::Doppler(DetectorConstruction *detConst)
Doppler::Doppler()
{
}

Doppler::~Doppler()
{}

void Doppler::SetCrysSize(G4double len, G4double rad, G4double fing)
{
 crysLength=len;
 crysRadius=rad;
 fingRadius=fing;
}

void Doppler::SetDetPos(G4int ring, G4int number, G4double r, G4double z, G4int ornt)
{
  //  Center of crystal position
 canR[ring][number] = r;
 canZ[ring][number] = z;
 orient[ring][number] = ornt;
}

G4double Doppler::Correct(G4double e)
{
  // Segment position relative to center of can
 E = e;
 //Ring = ID/100000;
 Ring = ID/1000000;
// Number = (ID%100000)/10000;
 Number = (ID%1000000)/100000;
 Z_ID = (ID%100000)/1000;
 Phi_ID = (ID%1000)/10;
 R_ID = (ID%10);

G4int R_segs = 1;
G4int Phi_segs = 4;
G4float Z_segs = 8.;
G4float Rcan = canR[Ring][Number];

 //Z_rel = orient[Ring][Number]*(Z_ID*crysLength/4.0 - 7.0/8.0*crysLength);
 Z_rel = orient[Ring][Number]*crysLength*(2*Z_ID/Z_segs - (Z_segs-1)/Z_segs);
 Phi_rel = 180*deg/Phi_segs + 360*deg/Phi_segs * Phi_ID;
 R_outer = (crysRadius-fingRadius)/R_segs*(R_ID+1)+fingRadius;
 R_inner = (crysRadius-fingRadius)/R_segs*(R_ID)+fingRadius;
 R_rel = sqrt((R_outer*R_outer-R_inner*R_inner)/2);   // Volume averaged radius of the cell
// R_rel = 0;
// segZ = canZ[Ring][Number] + Z_rel;
 segZ = canZ[Ring][Number] + Z_rel -.05*cm;
// segR = sqrt( (canR[Ring][Number]+R_rel*cos(Phi_rel)) * (canR[Ring][Number]+R_rel*cos(Phi_rel)) + R_rel*sin(Phi_rel)*R_rel*sin(Phi_rel));
 segR = sqrt( Rcan*Rcan + R_rel*R_rel + 2*Rcan*R_rel*cos(Phi_rel) );


 Gamma = 1/sqrt(1-Beta*Beta);
 u_lab = segZ/sqrt(segZ*segZ + segR*segR); // cos(theta_lab)
 E_prime = E*Gamma*(1.0-Beta*u_lab);

 return E_prime;
}

G4double Doppler::Correct(G4double e, G4double z, G4double r)
{
 E = e;
 Z = z;
 R = r;
 Gamma = 1/sqrt(1-Beta*Beta);
 u_lab = Z/sqrt(Z*Z + R*R);
 E_prime = E*Gamma*(1.0-Beta*u_lab);

 return E_prime;
}
/*
G4double Doppler::Correct(G4double e, G4double z)
{
 E = e;
 Z = z;
 Ring = ID/100000;
 Number = (ID%100000)/10000;
 Segment = (ID%100)/10;
 Quarter = (ID%10);
 segR = canR[Ring][Number] + crysRadius/sqrt(2.0)*cos(orient[Ring][Number]*(Quarter+1)*90*deg);

 Gamma = 1/sqrt(1-Beta*Beta);
 u_lab = Z/sqrt(Z*Z + segR*segR);
 E_prime = E*Gamma*(1.0-Beta*u_lab);

 return E_prime;
}*/
