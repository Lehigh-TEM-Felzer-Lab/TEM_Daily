/* *************************************************************
TCLMDATD.CPP - object to read and write the structure of the
                  climate data from files used by the
                  Terrestrial Ecosystem Model (TEM)

Modifications:

20060114 - DWK created by modifying tclmdat425.cpp
20060114 - DWK changed long year to int year in
           out(), outdel(), pctout() and pctoutdel()
20060114 - DWK changed char varname[9] to string varname in
           out(), outdel(), pctout() and pctoutdel()
20060114 - DWK changed char contnent[9] to string contnent in
           out(), outdel(), pctout() and pctoutdel()
20060114 - DWK changed Clmdata:: to Clmdata43::
20060114 - DWK added include tclmdat437.h and standard includes
20070105 - TWC changed name to tclmdat45

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


#include "tclmdatd.h"

/* *************************************************************
************************************************************* */

Clmdat45::Clmdat45( void )
{

  clmend = 1;
  lagpos = -99;
  curpos = 0;

};

/* *************************************************************
************************************************************* */

int Clmdat45::get( ifstream& ifile,
                   const int& CYCLE )
{

  int dm;

  lagpos = ifile.tellg();

  ifile >> col;
  ifile >> row;
  ifile >> varname;
  ifile >> carea;
  ifile >> year;

  ifile >> total;
  ifile >> max;
  ifile >> ave;
  ifile >> min;

  for( dm = 0; dm < CYCLE; ++dm ) { ifile >> mon[dm]; }

  ifile >> contnent;

  ifile.seekg( 0, ios::cur );
  curpos = ifile.tellg();

  if( curpos < (lagpos + 10) ) { clmend = -1; }

  return clmend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Clmdat45::getdel( FILE* ifile,
                      const int& CYCLE )
{

  char tempvarname[40];
  char tempcontnent[40];

  clmend = fscanf( ifile,
                   "%f,%f, %s ,%d,%d,%d,%d,%lf, %s",
                   &col,
                   &row,
                   tempvarname,
                   &carea,
                   &year,
                   &month,
                   &day,
                   mon+CYCLE,
                   tempcontnent );

  varname = tempvarname;
  contnent = tempcontnent;

  return clmend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Clmdat45::out( ofstream& ofile,
                     const float& col,
                     const float& row,
                     const string& varname,
                     const int& carea,
                     const int& year,
                     const int& CYCLE,
                     double mon[366],
                     const string& contnent )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;
  if (predtotl <= (MISSING*CYCLE)) { predtotl = MISSING; }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ' ';
  ofile << setprecision( 1 ) << row << ' ';
  ofile << varname << ' ';
  ofile << setprecision( 0 ) << carea << ' ';
  ofile << setprecision( 0 ) << year << ' ';

  ofile << setprecision( 1 ) << predtotl << ' ';
  ofile << setprecision( 1 ) << predmax << ' ';
  ofile << setprecision( 2 ) << predave << ' ';
  ofile << setprecision( 1 ) << predmin << ' ';

  for ( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 1 ) << mon[dm] << ' ';
  }

  ofile << contnent << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Clmdat45::outdel( ofstream& ofile,
                        const float& col,
                        const float& row,
                        const string& varname,
                        const int& carea,
                        const int& year,
                        const int& CYCLE,
                        double mon[366],
                        const string& contnent )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= (MISSING*CYCLE) ) { predtotl = MISSING; }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 4 );

  ofile << col << ",";
  ofile << setprecision( 4 ) << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 0 ) << year << ",";
  ofile << setprecision( 1 ) << predtotl << ",";
  ofile << setprecision( 1 ) << predmax << ",";
  ofile << setprecision( 2 ) << predave << ",";
  ofile << setprecision( 1 ) << predmin << ",";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ",";
  }

  ofile << " " << contnent;
  ofile << endl;

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Clmdat45::outdelm( ofstream& ofile,
                        const float& col,
                        const float& row,
                        const string& varname,
                        const int& carea,
                        const int& year,
                        const int& CYCLE,
                        double mon[12],
                        const string& contnent )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= (MISSING*CYCLE) ) { predtotl = MISSING; }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 4 );

  ofile << col << ",";
  ofile << setprecision( 4 ) << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 0 ) << year << ",";
  ofile << setprecision( 1 ) << predtotl << ",";
  ofile << setprecision( 1 ) << predmax << ",";
  ofile << setprecision( 2 ) << predave << ",";
  ofile << setprecision( 1 ) << predmin << ",";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 1 ) << mon[dm] << ",";
  }

  ofile << " " << contnent;
  ofile << endl;
};

/* *************************************************************
************************************************************* */

void Clmdat45::pctout( ofstream& ofile,
                        const float& col,
                        const float& row,
                        const string& varname,
                        const int& carea,
                        const int& year,
                        const int& CYCLE,
                        double mon[366],
                        const string& contnent )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= (MISSING*CYCLE) ) { predtotl = MISSING; }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ' ';
  ofile << setprecision( 1 ) << row << ' ';
  ofile << varname << ' ';
  ofile << setprecision( 0 ) << carea << ' ';
  ofile << setprecision( 0 ) << year << ' ';

  ofile << setprecision( 2 ) << predtotl << ' ';
  ofile << setprecision( 2 ) << predmax << ' ';
  ofile << setprecision( 2 ) << predave << ' ';
  ofile << setprecision( 2 ) << predmin << ' ';

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ' ';
  }

  ofile << contnent;
  ofile << endl;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Clmdat45::poutdel( ofstream& ofile,
                         const float& col,
                         const float& row,
                         const string& varname,
                         const int& carea,
                         const int& year,
                         const int& CYCLE,
                         double mon[366],
                         const string& contnent )
{

  int dm;
  double predtotl;
  double predmax;
  double predave;
  double predmin;

  predtotl = 0.0;
  predmax   = -900000.0;
  predmin   =  900000.0;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    if( mon[dm] > predmax ) { predmax = mon[dm]; }

    if( mon[dm] < predmin ) { predmin = mon[dm]; }

    predtotl += mon[dm];
  }

  predave = predtotl / (double) CYCLE;

  if( predtotl <= (MISSING*CYCLE) ) { predtotl = MISSING; }

  ofile.setf( ios::fixed,ios::floatfield );
  ofile.setf( ios::showpoint );
  ofile.precision( 1 );

  ofile << col << ",";
  ofile << setprecision( 1 ) << row << ", ";
  ofile << varname << " ,";
  ofile << setprecision( 0 ) << carea << ",";
  ofile << setprecision( 0 ) << year << ",";

  ofile << setprecision( 2 ) << predtotl << ",";
  ofile << setprecision( 2 ) << predmax << ",";
  ofile << setprecision( 2 ) << predave << ",";
  ofile << setprecision( 2 ) << predmin << ",";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofile << setprecision( 2 ) << mon[dm] << ",";
  }

  ofile << " " << contnent;
  ofile << endl;

};
