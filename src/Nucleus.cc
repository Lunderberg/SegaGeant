#include "Nucleus.hh"
//
Nucleus::Nucleus ()
  : a(24), z(11), e(0.0)
{;}
//
Nucleus::Nucleus (G4int a1, G4int z1, G4double e1)
{
  //
  //
  a = a1;
  z = z1;
  e = e1;
}
//
Nucleus::~Nucleus ()
{;}
//
std::ostream &operator << (std::ostream &s, const Nucleus &q)
//
//
// Definition of the insertion operator << to provide the nucleus limits to
// ostream.
//
{
  s <<"Atomic weight: " <<q.GetA()
    <<"Atomic number: " <<q.GetZ()
    <<"Excitation energy: "<<q.GetE();
  return s;
}
///////////////////////////////////////////////////////////////////////////////






