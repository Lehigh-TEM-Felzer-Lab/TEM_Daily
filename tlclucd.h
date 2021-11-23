/* *************************************************************
****************************************************************
TLCLUCD.H - determines both potential and "actual" land
               cover, and identifies land uses within a grid
               cell

20030624 - DWK created by modifying tlcluc431b.h
20031019 - DWK changed include from tbiome431a.h to tbiome51.h
20031019 - DWK changed Class TEMlcluc43 to class TEMlcluc51
20031019 - DWK changed inheritance of Biome43 to Biome51
20031019 - DWK changed char ilulcfname[MAXFNAME] and
           char ilulcend[25] to string ilulcfname and
           string ilulcend, respectively
20031019 - DWK changed include from tlcluc43.cpp to tlcluc51.cpp
           at bottom of file
20040714 - DWK changed include from tbiome51.h to tbiome60.h
20040714 - DWK changed class TEMlcluc51 to class TEMlcluc60
20040714 - DWK changed inheritance of Biome51 to inheritance of
           Biome60
20040714 - DWK changed include from tlcluc51.cpp to tlcluc60.cpp
           at bottom of file
20040716 - DWK added global const int MXFRF = 2000;
20040716 - DWK added include tcohordat433.h
20040716 - DWK changed include from tvegdat425.h to tvegdat433.h
20040716 - DWK changed include from lulcdat425.h to lulcdat433.h
20040716 - DWK added public functions getCohorts(), getVegtype()
           and initCohorts()
20040716 - DWK added public int backcastflag, nt backcastyears,
           Cohortdata43 cohorts, int endyr, FILE* frandom,
           int FRF, FILE* ifcohrts, FILE* ifvtype,
           string ivegfname, string ivegend, double prod10par,
           double prod100par, double sconvert, double slashpar,
           int startyr, long subarea, double vconvert,
           Vegdata43 vegtype and double vrespar
20040716 - DWK deleted FILE* ifpotveg and Vegdata potveg
20040716 - DWK changed  Lulcdata lulc to  Lulcdata43 lulc
20040716 - DWK deleted public function getPotentialVeg() and
           initPotentialVeg()
20040828 - DWK renamed public getCohorts() to be
           getNumberOfCohorts()
20040829 - DWK changed include from tcohortdat433.h to
           tmaxcohortdat60.h
20040829 - DWK renamed initCohorts() as initMaxCohorts()
20040829 - DWK renamed initLandUse() as initCohorts()
20040829 - DWK renamed getLandUse() as getCohort()
20040829 - DWK changed include from lulcdat433.h to
           lulcdat60.h
20040829 - DWK changed Lulcdata43 lulc to Lulcdata60 lulc
20040830 - DWK added public string imxcohrtend and
           string imxcohrtfname
20050622 - DWK changed include from tmaxcohortdat60.h to
           tmaxcohortdat601.h
20050622 - DWK changed include from lulcdat60.h to lulcdat601.h
20050622 - DWK changed include from tlcluc60.cpp to
           tlcluc601.cpp at bottom of file
20051117 - DWK changed include from tmaxcohortdat601.h to
           tmaxcohort602.h
20051117 - DWK changed include from tvegdat433.h to tvegdat602.h
20051117 - DWK changed include from lulcdat601.h to lulcdat602.h
20051117 - DWK changed include from tbiome60.h to tbiome602.h
20051117 - DWK changed public Vegdata43 vegtype to
           Vegdata60 vegtype
20051117 - DWK deleted tlcluc601.cpp from bottom of file
20060607 - DWK changed include from tmaxcohortdat602.h to
           tmaxcohort437.h
20060607 - DWK changed include from lulcdat602.h to lulcdat44.h
20070105 - TWC changed name to tlcluc45

****************************************************************
************************************************************* */

#ifndef TLCLUCD_H
#define TLCLUCD_H

const int MXFRI = 2000;

// Tlcluc45 uses the MaxCohortdat45 class
#include "tmaxcohortdat45.h"

// Tcluc45 uses the Lulcdat45 class
#include "lulcdat45a.h"

// Tlcluc45 inherits the Biome45 class
#include "tbiomed.h"

class Tlcluc45 : public Biome45
{

  public:

     Tlcluc45();

 /* ************************************************************
		 Public Functions
************************************************************* */

     int getCohort( FILE* flulc );

     int getNumberOfCohorts( FILE* fnchrts );

/* *************************************************************
		 Public Variables
************************************************************* */

     int agcmnt;

     MaxCohortdat45 cohorts;

     int currentveg;

     int endyr;

     FILE* ifnumcohrts;

     FILE* iflulc;

     string ilulcend;
     string ilulcfname;

     string imxcohrtend;
     string imxcohrtfname;

     int lastyr;

     Lulcdat45 lulc;

     int maxtype;

     int potveg;

     int startyr;

     // portion of carea in a particular cohort, km^2
     long subarea;

     int tlulcflag;

};

#endif
