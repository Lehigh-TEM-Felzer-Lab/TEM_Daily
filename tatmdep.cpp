/* **************************************************************
*****************************************************************
TATMDEP.CPP - determines both potential and "actual" land
                 cover, and identifies land uses within a grid
                 cell

20031019 - DWK created by modifying tlcluc431b.cpp
20030624 - DWK changed TEMlcluc43:: to TEMlcluc51::
20030624 - DWK changed inheritance of Biome43() to Biome51()
20040714 - DWK changed TEMlcluc51:: to TEMlcluc60::
20040714 - DWK changed inheritance of Biome43() to Biome60()
20040716 - DWK added getCohorts(), getVegtype() and initCohorts()
20040716 - DWK added cohorts.col, cohorts.row, cohorts.total,
           and vegtype.temveg to TEMlcluc60()
20040716 - DWK deleted potveg.col, potveg.row and potveg.temveg
           from TEMlcluc60()
20040716 - DWK deleted getPotentialVeg() and  initPotentialVeg()
20040828 - DWK renamed public getCohorts() to be
           getNumberOfCohorts()
20040829 - DWK renamed initCohorts() as initMaxCohorts()
20040829 - DWK renamed initLandUse() as initCohorts()
20040829 - DWK renamed getLandUse() as getCohort()
20070105 - TWC changed name to tlcluc45

*****************************************************************
************************************************************** */

#include<cstdio>

  using std::fscanf;
  using std::FILE;

#include<iostream>

  using std::cout;
  using std::ios;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setprecision;

#include<string>

  using std::string;


#include "tatmdep.h"

/* *************************************************************
************************************************************* */

Tatmdep::Tatmdep()
{

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Tatmdep::getAtmdep( FILE* fatmdep )
{

  int gisend = atmdep.getdel( fatmdep );

  if( -1 == gisend ) { return gisend; }

  return gisend;

};

/* *************************************************************
************************************************************* */
