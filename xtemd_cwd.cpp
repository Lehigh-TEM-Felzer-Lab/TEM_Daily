/* *************************************************************
XTEMD_CWD.CPP - Extrapolation version of the Terrestrial Ecosystem
               Model Version 4.5
****************************************************************


Modifications:

20060127 - DWK created by modifying xtem50b5.cpp
20060127 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20060127 - DWK changed include from tclmdat50b5.h to
           tclmdat437.h
20060127 - DWK changed include from tco2dat50b5.h to
           tco2dat437.h
20060127 - DWK changed include from elmnt50b5.h to elmnt437.h
20060127 - DWK changed include from latdat50b5.h to latdat437.h
20060127 - DWK changed include from telm50b5.h to telm437.h
20060127 - DWK changed global Elmnt50 elmnt to Elmnt43 elmnt
20060127 - DWK changed global TEMelmnt50 telmnt[MAXGRID] to
           TEMelmnt43 telmnt[MAXGRID]
20060127 - DWK changed global Clmdata50 girrdat[MAXRTIME] to
           Clmdata43 girrdat[MAXRTIME]
20060127 - DWK changed global Clmdata50 cldsdat[MAXRTIME] to
           Clmdata43 cldsdat[MAXRTIME]
20060127 - DWK changed global Clmdata50 nirrdat[MAXRTIME] to
           Clmdata43 nirrdat[MAXRTIME]
20060127 - DWK changed global Clmdata50 pardat[MAXRTIME] to
           Clmdata43 pardat[MAXRTIME]
20060127 - DWK changed global Clmdata50 tairdat[MAXRTIME] to
           Clmdata43 tairdat[MAXRTIME]
20060127 - DWK changed global Clmdata50 precdat[MAXRTIME] to
           Clmdata43 precdat[MAXRTIME]
20060127 - DWK changed global CO2data50 co2dat[MAXRTIME+1] to
           CO2data43 co2dat[MAXRTIME+1]
20060127 - DWK changed global
           MaxCohortdata50 mxcohrtdat[MAXRTIME] to
           MaxCohortdata43 mxcohrtdat[MAXRTIME]
20060127 - DWK changed global
           Lulcdata50 lulcdat[MAXRTIME][MAXCHRTS] to
           Lulcdata43 lulcdat[MAXRTIME][MAXCHRTS]
20060127 - DWK added global Clmdata43 o3dat[MAXRTIME]
20060127 - DWK added global FILE* ifo3
20070105 - TWC changed name to xtem45

*****************************************************************
************************************************************** */

#define ANSI_CPP
#define EXTRAPOLATE_TEM

//#define BORLAND_CPP

//#define DEBUGX

#include<cstdio>

  using std::fopen;
  using std::fclose;
  using std::printf;
  using std::sprintf;
  using std::fscanf;
  using std::FILE;

#include<iostream>

  using std::cout;
  using std::ios;
  using std::cerr;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setiosflags;
  using std::setw;
  using std::setprecision;

#include<cstdlib>

  using std::exit;

#include<cmath>

  using std::exp;
  using std::sin;

#include<vector>

  using std::vector;

#include<cctype>

  using std::toupper;

#include<cstring>

#include<string>

  using std::string;

#include<sstream>

  using std::ostringstream;

#ifdef ANSI_CPP

  #include<ctime>

  using std::time_t;
  using std::ctime;

#endif

#ifdef BORLAND_CPP

  #include<time>

  using std::time_t;
  using std::ctime;

#endif


#include "temconstsd.hpp"   // Global constants
#include "tclmdatd.h"       // Clmdat45 class
#include "tco2datd.h"       // CO2dat45 class
#include "elmnt45.h"         // Elmnt45 Class
#include "latdat45.h"        // Latdat45 class
#include "telmd_cwd.h"          // Telm45 Class

void initializeCLMGridCell( void );
void initializeLCLUCGridCell( void );
void initializeTEMGridCell( const int& CYCLE, const int ndays[] );
void initRun( void );
void updateTCLMGridCell( const int& pdyr,  const int& CYCLE, const int ndays[], const int& eqtr  );
void updateTLCLUCGridCell( const int& pdyr, const int& eqtr );
void updateTTEMGridCell( const int& pdyr, const int& CYCLE, const int ndays[] );

ofstream flog1;

Elmnt45 elmnt;
Telm45 telmnt[MAXGRID];

int CYCLE;
int ndays[12];
int year;
int i,j;
double sumdays;
int equil;
int RTIME;

long mxnumgrid;

int spinflag;
int spinoutfg;
int spinoutyrs;
int numspin;
int spintime;
int totsptime;
int transtime;

int temflag;
int istateflag;
int istateyear;
int ostateflag;
int ostateyear;

vector<string> clmpredmap( NUMATMS );
vector<string> tempredmap( NUMTEM );

int fatalerr;
int end1;
int icount;
int glob_count;

Clmdat45 girrdat[MAXRTIME];
Clmdat45 cldsdat[MAXRTIME];
Clmdat45 nirrdat[MAXRTIME];
Clmdat45 pardat[MAXRTIME];
Clmdat45 tairdat[MAXRTIME];
Clmdat45 precdat[MAXRTIME];
Clmdat45 trangedat[MAXRTIME];
Clmdat45 vprdat[MAXRTIME];
Clmdat45 ws10dat[MAXRTIME];

CO2dat45 co2dat[MAXRTIME+1];
Clmdat45 o3dat[MAXRTIME];

MaxCohortdat45 mxcohrtdat[MAXRTIME];
Lulcdat45 lulcdat[MAXRTIME][MAXCHRTS];
Atmdepdat ndepdat[MAXRTIME];

FILE* flonlat;

FILE* ifgirr;
FILE* ifnirr;
FILE* ifpar;
FILE* ifclds;
FILE* iftair;
FILE* ifprec;
FILE* iftrange;
FILE* ifvpr;
FILE* ifws10;
//ifstream ifco2;
FILE* ifco2;
FILE* ifo3;

FILE* ifnumchrts;
FILE* iflulc;
FILE* ifndep;

FILE* fstxt;
FILE* felev;

ifstream ifstate;  // Use TEMstate from a specified year
ofstream ofstate;  // Save TEMstate for a specified year
ofstream fclmpred[NUMATMS];
ofstream ftempred[NUMTEM];

int assignCO2 = 0;

/* *************************************************************
**********************START MAIN PROGRAM************************
************************************************************* */

int main()
{

cout << "starting program" << endl;
  int xdyr;
  int i;

  int eqtr;

  long grdcnt;

  /// Master input routine initRun reads in all relevant information for the specific run 
  /// from the file pointed to by tem_in.txt; using xml methods
   
cout << "entering initRun" << endl;
  initRun();

  
  telmnt[0].col = MISSING;
  telmnt[0].row = MISSING;
  telmnt[0].tem.totyr = -99;

  cout << endl;
  flog1 << endl << endl;

  #ifdef DEBUGX
    cout << " tem initialized " << endl;
  #endif  

  elmnt.show( flog1,
              telmnt[0].col,
              telmnt[0].row,
              telmnt[0].tem.totyr,
              telmnt[0].tem.inittol,
              telmnt[0].tem.veg.getERRCNT() );


  // Extrapolate TEM across region

  grdcnt = 0;

  while( grdcnt < mxnumgrid && 0 == fatalerr )   // Grid cell loop
  {

    // Load grid cell climate data into one node of CLM linked list

    for( xdyr = 0; xdyr < RTIME; ++xdyr )
    {

//    year = xdyr + telmnt[0].clm.startyr-1;
    year = xdyr + telmnt[0].clm.startyr;

    ndays[0] = 31;
    if((year%4) == 0.0)
    {
    ndays[1] = 29;
    }
    else
    {
    ndays[1] = 28;
    }
    ndays[2] = 31;
    ndays[3] = 30;
    ndays[4] = 31;
    ndays[5] = 30;
    ndays[6] = 31;
    ndays[7] = 31;
    ndays[8] = 30;
    ndays[9] = 31;
    ndays[10] = 30;
    ndays[11] = 31;

    sumdays = 0.0;
    for (i=0; i<12; i++)
    {
    sumdays += ndays[i];
    }
    CYCLE = sumdays;


//    for (j=0; j < sumdays; j++)
      eqtr = 0;

      updateTCLMGridCell( xdyr, CYCLE, ndays, eqtr ); 
  // Copy TEMclm results to output variables

  if( 1 == telmnt[0].clm.predflag )
  {
    if ( (1 == spinoutfg
         && telmnt[0].year < telmnt[0].clm.startyr)
         || (2 == spinoutfg
         && telmnt[0].year >= (telmnt[0].clm.startyr-spinoutyrs))
         || (telmnt[0].year >= telmnt[0].clm.startyr) 
         || (0 == spinoutfg) )
    {
      if( 1 == telmnt[0].clm.cldflag )
      {
        telmnt[0].carea = cldsdat[0].carea;
        telmnt[0].contnent = cldsdat[0].contnent;
      }
      else
      {
        telmnt[0].carea = nirrdat[0].carea;
        telmnt[0].contnent = nirrdat[0].contnent;
      }

       if(telmnt[0].intervalflag == 0)
       {
       telmnt[0].atmswritepred( fclmpred,
                               xdyr,
                               clmpredmap,
                               telmnt[0].natmspred,
                               CYCLE );
       }
       else if(telmnt[0].intervalflag == 1 )
       {
       telmnt[0].atmswritepredm( fclmpred,
                               xdyr,
                               clmpredmap,
                               telmnt[0].natmspred,
                               CYCLE );
       }
    }
  } 

      // load all years of climate data into telm.climate[][][] variable
//    } // end of day loop
    } // end of year loop

    #ifdef DEBUGX
      cout << " after updateTCLMGridCell " << endl;
    #endif  

    // Determine number of land use/land cover cohorts in a
    //   grid cell and load land cover data into cohorts of
    //   LULC linked list

    updateTLCLUCGridCell( 0, 0 );

    #ifdef DEBUGX
      cout << " after updateTLCLUCGridCell " << endl;
    #endif  

    // Initialize TEM to equilibrium conditions for all cohorts
    // using the baseline climate if starting from calibration data
    // (i.e. istateflag == 0) or read in initial conditions from
    // temstate file


//    if(((telmnt[0].clm.startyr-1)%4) == 0.0)
    if(((telmnt[0].clm.startyr)%4) == 0.0)
    {
    ndays[1] = 29;
    }
    else
    {
    ndays[1] = 28;
    }
    ndays[2] = 31;
    ndays[3] = 30;
    ndays[4] = 31;
    ndays[5] = 30;
    ndays[6] = 31;
    ndays[7] = 31;
    ndays[8] = 30;
    ndays[9] = 31;
    ndays[10] = 30;
    ndays[11] = 31;

    sumdays = 0.0;
    for (i=0; i<12; i++)
    {
    sumdays += ndays[i];
    }

    CYCLE = sumdays;


    initializeTEMGridCell(CYCLE, ndays);
    #ifdef DEBUGX
      cout << " after initializeTEMGridCell " << endl;
    #endif  

    // Begin simulation of transient climate and terrestrial
    //   ecosystem response

    if( 0 == equil )
    {

//      for( xdyr = 1; xdyr < RTIME; ++xdyr )
      for( xdyr = 0; xdyr < RTIME; ++xdyr )
      {
    
        year = xdyr + telmnt[0].clm.startyr;
        cout << "year = " << year << " " << xdyr << endl;

    ndays[0] = 31;
    if((year%4) == 0.0)
    {
    ndays[1] = 29;
    }
    else
    {
    ndays[1] = 28;
    }
    ndays[2] = 31;
    ndays[3] = 30;
    ndays[4] = 31;
    ndays[5] = 30;
    ndays[6] = 31;
    ndays[7] = 31;
    ndays[8] = 30;
    ndays[9] = 31;
    ndays[10] = 30;
    ndays[11] = 31;

    sumdays = 0.0;
    for (i=0; i<12; i++)
    {
    sumdays += ndays[i];
    } 

    CYCLE = sumdays;

  
//if(istateflag == 0)
//{
//    for (j=0; j < sumdays; j++)
//    {
//        cout << "j = " << j << endl;

        eqtr = 1;
        updateTCLMGridCell( xdyr, CYCLE, ndays, eqtr );
        
//    }
        // Run land cover module or read in land cover data from file
        // to update land cover characteristics for grid cell during year "dyr"

        updateTLCLUCGridCell( xdyr, eqtr );

//}
        // Run TEM for grid cell during year "dyr"

        updateTTEMGridCell( xdyr, CYCLE, ndays );

      }
//     }  // end of day loop
    }  // end of year loop

    elmnt.show( flog1,
                telmnt[0].col,
                telmnt[0].row,
                telmnt[0].tem.totyr,
                telmnt[0].tem.tol,
                telmnt[0].tem.veg.getERRCNT() );

    ++grdcnt;
  } // end of grid cell loop


  if( 0 == fatalerr )
  {
    cout << "Extrapolation successfully completed - Congratulations!" << endl;
    flog1 << "Extrapolation successfully completed - Congratulations!" << endl;
  }
  else
  {
    if( elmnt.grdcnt != -99 && elmnt.count <= elmnt.grdcnt )
    {
      cout << "FATAL ERROR! Program Terminated" << endl;
    }
    flog1 << "FATAL ERROR! Program Terminated" << endl;
  }

  // Finished processing all elements - close open files

  if( 0 == telmnt[0].lonlatflag ) { fclose( flonlat ); }

  if( 1 == telmnt[0].clm.predflag )
  {
    for( i = 0; i < telmnt[0].natmspred; ++i )
    {
      fclmpred[i].close();
    }
  }


  if( 1 == telmnt[0].clm.cldflag ) { fclose( ifclds ); }
  else { fclose( ifnirr ); }

  if( 0 == telmnt[0].clm.sradflag ) { fclose( ifgirr ); }

  if( 1 == telmnt[0].clm.parflag ) { fclose( ifpar ); }

  fclose( iftair );
  fclose( ifprec );

//  ifco2.close();

  fclose( ifco2 );
  fclose( ifvpr );
  fclose( iftrange );
  fclose( ifws10 );

  fclose( ifo3 );

  if( 1 == temflag )
  {
    fclose( ifnumchrts );
    fclose( iflulc );
    fclose( ifndep );

    fclose( fstxt );
    fclose( felev );

    for( i = 0; i < telmnt[0].ntempred; ++i )
    {
      ftempred[i].close();
    }

    if( istateflag != 0 ) { ifstate.close(); }
    if( ostateflag != 0 ) { ofstate.close(); }
  }

  cout << "Closed all files!" << endl << endl;
  flog1 << "Closed all files!" << endl << endl;
  
  //telmnt[0].tem.gofile.close();

  flog1.close();

  return 0;

};

