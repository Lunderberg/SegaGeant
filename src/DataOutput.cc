#include "DataOutput.hh"
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/stat.h>

DataOutput::DataOutput()
{
 datamessenger = new DataOutputMessenger(this);
 Beam = 0;
 outputdir=".";
 SegVerbose = false;
 TotalEVerbose = true;
}

DataOutput::~DataOutput()
{
 delete datamessenger;
}

void DataOutput::WriteSpectrum(G4int spec[], G4int channels, G4int counts, G4double bin, G4String FileName)
{
  // Name the file
  if(!Beam)
  {tag = nucleus;}
  if(Beam)
  {tag = "beta" + beta;}

  using namespace std;
  ofstream specfile;
  struct stat buffer;
  char c = 'a';
  string filename = outputdir + "/" + FileName + "_" + tag + "_" + c;

  // Check if output file already exists
  // If it does, append a letter [a-z]
  while(!stat(filename.c_str(),&buffer))
  {
    c++;
    filename = outputdir + "/" + FileName + "_" + tag + "_" + c;
//    filename.append("_");
//   filename.append(1,c);
  }

  // Open file
  specfile.open(filename.c_str());

  // Write Header
  if(!Beam)
  {
   specfile << "#################################" << G4endl;
   specfile << "# Source: " << nucleus << "\t\t#" << G4endl;
   specfile << "# Channels: " << channels << "\t\t#" << G4endl;
   specfile << "# keV/channel: " << bin << "\t\t#" << G4endl;
   specfile << "# Total counts: " << counts << "\t\t#" << G4endl;
   specfile << "#################################" << G4endl;
  }
 if(Beam)
 {
   specfile << "#################################" << G4endl;
   specfile << "# Beta: " << beta << "\t\t\t#" << G4endl;
   specfile << "# Channels: " << channels << "\t\t#" << G4endl;
   specfile << "# keV/channel: " << bin << "\t\t#" << G4endl;
   specfile << "# Total counts: " << counts << "\t\t#" << G4endl;
   specfile << "#################################" << G4endl;
 }

  //Write Spectrum 
  for(i=0;i<channels;i++)
  specfile << i << " " << spec[i] << G4endl;
  specfile.close();
}

void DataOutput::Write2DSpectrum(G4int spec[4000][10], G4int xchannels, G4int ychannels, G4String FileName)
{
  // Name the file
  if(!Beam)
  {tag = nucleus;}
  if(Beam)
  {tag = "beta" + beta;}

  using namespace std;
  ofstream specfile;
  struct stat buffer;
  char c = 'a';
  string filename = outputdir + "/" + FileName + "_" + tag + "_" + c;

  // Check if output file already exists
  // If it does, append a letter [a-z]
  while(!stat(filename.c_str(),&buffer))
  {
    c++;
    filename = outputdir + "/" + FileName + "_" + tag + "_" + c;
//    filename.append("_");
//   filename.append(1,c);
  }

  // Open file
  specfile.open(filename.c_str());

  // Write 2d spec
  for(i=ychannels-1;i>=0;i--)
  {
    for(j=0;j<xchannels;j++)
    {
      specfile << spec[i][j] <<  " ";
    }
    specfile << G4endl;
  }
  specfile.close();

}

void DataOutput::SetDir(G4String d)
{
struct stat buffer;
if(stat(d,&buffer))
 {
   G4cout << "Directory \"" << d << "\" does not exist.\nYou must first create the directory." << G4endl;
 }
 else
 {
   outputdir = d;
   G4cout << "Setting output directory to " << outputdir << G4endl;
 }
}

void DataOutput::SetFile(G4String f)
{
  FileName = f;
  G4cout << "Setting output filename to " << FileName << G4endl;
}

