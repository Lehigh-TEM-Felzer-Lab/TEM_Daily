/* **************************************************************
*****************************************************************
TELMD.H - Determines the interface among CLM, LCLUC and TEM
             modules

Modifications:

20060127 - DWK created by modifying telm50b5.h
20060127 - DWK changed include from tclm50b5.h to tclm437.h
20060127 - DWK changed include from telmntcohort50b5.hpp to
           telmntcohort437.hpp
20060127 - DWK changed include from tlcluc50b5.h to tlcluc437.h
20060127 - DWK changed include from ttem50b5.h to ttem437.h
20060127 - DWK changed include from tclmdat50b5.h to tclmdat437.h
20060127 - DWK changed include from tsoldat50b5.h to tsoldat437.h
20060127 - DWK changed include from telvdat50b5.h to telvdat437.h
20060127 - DWK changed include from ttemdat50b5.h to ttemdat437.h
20060127 - DWK changed Class TEMelmnt50 to Class TEMelmnt43
20060127 - DWK changed public TEMclm50 clm to TEMclm43 clm
20060127 - DWK changed public ElmntCohort50 cohort[MAXCHRTS] to
           ElmntCohort43
20060127 - DWK changed public TEMlcluc50 lcluc to
           TEMlcluc43 lcluc
20060127 - DWK changed public TTEM50 tem to TTEM43 tem
20060127 - DWK added const double& aot40 to private function
            int temgisqc()
20070105 - TWC modified name to telm45

*****************************************************************
************************************************************** */

#ifndef TELMD_CWD_H
#define TELMD_CWD_H

const int TQCZEROFLAG = 31;

//Modules representing climate and TEM

#include "tatmdep.h"
#include "tclmd.h"   // Telm45 uses the Tclm45 class

// Telm45 uses the Elmntcohort45
#include "telmntcohortd.hpp"

#include "tlclucd.h" // Telm45 uses the Tlcluc45 class
#include "ttemd_cwd.h"   // Telm45 uses the Ttem45 class

// Modules describing the interface with spatially explicit
//   data sets

#include "tclmdatd.h"  //Telm45 uses the Clmdat45 class
#include "tsoldat45.h"  //Telm45 uses the Soildat45 class
#include "telvdat45.h"  //Telm45 uses the Elevdat45 class
#include "ttemdatd.h"  //Telm45 uses the Temdat45 class

class Telm45
{

  public:

     Telm45();


/* *************************************************************
		 Public Function Declarations
************************************************************* */

     void atmswritemiss( ofstream fout[NUMATMS],
                         const vector<string>& predname,
                         const int& pdyr,
                         const int& atmspred,
                         const double value,
                         const int& CYCLE );

     void atmswritepred( ofstream fout[NUMATMS],
                         const int& pdyr,
                         const vector<string>& predname,
                         const int& atmspred,
                         const int& CYCLE );

     void atmswritepredm( ofstream fout[NUMATMS],
                         const int& pdyr,
                         const vector<string>& predname,
                         const int& atmspred,
                         const int& CYCLE );

     int coregerr( ofstream& rflog1,
                   const string& varname1,
                   const float& col1,
		           const float& row1,
                   const string& varname2,
                   const float& col2,
		           const float& row2 );

     int equilibrateTEM( const double& ptol,
                        const int& pichrt,
                        ofstream fout[NUMTEM],
                        const vector<string>& predname,
                        const int& spinoutfg,
                        int & year,
                        const int& CYCLE,
                        const int ndays[] );

     void getTEMCohortState( const int& pichrt,
                             const int& CYCLE );

     void initializeCohortTEMState( const int& pichrt,
                                    const int& CYCLE );

     void outputTEMday( const int& pdm );

     void outputTEMmonth( const int& pdm,
                          const int& CYCLE );

     void readCohortState( ifstream& ifstate,
                           const int& pichrt,
                           const int& CYCLE );

     void saveTEMCohortState( const int& pichrt,
                              const int& CYCLE );