/* *************************************************************
******************** End of Main *******************************
************************************************************* */


/* *************************************************************
************************************************************* */

void initializeCLMGridCell()
{

  ostringstream tempfname;
  string girrname;
  string cldsname;
  string nirrname;
  string parname;
  string tairname;
  string precname;
  string vprname;
  string trangename;
  string ws10name;
  string co2name;
  string o3name;
  string ndepname;


  // Open cloudiness or solar radiation file

  if( 0 == telmnt[0].clm.sradflag )
  {
    tempfname.str( "" );

    tempfname << telmnt[0].clm.igirrfname
              << telmnt[0].clm.igirrend;

    girrname = tempfname.str();

    ifgirr = fopen( girrname.c_str(), "r" );

    if( !ifgirr )
    {
      flog1 << endl << "Cannot open " << girrname;
      flog1 << " for GIRR data input" << endl << endl;

      exit( -1 );
    }
  }

  if( 1 == telmnt[0].clm.cldflag )
  {
    tempfname.str( "" );

    tempfname << telmnt[0].clm.icldsfname
              << telmnt[0].clm.icldsend;

    cldsname = tempfname.str();

    ifclds = fopen( cldsname.c_str(), "r" );

    if( !ifclds )
    {
      flog1 << endl << "Cannot open " << cldsname;
      flog1 << " for CLDS data input" << endl << endl;

      exit( -1 );
    }
  }
  else
  {
    tempfname.str( "" );

    tempfname << telmnt[0].clm.inirrfname
              << telmnt[0].clm.inirrend;

    nirrname = tempfname.str();

    ifnirr = fopen( nirrname.c_str(), "r" );

    if( !ifnirr )
    {
      flog1 << endl << "Cannot open " << nirrname;
      flog1 << " for NIRR data input" << endl << endl;

      exit( -1 );
    }
  }

  if( 1 == telmnt[0].clm.parflag )
  {
    tempfname.str( "" );

    tempfname << telmnt[0].clm.iparfname
              << telmnt[0].clm.iparend;

    parname = tempfname.str();

    ifpar = fopen( parname.c_str(), "r" );

    if( !ifpar )
    {
      flog1 << endl << "Cannot open " << parname;
      flog1 << " for PAR data input" << endl << endl;

      exit( -1 );
    }
  }


  // Open air temperature file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.itairfname
            << telmnt[0].clm.itairend;

  tairname = tempfname.str();

  iftair = fopen( tairname.c_str(), "r" );

  if( !iftair )
  {
    flog1 << endl << "Cannot open " << tairname;
    flog1 << " for TAIR data input" << endl << endl;

    exit( -1 );
  }


  // Open precipitation file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.iprecfname
            << telmnt[0].clm.iprecend;

  precname = tempfname.str();

  ifprec = fopen( precname.c_str(), "r" );


  if( !ifprec )
  {
    flog1 << endl << "Cannot open " << precname;
    flog1 << " for PREC data input" << endl << endl;

    exit( -1 );
  }

  // Open temperature range file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.itrangefname
            << telmnt[0].clm.itrangeend;


  trangename = tempfname.str();

  iftrange = fopen( trangename.c_str(), "r" );

  if( !iftrange )
  {
    flog1 << endl << "Cannot open " << trangename;
    flog1 << " for TRANGE data input" << endl << endl;

    exit( -1 );
  }


  // Open VPR file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.ivprfname
            << telmnt[0].clm.ivprend;

  vprname = tempfname.str();
  ifvpr = fopen( vprname.c_str(), "r" );

  if( !ifvpr )
  {
    flog1 << endl << "Cannot open " << vprname;
    flog1 << " for VPR data input" << endl << endl;

    exit( -1 );
  }

  // Open WS10 file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.iws10fname
            << telmnt[0].clm.iws10end;

  ws10name = tempfname.str();
  ifws10 = fopen( ws10name.c_str(), "r" );

  if( !ifws10 )
  {
    flog1 << endl << "Cannot open " << ws10name;
    flog1 << " for WS10 data input" << endl << endl;

    exit( -1 );
  }

  //  CO2



  if( 1 == telmnt[0].clm.tco2flag )
  {
    co2name = telmnt[0].clm.ico2fname + telmnt[0].clm.ico2end;

//    ifco2.open( co2name.c_str(), ios::in );
      ifco2 = fopen( co2name.c_str(), "r" );
//    cout << "co2 file = " << co2name.c_str() << endl;

    if( !ifco2 )
    {
      flog1 << endl << "Cannot open " << co2name;
      flog1 << " for CO2 data input" << endl;

     exit( -1 );
    }
  }

  // Open ozone file

  tempfname.str( "" );

  tempfname << telmnt[0].clm.io3fname
            << telmnt[0].clm.io3end;

  o3name = tempfname.str();

  ifo3 = fopen( o3name.c_str(), "r" );
  if( !ifo3 )
  {
    flog1 << endl << "Cannot open " << o3name;
    flog1 << " for O3 data input" << endl << endl;

    exit( -1 );
  }

 // Open ndep file
 //   
  tempfname.str( "" );

  tempfname << telmnt[0].atmdep.ndepfname
            << telmnt[0].atmdep.ndepend;

  ndepname = tempfname.str();

  ifndep = fopen( ndepname.c_str(), "r" );
//cout << "ndepname = " << ndepname << endl;
  if( !ifndep )
  {
    flog1 << endl << "Cannot open " << ndepname;
    flog1 << " for NDEP data input" << endl << endl;

    exit( -1 );
  }

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void initializeLCLUCGridCell( void )
{


  string mxcohrtname;
  string lulcname;
  ostringstream tempfname;


  // Open maximum cohort file

  tempfname.str( "" );

  tempfname << telmnt[0].lcluc.imxcohrtfname
            << telmnt[0].lcluc.imxcohrtend;

  mxcohrtname = tempfname.str();

  ifnumchrts = fopen( mxcohrtname.c_str(), "r" );

  if( !ifnumchrts )
  {
    flog1 << endl << "Cannot open " << mxcohrtname;
    flog1 << " for MXCOHRTS data input" << endl << endl;

    exit( -1 );
  }

  // Open land use/land cover cohort file

  tempfname.str( "" );

  tempfname << telmnt[0].lcluc.ilulcfname
            << telmnt[0].lcluc.ilulcend;

  lulcname = tempfname.str();

  iflulc = fopen( lulcname.c_str(), "r" );

  if( !iflulc )
  {
    flog1 << endl << "Cannot open " << lulcname;
    flog1 << " for LULCCHRT data input" << endl << endl;

    exit( -1 );
  }

};

/* *************************************************************
************************************************************* */


/* **************************************************************
************************************************************** */

void initializeTEMGridCell( const int& CYCLE, const int ndays[] )
{
  const int dyr = 0;
  int ichrt;

  // Set elevation and soil texture for TEM

  telmnt[0].setGIStopography( flog1, fatalerr, fstxt, felev );


/* *************************************************************
		BEGIN VEGETATION MOSAIC LOOP
************************************************************* */

  for( ichrt = 0; ichrt < telmnt[0].maxcohorts; ++ichrt )
  {
    if( istateflag > 0 )
    {
      // Read in initial TEM state determined in a previous
      //  TEM simulation to telmnt[0].cohort

      telmnt[0].readCohortState( ifstate, ichrt, CYCLE );

      // Pass telmnt[0].cohort information to TEM

      telmnt[0].getTEMCohortState( ichrt,
                                   CYCLE );
    }
    else
    {
      telmnt[0].setTEMequilState( flog1,
                                  equil,
                                  totsptime,
                                  ichrt,
                                  ftempred,
                                  tempredmap,
                                  CYCLE,
                                  spinoutfg,
                                  ndays );
      if( telmnt[0].tem.intflag == 0 )
      {
        if( elmnt.count < elmnt.grdcnt )
        {
          cout << "Integration terminated before attaining ";
          cout << "tolerance level" << endl;
        }

        flog1 << "Integration terminated before attaining ";
        flog1 << "tolerance level" << endl;

      }

      // Write out telmnt[0].cohort to output file for
      //   potential use in a future TEM simulation

      if( 1 == ostateflag )
      {
        telmnt[0].writeCohortState( ofstate, ichrt, CYCLE );
      }

      // Write selected TEM variables from telmnt[0].output to
      //   outfile files

      if( 1 == spinoutfg || 1 == equil )
      {
       if(telmnt[0].intervalflag == 0)
       {
        telmnt[0].temwritepred( ftempred,
                                tempredmap,
                                dyr,
                                ichrt,
                                telmnt[0].ntempred,
                                spinoutfg,
                                0,
                                CYCLE,
                                ndays );
       }
       else if(telmnt[0].intervalflag == 1 )
       {
        telmnt[0].temwritepredm( ftempred,
                                tempredmap,
                                dyr,
                                ichrt,
                                telmnt[0].ntempred,
                                spinoutfg,
                                0,
                                12,
                                ndays );
       }
       
      }
    } // End of istateflag else
  } // End of cohort loop

//  elmnt.show( flog1,
//              telmnt[0].col,
//              telmnt[0].row,
//              telmnt[0].tem.totyr,
//              telmnt[0].tem.tol );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void initRun( void )
{
  
  int i;
  int numcmnt;
  int icmnt;

  time_t starttime;
  
  double initco2;
  double co2level;
//  double dc2n;

  string ifilename;
  string clmoutvars;
  string clmoutfiles;
  string clmpredfile;  
  string cmntecd;

  string datfilenames;
  string temoutvars;
  string temoutfiles;
  string tempredfile;

  string logfilename;
  
  temflag = 1; 

  starttime = time( NULL );
  
  logfilename = "tem45.";
  logfilename.append( ctime( &starttime ), 24 );
  logfilename.append( ".log" );
  
  logfilename.replace( 9,1,1, '_' );
  logfilename.replace( 13,1,1, '_' );
  logfilename.replace( 16,1,1, '_' );
  logfilename.replace( 19,1,1, '.' );
  logfilename.replace( 22,1,1, '.' );
  logfilename.replace( 25,1,1, '_' );


  flog1.open( logfilename.c_str() );
//  flog1.open( "tem45.log" );

  telmnt[0].tem.gofile.open( telmnt[0].tem.goname.c_str(), ios::in );
  telmnt[0].tem.goxml.getXMLrootNode( telmnt[0].tem.gofile, "gofile" );
  flog1 << "using " << telmnt[0].tem.goname << " for information on this run " << endl;

/// Runmode and runtime information

  equil = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xequil" );
  flog1 << " running tem in " << ( ( equil == 0 ) ? "transient" : "equilibrium" ) << " mode " << endl;
  
//  RTIME = 1;
//  BSF CHANGED TO RTIME = 0
//
  RTIME = 0;

  telmnt[0].de_startyear = telmnt[0].clm.startyr;
  telmnt[0].de_nyears = 1;

  if( 0 == equil )
  {
    telmnt[0].clm.startyr = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "clmstartyr" );
    flog1 << " start transient run in " << telmnt[0].clm.startyr << endl;
    
    spinflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "spinflag" );
    flog1 << " initialize model using " << ((spinflag == 0)?"equilibration to long-term average climate":((spinflag==1)?"equilibration then spinup":"dynamic equilibration")) << endl;
    
    numspin = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "numspin" );
    if (1 == spinflag ) { flog1 << " model will be 'spun up' " << numspin << " times " << endl; }
    
    spintime = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "spintime" );
    if (1 == spinflag ) { flog1 << " each 'spin' will last " << spintime << " years " << endl; }
    
    telmnt[0].de_startyear = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "de_startyear" );
    if (2 == spinflag ) { flog1 << " startyear for dynamic equilibration: " << telmnt[0].de_startyear << endl; }
    
    telmnt[0].de_nyears = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "de_nyears" );
    if (2 == spinflag ) { flog1 << " number of years for dynamic equilibration loop: " << telmnt[0].de_nyears << endl; }
    
    transtime = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "transtime" );
    flog1 << " model will run for " << transtime << " years in transient mode following initialization" << endl;
      
    telmnt[0].tem.startyr = telmnt[0].clm.startyr;
    totsptime = 0;

    if( 0 == spinflag )
    {
      telmnt[0].de_startyear = telmnt[0].clm.startyr;
      telmnt[0].de_nyears = 1;
    }
    
    if( 1 == spinflag )
    {
      telmnt[0].de_startyear = telmnt[0].clm.startyr;
      telmnt[0].de_nyears = 1;
      totsptime = spintime * numspin;
      RTIME += totsptime;
    }
    
    if( 2 == spinflag )
    {
      numspin = 0;
      spintime = 0;
      totsptime = 0;
      RTIME += totsptime;
    }

    RTIME += transtime;
  }
  else
  {
    totsptime = RTIME;
    spintime = 1;
  }
  
