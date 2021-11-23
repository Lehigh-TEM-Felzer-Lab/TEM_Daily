/*
  TWC/BSF 2007-2009

 MAXCHRTS: change from 650 to 300
 MAXCMNT: from 13 to 17
 MAXRTIME: from 1100 to 900
 MAXWSTAT: from 3 to 5
 NUMATMS: from 10 to 18

*/

#ifndef ACCEPT_CONST
#define ACCEPT_CONST
  const int ACCEPT = 0;
#endif

// TEMVEG index of crops
#ifndef CROPVEG_CONST
#define CROPVEG_CONST
  const int CROPVEG = 50;
#endif

// Number of months in an annual cycle
//#ifndef CYCLE_CONST
//#define CYCLE_CONST
//  const int CYCLE = 12;
//#endif


// Maximum number of cohorts in a grid cell
#ifndef MAXCHRTS_CONST
#define MAXCHRTS_CONST
  const int MAXCHRTS = 1000;
#endif

// Maximum number of community types for TEM parameters
#ifndef MAXCMNT_CONST
#define MAXCMNT_CONST
  const int MAXCMNT = 21;
#endif

// Maximum number of ecosystem (i.e. carbon and nitrogen)
//   pool variables in TEM
#ifndef MAXESTAT_CONST
#define MAXESTAT_CONST
  const int MAXESTAT = 18;
#endif

// Maximum value of Fire Return Interval (years)
#ifndef MAXFRI_CONST
#define MAXFRI_CONST
  const int MAXFRI = 2000;
#endif

#ifndef MAXGRID_CONST
#define MAXGRID_CONST
  const int MAXGRID = 1;
#endif

// Maximum number of years in a simulation
#ifndef MAXRTIME_CONST
#define MAXRTIME_CONST
  const int MAXRTIME = 900;
#endif

// Maximum number of water pool variables
#ifndef MAXWSTAT_CONST
#define MAXWSTAT_CONST
  const int MAXWSTAT = 7;
#endif

// Default value for missing data
#ifndef MISSING_CONST
#define MISSING_CONST
  const double MISSING = -999999.9;
#endif

// Total number of TEMCLM output variables
#ifndef NUMATMS_CONST
#define NUMATMS_CONST
const int NUMATMS = 18;
#endif

// Maximum number of vegetation subtypes in a mosaic
#ifndef NUMMSAC_CONST
#define NUMMSAC_CONST
  const int NUMMSAC = 5;
#endif

// Maximum number of vegetation types
#ifndef NUMVEG_CONST
#define NUMVEG_CONST
  const int NUMVEG = 56;
#endif

// TEMVEG index of pasture
#ifndef PASTURE_CONST
#define PASTURE_CONST
  const int PASTURE = 51;
#endif

#ifndef REJECT_CONST
#define REJECT_CONST
  const int REJECT = 1;
#endif

// TEMVEG index of urban areas
#ifndef URBAN_CONST
#define URBAN_CONST
  const int URBAN = 40;
#endif

// constant indicating row number for display purposes in ctem
#ifndef VPARAMS_ROW_CONST
#define VPARAMS_ROW_CONST
  const int VPARAMS_ROW = 32;
#endif

#ifndef ZERO_CONST
#define ZERO_CONST
  const double ZERO = 0.000000;
#endif

/* *************************************************************
     Constants that are combinations of other constants
************************************************************* */

// Maximum number of pool variables in simulation
#ifndef MAXSTATE_CONST
#define MAXSTATE_CONST
  const int MAXSTATE = MAXWSTAT + MAXESTAT;
#endif

// Maximum number of state (pools plus fluxes) variables
//  used to simulate hydrology in TEM
#ifndef NUMWEQ_CONST
#define NUMWEQ_CONST
  const int NUMWEQ = MAXWSTAT + 15;
#endif

// Maximum number of state (pools plus fluxes) variables
//  used to simulate carbon and nitrogen dynamics in TEM
#ifndef NUMEEQ_CONST
#define NUMEEQ_CONST
  const int NUMEEQ = MAXESTAT + 67;
#endif

// Total number of state (pools plus fluxes variables
//  (carbon, nitrogen and water) used in TEM
#ifndef NUMEQ_CONST
#define NUMEQ_CONST
  const int NUMEQ = NUMWEQ + NUMEEQ;
#endif

// Total number of output variables from TEM
#ifndef NUMTEM_CONST
#define NUMTEM_CONST
  const int NUMTEM = NUMEQ + 99;
#endif

