/* *************************************************************
TELVDAT45.H - object to read and write the structure of
                 elevation data from/to files used by the Water
                 Balance Model

Modifications:

20060114 - DWK created by modifying telvdat425.h
20060114 - DWK changed class Elevdata to class Elevdata43
20060114 - DWK deleted telvdat425.cpp from bottom of file
20070105 - TWC changed name to telvdat45

****************************************************************
************************************************************* */

#ifndef TELVDAT45_H
#define TELVDAT45_H

class Elevdat45
{

  public:

     Elevdat45( void );

/* *************************************************************
                      Public Functions
************************************************************* */

     // read data structure.
     int get( ifstream& infile );

     int getdel( FILE* infile );

     //write data structure.
     void out( ofstream& ofile,
               const float& col,
               const float& row,
               const string& varname,
               const long& carea,
               const double& elev,
               const string& contnent );

     void outdel( ofstream& ofile,
                  const float& col,
                  const float& row,
                  const string& varname,
                  const long& carea,
                  const double& elev,
                  const string& contnent );


/* *************************************************************
                     Public Variables
************************************************************* */

     // column or longitude of grid cell (degrees)
     float col;

     // row or latitude of grid cell (degrees)
     float row;

     // "ELEV"
     string varname;

     // area covered by grid cell (sq. km)
     long carea;

     // elevation of grid cell (m)
     double elev;

     // name of continent containing grid cell
     string contnent;


  private:

/* *************************************************************
                      Private Variables
************************************************************* */

     int elvend;
     long curpos;
     long lagpos;

};

#endif