/// number of grid cells and what GIS data to use 
  
  mxnumgrid = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "mxnumgrid" );
  flog1 << " the model will be run for " << mxnumgrid << " grid cells " << endl;
  
  telmnt[0].lonlatflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "lonlatflag" );
  flog1 << " grid cells are located in input files by " << ((telmnt[0].lonlatflag==0)?"row/colum":"longitude/latitude") << endl;
  
  ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "lonlatfile" );
  if( 0 == telmnt[0].lonlatflag) { flog1 << " the file translating between row/column and longitude/latitude is " << ifilename << endl; }

  if( 0 == telmnt[0].lonlatflag )
  {
    flonlat = fopen( ifilename.c_str(), "r" );

    if( !flonlat )
    {
      cerr << endl << "Cannot open " << ifilename;
      cerr << " for data input" << endl;

      exit( -1 );
    }
  }

  elmnt.grdcnt = telmnt[0].tem.goxml.getXMLlong( telmnt[0].tem.gofile, "gofile", "grdcnt" );
  flog1 << " the logfile will include a timestamp after each of the first " << elmnt.grdcnt << " grid cells " << endl;
  
  elmnt.strtflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "strtflag" );
  flog1 << " the model will start " << ((elmnt.strtflag==1)?"at the beginning of":"partway through") << " the GIS files " << endl;
  
  elmnt.numskip = telmnt[0].tem.goxml.getXMLlong( telmnt[0].tem.gofile, "gofile", "numskip" );
  if( elmnt.strtflag == 0 ) { flog1 << elmnt.numskip  << " grid cells will be skipped " << endl; }
  
  elmnt.endflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "endflag" );
  flog1 << " the model will end " << ((elmnt.endflag==1)?"at the end of":"partway through") << " the GIS files " << endl;
  
  elmnt.numgrids = telmnt[0].tem.goxml.getXMLlong( telmnt[0].tem.gofile, "gofile", "numgrids" );
  if( elmnt.endflag == 0 ) { flog1 << " the model will run until " << elmnt.numgrids << " grid cells have been run " << endl; }

/// Climate inputs  

  telmnt[0].clm.sradflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "sradflag" );
  telmnt[0].clm.cldflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "cldflag" );
  telmnt[0].clm.parflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "parflag" );
  telmnt[0].clm.tcldsflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtcldsflag" );
  
  flog1 << " the TEMclm model " << ((telmnt[0].clm.sradflag==1)?"will":"will not") << " be run for solar radiation variables " << endl;
  flog1 << ((telmnt[0].clm.tcldsflag==1)?"transient ":"static ") << ((telmnt[0].clm.cldflag==1)?"cloudiness":"surface radiation") << " data will be used" << endl;
  if( telmnt[0].clm.sradflag == 0 ) { flog1 << " spatially explicit PAR data " << ((telmnt[0].clm.parflag==1)?"will":"will not") << " be used " << endl; }
  
  telmnt[0].clm.icldsfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "icldsfname" );
  telmnt[0].clm.icldsend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "icldsend" );
  telmnt[0].clm.inirrfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "inirrfname" );
  telmnt[0].clm.inirrend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "inirrend" );
  telmnt[0].clm.igirrfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "igirrfname" );
  telmnt[0].clm.igirrend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "igirrend" );
  telmnt[0].clm.iparfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iparfname" );
  telmnt[0].clm.iparend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iparend" );
  
  if( (telmnt[0].clm.cldflag == 1) && (telmnt[0].clm.sradflag == 1) ) 
  { 
    if( telmnt[0].clm.tcldsflag == 1 ) { flog1 << " filename for cloudiness data input: " << telmnt[0].clm.icldsfname + telmnt[0].clm.icldsend << endl; }
    else { flog1 << " filename for cloudiness data input: " << telmnt[0].clm.icldsfname << endl; }  
  }

  if( (telmnt[0].clm.cldflag == 0) && (telmnt[0].clm.sradflag == 1) )
  {
    if( telmnt[0].clm.tcldsflag == 1 ) { flog1 << " filename for solar radiation data input: " << telmnt[0].clm.inirrfname + telmnt[0].clm.inirrend << endl; }
    else { flog1 << " filename for solar radiation data input: " << telmnt[0].clm.inirrfname << endl; }
  }
  
  if( telmnt[0].clm.sradflag == 0 )
  {
    if( telmnt[0].clm.tcldsflag == 1 ) 
    {
      flog1 << " filename for top-of-atmosphere radiation data input: " << telmnt[0].clm.igirrfname + telmnt[0].clm.igirrend << endl;
      flog1 << " filename for surface radiation data input: " << telmnt[0].clm.inirrfname + telmnt[0].clm.inirrend << endl;
      flog1 << " filename for photosynthetically active radiation data input: " << telmnt[0].clm.iparfname + telmnt[0].clm.iparend << endl;
    }
    else
    {
      flog1 << " filename for top-of-atmosphere radiation data input: " << telmnt[0].clm.igirrfname << endl;
      flog1 << " filename for surface radiation data input: " << telmnt[0].clm.inirrfname << endl;
      flog1 << " filename for photosynthetically active radiation data input: " << telmnt[0].clm.iparfname << endl;
    }
  }


  telmnt[0].clm.ttairflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xttairflag" );
  telmnt[0].clm.itairfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "itairfname" );
  telmnt[0].clm.itairend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "itairend" );
  
  if( telmnt[0].clm.ttairflag == 1 ) { flog1 << " transient tair dataset: " << telmnt[0].clm.itairfname + telmnt[0].clm.itairend << endl; }
  else { flog1 << " long-term average tair dataset: " << telmnt[0].clm.itairfname << endl; }
  

  telmnt[0].clm.tprecflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtprecflag" );
  telmnt[0].clm.iprecfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iprecfname" );
  telmnt[0].clm.iprecend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iprecend" );

  if( telmnt[0].clm.tprecflag == 1 ) { flog1 << " transient prec dataset: " << telmnt[0].clm.iprecfname + telmnt[0].clm.iprecend << endl; }
  else { flog1 << " long-term average prec dataset: " << telmnt[0].clm.iprecfname << endl; }
  

  telmnt[0].clm.ttrangeflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xttrangeflag" );
  telmnt[0].clm.itrangefname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "itrangefname" );
  telmnt[0].clm.itrangeend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "itrangeend" );

  if( telmnt[0].clm.ttrangeflag == 1 ) { flog1 << " transient trange dataset: " << telmnt[0].clm.itrangefname + telmnt[0].clm.itrangeend << endl; }
  else { flog1 << " long-term average trange dataset: " << telmnt[0].clm.itrangefname << endl; }

  
  telmnt[0].clm.tvprflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtvprflag" );
  telmnt[0].clm.ivprfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ivprfname" );
  telmnt[0].clm.ivprend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ivprend" );
  
  if( telmnt[0].clm.tvprflag == 1 ) { flog1 << " transient vpr dataset: " << telmnt[0].clm.ivprfname + telmnt[0].clm.ivprend << endl; }
  else { flog1 << " long-term average vpr dataset: " << telmnt[0].clm.ivprfname << endl; }


  initco2 = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "initco2" );
  flog1 << " initial co2 concentration (ppmv) : " << initco2 << endl;   
  telmnt[0].clm.setINITCO2( initco2 );

  co2level = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "xco2level" );
  flog1 << " final equilibrium co2 concentration (ppmv) : " << co2level << endl;
  telmnt[0].clm.setCO2LEVEL( co2level );
  
  telmnt[0].clm.tco2flag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtco2flag" );
  telmnt[0].clm.ico2fname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ico2fname" );
  telmnt[0].clm.ico2end = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ico2end" );

  if( telmnt[0].clm.tco2flag == 1 ) { flog1 << " transient co2 dataset: " << telmnt[0].clm.ico2fname << endl; }
    
  
  telmnt[0].clm.to3flag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xto3flag" );
  telmnt[0].clm.io3fname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "io3fname" );
  telmnt[0].clm.io3end = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "io3end" );

  if( telmnt[0].clm.to3flag == 1 ) { flog1 << " transient ozone dataset: " << telmnt[0].clm.io3fname + telmnt[0].clm.io3end << endl; }
  else { flog1 << " long-term average ozone dataset: " << telmnt[0].clm.io3fname << endl; }
  
//
//  Input name of Ndep file
//
  telmnt[0].clm.tndepflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtndepflag" );
  telmnt[0].atmdep.ndepfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ndepfname" );
  telmnt[0].atmdep.ndepend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ndepend" );
  if( telmnt[0].clm.tndepflag == 1 ) { flog1 << " transient ndep dataset: " << telmnt[0].atmdep.ndepfname + telmnt[0].atmdep.ndepend << endl; }
  else { flog1 << " long-term average ndep dataset: " << telmnt[0].atmdep.ndepfname << endl; }