     void setCohortTEMState( const ElmntCohort45& firstchrt,
                             ElmntCohort45& targetchrt,
                             const int& CYCLE );

     int setGIStopography( ofstream& flog1,
                           int& ftlerr,
                           FILE* fstxt,
                           FILE*felev );

     void setTEMequilState( ofstream& rflog1,
                            const int& equil,
                            const int& totsptime,
                            const int& pichrt,
                            ofstream fout[NUMTEM],
                            const vector<string>& predname,
                            const int& CYCLE,
                            const int& spinoutfg,
                            const int ndays[] );

     void setTEMmiss( const int& pdyr,
                      const int& equil,
                      const int& totsptime,
                      const int& pichrt,
                      const int& CYCLE );

     void temwritepred( ofstream fout[NUMTEM],
                        const vector<string>& predname,
                        const int& pdyr,
                        const int& pichrt,
                        const int& ntempred,
                        const int& spinoutfg,
                        const int& equilflg,
                        const int& CYCLE,
                        const int ndays[12] );

     void temwritepredm( ofstream fout[NUMTEM],
                        const vector<string>& predname,
                        const int& pdyr,
                        const int& pichrt,
                        const int& ntempred,
                        const int& spinoutfg,
                        const int& equilflg,
                        const int& CYCLE,
                        const int ndays[12] );

     void updateTEMmonth( const int& equil,
                          const int& totsptime,
                          const int& outyr,
                          const int& pdm,
                          const int& pichrt, 
                          const int& CYCLE );

     void writeCohortState( ofstream& ofstate,
                            const int& pichrt,
                            const int& CYCLE );

/* *************************************************************
		 Public Variables
************************************************************* */

     int atmstotyr[MAXRTIME];

     // Mean annual air temperature
     double avetair[MAXRTIME];

     long carea;

     double climate[NUMATMS][366][MAXRTIME];
     double adep[1][MAXRTIME];

     Tclm45 clm;

     ElmntCohort45 cohort[MAXCHRTS];

     float col;

     string contnent;
     
     int de_nyears;
     int de_startyear;

     int fatalerr;

     double lat;

     Tlcluc45 lcluc;

     Tatmdep atmdep;

     double lon;

     int lonlatflag;

     // Maximum number of cohorts in an element
     //   during the current year
     int maxcohorts;

     // Maximum monthly air temperature during the year
     double mxtair[MAXRTIME];

     // Maximum number of "natural" cohorts in an element
     int natcohorts;

     // Number of climate predicted variables
     int natmspred;

     // Number of TEM predicted variables
     int ntempred;

     // Interval for Output Data
     int intervalflag;

//     vector<string> predstr;

     double output[NUMTEM][366];
     double outputm[NUMTEM][12];
     double outpute[5][12];

     int outyr;

     // Maximum number of cohorts in an element
     //   during the previous year
     int prvmxcohrts;

     string region;

     float row;

     long subarea;

     Ttem45 tem;

     int totpred;

     int ttotyr[MAXRTIME];

     int wrtyr;

     // Annual precipitation during the year
     double yrprec[MAXRTIME];

     int year;

     double vegceq[MAXRTIME];
     double soilceq[MAXRTIME];
     double vegneq[MAXRTIME];
     double soilneq[MAXRTIME];


/* *************************************************************
		 Private Function Declarations
************************************************************* */

  private:

     int temgisqc( const long& subarea,
		           const double& pctsand,
                   const double& pctsilt,
                   const double& pctclay,
                   const int& cmnt,
                   const double& elev,
                   const double& nirr,
                   const double& par,
		           const double& tair,
                   const double& mxtair,
                   const double& avtair,
                   const double& yrprec,
                   const double& rain,
                   const double& snowfall,
                   const double& co2,
                   const double& aot40,
                   const double& ndep,
				   const double& trange,
				   const double& vpd );

     int transqc( int& maxyears,
                  int& totyr,
                  const int& CYCLE,
                  double plantc[366] );

};

#endif
