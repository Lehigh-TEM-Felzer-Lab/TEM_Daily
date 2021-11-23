/* *************************************************************
TCO2DAT45.CPP - object to read and write the structure of
                   transient atmospheric CO2 data from/to files
                   used by the Terrestrial Ecosystem Model (TEM)

Modifications:

20060118 - DWK created by modifying tco2dat425.cpp
20060118 - DWK added include tco2dat437.h and standard includes
20060118 - DWK changed CO2data:: to CO2data43::
20070105 - TWC renamed to tco2dat45

****************************************************************
************************************************************* */

#include<cstdio>

  using std::fscanf;
  using std::FILE;

#include<iostream>

  using std::ios;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setprecision;

#include<string>

  using std::string;


#include "tco2dat45.h"


CO2dat45::CO2dat45( void )
{

  co2end = 1;
  lagpos = -99;
  curpos = 0;

};

/* *************************************************************
                    Public Functions
************************************************************* */

int CO2dat45::get( ifstream& infile )
{

  lagpos = infile.tellg();

  infile >> year >> mco2;

  infile.seekg( 0, ios::cur );

  curpos = infile.tellg();

  if( curpos < (lagpos + 5) ) { co2end = -1; }

  return co2end;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int CO2dat45::getdel( FILE* infile )
{

  co2end = fscanf( infile, "%f,%lf", &year, &mco2 );

  return co2end;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void CO2dat45::out( ofstream& ofile,
                     float& year,
                     double& mco2 )
{

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << year << ' ';
  ofile << setprecision( 4 ) << mco2;
  ofile << endl;


};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void CO2dat45::outdel( ofstream& ofile,
                        float& year,
                        double& mco2 )
{

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << year << ",";
  ofile << setprecision( 4 ) << mco2;
  ofile << endl;

};