//
//  Input nape of Wind file
//
//  telmnt[0].clm.iws10fname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iws10fname" );
//  flog1 << " long-term average ws10 dataset: " << telmnt[0].clm.iws10fname << endl;


  telmnt[0].clm.tws10flag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "xtws10flag" );
  telmnt[0].clm.iws10fname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iws10fname" );
  telmnt[0].clm.iws10end = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "iws10end" );

  if( telmnt[0].clm.tws10flag == 1 ) { flog1 << " transient wind speed dataset: " << telmnt[0].clm.iws10fname + telmnt[0].clm.iws10end << endl;
 }
  else { flog1 << " long-term average wind speed dataset: " << telmnt[0].clm.iws10fname << endl; }

 
  telmnt[0].clm.predflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "clmoutflag" );
  flog1 << " climate data " << ((telmnt[0].clm.predflag == 1)?"will":"will not") << " be output" << endl;
  
  telmnt[0].natmspred = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "nclmout" );
  flog1 << " output for " << telmnt[0].natmspred << " climate variables: " << endl ;
  
  telmnt[0].totpred = telmnt[0].natmspred;
  
  clmoutvars = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "clmoutvars" );
  clmoutfiles = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "clmoutfiles" );
    
  for(i = 0; i < telmnt[0].natmspred; ++i )
  {
    clmpredmap.at( i ) = clmoutvars.substr( 0, clmoutvars.find( "," ) );
    clmoutvars = clmoutvars.substr( clmoutvars.find( "," ) + 1, (clmoutvars.length( ) - clmoutvars.find( "," ) - 1) );
    
    clmpredfile = clmoutfiles.substr( 0, clmoutfiles.find( "," ) );
    fclmpred[i].open( clmpredfile.c_str(), ios::out );
    clmoutfiles = clmoutfiles.substr( clmoutfiles.find( "," ) + 1, (clmoutfiles.length( ) - clmoutfiles.find( "," ) - 1) );
    
    flog1 << "      " << clmpredfile << " opened for output of " << clmpredmap[i] << endl;    
  }

  initializeCLMGridCell();
  
/// Land use/land cover inputs

  telmnt[0].lcluc.startyr = telmnt[0].clm.startyr;

  cmntecd = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "cmntecd" );
  telmnt[0].lcluc.getvtype( cmntecd );
  flog1 << " the file describing vegetation mosaics is: " << cmntecd << endl;

  telmnt[0].lcluc.tlulcflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "tlulcflag" );
  flog1 << " transient land use " << ((telmnt[0].lcluc.tlulcflag==1)?"will":"will not") << " be used " << endl;
  
  telmnt[0].lcluc.imxcohrtfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "imxcohrtfname" );
  telmnt[0].lcluc.imxcohrtend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "imxcohrtend" );
  flog1 << " file describing number of cohorts in each grid cell: " << telmnt[0].lcluc.imxcohrtfname + telmnt[0].lcluc.imxcohrtend << endl;
  
  telmnt[0].lcluc.ilulcfname = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ilulcfname" );
  telmnt[0].lcluc.ilulcend = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ilulcend" );
  flog1 << " file describing land use cover and change in each cohort of each grid cell: " << telmnt[0].lcluc.ilulcfname + telmnt[0].lcluc.ilulcend << endl;
  
  telmnt[0].lcluc.lastyr = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "lulclastyr" );
  if( telmnt[0].lcluc.tlulcflag == 1 ) { flog1 << " the last year of transient cohort data is: " << telmnt[0].lcluc.lastyr << endl; }

  initializeLCLUCGridCell();

