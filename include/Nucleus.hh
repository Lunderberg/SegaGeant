#ifndef Nucleus_h
#define Nucleus_h 1

#include "globals.hh"
#include <iostream>
//#include "g4std/iostream"
////////////////////////////////////////////////////////////////////////////////
//
class Nucleus
{
  // class description
  // The G4Nucleus class is used to contain information identifying an
  // isotope (a,z,e)
  //
  // class description - end
public: // with description
  Nucleus ();
  //    Default constructor
  //
  Nucleus (G4int a, G4int z, G4double e);
  //    Constructor defining new isotope with A,Z.E
  //
  ~Nucleus();
  //  Destructor
  
private:
  G4int a;
  G4int z;
  G4double e;

  //
  //
  // INLINE DECLARATIONS/DEFINITIONS:
  //
public: // with description
  inline  G4int GetA () const {return a;}
  //    Returns the value of a
  inline  G4int GetZ () const {return z;}
  //    Returns the value of z
  inline  G4double GetE () const {return e;}
  //    Returns the value of e

  //
  //
  // DECLARATIONS OF FRIENDS OF THE CLASS.
  //
  friend std::ostream &operator << (std::ostream &s, const Nucleus &q);

};
////////////////////////////////////////////////////////////////////////////////
#endif



