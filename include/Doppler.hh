#ifndef Doppler_h
#define Doppler_h 1

#include "globals.hh"

class Doppler
{
 public:
   Doppler();
   ~Doppler();
   void SetCrysSize(G4double, G4double, G4double);
   void SetDetPos(G4int, G4int, G4double, G4double, G4int);
   void SetBeta(G4double b) {Beta = b;};
   void SetDetNo(G4int id) {ID = id;};
   G4double Correct(G4double);
   G4double Correct(G4double, G4double, G4double);
   G4double Correct(G4double, G4double);

 private:
   G4double crysLength, crysRadius, fingRadius;
   G4double canR[3][10];
   G4double canZ[3][10];
   G4int orient[3][10];
   G4double E, segZ, segR, Z, R, Z_rel, Phi_rel, R_inner, R_outer, R_rel;
   G4double Beta, Gamma, u_lab, E_prime;
   G4int ID, Ring, Number, Z_ID, Phi_ID, R_ID;
};

#endif