/// Vegetation inputs and output selection

  telmnt[0].ntempred = 0;

  if( 1 == temflag )
  {
    telmnt[0].tem.startyr = telmnt[0].clm.startyr;
    telmnt[0].tem.atms.setINITCO2( telmnt[0].clm.getINITCO2() );
    telmnt[0].tem.atms.setCO2LEVEL( telmnt[0].clm.getCO2LEVEL() );

    telmnt[0].tem.ag.tlulcflag = telmnt[0].lcluc.tlulcflag;

    
    //telmnt[0].tem.initrun( flog1 );
    
    telmnt[0].tem.avlnflag = 0; 
    telmnt[0].tem.nfeed = 0; 
    telmnt[0].tem.rheqflag = 0;
    
    telmnt[0].tem.avlnflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "avlnflag" );
    flog1 << " " << ((telmnt[0].tem.avlnflag==1)?"allowing":"not allowing") << " available N to fluctuate " << endl;
  
    telmnt[0].tem.nfeed = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "nfeed" );
    flog1 << " running TEM " << ((telmnt[0].tem.nfeed==1)?"with":"without") << " N feedback on GPP " << endl;
  
    telmnt[0].tem.baseline = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "baseline" );
    flog1 << " " << ((telmnt[0].tem.baseline==1)?"solving":"not solving") << " for baseline soil nitrogen" << endl;
    telmnt[0].tem.initbase = telmnt[0].tem.baseline;
  
    telmnt[0].tem.moistlim = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "moistlim" );
    flog1 << " moisture limitation is " << ((telmnt[0].tem.moistlim==1)?"on":"off") << endl;
  
    telmnt[0].tem.o3flag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "o3flag" );
    flog1 << " ozone damage to gpp is " << ((telmnt[0].tem.o3flag==1)?"on":"off") << endl;
  
    //telmnt[0].tem.equil = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "equil" );
    //rflog1 << " equil = " << equil << endl;
  
    telmnt[0].tem.strteq = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "strteq" );
    flog1 << " model will check for equilibrium after running for " << telmnt[0].tem.strteq << " years " << endl;
  
    telmnt[0].tem.rheqflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "rheqflag" );
    flog1 << " decomposition " << ((telmnt[0].tem.rheqflag==1)?"must":"is not required to") << " equilibrate " << endl;
  
    telmnt[0].tem.wtol = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "wtol" );
    flog1 << " absolute tolerance for the water cycle: " << telmnt[0].tem.wtol << endl;
  
    telmnt[0].tem.ctol = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "ctol" );
    flog1 << " absolute tolerance for the carbon cycle: " << telmnt[0].tem.ctol << endl;
  
    telmnt[0].tem.ntol = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "ntol" );
    flog1 << " absolute tolerance for the nitrogen cycle: " << telmnt[0].tem.ntol << endl;
    
    telmnt[0].tem.maxyears = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "maxyears" );
    flog1 << " maximum number of years for the model to run before initialization is complete: " << telmnt[0].tem.maxyears << " years " << endl;
    telmnt[0].tem.runsize = telmnt[0].tem.maxyears;
  
    telmnt[0].tem.maxnrun = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "maxnrun" );
    flog1 << " maximum number of attempts to reach a solution: " << telmnt[0].tem.maxnrun << endl;
  
    telmnt[0].tem.tauavg = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "tauavg" );
    flog1 << " averaging time for equilibration: " << telmnt[0].tem.tauavg << " years " << endl;
  
    telmnt[0].tem.veg.setDC2N( telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "dc2n" ) );
    flog1 << " factor for changing relative c:n per change in co2 concentration: " << telmnt[0].tem.veg.getDC2N() << endl;
      
    telmnt[0].tem.startyr = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "startyr" );
    flog1 << " first year for collection of output data: " << telmnt[0].tem.startyr << endl;
  
    telmnt[0].tem.endyr = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "endyr" );
    flog1 << " last year for collection of output data: " << telmnt[0].tem.endyr << endl;
  
    telmnt[0].tem.diffyr = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "diffyr" );
    flog1 << " time between years for output data (1 = annual): " << telmnt[0].tem.diffyr << endl;


    telmnt[0].tem.inittol = telmnt[0].tem.goxml.getXMLdouble( telmnt[0].tem.gofile, "gofile", "inittol" );
    flog1 << " proportional tolerance of the integrator: " << telmnt[0].tem.inittol << endl;
  
    telmnt[0].tem.maxit = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "maxit" );
    flog1 << " maximum number of tolerance-reductions in a month: " << telmnt[0].tem.maxit << endl;
  
    telmnt[0].tem.maxitmon = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "maxitmon" );
    flog1 << " maximum number of iterations in a month: " << telmnt[0].tem.maxitmon << endl;
 
    
    //telmnt[0].tem.askODE( flog1 );


    // Get vegetation type dependent parameters

    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "rootzecd" );
    telmnt[0].tem.soil.getrootz( ifilename );
    flog1 << " root zone parameters found in " << ifilename << endl;
    
    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "vegecd" );
    telmnt[0].tem.veg.getecd( ifilename );
    flog1 << " vegetation parameters found in " << ifilename << endl;
    
    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "conductecd" );
    telmnt[0].tem.veg.pen.getecd( ifilename );
	flog1 << " hydraulic and conductance-related parameters found in " << ifilename << endl;
	
	ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "mcrbecd" );
    telmnt[0].tem.microbe.getvegecd( ifilename );
    flog1 << " microbial parameters found in " << ifilename << endl;


    // Get parameters associated with human disturbance
    //   activities
    
    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "agecd" );
    telmnt[0].tem.ag.getecd( ifilename );
    flog1 << " agricultural parameters found in " << ifilename << endl;
    
    telmnt[0].tem.ag.tillflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "agtillflag" );
    flog1 << " agricultural tilling is " << ((telmnt[0].tem.ag.tillflag==1)?"on":"off") << endl;
    telmnt[0].tem.ag.irrgflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "agirrgflag" );
    flog1 << " agricultural irrigation is " << ((telmnt[0].tem.ag.irrgflag==1)?"on":"off") << endl;
    telmnt[0].tem.ag.fertflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "agfertflag" );
    flog1 << " agricultural fertilization is " << ((telmnt[0].tem.ag.fertflag==1)?"on":"off") << endl;
        

    numcmnt = MAXCMNT - 1;
    
    datfilenames = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "datfilenames" );
    
    for(icmnt = 0; icmnt < numcmnt; ++icmnt )
    {
      ifilename = datfilenames.substr( 0, datfilenames.find( "," ) );
      telmnt[0].tem.getsitecd( icmnt, ifilename );
      datfilenames = datfilenames.substr( datfilenames.find( "," ) + 1, (datfilenames.length( ) - datfilenames.find( "," ) - 1) );
     
      flog1 << " datfile " << ifilename << " opened for input of vegetation cmnt " << icmnt << endl;
    }

    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "fstexture" );
    fstxt = fopen( ifilename.c_str(), "r" );
    flog1 << ifilename << " opened for soil texture input " << endl;

    if( !fstxt )
    {
      flog1 << endl << "Cannot open " << ifilename;
      flog1 << " for data input" << endl;

      exit( -1 );
    }

    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "felev" );
    felev = fopen( ifilename.c_str(), "r" );
    flog1 << ifilename << " opened for elevation input " << endl;

    if( !felev )
    {
      flog1 << "\nCannot open " << ifilename;
      flog1 << " for data input" << endl;

      exit( -1 );
    }

    telmnt[0].tem.predflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "temoutflag" );
    flog1 << " output from TEM " << ((telmnt[0].tem.predflag==1)?"will":"will not be") << " written" << endl;
  
    telmnt[0].intervalflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "intervalflag" );
    flog1 << " output time interval " << ((telmnt[0].intervalflag==1)?"monthly":"daily") << endl;

    telmnt[0].ntempred = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "ntemout" );
    if( telmnt[0].tem.predflag > 0 ) { flog1 << " " << telmnt[0].ntempred << " output variables " << endl; }
    
    temoutvars = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "temoutvars" );
    temoutfiles = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "temoutfiles" );
    
    for(i = 0; i < telmnt[0].ntempred; ++i )
    {
      tempredmap.at( i ) = temoutvars.substr( 0, temoutvars.find( "," ) );
      temoutvars = temoutvars.substr( temoutvars.find( "," ) + 1, (temoutvars.length( ) - temoutvars.find( "," ) - 1) );
    
      tempredfile = temoutfiles.substr( 0, temoutfiles.find( "," ) );
      ftempred[i].open( tempredfile.c_str(), ios::out );
      temoutfiles = temoutfiles.substr( temoutfiles.find( "," ) + 1, (temoutfiles.length( ) - temoutfiles.find( "," ) - 1) );
    
      flog1 << tempredfile << " opened for output of " << tempredmap[i] << endl;    
    }
    
    spinoutfg = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "spinoutflag" );
    spinoutyrs = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "spinoutyrs" );
    if( telmnt[0].tem.predflag > 0 ) { flog1 << " output from spinup period " << ((spinoutfg==1)?"will":"will not be") << " written " << endl; }
    if( (telmnt[0].tem.predflag > 0) && ( spinoutfg == 1 )) { flog1 << "    for " << spinoutyrs << " years " << endl; }
    
    
    istateflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "istateflag" );
    flog1 << " input state ( 0 -> require initizlization; 1 -> file input at equilibrium; 2 -> file input for specific year) " << istateflag << endl;
    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "istatefile" );
    if( istateflag > 0 ) { flog1 << " input state file: " << ifilename << endl; }
    istateyear = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "istateyear" );
    if( istateflag == 2 ) { flog1 << " input state year: " << istateyear << endl; }
    
    if( istateflag > 0 ) { ifstate.open( ifilename.c_str(), ios::in ); }
    
    ostateflag = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "ostateflag" );
    flog1 << " output state ( 0 -> do not write TEMSTATE file; 1-> save TEMSTATE at equilibrium; 2 -> save TEMSTATE at a specific year) " << ostateflag << endl;
    ifilename = telmnt[0].tem.goxml.getXMLstring( telmnt[0].tem.gofile, "gofile", "ostatefile" );
    if( ostateflag > 0 ) { flog1 << " output state file: " << ifilename << endl; }
    ostateyear = telmnt[0].tem.goxml.getXMLint( telmnt[0].tem.gofile, "gofile", "ostateyear" );
    if( ostateflag == 2 ) { flog1 << " output state year: " << ostateyear << endl; }
    
    if( ostateflag > 0 ) { ofstate.open( ifilename.c_str(), ios::out ); }
    
  }
  
  telmnt[0].tem.gofile.close();

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void updateTCLMGridCell( const int& pdyr, const int& CYCLE, const int ndays[], const int& eqtr )
{

  const double Watts2cal = 1.0 / 0.4845;
  int dyr,day;
  int gisend;
  int sumdays;

  double avgtair, avgprec, avgtaird, avgphi, avgphish;

  avgtair = ZERO;
  avgprec = ZERO;
  avgtaird = ZERO;
  avgphi = ZERO;
  avgphish = ZERO;

  float lat = -999.9;

//cout << "updateTCLM type = " << telmnt[0].tem.veg.cmnt << endl;
  string SRADname;

  fatalerr = 0;


  if( 0 == pdyr and 0 == eqtr )
  {
    if( 1 == telmnt[0].clm.tcldsflag )
    {
      if( 0 == telmnt[0].clm.sradflag )
      {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
        for( day = 0; day < sumdays; ++day )
        {
          gisend = girrdat[dyr].getdel( ifgirr, day );

          if( -1 == gisend )
          {
            cout << "Ran out of GIRR data" << endl << endl;
            flog1 << "Ran out of GIRR data" << endl << endl;

            exit( -1 );
          }
        }
       }
      }


      if( 1 == telmnt[0].clm.cldflag )
      {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
        for( day = 0; day < sumdays; ++day )
        {
          gisend = cldsdat[dyr].getdel( ifclds, day );
//          cout << "CLDSIN = " << day << " " << pdyr << " " << cldsdat[pdyr].mon[day] << " " << pdyr << endl;

          if( -1 == gisend )
          {
            cout << "Ran out of Cloudiness data" << endl << endl;
            flog1 << "Ran out of Cloudiness data" << endl << endl;

            exit( -1 );
          }
        }
       }

        telmnt[0].col = cldsdat[0].col;
        telmnt[0].row = cldsdat[0].row;
        SRADname = "CLDINESS";
      }
      else
      {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
        for( day = 0; day < sumdays; ++day )
        {
          gisend = nirrdat[dyr].getdel( ifnirr, day );

          if( -1 == gisend )
          {
            cout << "Ran out of NIRR data" << endl << endl;
            flog1 << "Ran out of NIRR data" << endl << endl;

            exit( -1 );
          }
        }
       }

        telmnt[0].col = nirrdat[0].col;
        telmnt[0].row = nirrdat[0].row;
        SRADname = "NIRR";
      }

      if( 1 == telmnt[0].clm.parflag )
      {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
        for( day = 0; day < sumdays; ++day )
        {
          gisend = pardat[dyr].getdel( ifpar, day );

          if( -1 == gisend )
          {
            cout << "Ran out of PAR data" << endl << endl;
            flog1 << "Ran out of PAR data" << endl << endl;

            exit( -1 );
          }
        }
      }
    }
    }
    else  // 0 == telmnt[0].clm.tcldsflag
    {
      if( 0 == telmnt[0].clm.sradflag )
      {
        for( day = 0; day < CYCLE; ++day )
        {
        gisend = girrdat[0].getdel( ifgirr, day );

        if( -1 == gisend )
        {
          cout << "Ran out of GIRR data" << endl << endl;
          flog1 << "Ran out of GIRR data" << endl << endl;

          exit( -1 );
        }
       }
      }

      if( 1 == telmnt[0].clm.cldflag )
      {
        for( day = 0; day < CYCLE; ++day )
        {
        gisend = cldsdat[0].getdel( ifclds, day );

        if( -1 == gisend )
        {
          cout << "Ran out of Cloudiness data" << endl << endl;
          flog1 << "Ran out of Cloudiness data" << endl << endl;

          exit( -1 );
        }
        }

        telmnt[0].col = cldsdat[0].col;
        telmnt[0].row = cldsdat[0].row;
        SRADname = "CLDINESS";
      }
      else
      {
        for( day = 0; day < CYCLE; ++day )
        {
        gisend = nirrdat[0].getdel( ifnirr, day );

        if( -1 == gisend )
        {
          cout << "Ran out of NIRR data" << endl << endl;
          flog1 << "Ran out of NIRR data" << endl << endl;

          exit( -1 );
        }
        }

        telmnt[0].col = nirrdat[0].col;
        telmnt[0].row = nirrdat[0].row;
        SRADname = "NIRR";
      }

      if( 1 == telmnt[0].clm.parflag )
      {
        for( day = 0; day < CYCLE; ++day )
        {
        gisend = pardat[0].getdel( ifpar, day );

        if( -1 == gisend )
        {
          cout << "Ran out of PAR data" << endl << endl;
          flog1 << "Ran out of PAR data" << endl << endl;

          exit( -1 );
        }
        }
      }
    }

    // Look for spatial co-registration problems between
    //   cloudiness or net irradiance (NIRR) spatially explicit
    //   data and gross irradiance (GIRR) spatially explicit
    //   data

    if( 0 == telmnt[0].clm.sradflag )
    {
      fatalerr = telmnt[0].coregerr( flog1,
                                     SRADname,
                                     telmnt[0].col,
                                     telmnt[0].row,
                                     "GIRR",
                                     girrdat[0].col,
                                     girrdat[0].row );

      if( fatalerr != 0 ) { exit( -1 ); }
    }

    // Look for spatial co-registration problems between
    //   cloudiness or net irradiance (NIRR) spatially explicit
    //   data and photosynthetically active radiation (PAR)
    //   spatially explicit data

    if( 1 == telmnt[0].clm.parflag )
    {
      fatalerr = telmnt[0].coregerr( flog1,
                                     SRADname,
                                     telmnt[0].col,
                                     telmnt[0].row,
                                     "PAR",
                                     pardat[0].col,
                                     pardat[0].row );

      if( fatalerr != 0 ) { exit( -1 ); }
    }

    // Read in historical monthly air temperatures for grid cell

    if( 1 == telmnt[0].clm.ttairflag )
    {
       for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = tairdat[dyr].getdel( iftair, day );
//        cout << "tair = " << tairdat[pdyr].mon[day] <<  "  " << pdyr << " " << day << endl;

        if( -1 == gisend )
        {
          cout << "Ran out of Air Temperature data" << endl << endl;
          flog1 << "Ran out of Air Temperature data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = tairdat[0].getdel( iftair, day );

      if( -1 == gisend )
      {
        cout << "Ran out of Air Temperature data" << endl << endl;
        flog1 << "Ran out of Air Temperature data" << endl << endl;

        exit( -1 );
      }
     }
    }


    // Look for spatial co-registration problems between cloudiness and
    // air temperature spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "TAIR",
                                   tairdat[0].col,
                                   tairdat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }


    // Read in historical monthly precipitation for grid cell

    if( 1 == telmnt[0].clm.tprecflag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = precdat[dyr].getdel( ifprec, day );

        if( -1 == gisend )
        {
          cout << "Ran out of Precipitation data" << endl << endl;
          flog1 << "Ran out of Precipitation data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = precdat[0].getdel( ifprec, day );

      if( -1 == gisend )
      {
        cout << "Ran out of Precipitation data" << endl << endl;
        flog1 << "Ran out of Precipitation data" << endl << endl;

        exit( -1 );
      }
     }
    }

	// Look for spatial co-registration problems between cloudiness and
    // precipitation spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "PREC",
                                   precdat[0].col,
                                   precdat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }


	// Read in historical monthly daily temperature range for grid cell

    if( 1 == telmnt[0].clm.ttrangeflag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = trangedat[dyr].getdel( iftrange, day );

        if( -1 == gisend )
        {
          cout << "Ran out of Temp. range data" << endl << endl;
          flog1 << "Ran out of Temp. range data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = trangedat[0].getdel( iftrange, day );

      if( -1 == gisend )
      {
        cout << "Ran out of Temp. range data" << endl << endl;
        flog1 << "Ran out of Temp. range data" << endl << endl;

        exit( -1 );
      }
     }
    }

	// Look for spatial co-registration problems between cloudiness and
    // trange spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "TRANGE",
                                   trangedat[0].col,
                                   trangedat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }

	// Read in historical monthly vapor pressure for grid cell

    if( 1 == telmnt[0].clm.tvprflag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = vprdat[dyr].getdel( ifvpr, day );

          
        if( -1 == gisend )
        {
          cout << "Ran out of Vapor Pressure data" << endl << endl;
          flog1 << "Ran out of Vapor Pressure data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = vprdat[0].getdel( ifvpr, day );

      if( -1 == gisend )
      {
        cout << "Ran out of Vapor Pressure data" << endl << endl;
        flog1 << "Ran out of Vapor Pressure data" << endl << endl;

        exit( -1 );
      }
     }
    }


    // Look for spatial co-registration problems between cloudiness and
    // vapor pressure spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "VPD",
                                   vprdat[0].col,
                                   vprdat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }


    // Read in historical annual atmospheric CO2 data for
    //   globe

    if( 1 == telmnt[0].clm.tco2flag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = co2dat[dyr].getdel( ifco2, day );
//       cout << "CO2IN = " << day << " " << pdyr << " " << co2dat[pdyr].mon[day] << " " << pdyr << endl;

        if( -1 == gisend )
        {
          cout << "Ran out of CO2 data" << endl << endl;
          flog1 << "Ran out of CO2 data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = co2dat[0].getdel( ifco2, day );

      if( -1 == gisend )
      {
        cout << "Ran out of CO2 data" << endl << endl;
        flog1 << "Ran out of CO2 data" << endl << endl;

        exit( -1 );
      }
     }
    }

    // Look for spatial co-registration problems between cloudiness and
    // ozone spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "CO2",
                                   co2dat[0].col,
                                   co2dat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }

/*    if( 1 == telmnt[0].clm.tco2flag && 0 == assignCO2 )
    {
      for( day = 0; day < CYCLE; ++day )
      {
        gisend = co2dat[pdyr].getdel( ifco2, day );
        cout << "CO2 = " << co2dat[pdyr].mon[day] <<  endl;
      }

      assignCO2 = 1;
    } */

    // Read in historical monthly atmospheric CO2 data for
    //   grid cell

    if( 2 == telmnt[0].clm.tco2flag )
    {
      cout << "This feature has not been implemented yet ";
      cout << "in this TEM version!"  << endl;

      flog1 << "This feature has not been implemented yet ";
      flog1 << "in this TEM version!"  << endl;

      exit( -1 );
    }


    // Read in historical monthly ozone data for grid cell

    if( 1 == telmnt[0].clm.to3flag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = o3dat[dyr].getdel( ifo3, day );

        if( -1 == gisend )
        {
          cout << "Ran out of Ozone data" << endl << endl;
          flog1 << "Ran out of Ozone data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = o3dat[0].getdel( ifo3, day );

      if( -1 == gisend )
      {
        cout << "Ran out of Ozone data" << endl << endl;
        flog1 << "Ran out of Ozone data" << endl << endl;

        exit( -1 );
      }
     }
    }

    // Look for spatial co-registration problems between cloudiness and
    // ozone spatially explicit data sets

    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "OZONE",
                                   o3dat[0].col,
                                   o3dat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }

