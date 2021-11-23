/* *************************************************************
TELVDAT45.CPP - object to read and write the structure of
                  elevation data from/to files used by the Water
                  Balance Model

Modifications:

20060114 - DWK created by modifying telvdat425.cpp
20060114 - DWK added include telvdat437.h and standard includes
20060114 - DWK changed Elevdata:: to Elevdata43::
20070105 - TWC changed name to telvdat45

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


#include "telvdat45.h"



Elevdat45::Elevdat45( void )
{

  elvend = 1;
  lagpos = -99;
  curpos = 0;

};

/* *************************************************************
************************************************************* */

int Elevdat45::get( ifstream& infile )
{

  lagpos = infile.tellg();

  infile >> col;
  infile >> row;
  infile >> varname;
  infile >> carea;
  infile >> elev;
  infile >> contnent;

  infile.seekg( 0, ios::cur );

  curpos = infile.tellg();

  if( curpos < (lagpos + 10) ) { elvend = -1; }

  return elvend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Elevdat45::getdel( FILE* infile )
{
  char tmpvarname[80];
  char tmpcontnent[80];

  elvend = fscanf( infile,
                   "%f,%f, %s ,%ld,%lf, %s",
                   &col,
                   &row,
                   tmpvarname,
                   &carea,
                   &elev,
                   tmpcontnent );

  varname = tmpvarname;
  contnent = tmpcontnent;

  return elvend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Elevdat45::out( ofstream& ofile,
                      const float& col,
                      const float& row,
                      const string& varname,
                      const long& carea,
                      const double& elev,
                      const string& contnent )
{

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ' ';
  ofile << row << ' ';
  ofile << varname << ' ';
  ofile << setprecision( 0 ) << carea << ' ';
  ofile << setprecision( 1 ) << elev << ' ';
  ofile << contnent;
  ofile << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Elevdat45::outdel( ofstream& ofile,
                         const float& col,
                         const float& row,
                         const string& varname,
                         const long& carea,
                         const double& elev,
                         const string& contnent )
{

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ",";
  ofile << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 1 ) << elev << ", ";
  ofile << contnent;
  ofile << endl;

};

