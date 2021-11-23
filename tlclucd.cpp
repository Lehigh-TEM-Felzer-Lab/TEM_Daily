/* **************************************************************
*****************************************************************
TLCLUCD.CPP - determines both potential and "actual" land
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


#include "tlclucd.h"

/* *************************************************************
************************************************************* */

Tlcluc45::Tlcluc45() : Biome45()
{

  cohorts.col = -999.9;
  cohorts.row = -999.9;
  cohorts.total = -99;
  cohorts.natchrts = -99;

  lulc.year = -999;
  lulc.agstate = -99;
  lulc.agprevstate = -99;
  lulc.standage = -99;

  maxtype = -999;

  agcmnt = -999;
  cmnt = -999;

  lastyr = -999;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Tlcluc45::getCohort( FILE* flulc )
{

  int gisend = lulc.getdel( flulc );

  if( -1 == gisend ) { return gisend; }

  return gisend;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Tlcluc45::getNumberOfCohorts( FILE* fnchrts )
{

  int gisend = cohorts.getdel( fnchrts );

  if( -1 == gisend ) { return gisend; }

  return gisend;

};