//
//  Nitrogen Deposition
//
    if( 1 == telmnt[0].clm.tndepflag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    gisend = ndepdat[dyr].getdel( ifndep );
    if( -1 == gisend )
   {
     cout << "Ran out of Ndep data";
     cout << endl << endl;
     flog1 << "Ran out of Ndep data";
     flog1 << endl << endl;

     exit( -1 );
   }
   }

//   cout << "ndepdat = " << dyr << " " << ndepdat[dyr].col <<  " " << ndepdat[dyr].row << " " << ndepdat[dyr].adepval << endl;
    }
    else
    {
     gisend = ndepdat[0].getdel( ifndep );
   if( -1 == gisend )
   {
     cout << "Ran out of Ndep data";
     cout << endl << endl;
     flog1 << "Ran out of Ndep data";
     flog1 << endl << endl;

     exit( -1 );
   }
   }
// Check data for spatial coregistration errors
// 
   fatalerr = telmnt[0].coregerr( flog1,
                                  "Climate",
                                  telmnt[0].col,
                                  telmnt[0].row,
                                  "NDEP",
                                  ndepdat[0].col,
                                  ndepdat[0].row );

   if( fatalerr != 0 ) { exit( -1 ); }

    // Read in wind speed climatology


    if( 1 == telmnt[0].clm.tws10flag )
    {
        for( dyr = 0; dyr < transtime; ++dyr )
        {
    year = dyr + telmnt[0].clm.startyr;
    if((year%4) == 0.0)
    {
    sumdays = 366;
    }
    else
    {
    sumdays = 365;
    }
      for( day = 0; day < sumdays; ++day )
      {
        gisend = ws10dat[dyr].getdel( ifws10, day );

        if( -1 == gisend )
        {
          cout << "Ran out of wind speed data" << endl << endl;
          flog1 << "Ran out of wind speed data" << endl << endl;

          exit( -1 );
        }
      }
     }
    }
    else
    {
        for( day = 0; day < CYCLE; ++day )
        {
      gisend = ws10dat[0].getdel( ifws10, day );

      if( -1 == gisend )
      {
        cout << "Ran out of wind speed data" << endl << endl;
        flog1 << "Ran out of wind speed data" << endl << endl;

        exit( -1 );
      }
     }
    }

    // Look for spatial co-registration problems between cloudiness and
    // windspeed spatially explicit data sets
    
    fatalerr = telmnt[0].coregerr( flog1,
                                   SRADname,
                                   telmnt[0].col,
                                   telmnt[0].row,
                                   "WIND",
                                   ws10dat[0].col,            
                                   ws10dat[0].row );

    if( fatalerr != 0 ) { exit( -1 ); }

  }  //end of pdyr=0 loop

  if( 0 == pdyr ) { dyr = 0; }
  else if( istateflag < 2 && pdyr < (totsptime + 1) && spinflag == 1 )
  {
    dyr = (pdyr-1)%spintime + 1;
  }
  else
  {
    if( istateflag < 2 ) { dyr = pdyr - totsptime; }
    else{ dyr = pdyr; }
  }


telmnt[0].clm.yrsumday = ZERO;

 for( day = 0; day < CYCLE; ++day )
  {
  if( 0 == telmnt[0].clm.tcldsflag )
  {
    if( 0 == telmnt[0].clm.sradflag )
    {
        girrdat[dyr].mon[day] = girrdat[0].mon[day];
    }

    if( 1 == telmnt[0].clm.cldflag )
    {
        cldsdat[dyr].mon[day] = cldsdat[0].mon[day];
    }
    else
    {
        nirrdat[dyr].mon[day] = nirrdat[0].mon[day];
    }

    if( 1 == telmnt[0].clm.parflag )
    {
        pardat[dyr].mon[day] = pardat[0].mon[day];
    }
  }

  if( 0 == telmnt[0].clm.ttairflag )
{
//    tairdat[dyr].max = tairdat[0].max;
//    tairdat[dyr].ave = tairdat[0].ave;

      tairdat[dyr].mon[day] = tairdat[0].mon[day];
  }

  if( 0 == telmnt[0].clm.tprecflag )
  {
//    precdat[dyr].total = precdat[0].total;

      precdat[dyr].mon[day] = precdat[0].mon[day];
  }

  if( 0 == telmnt[0].clm.ttrangeflag )
  {
      trangedat[dyr].mon[day] = trangedat[0].mon[day];
  }

  if( 0 == telmnt[0].clm.tvprflag )
  {
      vprdat[dyr].mon[day] = vprdat[0].mon[day];
  }

  if( 0 == telmnt[0].clm.tws10flag )
  {
      ws10dat[dyr].mon[day] = ws10dat[0].mon[day];
  }


  if( 0 == telmnt[0].clm.to3flag )
  {
      o3dat[dyr].mon[day] = o3dat[0].mon[day];
  }

  if( 0 == telmnt[0].clm.tndepflag )
  {
      ndepdat[dyr] = ndepdat[0];
  }


  // Determine annual mean and maximum monthly air
  //   temperature for year for each grid cell

//  telmnt[0].mxtair[pdyr] = tairdat[dyr].max;
//  telmnt[0].avetair[pdyr] = tairdat[dyr].ave;


  // Determine annual precipitation for each grid cell

//  telmnt[0].yrprec[pdyr] = precdat[dyr].total;


  // Interpolate annual atmospheric CO2 concentrations
  //   to a monthly temporal resolution for grid cell
  //   Value in co2dat assumed to represent July atmospheric
  //   CO2 concentrations

    // Read in historical monthly ozone data for grid cell


/*  if( 1 == telmnt[0].clm.tco2flag && dyr > 0 )
  {
    if( pdyr < (totsptime+1) )
    {
        telmnt[0].climate[telmnt[0].clm.I_CO2][day][pdyr] = co2dat[0].mco2;
    }
    else
    {
//        if( dm < 6 )
        if( day < 182 )
        {
          telmnt[0].climate[telmnt[0].clm.I_CO2][day][pdyr] = co2dat[dyr-1].mco2
                                                       + ((day + 182)
                                                       * (co2dat[dyr].mco2
                                                       - co2dat[dyr-1].mco2));
//                                                       / (double) CYCLE);
        }
        else
        {
          if(dyr == transtime) {co2dat[dyr+1].mco2 = co2dat[dyr].mco2;}
          telmnt[0].climate[telmnt[0].clm.I_CO2][day][pdyr] = co2dat[dyr].mco2
                                                       + ((day - 182)
                                                       * (co2dat[dyr+1].mco2
                                                       - co2dat[dyr].mco2));
//                                                       / (double) CYCLE);

        }
    }

//    co2dat[dyr].year = telmnt[0].clm.co2year[pdyr] ;
  }
  else // 0 == telmnt[0].clm.tco2flag || 0 == dyr
  {
      telmnt[0].climate[telmnt[0].clm.I_CO2][day][pdyr] = telmnt[0].clm.getCO2LEVEL();
  } */

 // Assign information from files to telmnt.climate

    // Air temperature

    telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr] = tairdat[dyr].mon[day];

    // Daylength (hours)
    
    telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr] = telmnt[0].tem.atms.daylength( telmnt[0].row, day+1 );
    
    // Temperature Range

    telmnt[0].climate[telmnt[0].clm.I_TRANGE][day][pdyr] = trangedat[dyr].mon[day];

    // Daytime air temperature

    telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr] = telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr]
                            + 12.0*telmnt[0].climate[telmnt[0].clm.I_TRANGE][day][pdyr]/telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr]
                            * 1.732 / (2.0*3.14159) * sin( 3.14159 * telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr] / 24.0 );

//cout << "taird = " << day << " " << pdyr << " " << telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr] << " " << telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr] << " " << telmnt[0].climate[telmnt[0].clm.I_TRANGE][day][pdyr] << " " << telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr] << endl;

    #ifdef DEBUGX
      cout << " sin(1.57) test " << sin(1.57) << endl;
      printf( " tair = %8.21f, trange = %8.21f, taird = %8.21f ", telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr],
              telmnt[0].climate[telmnt[0].clm.I_TRANGE][day][pdyr], telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr] );
      cout << endl;
    #endif

    // Nighttime air temperature

    telmnt[0].climate[telmnt[0].clm.I_TAIRN][day][pdyr] = telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr]
                            - 12.0*telmnt[0].climate[telmnt[0].clm.I_TRANGE][day][pdyr]/(24.0 - telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr])
                            * 1.732 / (2.0*3.14159) * sin( 3.14159 * telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr] / 24.0 );
    
    // Precipitation

    telmnt[0].climate[telmnt[0].clm.I_PREC][day][pdyr] = precdat[dyr].mon[day];

    // Rain and Snowfall


    telmnt[0].clm.precsplt( telmnt[0].climate[telmnt[0].clm.I_PREC][day][pdyr],
                            telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr],
                            telmnt[0].climate[telmnt[0].clm.I_RAIN][day][pdyr],
                            telmnt[0].climate[telmnt[0].clm.I_SNWFAL][day][pdyr] );

//
//  Calculate vapor pressure deficit from vapor pressure
//

    telmnt[0].climate[telmnt[0].clm.I_VPR][day][pdyr] = vprdat[dyr].mon[day];

    telmnt[0].climate[telmnt[0].clm.I_VPDD][day][pdyr] = (10.*0.61078*exp((17.27*telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr])
                                                  /(telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr]+237.3)))-vprdat[dyr].mon[day];
    if(telmnt[0].climate[telmnt[0].clm.I_VPDD][day][pdyr] < ZERO) { telmnt[0].climate[telmnt[0].clm.I_VPDD][day][pdyr] = ZERO; }


    telmnt[0].climate[telmnt[0].clm.I_VPDN][day][pdyr] = (10.*0.61078*exp((17.27*telmnt[0].climate[telmnt[0].clm.I_TAIRN][day][pdyr])
                                                  /(telmnt[0].climate[telmnt[0].clm.I_TAIRN][day][pdyr]+237.3)))-vprdat[dyr].mon[day];
    if(telmnt[0].climate[telmnt[0].clm.I_VPDN][day][pdyr] < ZERO) { telmnt[0].climate[telmnt[0].clm.I_VPDN][day][pdyr] = ZERO; }


    // AOT40 ozone index

    telmnt[0].climate[telmnt[0].clm.I_AOT40][day][pdyr] = o3dat[dyr].mon[day];

//
//   Ndep
//   
     telmnt[0].adep[0][pdyr]= ndepdat[pdyr].adepval;
//  cout << "ndepval = " << telmnt[0].tem.ndepval << endl;
//
    // 10-meter windspeed
    
    telmnt[0].climate[telmnt[0].clm.I_WS10][day][pdyr] = ws10dat[dyr].mon[day];

    // CO2

    telmnt[0].climate[telmnt[0].clm.I_CO2][day][pdyr] = co2dat[dyr].mon[day];

  if(telmnt[0].row >= 0.0) { telmnt[0].tem.veg.hemisphere = 1; }
  else { telmnt[0].tem.veg.hemisphere = 0; }

//**************************************************************


  // Calculate GIRR during first year of simulation
  //   (Note: use same values throughout simulation)

  if( 1 == telmnt[0].clm.sradflag )
  {
    if( 1 == telmnt[0].lonlatflag ) { lat = telmnt[0].row; }


      telmnt[0].climate[telmnt[0].clm.I_GIRR][day][pdyr] = telmnt[0].clm.xgirr( lat,
                                                                               day+1,
                                                                       telmnt[0].clm.yrsumday,
                                                                       ndays );
//     cout << "girr = " << pdyr << " " << day << " " << telmnt[0].climate[telmnt[0].clm.I_GIRR][day][pdyr] << endl;
  }
  else
  {
      telmnt[0].climate[telmnt[0].clm.I_GIRR][day][pdyr] = girrdat[dyr].mon[day] * Watts2cal;
  }

  // Calculate NIRR, CLDINESS and PAR or retrieve from earlier calculations

    if( 1 == telmnt[0].clm.cldflag )
    {
      telmnt[0].climate[telmnt[0].clm.I_CLDS][day][pdyr] = cldsdat[dyr].mon[day];

      telmnt[0].climate[telmnt[0].clm.I_NIRR][day][pdyr] = telmnt[0].clm.xnirr( telmnt[0].climate[telmnt[0].clm.I_CLDS][day][pdyr],
	                                                                 telmnt[0].climate[telmnt[0].clm.I_GIRR][day][pdyr] );
    }
    else
    {
      telmnt[0].climate[telmnt[0].clm.I_NIRR][day][pdyr] = nirrdat[dyr].mon[day] * Watts2cal;

      telmnt[0].climate[telmnt[0].clm.I_CLDS][day][pdyr] = telmnt[0].clm.mkclds( telmnt[0].climate[telmnt[0].clm.I_GIRR][day][pdyr],
                                                                          telmnt[0].climate[telmnt[0].clm.I_NIRR][day][pdyr] );
    }

    if( 0 == telmnt[0].clm.parflag )
    {

      telmnt[0].climate[telmnt[0].clm.I_PAR][day][pdyr]  = telmnt[0].clm.xpar( telmnt[0].climate[telmnt[0].clm.I_CLDS][day][pdyr],
                                                                        telmnt[0].climate[telmnt[0].clm.I_NIRR][day][pdyr] );
    }
    else
    {
      telmnt[0].climate[telmnt[0].clm.I_PAR][day][pdyr] = pardat[dyr].mon[day] * Watts2cal;
    }

  if(pdyr == 0)
  {
//      avgprec += telmnt[0].climate[telmnt[0].clm.I_PREC][day][pdyr]/12.0;
//      avgtair += telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr]/12.0;
//      avgtaird += telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr]/12.0;
      avgprec += telmnt[0].climate[telmnt[0].clm.I_PREC][day][pdyr]/365.0;
      avgtair += telmnt[0].climate[telmnt[0].clm.I_TAIR][day][pdyr]/365.0;
      avgtaird += telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][pdyr]/365.0;
    telmnt[0].tem.veg.setRPREC( avgprec );
    //telmnt[0].tem.veg.setRTAIR( avgtair );
    telmnt[0].tem.veg.setRTAIR( avgtaird );
    if( day == 0 ) { telmnt[0].tem.veg.setTOPT( telmnt[0].tem.veg.getTOPTMIN( telmnt[0].tem.veg.cmnt ) ); }
    if( telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0] > telmnt[0].tem.veg.getTOPT() )
    {
//      cout << "resetting topt = " << telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0] << endl;
      telmnt[0].tem.veg.setTOPT( telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0] );
    }
//    cout << "topt in xtem = " << pdyr << " " << day << " " << telmnt[0].tem.veg.getTOPT() << " " << telmnt[0].tem.veg.cmnt << endl;

    if( day == 0 ) { telmnt[0].tem.veg.setTOPTMIC( telmnt[0].tem.veg.getTOPTMIN( telmnt[0].tem.veg.cmnt ) ); }
    if( telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0] > telmnt[0].tem.veg.getTOPTMIC() )
    {
      telmnt[0].tem.veg.setTOPTMIC( telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0] );
    }

    telmnt[0].tem.veg.phenology( telmnt[0].tem.veg.cmnt,
                       day,
                       telmnt[0].climate[telmnt[0].clm.I_TAIR][day][0],
                       telmnt[0].climate[telmnt[0].clm.I_TAIRD][day][0],
                       telmnt[0].climate[telmnt[0].clm.I_TAIRN][day][0],
                       1.0,
                       1.0,
                       0.0,
                       telmnt[0].climate[telmnt[0].clm.I_DAYL][day][0]);

//    avgphi += telmnt[0].tem.veg.getPHI()/12.0;
    avgphi += telmnt[0].tem.veg.getPHI()/365.0;
    avgphish += telmnt[0].tem.veg.getPHI()/30.0;

  }
    telmnt[0].tem.veg.setRPHI( avgphi );
    telmnt[0].tem.veg.setRPHISH( avgphish );
    telmnt[0].tem.veg.setRTAIRPHI( avgtaird * avgphi );

 // previous-day variables
  telmnt[0].tem.atms.setPREVTAIR( telmnt[0].climate[telmnt[0].clm.I_TAIR][day-1][0] );
  telmnt[0].tem.atms.setPREV2TAIR( telmnt[0].climate[telmnt[0].clm.I_TAIR][day-2][0] );
  telmnt[0].tem.soil.setPREVSPACK( ZERO );
  telmnt[0].tem.atms.setPREVCO2( telmnt[0].tem.atms.getCO2LEVEL() );
} // end of day loop
//
//    CALCULATE SLOPE OF DAYLENGTH
//
        for( day = 0; day < CYCLE-1; ++day )
        {

      telmnt[0].tem.veg.daylslope[day]  = telmnt[0].climate[telmnt[0].clm.I_DAYL][day+1][pdyr] - telmnt[0].climate[telmnt[0].clm.I_DAYL][day][pdyr];
     
        }


  telmnt[0].tem.initializeState( 12 );

  // Set year

//
//  BSF CHANGED YEAR DEFINITION
//
  telmnt[0].year = telmnt[0].clm.startyr
                   - totsptime
//                   - 1
                   + pdyr;

  // Copy TEMclm results to output variables

/*  if( 1 == telmnt[0].clm.predflag )
  {
    if ( (1 == spinoutfg
         && telmnt[0].year < telmnt[0].clm.startyr)
         || (2 == spinoutfg
         && telmnt[0].year >= (telmnt[0].clm.startyr-spinoutyrs))
         || (telmnt[0].year >= telmnt[0].clm.startyr) )
    {
      if( 1 == telmnt[0].clm.cldflag )
      {
        telmnt[0].carea = cldsdat[0].carea;
        telmnt[0].contnent = cldsdat[0].contnent;
      }
      else
      {
        telmnt[0].carea = nirrdat[0].carea;
        telmnt[0].contnent = nirrdat[0].contnent;
      }

      telmnt[0].atmswritepred( fclmpred,
                               pdyr,
                               clmpredmap,
                               telmnt[0].natmspred );
    }
  } */

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void updateTLCLUCGridCell( const int& pdyr, const int& eqtr )
{

  int dyr;
  int tstyr;
  int gisend;
  int ichrt;

//  gridlulc.gotoFirstNode();   // start at first LULCListNode

  fatalerr = 0;

  if( 0 == pdyr && 0 == eqtr )
  {
    // Get the total number of cohorts in the grid cell

    if( 1 == telmnt[0].lcluc.tlulcflag )
    {
      for( dyr = 0; dyr < (transtime); ++dyr )
      {
        gisend = mxcohrtdat[dyr].getdel( ifnumchrts );


        if( -1 == gisend )
        {
          cout << "Ran out of Number of Cohorts data";
          cout << endl << endl;
          flog1 << "Ran out of Number of Cohorts data";
          flog1 << endl << endl;

          exit( -1 );
        }

        // Check data for spatial coregistration errors

        fatalerr = telmnt[0].coregerr( flog1,
                                       "Climate",
                                       telmnt[0].col,
                                       telmnt[0].row,
                                      "MAXCOHORTS",
                                       mxcohrtdat[dyr].col,
                                       mxcohrtdat[dyr].row );

        if( fatalerr != 0 ) { exit( -1 ); }
      }
    }
    else // 0 == telmnt[0].lcluc.tlulcflag
    {
      gisend = mxcohrtdat[0].getdel( ifnumchrts );

      if( -1 == gisend )
      {
        cout << "Ran out of Number of Cohorts data";
        cout << endl << endl;
        flog1 << "Ran out of Number of Cohorts data";
        flog1 << endl << endl;

        exit( -1 );
      }

      // Check data for spatial coregistration errors

      fatalerr = telmnt[0].coregerr( flog1,
                                     "Climate",
                                     telmnt[0].col,
                                     telmnt[0].row,
                                     "MAXCOHORTS",
                                     mxcohrtdat[0].col,
                                     mxcohrtdat[0].row );

      if( fatalerr != 0 ) { exit( -1 ); }

      for( dyr = 1; dyr < (transtime); ++dyr )
      {
        mxcohrtdat[dyr].col = mxcohrtdat[0].col;
        mxcohrtdat[dyr].row = mxcohrtdat[0].row;
        mxcohrtdat[dyr].carea = mxcohrtdat[0].carea;
        mxcohrtdat[dyr].year = mxcohrtdat[0].year + dyr;
        mxcohrtdat[dyr].total = mxcohrtdat[0].total;
        mxcohrtdat[dyr].natchrts = mxcohrtdat[0].natchrts;
        mxcohrtdat[dyr].contnent = mxcohrtdat[0].contnent;
      }
    }


    // Get land use/land cover cohort data for  the grid cell

    if( 1 == telmnt[0].lcluc.tlulcflag )
    {
      for( dyr = 0; dyr < (transtime); ++dyr )
      {
        for( ichrt = 0; ichrt < mxcohrtdat[dyr].total; ++ichrt )
        {
          gisend = lulcdat[dyr][ichrt].getdel( iflulc );
//          cout << "land type = " << dyr << " " << lulcdat[dyr][ichrt].potveg << " " << lulcdat[dyr][ichrt].currentveg << " " << lulcdat[dyr][ichrt].subtype << endl;

          if( -1 == gisend )
          {
            flog1 << "Ran out of Land cover/land use data";
            flog1 << endl << endl;

            exit( -1 );
          }

          // Check data for spatial coregistration errors

          fatalerr = telmnt[0].coregerr( flog1,
                                         "Climate",
                                         telmnt[0].col,
                                         telmnt[0].row,
                                         "LULC",
                                         lulcdat[dyr][ichrt].col,
                                         lulcdat[dyr][ichrt].row );

          if( fatalerr != 0 ) { exit( -1 ); }
        }
      }
    }
    else // 0 == telmnt[0].lcluc.tlulcflag
    {
      for( ichrt = 0; ichrt < mxcohrtdat[0].total; ++ichrt )
      {
        gisend = lulcdat[0][ichrt].getdel( iflulc );

        if( -1 == gisend )
        {
          flog1 << "Ran out of Land cover/land use data";
          flog1 << endl << endl;

          exit( -1 );
        }

        fatalerr = telmnt[0].coregerr( flog1,
                                       "Climate",
                                       telmnt[0].col,
                                       telmnt[0].row,
                                       "LULC",
                                       lulcdat[0][ichrt].col,
                                       lulcdat[0][ichrt].row );

        if( fatalerr != 0 ) { exit( -1 ); }

        for( dyr = 1; dyr < (transtime); ++dyr )
        {
          lulcdat[dyr][ichrt].year = lulcdat[0][ichrt].year + dyr;
          lulcdat[dyr][ichrt].isrccohort = lulcdat[0][ichrt].isrccohort;
          lulcdat[dyr][ichrt].standage = lulcdat[0][ichrt].standage;
          lulcdat[dyr][ichrt].chrtarea = lulcdat[0][ichrt].chrtarea;
          lulcdat[dyr][ichrt].potveg = lulcdat[0][ichrt].potveg;
          lulcdat[dyr][ichrt].currentveg = lulcdat[0][ichrt].currentveg;
          lulcdat[dyr][ichrt].subtype = lulcdat[0][ichrt].subtype;
          lulcdat[dyr][ichrt].agstate = lulcdat[0][ichrt].agstate;
          lulcdat[dyr][ichrt].agprevstate = lulcdat[0][ichrt].agprevstate;
          lulcdat[dyr][ichrt].tillflag = lulcdat[0][ichrt].tillflag;
          lulcdat[dyr][ichrt].fertflag = lulcdat[0][ichrt].fertflag;
          lulcdat[dyr][ichrt].irrgflag = lulcdat[0][ichrt].irrgflag;
          lulcdat[dyr][ichrt].disturbflag = lulcdat[0][ichrt].disturbflag;
          lulcdat[dyr][ichrt].disturbmonth = lulcdat[0][ichrt].disturbmonth;
          lulcdat[dyr][ichrt].FRI = lulcdat[0][ichrt].FRI;
          lulcdat[dyr][ichrt].slashpar = lulcdat[0][ichrt].slashpar;
          lulcdat[dyr][ichrt].vconvert = lulcdat[0][ichrt].vconvert;
          lulcdat[dyr][ichrt].prod10par = lulcdat[0][ichrt].prod10par;
          lulcdat[dyr][ichrt].prod100par = lulcdat[0][ichrt].prod100par;
          lulcdat[dyr][ichrt].vrespar = lulcdat[0][ichrt].vrespar;
          lulcdat[dyr][ichrt].sconvert = lulcdat[0][ichrt].sconvert;
          lulcdat[dyr][ichrt].region = lulcdat[0][ichrt].region;
        }
      }
    }
  } // end of 0 == pdyr

  if( 0 == pdyr ) { tstyr = 0; }
  else if( istateflag < 2 && pdyr < (totsptime+1) && spinflag == 1 )
  {
    tstyr = 1;
  }
  else
  {
    if( istateflag < 2 )
    {
      tstyr = pdyr - totsptime;
    }
    else { tstyr = pdyr; }
  }

  // Check data for temporal coregistration errors in mxcohrtdat

  if( 1 == telmnt[0].lcluc.tlulcflag
      && pdyr >= (totsptime+1)
      && telmnt[0].year != mxcohrtdat[tstyr].year )
  {
    cout << " Year in CLM data does not match ";
    cout << " Year in MAXCOHORTS data" << endl;
    cout << " at Lon = " << telmnt[0].col;
    cout << "  Lat = " << telmnt[0].row << endl;
    cout << "  CLM year = " << telmnt[0].year;
    cout << "  MAXCOHORTS year = " << mxcohrtdat[tstyr].year;
    cout << endl << endl;

    flog1 << " Year in CLM data does not match ";
    flog1 << " Year in MAXCOHORTS data" << endl << endl;
    flog1 << " at Lon = " << telmnt[0].col;
    flog1 << "  Lat = " << telmnt[0].row << endl;
    flog1 << " CLM year = " << telmnt[0].year;
    flog1 << " MAXCOHORTS year = " << mxcohrtdat[tstyr].year;
    flog1 << endl << endl;

    exit( -1 );
  }


  // Pass mxcohortdat information to telmnt[0]

  telmnt[0].maxcohorts = mxcohrtdat[tstyr].total;
  telmnt[0].natcohorts = mxcohrtdat[tstyr].natchrts;
  telmnt[0].carea = mxcohrtdat[tstyr].carea;
  telmnt[0].contnent = mxcohrtdat[tstyr].contnent;

  if( 0 == tstyr )
  {
    telmnt[0].prvmxcohrts = mxcohrtdat[tstyr].total;
  }
  else
  {
    telmnt[0].prvmxcohrts = mxcohrtdat[tstyr-1].total;
  }


  for( ichrt = 0; ichrt < telmnt[0].maxcohorts; ++ichrt )
  {
    // Check data for temporal coregistration errors in lulcdat

    if( 1 == telmnt[0].lcluc.tlulcflag
        && pdyr >= (totsptime+1)
        && telmnt[0].year != lulcdat[tstyr][ichrt].year )
    {
      cout << " Year in CLM data does not match ";
      cout << " Year in LCLUC data" << endl;
      cout << " at Lon = " << telmnt[0].col;
      cout << "  Lat = " << telmnt[0].row << endl;
      cout << "  CLM year = " << telmnt[0].year;
      cout << "  LCLUC year = " << lulcdat[tstyr][ichrt].year;
      cout << " for cohort " << (ichrt+1);
      cout << endl << endl;

      flog1 << " Year in CLM data does not match ";
      flog1 << " Year in LCLUC data" << endl << endl;
      flog1 << " at Lon = " << telmnt[0].col;
      flog1 << "  Lat = " << telmnt[0].row << endl;
      flog1 << " CLM year = " << telmnt[0].year;
      flog1 << " LCLUC year = " << lulcdat[tstyr][ichrt].year;
      cout << " for cohort " << (ichrt+1);
      flog1 << endl << endl;

      exit( -1 );
    }


    // Pass lulcdat information to telmnt[0].cohort

    telmnt[0].cohort[ichrt].srcCohort = lulcdat[tstyr][ichrt].isrccohort;
    telmnt[0].cohort[ichrt].standage = lulcdat[tstyr][ichrt].standage;
    telmnt[0].cohort[ichrt].chrtarea = lulcdat[tstyr][ichrt].chrtarea;
    telmnt[0].cohort[ichrt].potveg = lulcdat[tstyr][ichrt].potveg;
    telmnt[0].cohort[ichrt].currentveg = lulcdat[tstyr][ichrt].currentveg;
    telmnt[0].cohort[ichrt].subtype = lulcdat[tstyr][ichrt].subtype;
//cout << "prevsubtype = " << telmnt[0].cohort[ichrt].subtype << " " << tstyr << " " << ichrt << endl;
    telmnt[0].cohort[ichrt].agstate = lulcdat[tstyr][ichrt].agstate;
    telmnt[0].cohort[ichrt].agprvstate = lulcdat[tstyr][ichrt].agprevstate;
    telmnt[0].cohort[ichrt].tillflag = lulcdat[tstyr][ichrt].tillflag;
    telmnt[0].cohort[ichrt].fertflag = lulcdat[tstyr][ichrt].fertflag;
    telmnt[0].cohort[ichrt].irrgflag = lulcdat[tstyr][ichrt].irrgflag;
    telmnt[0].cohort[ichrt].disturbflag = lulcdat[tstyr][ichrt].disturbflag;
    telmnt[0].cohort[ichrt].disturbmonth = lulcdat[tstyr][ichrt].disturbmonth;

    // If cohort is in agriculture the during first year of the
    //   historical study period, convert cohort to agriculture
    //   during the first year of the transient spinup.  Keep
    //   cohort in agriculture throughout spinup period

/*    if( 1 == pdyr
        && istateflag < 2
        && 1 == pdyr
        && 1 == telmnt[0].cohort[ichrt].agstate
        && 1 == telmnt[0].cohort[ichrt].agprvstate)
    {
      telmnt[0].cohort[ichrt].agprvstate = 0;
      telmnt[0].cohort[ichrt].disturbflag = 1;
      telmnt[0].cohort[ichrt].disturbmonth = 1;
    } */

   if( pdyr > 1
       && pdyr < (totsptime+2)
       && istateflag < 2
       && 1 == telmnt[0].cohort[ichrt].agstate
       && 1 == telmnt[0].cohort[ichrt].agprvstate )
   {
      telmnt[0].prvmxcohrts = mxcohrtdat[tstyr].total;
   }

    telmnt[0].cohort[ichrt].FRI = lulcdat[tstyr][ichrt].FRI;
    telmnt[0].cohort[ichrt].slashpar = lulcdat[tstyr][ichrt].slashpar;
    telmnt[0].cohort[ichrt].vconvert = lulcdat[tstyr][ichrt].vconvert;
    telmnt[0].cohort[ichrt].prod10par = lulcdat[tstyr][ichrt].prod10par;
    telmnt[0].cohort[ichrt].prod100par = lulcdat[tstyr][ichrt].prod100par;
    telmnt[0].cohort[ichrt].vrespar = lulcdat[tstyr][ichrt].vrespar;
    telmnt[0].cohort[ichrt].sconvert = lulcdat[tstyr][ichrt].sconvert;
    telmnt[0].region = lulcdat[tstyr][ichrt].region;

//    if( 0 == tstyr )
//    {
//      telmnt[0].cohort[ichrt].prvchrtarea = lulcdat[tstyr][ichrt].chrtarea;
//    }
//    else
//    {
//      telmnt[0].cohort[ichrt].prvchrtarea = lulcdat[tstyr-1][ichrt].chrtarea;
//    }

    telmnt[0].cohort[ichrt].cmnt = telmnt[0].lcluc.getCommunityType( lulcdat[tstyr][ichrt].subtype );
//cout << "subtype = " << lulcdat[tstyr][ichrt].subtype << " " <<  telmnt[0].cohort[ichrt].cmnt << endl;
    telmnt[0].cohort[ichrt].agcmnt = telmnt[0].cohort[ichrt].cmnt;

//    if( pdyr > 0 && 1 == lulcdat[tstyr][ichrt].agstate )
   if( 1 == lulcdat[tstyr][ichrt].agstate || 2 == lulcdat[tstyr][ichrt].agstate )
    {
      telmnt[0].cohort[ichrt].agcmnt = telmnt[0].lcluc.getCommunityType( lulcdat[tstyr][ichrt].currentveg );
    }
cout << "lulc type = " << ichrt << " " << telmnt[0].cohort[ichrt].cmnt << " " << telmnt[0].cohort[ichrt].agcmnt << " " << lulcdat[tstyr][ichrt].agstate << endl;
  }

};

/* *************************************************************
************************************************************* */


/* **************************************************************
************************************************************** */

void updateTTEMGridCell( const int& pdyr, const int& CYCLE, const int ndays[] )
{
  int dm;

  int ichrt;

  int tchrt;

  double avgphish = ZERO;
/* *************************************************************
            INITIALIZE TEM STATE FOR NEW COHORTS
************************************************************* */

  if( telmnt[0].maxcohorts > telmnt[0].prvmxcohrts )
  {
    for ( ichrt = telmnt[0].prvmxcohrts;
          ichrt < telmnt[0].maxcohorts;
          ++ichrt )
    {
      tchrt = telmnt[0].cohort[ichrt].srcCohort - 1;

      telmnt[0].setCohortTEMState( telmnt[0].cohort[tchrt],
                                   telmnt[0].cohort[ichrt],
                                   CYCLE );
    }
  }

/* *************************************************************
                      UPDATE TEM FOR GRID CELL
************************************************************* */


/* *************************************************************
                 BEGIN VEGETATION MOSAIC LOOP
************************************************************* */

  for( ichrt = 0; ichrt < telmnt[0].maxcohorts; ++ichrt )
  {
    // Get vegetation community type of cohort
 
    telmnt[0].tem.veg.cmnt = telmnt[0].cohort[ichrt].cmnt;

    cout << "cohort = " << ichrt << " " << telmnt[0].tem.veg.cmnt << endl;
 
    // Determine soil characteristics for cohort
 
    telmnt[0].tem.soil.xtext( telmnt[0].tem.veg.cmnt,
                              telmnt[0].tem.soil.getPCTSILT(),
                              telmnt[0].tem.soil.getPCTCLAY() );
 
    for( dm = 0; dm < CYCLE; ++dm )
    {
       avgphish += telmnt[0].tem.veg.getPHI()/30.0;
    }
     telmnt[0].tem.veg.setRPHISH( avgphish );
//     cout << "RPHISH = " << telmnt[0].tem.veg.getRPHISH() << endl;

    for( dm = 0; dm < CYCLE; ++dm )
    {

      // Run TEM
//      cout << "month = " << dm << endl;
      telmnt[0].updateTEMmonth( equil,
                                totsptime,
                                pdyr,
                                dm,
                                ichrt,
                                CYCLE );
 
    } // end of CYCLE loop
 
    if( 2 == ostateflag && telmnt[0].tem.totyr == ostateyear )
    {
      telmnt[0].writeCohortState( ofstate, ichrt, CYCLE );
    }
 
//    cout << "diag = " << telmnt[0].tem.totyr << " " << telmnt[0].tem.startyr << " " << telmnt[0].tem.endyr << " " << spinoutyrs << endl;
    if ( (1 == spinoutfg && telmnt[0].tem.totyr < telmnt[0].tem.startyr)
          || (2 == spinoutfg
          && telmnt[0].tem.totyr >= (telmnt[0].tem.startyr-spinoutyrs))
          || (telmnt[0].tem.totyr >= telmnt[0].tem.startyr
          && telmnt[0].tem.totyr <= telmnt[0].tem.endyr)
          && 0 == (telmnt[0].wrtyr%telmnt[0].tem.diffyr) )
    {
 
      // Output TEM transient results for specified years to files
 
       if(telmnt[0].intervalflag == 0)
       {
        telmnt[0].temwritepred( ftempred,
                                tempredmap,
                                pdyr,
                                ichrt,
                                telmnt[0].ntempred,
                                spinoutfg,
                                1,
                                CYCLE,
                                ndays );
       }
       else if(telmnt[0].intervalflag == 1 )
       {
        telmnt[0].temwritepredm( ftempred,
                                tempredmap,
                                pdyr,
                                ichrt,
                                telmnt[0].ntempred,
                                spinoutfg,
                                1,
                                12,
                                ndays );
       }

    }
 
  } // End of cohort loop
 
};


