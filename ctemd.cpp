/* **************************************************************
*****************************************************************
CTEMD.CPP - Calibration version of the Terrestrial Ecosystem
	          Model Version 4.5

Modifications:

20060127 - DWK created by modifying ctem50b5.cpp
20060127 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20060127 - DWK changed include from tclm50b5.h to tclm437.h
20060127 - DWK changed include from ttem50b5.h to ttem437.h
20060127 - DWK added C_O3FLAG to enum ecalkey
20060127 - DWK deleted enum stcalkey and associated functions
20060127 - DWK deleted void pcdisplayYrSTM(), void stcalibrate()
           and void updateSoilTempPar()
20060127 - DWK deleted global double dst10[CYCLE]
20060127 - DWK changed global TEMclm50 clm to TEMclm43 clm
20060127 - DWK changed global TTEM50 tem to TTEM43 tem
20060127 - DWK changed global double climate[NUMATMS][CYCLE+1]
           to double climate[NUMATMS][CYCLE]
20060127 - DWK deleted global stcalkey stvar;
2007  - TWC/BSF summary
  MAXCAL = 26, MAXWCAL = 13, wcalver[MAXWCAL]
  GDDMIN, GDDSEED, GDDHARVST, TKILL: add to ecalkey,
    displayECalibPar, setECalVar, updateECalibPar
  canevk, pmiss, maxint, soilbare, WLEAF, CDLEAF, krad,
    kvpd, lsc, minlwp: add to wcalkey, displayWCalibPar,
	updateWCalibPar
  main: set fert1950flag, irrgflag, state, prvstate to 0
    pass monthly climate info. to TEM: VPD, TRANGE, DAYL
  calclmin: add yrpar, sumPME
    read in climate data: add VPD, TRANGE
	add tem.hemisphere
	call function daylength
  pcdisplayClm: add VPD, TRANGE
  resetInitState: comment out tem.ag.state = 0 and
    tem.ag.prvstate = 0
	add previous month's vars: PRVLAI, PRVLEAFADD, PRVLEAFDROP,
	 PRVLEAFEFCY, PREVPAR, PRVPME
  updateECalibPar: also add KRA, KRB, remove RMAX

*****************************************************************
************************************************************** */

#define CALIBRATE_TEM

//#define AUTOCAL

#define ANSI_CPP

//#define BORLAND_CPP

#ifdef ANSI_CPP
  #include<cstdio>

  using std::fopen;
  using std::fclose;
  using std::printf;
  using std::sprintf;
  using std::fscanf;
  using std::FILE;

  #include<curses.h>

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

  #include<cmath>
  
  #include<vector>

  using std::vector;

  #include<cstring>

  #include<string>

  using std::string;
  
  #include<sstream>

  using std::ostringstream;

  #include<ctime>

  using std::time_t;
  using std::ctime;
#endif

#ifdef BORLAND_CPP
  #include<cstdio>

  using std::fopen;
  using std::fclose;
  using std::printf;
  using std::sprintf;
  using std::fscanf;
  using std::FILE;

  #include<conio.h>

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

  #include<stdlib>

  #include<math>

  #include<vector>

  using std::vector;

  #include<cctype>

  using std::toupper;

  #include<cstring>

  #include<string>

  using std::string;
  
  #include<sstream>

  using std::ostringstream;

  #include<time>

  using std::time_t;
  using std::ctime;
#endif

const int MAXTCAL = 72;
const int YRAVGS_ROW = 20;
const int INSTR_ROW = 40;


#include "temconstsd.hpp"

//Modules representing climate and TEM

#include "tclmd.h"    // Tclm45 Class
#include "tclmdatd.h" // Tclmdat45 Class
#include "tco2datd.h"       // CO2dat45 class
#include "tsoldat45.h" // Soildat45 class
#include "telvdat45.h" // Elevdat45 class
#include "tatmdep.h" // Atmospheric deposition class
#include "ttemd_cwd.h"    // Ttem45 Class

Tatmdep atmdep;
            
enum tcalkey { T_TOL,      T_TAVG,     T_CO2,      T_AVLNFLAG, 
               T_NFEED,    T_BLINE,    T_MOISTLIM, T_O3FLAG,   
               T_RESET,    T_MANFLAG,
               // 10 "settings" variables
               T_GDDMIN,   T_GDDSEED,  T_GDDHARVST, T_TKILL,
               T_AGSTATE,  T_AGPROD,   T_TILLFLAG,  T_FERTFLAG,
               T_IRRGFLAG,
               // 9 "ag-related" variables
               T_CMAX,     T_KRA,      T_KDC,
               T_TAULEAF,  T_TAUSTEMA, T_TAUSTEMI,  T_TAUROOT,
               T_TAUSEED,  T_NMAX,      T_MNUP,
               T_CNSOL,
               T_CNLTR,    T_MNFIX,    T_NLOSS,
               // 19 "datfile-related" variables
               T_SLA,        T_MOPT,
               // 2 "vegfile-related" variables
               T_GROOT,    T_KSTEM,    T_CDLEAF,    T_WLEAF,
               T_GSMIN,     T_GSA,
               T_KEXT,     T_ALBEDO,   T_LSCMIN,    T_RHOSTEM,
               // 12 "conductance-related" variables
               T_OPTCN0,   T_OPTCN1,   T_OPTCN2,    T_OPTCN3,
               T_OPTCN4,   T_OPTCN5,   T_OPTCN6,    T_OPTCN7,
               T_OPTCN8,   T_OPTCN9,   
               // 10 optional c/n fluxes
               T_OPTH2O0,  T_OPTH2O1,  T_OPTH2O2,   T_OPTH2O3,
               T_OPTH2O4,  T_OPTH2O5,  T_OPTH2O6,   T_OPTH2O7
               // 8 optional water fluxes
               }; 
                
                  


/* *************************************************************
		 Function Declarations
************************************************************* */

// structure of new methods next, prev, jump, back on tcalkey allow sub-loops
// for cycling between variables of similar type within the larger list

inline tcalkey& next( tcalkey& s )
{
  if(T_MANFLAG == s) { return s = T_TOL; }
  else if(T_IRRGFLAG == s) { return s = T_GDDMIN; }
  else if(T_NLOSS == s) { return s = T_CMAX; }
  else if(T_MOPT == s) { return s = T_SLA; }
  else if(T_RHOSTEM == s) { return s = T_GROOT; }
  else if(T_OPTCN9 == s) { return s = T_OPTCN0; }
  else if(T_OPTH2O7 == s) { return s = T_OPTH2O0; }
  else { return s = tcalkey(s+1); }
}

inline tcalkey& prev( tcalkey& s )
{
  if(T_TOL == s) { return s = T_MANFLAG; }
  else if(T_GDDMIN == s) { return s = T_IRRGFLAG; }
  else if(T_CMAX == s) { return s = T_NLOSS; }
  else if(T_SLA == s) { return s = T_MOPT; }
  else if(T_GROOT == s) { return s = T_RHOSTEM; }
  else if(T_OPTCN0 == s) { return s = T_OPTCN9; }
  else if(T_OPTH2O0 == s) { return s = T_OPTH2O7; }
  else { return s = tcalkey(s-1); }
}

inline tcalkey& jump(tcalkey& s)
{
  if((T_TOL <= s) && (T_MANFLAG >= s)) { return s = T_GDDMIN; }
  if((T_GDDMIN <= s) && (T_IRRGFLAG >= s)) { return s = T_CMAX; }
  if((T_CMAX <= s) && (T_NLOSS >= s)) { return s = T_SLA; }
  if((T_SLA <= s) && (T_MOPT >= s)) { return s = T_GROOT; }
  if((T_GROOT <= s) && (T_RHOSTEM >= s)) { return s = T_OPTCN0; }
  if((T_OPTCN0 <= s) && (T_OPTCN9 >= s)) { return s = T_OPTH2O0; }
  if((T_OPTH2O0 <= s) && (T_OPTH2O7 >= s)) { return s = T_TOL; }
  else { return s = tcalkey(s); }       
}

inline tcalkey& back(tcalkey& s)
{
  if((T_TOL <= s) && (T_MANFLAG >= s)) { return s = T_OPTH2O0; }
  if((T_GDDMIN <= s) && (T_IRRGFLAG >= s)) { return s = T_TOL; }
  if((T_CMAX <= s) && (T_NLOSS >= s)) { return s = T_GDDMIN; }
  if((T_SLA <= s) && (T_MOPT >= s)) { return s = T_CMAX; }
  if((T_GROOT <= s) && (T_RHOSTEM >= s)) { return s = T_SLA; }
  if((T_OPTCN0 <= s) && (T_OPTCN9 >= s)) { return s = T_GROOT; }
  if((T_OPTH2O0 <= s) && (T_OPTH2O7 >= s)) { return s = T_OPTCN0; }
  else { return s = tcalkey(s); }       
}

void autocalibrate( void );

void calibrate( int k ); //// TWC

void displayCalibPar( tcalkey& tcal ); //// TWC

void initializeCLMGridCell( void ); /// TWC

void initRun( void ); /// TWC

void pcdisplayYear( const int& manflag ); //// TWC

void resetInitState( void );
void resetInitStateCalib( void );

void setCalibVar( void ); //// TWC

int setGIStopography( ofstream& flog1,
                      FILE* fstxt,
                      FILE* felev ); /// TWC

int testEquil( void );

void updateCalibPar( tcalkey& tcal,
                     const double& factor,
                     int& t); //// TWC
                     
void updateTCLMGridCell( const int& pdyr,
                         const int& CYCLE,
                         const int ndays[] );

// *************************************************************

int agProdFlag;

double climate[NUMATMS][366][MAXRTIME];
double adep[1][MAXRTIME];
Tclm45 clm;

Ttem45 tem;

int CYCLE;
int ndays[12];
int year;
int i,j;
double sumdays;

int reset;

int endgrid;

int equilsol;

int firstcal;

int xdyr = 0;
int newclm = 0;
int newtext = 0;

double tcalvar[MAXTCAL]; //// TWC

tcalkey tvar; //// TWC


int ivar;

int kdinflg = 0;
int stateflag = 0;
int manflag = 0;
int spinflag = 0;

int RTIME;
int firstyear;
int cdyear;
int de_startyear; 
int de_nyears; 

int calibcmnt; // community that is being calibrated

double col;
double row;
double lon;
double lat;
double elev;
string contnent;

#ifdef AUTOCAL
  double nppsat_t; 
  double npp_t; 
  double gpp_t; 
  double vegc_t; 
  double soilc_t; 
  double avln_t; // target values for calibration
  
  double autocal_tol; // proportional tolerance for auto-calibration
  double init_adj_fac;
  double adj_fac;
  double nmax_fac = 1.0;
    
  int calstep = 0;
  int tempcalstep = 0;
  int target_error[14];
#endif

Clmdat45 girrdat[MAXRTIME];
Clmdat45 cldsdat[MAXRTIME];
Clmdat45 nirrdat[MAXRTIME];
Clmdat45 pardat[MAXRTIME];
Clmdat45 tairdat[MAXRTIME];
Clmdat45 precdat[MAXRTIME];
Clmdat45 trangedat[MAXRTIME];
Clmdat45 vprdat[MAXRTIME];
Clmdat45 ws10dat[MAXRTIME];
Clmdat45 o3dat[MAXRTIME];
CO2dat45 co2dat[MAXRTIME+1];

Atmdepdat ndepdat[MAXRTIME];

double vegceq[MAXRTIME];
double soilceq[MAXRTIME];
double vegneq[MAXRTIME];
double soilneq[MAXRTIME];

FILE* ifgirr;
FILE* ifnirr;
FILE* ifpar;
FILE* ifclds;
FILE* iftair;
FILE* ifprec;
FILE* iftrange;
FILE* ifvpr;
FILE* ifws10;
FILE* ifo3;
FILE* ifco2;
FILE* ifndep;

FILE* fstxt;
FILE* felev;

string calclm;
string tclds;
string ttair;
string tprec;
string td40;
string tdtr;
string tvapr;
string tws10;

ofstream flog1;
char flog_buffer[500];

/* *************************************************************
**********************START MAIN PROGRAM************************
************************************************************* */

int main()
{

  int dm;
  int k;
  int key;

  reset = 1;
  agProdFlag = 0;
  tem.ag.tillflag = 0;
  tem.ag.fertflag = 0;
  tem.ag.fert1950flag = 0;
  tem.ag.irrgflag = 0;
  tem.ag.irrg1950flag = 0;
  tem.ag.state = 0;
  tem.ag.prvstate = 0;
  
  tem.tauavg = 1.0;

  cout << "Reading input information from " << tem.goname << endl;

  /// Master initialization rourtine reads in all relevant run information
  /// from file pointed to by tem_in.txt 
  initRun();
  
  if( spinflag == 2 ) { firstyear = (de_startyear + 1 - clm.startyr); }
  else { firstyear = 0; }
  
  for( xdyr = 0; xdyr < RTIME; ++xdyr )
  {
    year = xdyr + clm.startyr;

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
//    {
    updateTCLMGridCell( xdyr, CYCLE, ndays ); 
    /// load all years of climate data into climate[][][] variable
//    }
//cout << "out of updateTCLMGridCell" << endl;
  }
  
  /// initialize curses
  initscr();
  clear();
  echo();
  cbreak();
  key = keypad(stdscr, 1);

  /// reset initial state of TEM
  resetInitState();

  firstcal = 1;
  tem.topwind = 0;

  clear();
  
  //see tem constructor for meanings of default values
  tcalvar[T_OPTCN0] = 1; 
  tcalvar[T_OPTCN1] = 3; 
  tcalvar[T_OPTCN2] = 4; 
  tcalvar[T_OPTCN3] = 11; 
  tcalvar[T_OPTCN4] = 30; 
  tcalvar[T_OPTCN5] = 35; 
  tcalvar[T_OPTCN6] = 50; 
  tcalvar[T_OPTCN7] = 52; 
  tcalvar[T_OPTCN8] = 17; 
  tcalvar[T_OPTCN9] = 66; 
    
  tcalvar[T_OPTH2O0] = 4; 
  tcalvar[T_OPTH2O1] = 5; 
  tcalvar[T_OPTH2O2] = 36; 
  tcalvar[T_OPTH2O3] = 13; 
  tcalvar[T_OPTH2O4] = 20; 
  tcalvar[T_OPTH2O5] = 10; 
  tcalvar[T_OPTH2O6] = 34; 
  tcalvar[T_OPTH2O7] = 14;
  
  move(INSTR_ROW , 1);
  printw( " Keyboard commands for calibration: \n" );
  printw( "      Press 'End' to pause simulation  \n" );
  printw( "      Press 'Enter' to resume simulation  \n" );
  printw( "      Press 'Insert' to type in a value for a parameter/variable  \n" );
  printw( "      Press the right or left arrow to cycle between parameters/variables in the same category \n" );
  printw( "      Press the up or down arrows to change the category of parameters/variables \n" );
  printw( "      Press the page up or page down buttons to increment or decrement the chosen parameter/variable \n" );
  printw( "      Press 'Home' to quit \n" );
  refresh();
  
  #ifdef AUTOCAL
    tem.tauavg = 21.0;
  #endif
  
  while( xdyr > -10 )
  {
    key = nodelay(stdscr, 1);
    noecho();
    
    if( spinflag == 2 ) { cdyear = (de_startyear + 1 - clm.startyr) + (xdyr%de_nyears); }
    else { cdyear = 0; }

//    cout << "xdyr = " << xdyr << " " << cdyear << " " << de_startyear << " " << clm.startyr << " " << de_nyears << endl;

    tem.atms.setNDEP( adep[0][cdyear] );
     
    for( dm = 0; dm < CYCLE; ++dm )
    {
      // Pass daily climate information to TEM

      tem.atms.setGIRR( climate[clm.I_GIRR][dm][cdyear] );
      tem.atms.setCLDS( climate[clm.I_CLDS][dm][cdyear] );
      tem.atms.setNIRR( climate[clm.I_NIRR][dm][cdyear] );
      tem.atms.setPAR(  climate[clm.I_PAR][dm][cdyear] );
      tem.atms.setTAIR( climate[clm.I_TAIR][dm][cdyear] );
      tem.atms.setTAIRD( climate[clm.I_TAIRD][dm][cdyear] );
      tem.atms.setTAIRN( climate[clm.I_TAIRN][dm][cdyear] );
      tem.atms.setPREC( climate[clm.I_PREC][dm][cdyear] );
      tem.atms.setRAIN( climate[clm.I_RAIN][dm][cdyear] );
      tem.atms.setSNOWFALL( climate[clm.I_SNWFAL][dm][cdyear] );
      tem.atms.setCO2( climate[clm.I_CO2][dm][0] );
      tem.atms.setVPR( climate[clm.I_VPR][dm][cdyear] );
      tem.atms.setVPDD( climate[clm.I_VPDD][dm][cdyear] );
      tem.atms.setVPDN( climate[clm.I_VPDN][dm][cdyear] );
      tem.atms.setWS10( climate[clm.I_WS10][dm][cdyear] );
      tem.atms.setTRANGE( climate[clm.I_TRANGE][dm][cdyear] );
      tem.atms.setDAYL( climate[clm.I_DAYL][dm][cdyear] );
      tem.atms.setAOT40( climate[clm.I_AOT40][dm][cdyear] );

//      cout << "diag = " << tem.atms.getGIRR() << " " << tem.atms.getCLDS() << " " << tem.atms.getNIRR() << " " << tem.atms.getPAR() << " " << tem.atms.getTAIR() << " " << tem.atms.getTAIRD() << " " << tem.atms.getTAIRN() << " " << tem.atms.getPREC() << " " << tem.atms.getRAIN() << " " << tem.atms.getSNOWFALL() << " " << tem.atms.getCO2() << " " << tem.atms.getVPR() << " " << tem.atms.getVPDD() << " " << tem.atms.getVPDN() << " " << tem.atms.getWS10() << " " << tem.atms.getTRANGE() << " " << tem.atms.getDAYL() << " " << tem.atms.getAOT40() << endl;
      tem.stepmonth( xdyr,
                     dm,
                     tem.intflag,
                     tem.tol,
                     CYCLE,
                     1 );

//  move(INSTR_ROW - 1 , 1);
//  printw( " tem.I_SOLC = %1d, tem.y[I_SOLC] = %6.1lf : \n", tem.I_SOLC, tem.getY(tem.I_SOLC) );

     tem.coutputTEMmonth( dm, CYCLE);

    }


for(dm = 0; dm < 12; dm++)
{

  #ifdef CALIBRATE_TEM
    // Display monthly results to DOS screen
    tem.pcdisplayMonth( xdyr, dm );
  #endif

}
    
    vegceq[(xdyr%de_nyears)] = tem.veg.getVEGC();
    soilceq[(xdyr%de_nyears)] = tem.getY(tem.I_SOLC);
    vegneq[(xdyr%de_nyears)] = tem.veg.getVEGN();
    soilneq[(xdyr%de_nyears)] = tem.getY(tem.I_SOLN);


    ++xdyr;
    
    #ifdef AUTOCAL
      tem.equil = 0;
      autocalibrate();
    #endif

// Check to see if steady state conditions have been reached.

    if( 1 == equilsol ) { endgrid = 1; }

    if( 1 == tem.equil && xdyr >= tem.strteq && 0 == equilsol )
    {
      equilsol = tem.testEquilibrium(xdyr,
                                     de_nyears,
                                     vegceq[(xdyr%de_nyears)],
                                     soilceq[(xdyr%de_nyears)],
                                     vegneq[(xdyr%de_nyears)],
                                     soilneq[(xdyr%de_nyears)] );
    }

    pcdisplayYear( manflag );

    if( 1 == tem.adapttol && (xdyr+1) == tem.maxyears )
    {
      endgrid = 1;
      tem.tolbomb = 1;
    }

    if( 1 == manflag || 1 == endgrid )
    {
      while(1)
      {
        key = nodelay(stdscr, 0); //set getch to blocking input for calibrate
        k = getch();
        calibrate( k );
        if ( getch() == 10 ) { break; } // enter key resumes flow
      }
    }

    k = getch();
    if( k == KEY_END ) // end key
    {
      while(1)
      {
        key = nodelay(stdscr, 0);  //set getch to blocking input for calibrate
        k = getch(); 
        calibrate( k );
        if ( getch() == 10 ) // enter key resumes normal program flow
        { 
          key = nodelay(stdscr, 1); // reset getch to non-blocking input
          break; 
        }
      }
    }
    else if( k == KEY_HOME ) { break; } // 'Home' ends the program
  } // end of main while loop

  // close all input files and end window
  endwin();
  
  if( 1 == clm.cldflag ) { fclose( ifclds ); }
  else { fclose( ifnirr ); }
  if( 0 == clm.sradflag ) { fclose( ifgirr ); }
  if( 1 == clm.parflag ) { fclose( ifpar ); }

  fclose( iftair );
  fclose( ifprec );
  fclose( ifvpr );
  fclose( ifco2 );
  fclose( iftrange );
  fclose( ifws10 );
  fclose( ifo3 );
  fclose( ifndep );

  fclose( fstxt );
  fclose( felev );
  
  tem.gofile.close();
  flog1.close();

  cout << " Closed all files! " << endl;

  return 0;
};

/* *************************************************************
*********************END OF MAIN PROGRAM************************
************************************************************* */


/* *************************************************************
************************************************************** */
#ifdef AUTOCAL
void autocalibrate( void )
{

  reset = 0;
  
  if( xdyr == 1 )
  {
    ifstream autocal_fin;
    autocal_fin.open( "autocal.info.txt", ios::in );

    autocal_fin.ignore(30,'=');
    autocal_fin >> nppsat_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> npp_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> gpp_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> vegc_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> soilc_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> avln_t;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> autocal_tol;
    
    autocal_fin.ignore(30,'=');
    autocal_fin >> init_adj_fac;
        
    autocal_fin.close();
  }
  
  if( xdyr%((int)(5*tem.tauavg)) == 0 )
  {
    move(INSTR_ROW , 50);
    printw( " In autocalibration mode, step %d ", calstep );
    refresh();
    tempcalstep = calstep;
    switch( tempcalstep )
    {
      case 0: // crank nmax up until no more n-lim 
        if( tem.veg.yrgpp < 0.99*tem.veg.yringpp ) 
        { 
          tem.veg.setNMAX( 2.0*tem.veg.getNMAX() );

          move(INSTR_ROW+1 , 50);
          printw( " NMAX = %8.3lf ", tem.veg.getNMAX() );
          refresh();

          nmax_fac *= 2.0;
        }
        else 
        { 
          calstep ++; 
          adj_fac = init_adj_fac;
          target_error[1] = 0;
        } 
        break;
      case 1: // adjust cmax to get nppsat ~ nppsat_t
        if( fabs(tem.veg.yrnpp - nppsat_t)/nppsat_t < autocal_tol ) 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[1] = 0;
          target_error[2] = 0; 
          tem.veg.setNMAX( tem.veg.getNMAX()/nmax_fac );
        }
        else if( tem.veg.yrnpp - nppsat_t < 0 && target_error[1] <= 0 ) 
        { 
          target_error[1] = -1;
          tem.veg.setCMAX( (1.0 + adj_fac)*tem.veg.getCMAX() ); 
        }
        else if( tem.veg.yrnpp - nppsat_t < 0 && target_error[1] > 0 ) 
        { 
          target_error[1] = -1;
          adj_fac /= 2.0;
          tem.veg.setCMAX( (1.0 + adj_fac)*tem.veg.getCMAX() ); 
        }
        else if( tem.veg.yrnpp - nppsat_t > 0 && target_error[1] >= 0 ) 
        { 
          target_error[1] = 1;
          tem.veg.setCMAX( (1.0 - adj_fac)*tem.veg.getCMAX() ); 
        }
        else if( tem.veg.yrnpp - nppsat_t > 0 && target_error[1] < 0 ) 
        { 
          target_error[1] = 1;
          adj_fac /= 2.0;
          tem.veg.setCMAX( (1.0 - adj_fac)*tem.veg.getCMAX() ); 
        }
        move(INSTR_ROW+1 , 50);
        printw( " CMAX = %8.3lf ", tem.veg.getCMAX() );
        refresh();
        break;          
      case 2: // adjust nmax or kra to get npp ~ npp_t
        if( fabs(tem.veg.yrnpp - npp_t)/npp_t < autocal_tol ) 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[2] = 0;
          target_error[3] = 0;  
        }
        else if( (tem.veg.yrinnpp - tem.veg.yrnpp) >= autocal_tol*tem.veg.yrnpp ) // i.e. n-limited
		{ // if n-limited, increasing nmax increases npp
		  if( tem.veg.yrnpp - npp_t < 0 && target_error[2] <= 0 ) 
          { 
            target_error[2] = -1;
            tem.veg.setNMAX( (1.0 + adj_fac)*tem.veg.getNMAX() ); 
          }
          else if( tem.veg.yrnpp - npp_t < 0 && target_error[2] > 0 ) 
          { 
            target_error[2] = -1;
            adj_fac /= 2.0;
            tem.veg.setNMAX( (1.0 + adj_fac)*tem.veg.getNMAX() ); 
          }
          else if( tem.veg.yrnpp - npp_t > 0 && target_error[2] >= 0 ) 
          { 
            target_error[2] = 1;
            tem.veg.setNMAX( (1.0 - adj_fac)*tem.veg.getNMAX() ); 
          }
          else if( tem.veg.yrnpp - npp_t > 0 && target_error[2] < 0 ) 
          { 
            target_error[2] = 1;
            adj_fac /= 2.0;
            tem.veg.setNMAX( (1.0 - adj_fac)*tem.veg.getNMAX() ); 
          }
          move(INSTR_ROW+1 , 50);
          printw( " NMAX = %8.3lf ", tem.veg.getNMAX() );
          refresh();
		}
		else if( (tem.veg.yrinnpp - tem.veg.yrnpp) < autocal_tol*tem.veg.yrnpp ) // i.e. non n-limited
		{ // if not n-limited, decreasing kra increases npp
		  if( tem.veg.yrnpp - npp_t < 0 && target_error[2] <= 0 ) 
          { 
            target_error[2] = -1;
            tem.veg.setKRA( (1.0 - adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrnpp - npp_t < 0 && target_error[2] > 0 ) 
          { 
            target_error[2] = -1;
            adj_fac /= 2.0;
            tem.veg.setKRA( (1.0 - adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrnpp - npp_t > 0 && target_error[2] >= 0 ) 
          { 
            target_error[2] = 1;
            tem.veg.setKRA( (1.0 + adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrnpp - npp_t > 0 && target_error[2] < 0 ) 
          { 
            target_error[2] = 1;
            adj_fac /= 2.0;
            tem.veg.setKRA( (1.0 + adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          move(INSTR_ROW+1 , 50);
          printw( " KRA = %8.3lf ", tem.veg.getKRA( tem.veg.cmnt ) );
          refresh();
		}

        break;          
      case 3: // adjust kra or cmax to get gpp ~ gpp_t
        if( fabs(tem.veg.yrgpp - gpp_t)/gpp_t < autocal_tol ) 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[3] = 0;
          target_error[4] = 0;
          tem.veg.setNMAX( nmax_fac*tem.veg.getNMAX() ); 
        }
        else if( (tem.veg.yrinnpp - tem.veg.yrnpp) >= autocal_tol*tem.veg.yrnpp ) // i.e. n-limited
		{ // if n-limited, increasing kra increases gpp
		  if( tem.veg.yrgpp - gpp_t < 0 && target_error[3] <= 0 ) 
          { 
            target_error[3] = -1;
            tem.veg.setKRA( (1.0 + adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrgpp - gpp_t < 0 && target_error[3] > 0 ) 
          { 
            target_error[3] = -1;
            adj_fac /= 2.0;
            tem.veg.setKRA( (1.0 + adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrgpp - gpp_t > 0 && target_error[3] >= 0 ) 
          { 
            target_error[3] = 1;
            tem.veg.setKRA( (1.0 - adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          else if( tem.veg.yrgpp - gpp_t > 0 && target_error[3] < 0 ) 
          { 
            target_error[3] = 1;
            adj_fac /= 2.0;
            tem.veg.setKRA( (1.0 - adj_fac)*tem.veg.getKRA( tem.veg.cmnt ), tem.veg.cmnt ); 
          }
          move(INSTR_ROW+1 , 50);
          printw( " KRA = %8.3lf ", tem.veg.getKRA( tem.veg.cmnt ) );
          refresh();
		}
        else if( (tem.veg.yrinnpp - tem.veg.yrnpp) < autocal_tol*tem.veg.yrnpp ) // i.e. non n-limited
		{ // if not n-limited, increasing cmax increases gpp
		  if( tem.veg.yrgpp - gpp_t < 0 && target_error[3] <= 0 ) 
          { 
            target_error[3] = -1;
            tem.veg.setCMAX( (1.0 + adj_fac)*tem.veg.getCMAX()); 
          }
          else if( tem.veg.yrgpp - gpp_t < 0 && target_error[3] > 0 ) 
          { 
            target_error[3] = -1;
            adj_fac /= 2.0;
            tem.veg.setCMAX( (1.0 + adj_fac)*tem.veg.getCMAX()); 
          }
          else if( tem.veg.yrgpp - gpp_t > 0 && target_error[3] >= 0 ) 
          { 
            target_error[3] = 1;
            tem.veg.setCMAX( (1.0 - adj_fac)*tem.veg.getCMAX()); 
          }
          else if( tem.veg.yrgpp - gpp_t > 0 && target_error[3] < 0 ) 
          { 
            target_error[3] = 1;
            adj_fac /= 2.0;
            tem.veg.setCMAX( (1.0 - adj_fac)*tem.veg.getCMAX()); 
          }
          move(INSTR_ROW+1 , 50);
          printw( " CMAX = %8.3lf ", tem.veg.getCMAX() );
          refresh();
		}
        break;          
      case 4: // increase nmax to get back to nppsat             
        if( tem.veg.yrgpp < 0.99*tem.veg.yringpp ) 
        { 
          tem.veg.setNMAX( 2.0*tem.veg.getNMAX() );
          nmax_fac *= 2.0; 
        }
        else 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[5] = 0; 
        }
        break;          
      case 5: // check that nppsat ~ nppsat_t still holds
        if( fabs(tem.veg.yrnpp - nppsat_t)/nppsat_t < autocal_tol ) 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[6] = 0; 
          tem.veg.setNMAX( tem.veg.getNMAX()/nmax_fac );
        }
        else { calstep = 1; } 
        break;
      case 6: // wait before next step for vegetation to come back down from high nmax
        calstep ++;
        adj_fac = init_adj_fac;
        target_error[7] = 0;
        break;          
      case 7: // check that npp ~ npp_t still holds
        if( fabs(tem.veg.yrnpp - npp_t)/npp_t < autocal_tol ) 
        { 
          calstep ++; 
          adj_fac = init_adj_fac;
          target_error[8] = 0;
        }
        else { calstep = 2; }
        break;          
      case 8: // adjust tauheartwood to get vegc ~ vegc_t
        if( fabs(tem.veg.yrcarbon - vegc_t)/vegc_t < autocal_tol ) 
        { 
          calstep ++; 
          adj_fac = init_adj_fac;
          target_error[8] = 0;
          target_error[9] = 0;
        }
        else if( tem.veg.yrcarbon - vegc_t < 0 && target_error[8] <= 0 ) 
        { 
          target_error[8] = -1;
          tem.veg.setTAUHEARTWOOD( (1.0 + adj_fac)*tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ), tem.veg.cmnt ); 
        }
        else if( tem.veg.yrcarbon - vegc_t < 0 && target_error[8] > 0 ) 
        { 
          target_error[8] = -1;
          adj_fac /= 2.0;
          tem.veg.setTAUHEARTWOOD( (1.0 + adj_fac)*tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ), tem.veg.cmnt ); 
        }
        else if( tem.veg.yrcarbon - vegc_t > 0 && target_error[8] >= 0 ) 
        { 
          target_error[8] = 1;
          tem.veg.setTAUHEARTWOOD( (1.0 - adj_fac)*tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ), tem.veg.cmnt ); 
        }
        else if( tem.veg.yrcarbon - vegc_t > 0 && target_error[8] < 0 ) 
        { 
          target_error[8] = 1;
          adj_fac /= 2.0;
          tem.veg.setTAUHEARTWOOD( (1.0 - adj_fac)*tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ), tem.veg.cmnt ); 
        }
        move(INSTR_ROW+1 , 50);
        printw( " TAUHW = %8.3lf ", tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ) );
        refresh();
        break;            
      case 9: // check that npp ~ npp_t and gpp ~gpp_t
        if( (fabs(tem.veg.yrgpp - gpp_t)/gpp_t < autocal_tol)
            &&(fabs(tem.veg.yrnpp - npp_t)/npp_t < autocal_tol) )
        {
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[10] = 0;
        }
        else { calstep = 2; }
        break;          
      case 10: // adjust kdc to get soilc ~ soilc_t
        if( fabs(tem.soil.yrorgc - soilc_t)/soilc_t < autocal_tol ) 
        { 
          calstep ++; 
          adj_fac = init_adj_fac;
          target_error[10] = 0;
          target_error[11] = 0;
        }
        else if( tem.soil.yrorgc - soilc_t > 0 && target_error[10] >= 0 ) 
        { 
          target_error[10] = 1;
          tem.microbe.setKDC( (1.0 + adj_fac)*tem.microbe.getKDC() ); 
        }
        else if( tem.soil.yrorgc - soilc_t > 0 && target_error[10] < 0 ) 
        { 
          target_error[10] = 1;
          adj_fac /= 2.0;
          tem.microbe.setKDC( (1.0 + adj_fac)*tem.microbe.getKDC() ); 
        }
        else if( tem.soil.yrorgc - soilc_t < 0 && target_error[10] <= 0 ) 
        { 
          target_error[10] = -1;
          tem.microbe.setKDC( (1.0 - adj_fac)*tem.microbe.getKDC() ); 
        }
        else if( tem.soil.yrorgc - soilc_t < 0 && target_error[10] > 0 ) 
        { 
          target_error[10] = -1;
          adj_fac /= 2.0;
          tem.microbe.setKDC( (1.0 - adj_fac)*tem.microbe.getKDC() ); 
        }
        move(INSTR_ROW+1 , 50);
        printw( " KDC = %8.6lf ", tem.microbe.getKDC() );
        refresh();
        break;            
      case 11: // adjust mnup to get avln ~ avln_t
        if( fabs(tem.soil.yravln - avln_t)/avln_t < autocal_tol ) 
        { 
          calstep ++; 
          adj_fac = init_adj_fac;
          target_error[11] = 0;
          target_error[12] = 0;
        }
        else if( tem.soil.yravln - avln_t > 0 && target_error[11] >= 0 ) 
        { 
          target_error[11] = 1;
          tem.microbe.setNUP( (1.0 + adj_fac)*tem.microbe.getNUP() ); 
        }
        else if( tem.soil.yravln - avln_t > 0 && target_error[11] < 0 ) 
        { 
          target_error[11] = 1;
          adj_fac /= 2.0;
          tem.microbe.setNUP( (1.0 + adj_fac)*tem.microbe.getNUP() ); 
        }
        else if( tem.soil.yravln - avln_t < 0 && target_error[11] <= 0 ) 
        { 
          target_error[11] = -1;
          tem.microbe.setNUP( (1.0 - adj_fac)*tem.microbe.getNUP() ); 
        }
        else if( tem.soil.yravln - avln_t < 0 && target_error[11] > 0 ) 
        { 
          target_error[11] = -1;
          adj_fac /= 2.0;
          tem.microbe.setNUP( (1.0 - adj_fac)*tem.microbe.getNUP() ); 
        }
        move(INSTR_ROW+1 , 50);
        printw( " MNUP = %8.3lf ", tem.microbe.getNUP() );
        refresh();
        break;          
      case 12: // repeat step 2: adjust nmax to get npp ~ npp_t 
        if( fabs(tem.veg.yrnpp - npp_t)/npp_t < autocal_tol ) 
        { 
          calstep ++;
          adj_fac = init_adj_fac;
          target_error[12] = 0;
          target_error[13] = 0; 
        }
        else if( tem.veg.yrnpp - npp_t < 0 && target_error[12] <= 0 ) 
        { 
          target_error[12] = -1;
          tem.veg.setNMAX( (1.0 + adj_fac)*tem.veg.getNMAX() ); 
        }
        else if( tem.veg.yrnpp - npp_t < 0 && target_error[12] > 0 ) 
        { 
          target_error[12] = -1;
          adj_fac /= 2.0;
          tem.veg.setNMAX( (1.0 + adj_fac)*tem.veg.getNMAX() ); 
        }
        else if( tem.veg.yrnpp - npp_t > 0 && target_error[12] >= 0 ) 
        { 
          target_error[12] = 1;
          tem.veg.setNMAX( (1.0 - adj_fac)*tem.veg.getNMAX() ); 
        }
        else if( tem.veg.yrnpp - npp_t > 0 && target_error[12] < 0 ) 
        { 
          target_error[12] = 1;
          adj_fac /= 2.0;
          tem.veg.setNMAX( (1.0 - adj_fac)*tem.veg.getNMAX() ); 
        }
        move(INSTR_ROW+1 , 50);
        printw( " NMAX = %8.3lf ", tem.veg.getNMAX() );
        refresh();
        break;            
      case 13: // check that avln ~ avln_t still holds -- if not, go back to step 10
        if( fabs(tem.soil.yravln - avln_t)/avln_t < autocal_tol ) 
        { 
          calstep = 0;
          nmax_fac = 1.0;
          init_adj_fac /= 1.25;
          tem.tauavg *= 2.0;
          autocal_tol /= 2.0;
          
          move(INSTR_ROW+2 , 50);
          printw( " TAUAVG now = %5.1lf ", tem.tauavg );
          refresh();
          // redo calibration with higher precision 
        }
        else { calstep = 11; }   
        break;
      default:
        break;          
    }
  }


};
#endif
/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void calibrate( int t )
{
  int iopt;
  double tfactor = 1.0;
  double invar;
  int readerror = 0;
  char istr[80];

  endgrid = 0;

  move( VPARAMS_ROW - 1, 45 );
  printw( "AN = %1d, NF = %1d, BL = %1d, MF = %1d, RS = %1d",
          tem.avlnflag,
          tem.nfeed,
          tem.baseline,
          tem.moistlim,
          reset );

  for( iopt = 0; iopt < ESY; iopt ++)
  {
    move(1, 8+8*iopt);
    tem.displayOptionalEflx( tem.sey[iopt] );
  }

  for( iopt = 0; iopt < WSY; iopt ++)
  {
    move(1, 8+8*ESY+8*iopt);
    tem.displayOptionalWflx( tem.swy[iopt] );
  }  
  
  move(VPARAMS_ROW-1, 1);
  printw("CALIBRATION INPUT: ");
  
  move(VPARAMS_ROW, 1);
  printw("SETTINGS:          ");
  
  move(VPARAMS_ROW+1, 1);
  printw("AGFILE:            ");
  
  move(VPARAMS_ROW+2, 1);
  printw("DATFILE:           ");
  
  move(VPARAMS_ROW+3, 1);
  printw("VEGFILE:           ");
  
  move(VPARAMS_ROW+4, 1);
  printw("CONDUCTFILE:       ");
  
  move(VPARAMS_ROW+5, 1);
  printw("OPTIONAL C/N:      ");
  
  move(VPARAMS_ROW+6, 1);
  printw("OPTIONAL H2O:      ");
  
  refresh();

  if( 1 == firstcal )
  {
    tvar = T_TOL;
    firstcal = 0;
  }

  displayCalibPar( tvar );
  
  refresh();

  while( t == KEY_END // end key
         || t == KEY_UP // up arrow
         || t == KEY_PPAGE // page up 
         || t == KEY_LEFT // left arrow
         || t == KEY_RIGHT // right arrow
         || t == KEY_DOWN // down arrow
         || t == KEY_NPAGE // page down 
         || t == KEY_IC )// 0 = insert key
  {

    if( 0 == tem.tolbomb && 0 == tem.intbomb )
    {
      t = getch();
      if( 0 == t ) { t = getch(); }
    }
    else
    {

      // If simulation reaches RUNSIZE automatically adjust the
      //   tolerance level (i.e. tol)
      if( 1 == tem.tolbomb )
      {
        tem.retry = 1;
        tem.tolbomb = 0;
        tem.nattempt += 1;
        tvar = T_TOL;

        if( tem.nattempt < tem.maxnrun )
        {
	      tem.tol /= 10.0;
	      t = 0;
        }
        else
        {
	      tem.tol = tem.inittol;
	      tem.nattempt = 0;
        }
      }

      // If simulation "black holes", allow user to
      // adjust the tolerance level
      if( 1 == tem.intbomb )
      {
        tem.retry = 1;
        tem.intbomb = 0;
        tvar = T_TOL;
      }
    }

    if( 1 == tem.intbomb ) { tvar = T_TOL; }

    // Set calvar[] with current TEM parameter values
    setCalibVar();

    tfactor = 1.0;

    // Select or modify a specific calibration parameter
    //   represented by calvar[]

    switch( t )
    {
      case KEY_END: break;
      case KEY_NPAGE: tfactor = 1.01; break;
      case KEY_PPAGE: tfactor = 0.99; break;
      case KEY_RIGHT: next( tvar ); break;
      case KEY_LEFT: prev( tvar ); break;
      case KEY_UP: back( tvar ); break;
      case KEY_DOWN: jump( tvar ); break;
      case KEY_IC: if( tcalkey( tvar ) < MAXTCAL )
               {
                 echo();
                 readerror = getstr( istr );
	             invar = atof( istr );
	             tcalvar[tvar] = invar;
	             noecho();
	           }
	           break;
      default: t = 0; break;
    }

    if( 0 == t ) break;

    // Update TEM calibration parameters with new calvar[] value

    updateCalibPar( tvar, tfactor, t );
    displayCalibPar( tvar );
    refresh();
  }


  if( 1 == reset || 1 == tem.retry )
  {
    tem.retry = 0;
    xdyr = 0;

    resetInitStateCalib();
  }


  if( 1 == newtext || 1 == newclm )
  {
    move( 1,1 );
    clear();

    tem.setELMNTecd( tem.veg.cmnt,
                     tem.soil.getPSIPLUSC() );

    tem.topwind = 0;
    newclm = 0;
    newtext = 0;
  }

  if( 1 == equilsol )
  {
    equilsol = 0;
    xdyr = 0;
  }

};

/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************** */

void displayCalibPar( tcalkey& tcal )
{

  switch( tcal )
  {
    case T_TOL:     move( VPARAMS_ROW, 20 );
                    
                    printw( "INTEGRATOR TOLERANCE = %6.4lf ",
                            tem.tol );
                    break;

	case T_TAVG:    move( VPARAMS_ROW, 20 );
                    
                    printw( "'YR' AVERAGING TIME  = %6.1lf ",
                            tem.tauavg );
                    break;
    
    case T_CO2:     move( VPARAMS_ROW, 20 );
                             
                    printw( "CO2, PPMV            = %6.1lf ",
                            tem.atms.getCO2LEVEL() );
                    break;
                    
    case T_AVLNFLAG: move( VPARAMS_ROW, 20 );
                      
                    if( 1 == tem.avlnflag )
                    {
                      printw( "AVAILN = ON                   " );
                    }
	                if( 0 == tem.avlnflag )
	                {
	                  printw( "AVAILN = OFF                  " );
	                }
	                break;

    case T_NFEED:   move( VPARAMS_ROW, 20 );
                    
                    if( 1 == tem.nfeed )
                    {
                      printw( "NFEED = ON                    " );
                    }
	                if( 0 == tem.nfeed )
	                {
	                  printw( "NFEED = OFF                   " );
	                }
	                break;

    case T_BLINE:   move( VPARAMS_ROW, 20 );
                    
                    if( 1 == tem.baseline )
                    {
                      printw( "BLINE = ON                   " );
                    }
	                if( 0 == tem.baseline )
	                {
	                  printw( "BLINE = OFF                  " );
	                }
	                break;

    case T_MOISTLIM:  move( VPARAMS_ROW, 20 );
                      
                    if( 1 == tem.moistlim )
                    {
                      printw( "MOISTLIM = ON                " );
                    }
	                if( 0 == tem.moistlim )
	                {
	                  printw( "MOISTLIM = OFF               " );
	                }
	                break;
	            
    case T_O3FLAG:  move( VPARAMS_ROW, 20 );
                    
                    if( 1 == tem.o3flag )
	                {
	                  printw( "O3FLAG = ON                  " );
	                }
	                if( 0 == tem.o3flag )
	                {
	                  printw( "O3FLAG = OFF                 " );
                    }
	                break;
                
    case T_RESET:   move( VPARAMS_ROW, 20 );
                    
                    if( 1 == reset )
                    {
                      printw( "reset = ON                   " );
                    }
	                else 
                    { 
                      printw( "reset = OFF                  " ); 
                    }
	                break;

    case T_MANFLAG: move( VPARAMS_ROW, 20 );
                     
                    if( 1 == manflag )
                    {
                      printw( "manual = ON                  " );
                    }
	                else 
                    { 
                      printw( "manual = OFF                 " ); 
                    }
	                break;
	            
//// End of 11 settings                
                
    case T_GDDMIN:  move( VPARAMS_ROW+1, 20 );
                    
                    printw( "GDDMIN = %8.2lf                 ",
                            tem.ag.getGDDMIN( tem.ag.cmnt ) );
                    break;

    case T_GDDSEED: move( VPARAMS_ROW+1, 20 );
                    
                    printw( "GDDSEED = %8.2lf                ",
                            tem.ag.getGDDSEED( tem.ag.cmnt ) );
                    break;

    case T_GDDHARVST:move( VPARAMS_ROW+1, 20 );
                    
                    printw( "GDDHARVST = %8.2lf              ",
                             tem.ag.getGDDHARVST( tem.ag.cmnt ) );
                    break;

    case T_TKILL:   move( VPARAMS_ROW+1, 20 );
                    
                    printw( "TKILL = %6.2lf                  ",
                            tem.ag.getTKILL( tem.ag.cmnt ) );
                    break;
                    
/*    case T_AGSTATE: move( VPARAMS_ROW+1, 20 );
                    
                    if(1 == tem.ag.state )
                    {
                      printw( "agstate = ON                  " );
                    }
	                else 
                    { 
                      printw( "agstate = OFF                 " ); 
                    }
	                break; */

    case T_AGSTATE:    move( VPARAMS_ROW+1, 20 );

                    printw( "agstate = %d                   ",
                            tem.ag.state );
                    break;


    case T_AGPROD:  move( VPARAMS_ROW+1, 20 );
                    
                    if(1 == agProdFlag )
                    {
                      printw( "agprod = ON                   " );
                    }
	                else 
                    { 
                      printw( "agprod = OFF                  " ); 
                    }
	                break;

    case T_TILLFLAG:move( VPARAMS_ROW+1, 20 );
                        
                    if( 1 == tem.ag.tillflag )
                    {
                      printw( "tillflag = ON                 " );
                    }
	                else 
                    { 
                      printw( "tillflag = OFF                " ); 
                    }
	                break;

    case T_FERTFLAG:move( VPARAMS_ROW+1, 20 );
                        
                    if( 1 == tem.ag.fert1950flag )
                    {
                      printw( "fertflag = ON                 " );
                    }
	                else 
                    { 
                      printw( "fertflag = OFF                " ); 
                    }
	                break;

    case T_IRRGFLAG:move( VPARAMS_ROW+1, 20 );
                    
                    if( 1 == tem.ag.irrg1950flag )
                    {
                      printw( "irrgflag = ON                 " );
                    }
	                else 
                    { 
                      printw( "irrgflag = OFF                " ); 
                    }
	                break;   
                
//// End of 9 agfile variables    

    case T_CMAX:    move( VPARAMS_ROW+2, 20 );
                    
                    printw( "CMAX = %8.4lf                   ",
                            tem.veg.getCMAX() );
                    break;

    case T_KRA:     move( VPARAMS_ROW+2, 20 );
                    
                    printw( "KRA = %8.6lf                    ",
                            tem.veg.getKRA( tem.veg.cmnt ) );
                    break;

    case T_KDC:     move( VPARAMS_ROW+2, 20 );
                    
                    printw( "KDC = %8.6lf                    ",
                            tem.microbe.getKDC() );
                    break;

    case T_TAULEAF: move( VPARAMS_ROW+2, 20 );
                      
                    printw( "TAULEAF = %8.4lf                ",
                            tem.veg.getTAULEAF( tem.veg.cmnt ) );
                    break;

    case T_TAUSTEMA:move( VPARAMS_ROW+2, 20 );
                       
                    printw( "TAUSAPWOOD = %8.4lf             ",
                            tem.veg.getTAUSAPWOOD( tem.veg.cmnt ) );
                    break;

    case T_TAUSTEMI:move( VPARAMS_ROW+2, 20 );
                       
                    printw( "TAUHEARTWOOD = %8.4lf           ",
                        tem.veg.getTAUHEARTWOOD( tem.veg.cmnt ) );
                    break;

    case T_TAUROOT: move( VPARAMS_ROW+2, 20 );
                      
                    printw( "TAUROOT = %8.4lf                ",
                        tem.veg.getTAUROOT( tem.veg.cmnt ) );
                    break;

    case T_TAUSEED: move( VPARAMS_ROW+2, 20 );
                      
                    printw( "TAUSEED = %8.4lf                ",
                        tem.veg.getTAUSEED( tem.veg.cmnt ) );
                    break;
                    
    case T_NMAX:    move( VPARAMS_ROW+2, 20 );
                    
                    printw( "NMAX = %8.2lf                   ",
                            tem.veg.getNMAX() );
                    break;

    case T_MNUP:    move( VPARAMS_ROW+2, 20 );
                    
                    printw( "MNUP = %8.4lf                   ",
                            tem.microbe.getNUP() );
                    break;
                    
    case T_CNSOL:   move( VPARAMS_ROW+2, 20 );
                    
                    printw( "CNSOIL = %8.4lf                 ",
                            tem.microbe.getCNSOIL( tem.veg.cmnt ) );
                    break;

    case T_CNLTR:   move( VPARAMS_ROW+2, 20 );
                    
                    printw( "CNLTR = %8.4lf                  ",
                            tem.veg.getCNLTR( tem.veg.cmnt ) );
                    break;
                    

    case T_NLOSS :  move( VPARAMS_ROW+2, 20 );
                     
                    printw( "NLOSS = %8.2lf                  ",
	                        tem.soil.getNLOSS( tem.veg.cmnt ) );
	                break;
     
//// End of 19 datfile variables

    case T_SLA:     move( VPARAMS_ROW+3, 20 );
                    
                    printw( "SLA = %8.4lf                    ",
                            tem.veg.getSLA( tem.veg.cmnt ) );
                    break;

    case T_MOPT:    move( VPARAMS_ROW+3, 20 );
                    
                    printw( "MOPT = %8.4lf                   ",
                            tem.microbe.getMOISTOPT( tem.veg.cmnt ) );
                    break;

//// End of 2 vegfile variables

    case T_GROOT:   move( VPARAMS_ROW+4, 20 );
                    
                    printw( "GROOT = %8.4lf                  ",
                        tem.veg.pen.getGROOT( tem.veg.cmnt ) );
                    break;
                    
    case T_KSTEM:   move( VPARAMS_ROW+4, 20 );
                    
                    printw( "KSTEM = %8.1lf                  ",
                        tem.veg.pen.getKSTEM( tem.veg.cmnt ) );
                    break;
                    
    case T_CDLEAF:  move( VPARAMS_ROW+4, 20 );
                    
                    printw( "CDLEAF = %8.4lf                 ",
                        tem.veg.pen.getCDLEAF( tem.veg.cmnt ) );
                    break;

    case T_WLEAF:   move( VPARAMS_ROW+4, 20 );
                    
                    printw( "WLEAF = %8.4lf                  ",
                        tem.veg.pen.getWLEAF( tem.veg.cmnt ) );
                    break;
                   
    case T_GSMIN:   move( VPARAMS_ROW+4, 20 );
                    
                    printw( "GSMIN = %8.4lf                  ",
                        tem.veg.pen.getGSMIN( tem.veg.cmnt ) );
                    break;

    case T_GSA:     move( VPARAMS_ROW+4, 20 );
                    
                    printw( "GSA= %8.4lf                     ",
                        tem.veg.pen.getGSA( tem.veg.cmnt ) );
                    break;

    case T_KEXT:    move( VPARAMS_ROW+4, 20 );
                    
                    printw( "KEXT = %8.4lf                   ",
                        tem.veg.pen.getKEXT( tem.veg.cmnt ) );
                    break;

    case T_ALBEDO:  move( VPARAMS_ROW+4, 20 );
                    
                    printw( "ALBEDO = %8.4lf                 ",
                         tem.veg.pen.getALBEDO( tem.veg.cmnt ) );
                    break;
                     
    case T_LSCMIN:  move( VPARAMS_ROW+4, 20 );
                    
                    printw( "LSCMIN = %8.4lf                 ",
                         tem.veg.pen.getLSCMIN( tem.veg.cmnt ) );
                    break;
                     
    case T_RHOSTEM: move( VPARAMS_ROW+4, 20 );
                    
                    printw( "RHOSTEM = %8.4lf                ",
                         tem.veg.pen.getRHOSTEM( tem.veg.cmnt ) );
                    break;
    
//// End of 12 conductfile variables
    
    
    case T_OPTCN0:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[0] );
	                break;
	                
    case T_OPTCN1:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[1] );
	                break;
	                
    case T_OPTCN2:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[2] );
	                break;
	                
    case T_OPTCN3:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[3] );
	                break;
	                
    case T_OPTCN4:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[4] );
	                break;
	                
    case T_OPTCN5:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[5] );
	                break;
	                
    case T_OPTCN6:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[6] );
	                break;
	                
    case T_OPTCN7:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[7] );
	                break;
	                
    case T_OPTCN8:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[8] );
	                break;
	                
    case T_OPTCN9:  move( VPARAMS_ROW+5, 20 );
                    
                    tem.displayOptionalEflx( tem.sey[9] );
	                break;

//// End of 10 optional c/n fluxes	            
        
    case T_OPTH2O0: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[0] );
	    	        break;
	    	        
    case T_OPTH2O1: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[1] );
	    	        break;
	    	        
    case T_OPTH2O2: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[2] );
	    	        break;
	    	        
    case T_OPTH2O3: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[3] );
	    	        break;
	    	        
    case T_OPTH2O4: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[4] );
	    	        break;
	    	        
    case T_OPTH2O5: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[5] );
	    	        break;
	    	        
    case T_OPTH2O6: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[6] );
	    	        break;
	    	        
    case T_OPTH2O7: move( VPARAMS_ROW+6, 20 );
                        
                    tem.displayOptionalWflx( tem.swy[7] );
	    	        break;
        
  //// End of 8 optional h2o fluxes
  }

};

/* *************************************************************
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

  if( 0 == clm.sradflag )
  {
    tempfname.str( "" );

    tempfname << clm.igirrfname
              << clm.igirrend;

    girrname = tempfname.str();

    ifgirr = fopen( girrname.c_str(), "r" );

    if( !ifgirr )
    {
      flog1 << endl << "Cannot open " << girrname;
      flog1 << " for GIRR data input" << endl << endl;

      exit( -1 );
    }
  }

  if( 1 == clm.cldflag )
  {
    tempfname.str( "" );

    tempfname << clm.icldsfname
              << clm.icldsend;

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

    tempfname << clm.inirrfname
              << clm.inirrend;

    nirrname = tempfname.str();

    ifnirr = fopen( nirrname.c_str(), "r" );

    if( !ifnirr )
    {
      flog1 << endl << "Cannot open " << nirrname;
      flog1 << " for NIRR data input" << endl << endl;

      exit( -1 );
    }
  }

  if( 1 == clm.parflag )
  {
    tempfname.str( "" );

    tempfname << clm.iparfname
              << clm.iparend;

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

  tempfname << clm.itairfname
            << clm.itairend;

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

  tempfname << clm.iprecfname
            << clm.iprecend;

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

  tempfname << clm.itrangefname
            << clm.itrangeend;


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

  tempfname << clm.ivprfname
            << clm.ivprend;

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

  tempfname << clm.iws10fname
            << clm.iws10end;

  ws10name = tempfname.str();
  ifws10 = fopen( ws10name.c_str(), "r" );

  if( !ifws10 )
  {
    flog1 << endl << "Cannot open " << ws10name;
    flog1 << " for WS10 data input" << endl << endl;

    exit( -1 );
  }

  //  CO2

  if( 1 == clm.tco2flag )
  {
    co2name = clm.ico2fname + clm.ico2end;

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

  tempfname << clm.io3fname
            << clm.io3end;

  o3name = tempfname.str();

  ifo3 = fopen( o3name.c_str(), "r" );
  if( !ifo3 )
  {
    flog1 << endl << "Cannot open " << o3name;
    flog1 << " for O3 data input" << endl << endl;

    exit( -1 );
  }

  // Open ndep file
  tempfname.str( "" );

  tempfname << atmdep.ndepfname
            << atmdep.ndepend;

  ndepname = tempfname.str();

  ifndep = fopen( ndepname.c_str(), "r" );
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

void initRun( void )
{
//  int i;
  int numcmnt;
  int icmnt;
  
  double initco2;
  double co2level;
//  double dc2n;

  string ifilename;
  string datfilenames;



 // Open log file
  flog1.open( "ctem45.log" );

  tem.gofile.open( tem.goname.c_str(), ios::in );
  tem.goxml.getXMLrootNode( tem.gofile, "gofile" );
  flog1 << "using " << tem.goname << " for information on this run " << endl;

  de_startyear = clm.startyr;
  de_nyears = 1;

  clm.startyr = tem.goxml.getXMLint( tem.gofile, "gofile", "clmstartyr" );
  flog1 << " starting year for climate datasets is " << clm.startyr << endl;
    
  spinflag = tem.goxml.getXMLint( tem.gofile, "gofile", "spinflag" );
  flog1 << " initialize model using " << ((spinflag == 0)?"equilibration to long-term average climate":((spinflag==1)?"equilibration then spinup":"dynamic equilibration")) << endl;
    
  de_startyear = tem.goxml.getXMLint( tem.gofile, "gofile", "de_startyear" );
  if (2 == spinflag ) { flog1 << " startyear for dynamic equilibration: " << de_startyear << endl; }
    
  de_nyears = tem.goxml.getXMLint( tem.gofile, "gofile", "de_nyears" );
  if (2 == spinflag ) { flog1 << " number of years for dynamic equilibration loop: " << de_nyears << endl; }
  
  RTIME = tem.goxml.getXMLint( tem.gofile, "gofile", "transtime" );
  flog1 << " there are " << RTIME << " years of climate data in the transient datasets " << endl;
    
  tem.startyr = clm.startyr;

  if( 0 == spinflag )
  {
    de_startyear = clm.startyr;
    de_nyears = 1;
  }
    
  if( 1 == spinflag )
  {
    cerr << " spinflag = 1 not a legitimate option for ctem; use spinflag = 0 or spinflag = 2 " << endl;
    exit(-1);   
  }

/// Climate inputs  

  clm.sradflag = tem.goxml.getXMLint( tem.gofile, "gofile", "sradflag" );
  clm.cldflag = tem.goxml.getXMLint( tem.gofile, "gofile", "cldflag" );
  clm.parflag = tem.goxml.getXMLint( tem.gofile, "gofile", "parflag" );
  clm.tcldsflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtcldsflag" );
  
  flog1 << " the TEMclm model " << ((clm.sradflag==1)?"will":"will not") << " be run for solar radiation variables " << endl;
  flog1 << ((clm.tcldsflag==1)?"transient ":"static ") << ((clm.cldflag==1)?"cloudiness":"surface radiation") << " data will be used" << endl;
  if( clm.sradflag == 0 ) { flog1 << " spatially explicit PAR data " << ((clm.parflag==1)?"will":"will not") << " be used " << endl; }
  
  clm.icldsfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "icldsfname" );
  clm.icldsend = tem.goxml.getXMLstring( tem.gofile, "gofile", "icldsend" );
  clm.inirrfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "inirrfname" );
  clm.inirrend = tem.goxml.getXMLstring( tem.gofile, "gofile", "inirrend" );
  clm.igirrfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "igirrfname" );
  clm.igirrend = tem.goxml.getXMLstring( tem.gofile, "gofile", "igirrend" );
  clm.iparfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "iparfname" );
  clm.iparend = tem.goxml.getXMLstring( tem.gofile, "gofile", "iparend" );
  
  if( (clm.cldflag == 1) && (clm.sradflag == 1) ) 
  { 
    if( clm.tcldsflag == 1 ) { flog1 << " filename for cloudiness data input: " << clm.icldsfname + clm.icldsend << endl; }
    else { flog1 << " filename for cloudiness data input: " << clm.icldsfname << endl; }  
  }

  if( (clm.cldflag == 0) && (clm.sradflag == 1) )
  {
    if( clm.tcldsflag == 1 ) { flog1 << " filename for solar radiation data input: " << clm.inirrfname + clm.inirrend << endl; }
    else { flog1 << " filename for solar radiation data input: " << clm.inirrfname << endl; }
  }
  
  if( clm.sradflag == 0 )
  {
    if( clm.tcldsflag == 1 ) 
    {
      flog1 << " filename for top-of-atmosphere radiation data input: " << clm.igirrfname + clm.igirrend << endl;
      flog1 << " filename for surface radiation data input: " << clm.inirrfname + clm.inirrend << endl;
      flog1 << " filename for photosynthetically active radiation data input: " << clm.iparfname + clm.iparend << endl;
    }
    else
    {
      flog1 << " filename for top-of-atmosphere radiation data input: " << clm.igirrfname << endl;
      flog1 << " filename for surface radiation data input: " << clm.inirrfname << endl;
      flog1 << " filename for photosynthetically active radiation data input: " << clm.iparfname << endl;
    }
  }


  clm.ttairflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xttairflag" );
  clm.itairfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "itairfname" );
  clm.itairend = tem.goxml.getXMLstring( tem.gofile, "gofile", "itairend" );
  
  if( clm.ttairflag == 1 ) { flog1 << " transient tair dataset: " << clm.itairfname + clm.itairend << endl; }
  else { flog1 << " long-term average tair dataset: " << clm.itairfname << endl; }
  

  clm.tprecflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtprecflag" );
  clm.iprecfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "iprecfname" );
  clm.iprecend = tem.goxml.getXMLstring( tem.gofile, "gofile", "iprecend" );

  if( clm.tprecflag == 1 ) { flog1 << " transient prec dataset: " << clm.iprecfname + clm.iprecend << endl; }
  else { flog1 << " long-term average prec dataset: " << clm.iprecfname << endl; }
  

  clm.ttrangeflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xttrangeflag" );
  clm.itrangefname = tem.goxml.getXMLstring( tem.gofile, "gofile", "itrangefname" );
  clm.itrangeend = tem.goxml.getXMLstring( tem.gofile, "gofile", "itrangeend" );

  if( clm.ttrangeflag == 1 ) { flog1 << " transient trange dataset: " << clm.itrangefname + clm.itrangeend << endl; }
  else { flog1 << " long-term average trange dataset: " << clm.itrangefname << endl; }

  
  clm.tvprflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtvprflag" );
  clm.ivprfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "ivprfname" );
  clm.ivprend = tem.goxml.getXMLstring( tem.gofile, "gofile", "ivprend" );
  
  if( clm.tvprflag == 1 ) { flog1 << " transient vpr dataset: " << clm.ivprfname + clm.ivprend << endl; }
  else { flog1 << " long-term average vpr dataset: " << clm.ivprfname << endl; }


  initco2 = tem.goxml.getXMLdouble( tem.gofile, "gofile", "initco2" );
  flog1 << " initial co2 concentration (ppmv) : " << initco2 << endl;   
  clm.setINITCO2( initco2 );

  co2level = tem.goxml.getXMLdouble( tem.gofile, "gofile", "xco2level" );
  flog1 << " final equilibrium co2 concentration (ppmv) : " << co2level << endl;
  clm.setCO2LEVEL( co2level );
  
  clm.tco2flag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtco2flag" );
  clm.ico2fname = tem.goxml.getXMLstring( tem.gofile, "gofile", "ico2fname" );
  clm.ico2end = tem.goxml.getXMLstring( tem.gofile, "gofile", "ico2end" );

  if( clm.tco2flag == 1 ) { flog1 << " transient co2 dataset: " << clm.ico2fname << endl; }

  clm.to3flag = tem.goxml.getXMLint( tem.gofile, "gofile", "xto3flag" );
  clm.io3fname = tem.goxml.getXMLstring( tem.gofile, "gofile", "io3fname" );
  clm.io3end = tem.goxml.getXMLstring( tem.gofile, "gofile", "io3end" );

  if( clm.to3flag == 1 ) { flog1 << " transient o3 dataset: " << clm.io3fname + clm.io3end << endl; }
  else { flog1 << " long-term average o3 dataset: " << clm.io3fname << endl; }
  
//  Input name of Ndep file
//
  clm.tndepflag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtndepflag" );
  atmdep.ndepfname = tem.goxml.getXMLstring( tem.gofile, "gofile", "ndepfname" );
  atmdep.ndepend = tem.goxml.getXMLstring( tem.gofile, "gofile", "ndepend" );
  if( clm.tndepflag == 1 ) { flog1 << " transient ndep dataset: " << atmdep.ndepfname + atmdep.ndepend << endl; }
  else { flog1 << " long-term average ndep dataset: " << atmdep.ndepfname << endl; }

  clm.tws10flag = tem.goxml.getXMLint( tem.gofile, "gofile", "xtws10flag" );
  clm.iws10fname = tem.goxml.getXMLstring( tem.gofile, "gofile", "iws10fname" );
  clm.iws10end = tem.goxml.getXMLstring( tem.gofile, "gofile", "iws10end" );

  if( clm.tws10flag == 1 ) { flog1 << " transient ws dataset: " << clm.iws10fname + clm.iws10end << endl; }
  else { flog1 << " long-term average ws10 dataset: " << clm.iws10fname << endl; }
  
 
  initializeCLMGridCell();
  
/// Vegetation inputs and output selection

  tem.startyr = clm.startyr;
  tem.atms.setINITCO2( clm.getINITCO2() );
  tem.atms.setCO2LEVEL( clm.getCO2LEVEL() );

  tem.avlnflag = tem.nfeed = tem.rheqflag = 0;
    
  tem.avlnflag = tem.goxml.getXMLint( tem.gofile, "gofile", "avlnflag" );
  flog1 << " " << ((tem.avlnflag==1)?"allowing":"not allowing") << " available N to fluctuate " << endl;
  
  tem.nfeed = tem.goxml.getXMLint( tem.gofile, "gofile", "nfeed" );
  flog1 << " running TEM " << ((tem.nfeed==1)?"with":"without") << " N feedback on GPP " << endl;
  
  tem.baseline = tem.goxml.getXMLint( tem.gofile, "gofile", "baseline" );
  flog1 << " " << ((tem.baseline==1)?"solving":"not solving") << " for baseline soil nitrogen" << endl;
  tem.initbase = tem.baseline;
  
  tem.moistlim = tem.goxml.getXMLint( tem.gofile, "gofile", "moistlim" );
  flog1 << " moisture limitation is " << ((tem.moistlim==1)?"on":"off") << endl;
  
  tem.o3flag = tem.goxml.getXMLint( tem.gofile, "gofile", "o3flag" );
  flog1 << " ozone damage to gpp is " << ((tem.o3flag==1)?"on":"off") << endl;
  
  tem.equil = tem.goxml.getXMLint( tem.gofile, "gofile", "ctequil" );
  flog1 << " model " << ((tem.equil==1)?"will":"will not") << " stop when equilibrated " << endl;
  
  tem.strteq = tem.goxml.getXMLint( tem.gofile, "gofile", "strteq" );
  if( tem.equil == 1 ) { flog1 << " model will check for equilibrium after running for " << tem.strteq << " years " << endl; }
  
  tem.rheqflag = tem.goxml.getXMLint( tem.gofile, "gofile", "rheqflag" );
  if( tem.equil == 1 ) { flog1 << " decomposition " << ((tem.rheqflag==1)?"must":"is not required to") << " equilibrate " << endl; }
  
  tem.wtol = tem.goxml.getXMLdouble( tem.gofile, "gofile", "wtol" );
  if( tem.equil == 1 ) { flog1 << " absolute tolerance for the water cycle: " << tem.wtol << endl; }
  
  tem.ctol = tem.goxml.getXMLdouble( tem.gofile, "gofile", "ctol" );
  if( tem.equil == 1 ) { flog1 << " absolute tolerance for the carbon cycle: " << tem.ctol << endl; }
  
  tem.ntol = tem.goxml.getXMLdouble( tem.gofile, "gofile", "ntol" );
  if( tem.equil == 1 ) { flog1 << " absolute tolerance for the nitrogen cycle: " << tem.ntol << endl; }
    
  tem.maxyears = tem.goxml.getXMLint( tem.gofile, "gofile", "maxyears" );
  if( tem.equil == 1 ) { flog1 << " maximum number of years for the model to run before initialization is complete: " << tem.maxyears << " years " << endl; }
  tem.runsize = tem.maxyears;
  
  tem.maxnrun = tem.goxml.getXMLint( tem.gofile, "gofile", "maxnrun" );
  if( tem.equil == 1 ) { flog1 << " maximum number of attempts to reach a solution: " << tem.maxnrun << endl; }
  
  tem.tauavg = tem.goxml.getXMLdouble( tem.gofile, "gofile", "tauavg" );
  flog1 << " averaging time for equilibration: " << tem.tauavg << " years " << endl;
  
  tem.veg.setDC2N( tem.goxml.getXMLdouble( tem.gofile, "gofile", "dc2n" ) );
  flog1 << " factor for changing relative c:n per change in co2 concentration: " << tem.veg.getDC2N() << endl;
  
  tem.inittol = tem.goxml.getXMLdouble( tem.gofile, "gofile", "inittol" );
  flog1 << " proportional tolerance of the integrator: " << tem.inittol << endl;
 
  tem.maxit = tem.goxml.getXMLint( tem.gofile, "gofile", "maxit" );
  flog1 << " maximum number of tolerance-reductions in a month: " << tem.maxit << endl;
  
  tem.maxitmon = tem.goxml.getXMLint( tem.gofile, "gofile", "maxitmon" );
  flog1 << " maximum number of iterations in a month: " << tem.maxitmon << endl;
 
  // Get vegetation type dependent parameters

  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "rootzecd" );
  tem.soil.getrootz( ifilename );
  flog1 << " root zone parameters found in " << ifilename << endl;
    
  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "vegecd" );
  tem.veg.getecd( ifilename );
  flog1 << " vegetation parameters found in " << ifilename << endl;
    
  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "conductecd" );
  tem.veg.pen.getecd( ifilename );
  flog1 << " hydraulic and conductance-related parameters found in " << ifilename << endl;
	
  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "mcrbecd" );
  tem.microbe.getvegecd( ifilename );
  flog1 << " microbial parameters found in " << ifilename << endl;


  // Get parameters associated with human disturbance
  //   activities
    
  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "agecd" );
  tem.ag.getecd( ifilename );
  flog1 << " agricultural parameters found in " << ifilename << endl;
    
  numcmnt = MAXCMNT - 1;
    
  datfilenames = tem.goxml.getXMLstring( tem.gofile, "gofile", "datfilenames" );
    
  for(icmnt = 0; icmnt < numcmnt; ++icmnt )
  {
    ifilename = datfilenames.substr( 0, datfilenames.find( "," ) );
    tem.getsitecd( icmnt, ifilename );
    datfilenames = datfilenames.substr( datfilenames.find( "," ) + 1, (datfilenames.length( ) - datfilenames.find( "," ) - 1) );
     
    flog1 << " datfile " << ifilename << " opened for input of vegetation cmnt " << icmnt << endl;
  }

  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "fstexture" );
  fstxt = fopen( ifilename.c_str(), "r" );
  flog1 << ifilename << " opened for soil texture input " << endl;

  if( !fstxt )
  {
    flog1 << endl << "Cannot open " << ifilename;
    flog1 << " for data input" << endl;

    exit( -1 );
  }

  ifilename = tem.goxml.getXMLstring( tem.gofile, "gofile", "felev" );
  felev = fopen( ifilename.c_str(), "r" );
  flog1 << ifilename << " opened for elevation input " << endl;

  if( !felev )
  {
    flog1 << "\nCannot open " << ifilename;
    flog1 << " for data input" << endl;

    exit( -1 );
  }
  
  setGIStopography( flog1, fstxt, felev );

  calibcmnt = tem.goxml.getXMLint( tem.gofile, "gofile", "calibcmnt" );
  flog1 << "calibrating for vegetation type " << calibcmnt << endl;
  
  tem.veg.cmnt = calibcmnt;
  tem.ag.cmnt = calibcmnt;

};
/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void pcdisplayYear( const int& manflag )
{
  move( YRAVGS_ROW,1 );
  printw( "YRVEGC = %7.1lf  | YRVEGN =  %6.2lf   | YRPLEAF =   %5.3lf | YRNIN   =  %6.3lf | YRPREC = %7.2lf | ROOTZMM  = %7.2lf || AGYIELDC = %6.2lf",
          tem.veg.yrcarbon, tem.veg.yrnitrogen, tem.veg.yrpleaf, tem.soil.yrnin, (tem.atms.yrrain + tem.atms.yrsnowfall), tem.soil.getROOTZ()*1000.0, tem.ag.yrformPROD1C );
               
  move( YRAVGS_ROW+1,1 );
  printw( "YRSOLC = %7.1lf  | YRSOLN = %6.1lf    | YRPSTEM =   %5.3lf | YRNLOST =  %6.3lf | YREET  = %7.2lf | FLDCAPMM = %7.2lf || AGYIELDN = %6.2lf",
          tem.soil.yrorgc, tem.soil.yrorgn, tem.veg.yrpsapwood, tem.soil.yrnlost, tem.soil.yreet, tem.soil.getFLDCAP(), tem.ag.yrformPROD1N );
          
  move( YRAVGS_ROW+2,1 );
//  printw( "INGPP  =  %7.2lf | AVALN  =    %6.4lf | YRPROOT =   %5.3lf | A->ILC  = %6.2lf  | YRWYLD = %7.2lf | WILTPTMM = %7.2lf || AGSTUBC  = %6.2lf",
  printw( "INGPP  =  %7.2lf | AVALN  =    %6.4lf | YRPROOT =   %5.3lf | YRLCHDIN  = %6.2lf  | YRWYLD = %7.2lf | WILTPTMM = %7.2lf || AGSTUBC  = %6.2lf",
//          tem.veg.yringpp, tem.soil.yravln, tem.veg.yrproot, tem.veg.yrallocilc, tem.soil.yrh2oyld, tem.soil.getWILTPT(), tem.ag.yrstubC );
          tem.veg.yringpp, tem.soil.yravln, tem.veg.yrproot, tem.soil.yrlchdin, tem.soil.yrh2oyld, tem.soil.getWILTPT(), tem.ag.yrstubC );
          
  move( YRAVGS_ROW+3,1 );
  printw( "GPP    =  %7.2lf | NRSRB  =    %6.4lf | YRSTRN  = %6.2lf  | A->ISC  = %6.2lf  | YRRAIN = %7.2lf | AWCAPMM  = %7.2lf || AGSTUBN  = %6.2lf",
          tem.veg.yrgpp, tem.veg.yrnrsorb, tem.veg.yrstructn, tem.veg.yrallocisc, tem.atms.yrrain, tem.soil.getAWCAPMM(), tem.ag.yrstubN );
          
  move( YRAVGS_ROW+4,1 );
  printw( "INNPP  =  %7.2lf | INNUP  =   %6.3lf  | LABILN  =  %5.2lf  | A->IRC  = %6.2lf  | YRSNOW = %7.2lf | YRAVLW   = %7.2lf || A->SEEDC = %6.2lf",
          tem.veg.yrinnpp, tem.veg.yrinnup, tem.veg.yrstoren, tem.veg.yrallocirc, tem.atms.yrsnowfall, tem.soil.yravlh2o, tem.veg.yrallocseedc );
  
  move( YRAVGS_ROW+5,1 );
  printw( "NPP    =  %7.2lf | NUP    =   %6.3lf  | YRVEGCN = %6.2lf  | RMLEAF  = %6.2lf  | SNWINF = %7.2lf | YRGRNDW  = %7.2lf || A->SEEDN = %6.2lf",
          tem.veg.yrnpp, tem.veg.yrnup, tem.veg.yrc2n, tem.veg.yrrmleaf, tem.soil.yrsnowinf, (tem.soil.yrrgrndh2o + tem.soil.yrsgrndh2o), tem.veg.yrallocseedn );
          
  move( YRAVGS_ROW+6,1 );
  printw( "YRLTRC =  %7.2lf | YRLTRN =   %6.3lf  | YRSOLCN =  %5.2lf  | RMSTEM  = %6.2lf  | RPERC  = %7.2lf | YRSMOIST = %7.2lf || YRPSEED = %5.3lf",
//          tem.veg.yrltrc, tem.veg.yrltrn, tem.soil.yrc2n, tem.veg.yrrmsapwood, tem.soil.yrrperc, tem.soil.yrsmoist, tem.ag.yrgrowdd );
          tem.veg.yrltrc, tem.veg.yrltrn, tem.soil.yrc2n, tem.veg.yrrmsapwood, tem.soil.yrrperc, tem.soil.yrsmoist, tem.veg.yrpseed );
          
  move( YRAVGS_ROW+7,1 );
//  printw( "YRRH   =  %7.2lf | NMIN   =   %6.3lf  | YRNEP   = %6.2lf  | RMROOT  = %6.2lf  | SPERC  = %7.2lf | YRVSM    = %7.2lf || YRFROST  = %5.3lf",
  printw( "YRRH   =  %7.2lf | NMIN   =   %6.3lf  | YRNEP   = %6.2lf  | RMROOT  = %6.2lf  | SPERC  = %7.2lf | YRVSM    = %7.2lf || YRFROST  = %5.3lf",
          tem.microbe.yrrh, tem.microbe.yrnmin, tem.yrnep, tem.veg.yrrmroot, tem.soil.yrsperc, tem.soil.yrvsm*100.0, tem.ag.yrfrost );
   
  move( YRAVGS_ROW+8,1 );
  
  refresh();

};
/* *************************************************************
************************************************************* */

/***************************************************************
***************************************************************/

void resetInitState( void )
{
  int dm;
  double avgphi = ZERO;
  double avgtair = ZERO;

  xdyr = 0;
  endgrid = 0;
  equilsol = 0;
  ivar = 1;
  
  tem.endeq = 1;
  tem.intflag = 0;
  tem.retry = 0;
  tem.tol = tem.inittol;
  
  for( dm = 0; dm < CYCLE; ++dm )
  {
    climate[clm.I_CO2][dm][0] = tem.atms.getCO2LEVEL();
    
    avgtair += climate[clm.I_TAIRD][dm][firstyear]/CYCLE;
    
    if( dm == 0 ) { tem.veg.setTOPT( tem.veg.getTOPTMIN( tem.veg.cmnt ) ); }
    if( climate[clm.I_TAIRD][dm][0] > tem.veg.getTOPT() ) 
    { 
      tem.veg.setTOPT( climate[clm.I_TAIRD][dm][firstyear] ); 
    }
  }
  

  tem.veg.setRTAIR( avgtair );
  
  for( dm = 0; dm < CYCLE; ++dm )
  {
    tem.veg.phenology( tem.veg.cmnt,
                       dm,
                       climate[clm.I_TAIR][dm][firstyear],
                       climate[clm.I_TAIRD][dm][firstyear],
                       climate[clm.I_TAIRN][dm][firstyear],
                       1.0,
                       1.0,
                       0.0,
                       climate[clm.I_DAYL][dm][0]);
                       
    avgphi += tem.veg.getPHI()/CYCLE;
  }
  
  tem.veg.setRPHI( avgphi );
  
    
  tem.soil.xtext( tem.veg.cmnt, 
                  tem.soil.getPCTSILT(), 
                  tem.soil.getPCTCLAY() );
  
  // previous-month variables
  tem.atms.setPREVTAIR( climate[clm.I_TAIR][CYCLE-1][firstyear] );
  tem.atms.setPREV2TAIR( climate[clm.I_TAIR][CYCLE-2][firstyear] );
  tem.soil.setPREVSPACK( ZERO );
  tem.atms.setPREVCO2( tem.atms.getCO2LEVEL() );

  tem.initializeState( CYCLE );
  
  move(INSTR_ROW,1);
  printw( " Keyboard commands for calibration: \n" );
  printw( "      Press 'End' to pause simulation  \n" );
  printw( "      Press 'Enter' to resume simulation  \n" );
  printw( "      Press 'Insert' to type in a value for a parameter/variable  \n" );
  printw( "      Press the right or left arrow to cycle between parameters/variables in the same category \n" );
  printw( "      Press the up or down arrows to change the category of parameters/variables \n" );
  printw( "      Press the page up or page down buttons to increment or decrement the chosen parameter/variable \n" );
  printw( "      Press 'Home' to quit \n" );
  
  refresh(); 

};

/* *************************************************************
************************************************************* */
void resetInitStateCalib( void )
{
  int dm;
  double avgphi = ZERO;
  double avgtair = ZERO;

  xdyr = 0;
  endgrid = 0;
  equilsol = 0;
  ivar = 1;

  tem.endeq = 1;
  tem.intflag = 0;
  tem.retry = 0;
  tem.tol = tem.inittol;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    climate[clm.I_CO2][dm][0] = tem.atms.getCO2LEVEL();

    avgtair += climate[clm.I_TAIRD][dm][firstyear]/CYCLE;

    if( dm == 0 ) { tem.veg.setTOPT( tem.veg.getTOPTMIN( tem.veg.cmnt ) ); }
    if( climate[clm.I_TAIRD][dm][0] > tem.veg.getTOPT() )
    {
      tem.veg.setTOPT( climate[clm.I_TAIRD][dm][firstyear] );
    }
  }

  tem.veg.setRTAIR( avgtair );

  for( dm = 0; dm < CYCLE; ++dm )
  {
    tem.veg.phenology( tem.veg.cmnt,
                       dm,
                       climate[clm.I_TAIR][dm][firstyear],
                       climate[clm.I_TAIRD][dm][firstyear],
                       climate[clm.I_TAIRN][dm][firstyear],
                       1.0,
                       1.0,
                       0.0,
                       climate[clm.I_DAYL][dm][0]);

    avgphi += tem.veg.getPHI()/CYCLE;
  }

  tem.veg.setRPHI( avgphi );

 tem.soil.xtext( tem.veg.cmnt,
                  tem.soil.getPCTSILT(),
                  tem.soil.getPCTCLAY() );

  // previous-month variables
  tem.atms.setPREVTAIR( climate[clm.I_TAIR][CYCLE-1][firstyear] );
  tem.atms.setPREV2TAIR( climate[clm.I_TAIR][CYCLE-2][firstyear] );
  tem.soil.setPREVSPACK( ZERO );
  tem.atms.setPREVCO2( tem.atms.getCO2LEVEL() );

  tem.initializecalibState( CYCLE );

  move(INSTR_ROW,1);
  printw( " Keyboard commands for calibration: \n" );
  printw( "      Press 'End' to pause simulation  \n" );
  printw( "      Press 'Enter' to resume simulation  \n" );
  printw( "      Press 'Insert' to type in a value for a parameter/variable  \n" );
  printw( "      Press the right or left arrow to cycle between parameters/variables in the same category \n" );
  printw( "      Press the up or down arrows to change the category of parameters/variables \n" );
  printw( "      Press the page up or page down buttons to increment or decrement the chosen parameter/variable \n" );
  printw( "      Press 'Home' to quit \n" );

  refresh();

};

/***************************************************************
***************************************************************/

void setCalibVar( void )
{

  tcalvar[T_TOL] =  tem.tol;
  tcalvar[T_TAVG] = tem.tauavg;
  tcalvar[T_CO2] =  tem.atms.getCO2LEVEL();
  
  tcalvar[T_GDDMIN] = tem.ag.getGDDMIN( tem.ag.cmnt );
  tcalvar[T_GDDSEED] = tem.ag.getGDDSEED( tem.ag.cmnt );
  tcalvar[T_GDDHARVST] = tem.ag.getGDDHARVST( tem.ag.cmnt );
  tcalvar[T_TKILL] = tem.ag.getTKILL( tem.ag.cmnt );
  
  tcalvar[T_CMAX]  =  tem.veg.getCMAX();
  tcalvar[T_KRA]  =  tem.veg.getKRA( tem.veg.cmnt );
  tcalvar[T_KDC]  =  tem.microbe.getKDC();
  tcalvar[T_TAULEAF]  =  tem.veg.getTAULEAF( tem.veg.cmnt );
  tcalvar[T_TAUSTEMA]  =  tem.veg.getTAUSAPWOOD( tem.veg.cmnt );
  tcalvar[T_TAUSTEMI]  =  tem.veg.getTAUHEARTWOOD( tem.veg.cmnt );
  tcalvar[T_TAUROOT]  =  tem.veg.getTAUROOT( tem.veg.cmnt );
  tcalvar[T_TAUSEED]  =  tem.veg.getTAUSEED( tem.veg.cmnt );
  tcalvar[T_NMAX]  =  tem.veg.getNMAX();
  tcalvar[T_MNUP]  =  tem.microbe.getNUP();
  tcalvar[T_CNSOL] =  tem.microbe.getCNSOIL( tem.veg.cmnt );
  tcalvar[T_CNLTR]  =  tem.veg.getCNLTR( tem.veg.cmnt );
  tcalvar[T_NLOSS] =  tem.soil.getNLOSS( tem.veg.cmnt );
  
  tcalvar[T_SLA] =  tem.veg.getSLA( tem.veg.cmnt );
  tcalvar[T_MOPT] =  tem.microbe.getMOISTOPT( tem.veg.cmnt );
  
  tcalvar[T_GROOT]  =  tem.veg.pen.getGROOT( tem.veg.cmnt );
  tcalvar[T_KSTEM]  =  tem.veg.pen.getKSTEM( tem.veg.cmnt );
  tcalvar[T_CDLEAF]  =  tem.veg.pen.getCDLEAF( tem.veg.cmnt );
  tcalvar[T_WLEAF]  =  tem.veg.pen.getWLEAF( tem.veg.cmnt );
  tcalvar[T_GSMIN]  =  tem.veg.pen.getGSMIN( tem.veg.cmnt );
  tcalvar[T_GSA]  =  tem.veg.pen.getGSA( tem.veg.cmnt );
  tcalvar[T_KEXT]  =  tem.veg.pen.getKEXT( tem.veg.cmnt );
  tcalvar[T_ALBEDO]  =  tem.veg.pen.getALBEDO( tem.veg.cmnt );
  tcalvar[T_LSCMIN]  =  tem.veg.pen.getLSCMIN( tem.veg.cmnt );
  tcalvar[T_RHOSTEM]  =  tem.veg.pen.getRHOSTEM( tem.veg.cmnt );

};

/***************************************************************
***************************************************************/


/* **************************************************************
************************************************************** */

int setGIStopography( ofstream& rflog1,
                      FILE* fstxt,
                      FILE* felev )
{

  int gisend;

  Soildat45 fao;
  Elevdat45 elv;

  gisend = fao.getdel( fstxt );

  if( -1 == gisend )
  {
    rflog1 << "Ran out of Soil texture data" << endl << endl;

    exit( -1 );
  }

  tem.soil.setPCTSAND( fao.pctsand );
  tem.soil.setPCTSILT( fao.pctsilt );
  tem.soil.setPCTCLAY( fao.pctclay );

  gisend = elv.getdel( felev );

  if( gisend == -1 )
  {
    rflog1 << "Ran out of Elevation data" << endl << endl;

    exit( -1 );
  }

  tem.elev = elv.elev;

  return gisend;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void updateCalibPar( tcalkey& tcal,
                     const double& factor, 
                     int& t )
{
  int dm;
  int readerror = 0;
  char istr[80];

  switch( tcal )
  {
    case T_TOL:     tem.tol = tcalvar[T_TOL] * factor;                     break;

    case T_TAVG:    tem.tauavg = tcalvar[T_TAVG] * factor;                 break;
                    
    case T_CO2:     tem.atms.setCO2LEVEL( (tcalvar[T_CO2] * factor) );
                    for( dm = 0; dm < CYCLE; ++dm )
                    {
	                  climate[clm.I_CO2][dm][0] = tem.atms.getCO2LEVEL();
	                }        
                    break;
                    
    case T_AVLNFLAG:        if( factor != 1 ) { tem.avlnflag += 1; }
	                        if( tem.avlnflag > 1 ) { tem.avlnflag = 0; }      
                            break;

    case T_NFEED:           if( factor != 1 ) { tem.nfeed += 1; }
	                        if( tem.nfeed > 1 ) { tem.nfeed = 0; }    
               	            break;

    case T_BLINE:           if( factor != 1 ) { tem.baseline += 1; }
	                        if( tem.baseline > 1 ) { tem.baseline = 0; } 
                            break;

    case T_MOISTLIM:        if( factor != 1 ) { tem.moistlim += 1; }
	                        if( tem.moistlim > 1 ) { tem.moistlim = 0; }      
                            break;
	            
    case T_O3FLAG:          if( factor != 1 ) { tem.o3flag += 1; }
	                        if( tem.o3flag > 1 ) { tem.o3flag = 0; }    
                            break;
                
    case T_RESET:  if( factor != 1 ) { reset += 1; }
                   if( reset > 1 ) { reset = 0; }
                   break;

    case T_MANFLAG:if( factor != 1 ) { manflag += 1; }
	               if( manflag > 1 ) { manflag = 0; }     
                   break;
	            
//// End of 9 settings
    
    case T_GDDMIN:  tem.ag.setGDDMIN( (tcalvar[T_GDDMIN] * factor),
                                       tem.ag.cmnt );
                    break;

    case T_GDDSEED: tem.ag.setGDDSEED( (tcalvar[T_GDDSEED] * factor),
                                         tem.ag.cmnt );
                    break;

    case T_GDDHARVST: tem.ag.setGDDHARVST( (tcalvar[T_GDDHARVST] * factor),
                                             tem.ag.cmnt );
                      break;

    case T_TKILL:   tem.ag.setTKILL( (tcalvar[T_TKILL] * factor),
                                         tem.ag.cmnt );
                    break;
                    
    case T_AGSTATE: if( factor == 1.01 ) { tem.ag.state += 1; }
                        if( factor == 0.99 ) { tem.ag.state -= 1; }
	                if( tem.ag.state > 3 ) { tem.ag.state = 0; }    
	                if( tem.ag.state < 0 ) { tem.ag.state = 0; }    
//cout << "factor = " << factor << " " << tem.ag.state << endl;
                    break;

    case T_AGPROD:  if( factor != 1 ) { agProdFlag += 1; }
	                if( agProdFlag > 1 ) { agProdFlag = 0; }    
                    break;

    case T_TILLFLAG: if( factor != 1 ) { tem.ag.tillflag += 1; }
	                 if( tem.ag.tillflag > 1 ) { tem.ag.tillflag = 0; }        
                     break;

    case T_FERTFLAG: if( factor != 1 )
                     {
                       tem.ag.fertflag += 1;
                       tem.ag.fert1950flag += 1;
                     }
	                 if( tem.ag.fert1950flag > 1 )
	                 {
	                   tem.ag.fertflag = 0;
	                   tem.ag.fert1950flag = 0;
	                 }        
                     break;

    case T_IRRGFLAG: if( factor != 1 )
                     {
                       tem.ag.irrgflag += 1;
                       tem.ag.irrg1950flag += 1;
                     }
                         if( tem.ag.irrg1950flag > 1 )
                         {
                           tem.ag.irrgflag = 0;
                           tem.ag.irrg1950flag = 0;
                         }
                     break;

//    case T_IRRGFLAG: if( factor != 1 ) { tem.ag.irrgflag += 1; }
//	                 if( tem.ag.irrgflag > 1 ) { tem.ag.irrgflag = 0; }    
//                     break;   
                
//// End of 9 agfile variables

    case T_CMAX:    tem.veg.setCMAX( (tcalvar[T_CMAX] * factor) );
                    break;

    case T_KRA:     tem.veg.setKRA( (tcalvar[T_KRA] * factor),
                                  tem.veg.cmnt );
                    break;

    case T_KDC:     tem.microbe.setKDC( (tcalvar[T_KDC] * factor) );
                    break;

    case T_TAULEAF: tem.veg.setTAULEAF( (tcalvar[T_TAULEAF] * factor),
                                       tem.veg.cmnt );  
                    break;

    case T_TAUSTEMA:tem.veg.setTAUSAPWOOD( (tcalvar[T_TAUSTEMA] * factor),
                                   tem.veg.cmnt );   
                    break;

    case T_TAUSTEMI:tem.veg.setTAUHEARTWOOD( (tcalvar[T_TAUSTEMI] * factor),
                               tem.veg.cmnt );   
                    break;

    case T_TAUROOT: tem.veg.setTAUROOT( (tcalvar[T_TAUROOT] * factor),
                                   tem.veg.cmnt );  
                    break;

    case T_TAUSEED: tem.veg.setTAUSEED( (tcalvar[T_TAUSEED] * factor),
                               tem.veg.cmnt );  
                    break;
                    
    case T_NMAX:    tem.veg.setNMAX( (tcalvar[T_NMAX] * factor) );
                    break;

    case T_MNUP:    tem.microbe.setNUP( (tcalvar[T_MNUP] * factor) );
                    break;
                    
    case T_CNSOL:   tem.microbe.setCNSOIL( (tcalvar[T_CNSOL] * factor),
                                            tem.veg.cmnt );
                    break;

    case T_CNLTR:   tem.veg.setCNLTR( (tcalvar[T_CNLTR] * factor),
                           tem.veg.cmnt );
                    break;
                    

    case T_NLOSS :  tem.soil.setNLOSS( (tcalvar[T_NLOSS] * factor),
                                       tem.veg.cmnt ); 
                    break;
     
//// End of 19 datfile variables

    case T_SLA:     tem.veg.setSLA( (tcalvar[T_SLA] * factor),
                                     tem.veg.cmnt );
                    break;

    case T_MOPT:    tem.microbe.setMOISTOPT( (tcalvar[T_MOPT] * factor),
                                              tem.veg.cmnt );
                    break;

//// End of 2 vegfile variables

    case T_GROOT:   tem.veg.pen.setGROOT( (tcalvar[T_GROOT] * factor),
                                   tem.veg.cmnt );
                    break;
                    
    case T_KSTEM:   tem.veg.pen.setKSTEM( (tcalvar[T_KSTEM] * factor),
                                   tem.veg.cmnt );
                    break;
                    
    case T_CDLEAF:  tem.veg.pen.setCDLEAF( (tcalvar[T_CDLEAF] * factor),
                                              tem.veg.cmnt );
                    break;

    case T_WLEAF:   tem.veg.pen.setWLEAF( (tcalvar[T_WLEAF] * factor),
                                          tem.veg.cmnt );
                    break;
                   
    case T_GSMIN:   tem.veg.pen.setGSMIN( (tcalvar[T_GSMIN] * factor),
                                              tem.veg.cmnt );
                    break;

    case T_GSA:     tem.veg.pen.setGSA( (tcalvar[T_GSA] * factor),
                                              tem.veg.cmnt );
                    break;

    case T_KEXT:    tem.veg.pen.setKEXT( (tcalvar[T_KEXT] * factor),
                                              tem.veg.cmnt );
                    break;

    case T_ALBEDO:  tem.veg.pen.setALBEDO( (tcalvar[T_ALBEDO] * factor),
                                              tem.veg.cmnt );
                    break;
                     
    case T_LSCMIN:  tem.veg.pen.setLSCMIN( (tcalvar[T_LSCMIN] * factor),
                                              tem.veg.cmnt );
                    break;
                     
    case T_RHOSTEM: tem.veg.pen.setRHOSTEM( (tcalvar[T_RHOSTEM] * factor),
                                              tem.veg.cmnt );
                    break;
    
//// End of 12 conductfile variables
    
    
    case T_OPTCN0:  if( factor > 1 )
                    {
                      tem.next( tem.sey[0] );
                      tcalvar[T_OPTCN0] = (int(tcalvar[T_OPTCN0])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[0]);
	                  tcalvar[T_OPTCN0] = (NEKEY+int(tcalvar[T_OPTCN0])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN0]) <= NEKEY )
	                {
                      tem.sey[0] = tem.keyjump( int(tcalvar[T_OPTCN0]), tem.sey[0] );
                    } 
                    break;
	                
    case T_OPTCN1:  if( factor > 1 )
                    {
                      tem.next( tem.sey[1] );
                      tcalvar[T_OPTCN1] = (int(tcalvar[T_OPTCN1])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[1]);
	                  tcalvar[T_OPTCN1] = (NEKEY+int(tcalvar[T_OPTCN1])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN1]) <= NEKEY )
	                {
                      tem.sey[1] = tem.keyjump( int(tcalvar[T_OPTCN1]), tem.sey[1] );
                    } 
                    break;
	                
    case T_OPTCN2:  if( factor > 1 )
                    {
                      tem.next( tem.sey[2] );
                      tcalvar[T_OPTCN2] = (int(tcalvar[T_OPTCN2])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[2]);
	                  tcalvar[T_OPTCN2] = (NEKEY+int(tcalvar[T_OPTCN2])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN2]) <= NEKEY )
	                {
                      tem.sey[2] = tem.keyjump( int(tcalvar[T_OPTCN2]), tem.sey[2] );
                    } 
                    break;
	                
    case T_OPTCN3:  if( factor > 1 )
                    {
                      tem.next( tem.sey[3] );
                      tcalvar[T_OPTCN3] = (int(tcalvar[T_OPTCN3])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[3]);
	                  tcalvar[T_OPTCN3] = (NEKEY+int(tcalvar[T_OPTCN3])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN3]) <= NEKEY )
	                {
                      tem.sey[3] = tem.keyjump( int(tcalvar[T_OPTCN3]), tem.sey[3] );
                    } 
                    break;
	                
    case T_OPTCN4:  if( factor > 1 )
                    {
                      tem.next( tem.sey[4] );
                      tcalvar[T_OPTCN4] = (int(tcalvar[T_OPTCN4])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[4]);
	                  tcalvar[T_OPTCN4] = (NEKEY+int(tcalvar[T_OPTCN4])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN4]) <= NEKEY )
	                {
                      tem.sey[4] = tem.keyjump( int(tcalvar[T_OPTCN4]), tem.sey[4] );
                    } 
                    break;
	                
    case T_OPTCN5:  if( factor > 1 )
                    {
                      tem.next( tem.sey[5] );
                      tcalvar[T_OPTCN5] = (int(tcalvar[T_OPTCN5])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[5]);
	                  tcalvar[T_OPTCN5] = (NEKEY+int(tcalvar[T_OPTCN5])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN5]) <= NEKEY )
	                {
                      tem.sey[5] = tem.keyjump( int(tcalvar[T_OPTCN5]), tem.sey[5] );
                    } 
                    break;
	                
    case T_OPTCN6:  if( factor > 1 )
                    {
                      tem.next( tem.sey[6] );
                      tcalvar[T_OPTCN6] = (int(tcalvar[T_OPTCN6])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[6]);
	                  tcalvar[T_OPTCN6] = (NEKEY+int(tcalvar[T_OPTCN6])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN6]) <= NEKEY )
	                {
                      tem.sey[6] = tem.keyjump( int(tcalvar[T_OPTCN6]), tem.sey[6] );
                    } 
                    break;
	                
    case T_OPTCN7:  if( factor > 1 )
                    {
                      tem.next( tem.sey[7] );
                      tcalvar[T_OPTCN7] = (int(tcalvar[T_OPTCN7])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[7]);
	                  tcalvar[T_OPTCN7] = (NEKEY+int(tcalvar[T_OPTCN7])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN7]) <= NEKEY )
	                {
                      tem.sey[7] = tem.keyjump( int(tcalvar[T_OPTCN7]), tem.sey[7] );
                    } 
                    break;
	                
    case T_OPTCN8:  if( factor > 1 )
                    {
                      tem.next( tem.sey[8] );
                      tcalvar[T_OPTCN8] = (int(tcalvar[T_OPTCN8])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[8]);
	                  tcalvar[T_OPTCN8] = (NEKEY+int(tcalvar[T_OPTCN8])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN8]) <= NEKEY )
	                {
                      tem.sey[8] = tem.keyjump( int(tcalvar[T_OPTCN8]), tem.sey[8] );
                    } 
                    break;
	                
    case T_OPTCN9:  if( factor > 1 )
                    {
                      tem.next( tem.sey[9] );
                      tcalvar[T_OPTCN9] = (int(tcalvar[T_OPTCN9])+1)%NEKEY;
                    }
	                if( factor < 1 )
	                {
	                  tem.prev(tem.sey[9]);
	                  tcalvar[T_OPTCN9] = (NEKEY+int(tcalvar[T_OPTCN9])-1)%NEKEY;
	                }
	                if( int(tcalvar[T_OPTCN9]) <= NEKEY )
	                {
                      tem.sey[9] = tem.keyjump( int(tcalvar[T_OPTCN9]), tem.sey[9] );
                    } 
                    break;

//// End of 10 optional c/n fluxes
   
    case T_OPTH2O0: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[0] );
   		              tcalvar[T_OPTH2O0] = (int(tcalvar[T_OPTH2O0])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[0] );
   		              tcalvar[T_OPTH2O0] = (NWKEY+int(tcalvar[T_OPTH2O0])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O0]) <= NWKEY )
	                {
                      tem.swy[0] = tem.keyjump( int(tcalvar[T_OPTH2O0]), tem.swy[0] );
                    }        
                    break;
	    	        
    case T_OPTH2O1: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[1] );
   		              tcalvar[T_OPTH2O1] = (int(tcalvar[T_OPTH2O1])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[1] );
   		              tcalvar[T_OPTH2O1] = (NWKEY+int(tcalvar[T_OPTH2O1])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O1]) <= NWKEY )
	                {
                      tem.swy[1] = tem.keyjump( int(tcalvar[T_OPTH2O1]), tem.swy[1] );
                    }        
                    break;
	    	        
    case T_OPTH2O2: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[2] );
   		              tcalvar[T_OPTH2O2] = (int(tcalvar[T_OPTH2O2])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[2] );
   		              tcalvar[T_OPTH2O2] = (NWKEY+int(tcalvar[T_OPTH2O2])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O2]) <= NWKEY )
	                {
                      tem.swy[2] = tem.keyjump( int(tcalvar[T_OPTH2O2]), tem.swy[2] );
                    }        
                    break;
	    	        
    case T_OPTH2O3: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[3] );
   		              tcalvar[T_OPTH2O3] = (int(tcalvar[T_OPTH2O3])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[3] );
   		              tcalvar[T_OPTH2O3] = (NWKEY+int(tcalvar[T_OPTH2O3])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O3]) <= NWKEY )
	                {
                      tem.swy[3] = tem.keyjump( int(tcalvar[T_OPTH2O3]), tem.swy[3] );
                    }        
                    break;
	    	        
    case T_OPTH2O4: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[4] );
   		              tcalvar[T_OPTH2O4] = (int(tcalvar[T_OPTH2O4])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[4] );
   		              tcalvar[T_OPTH2O4] = (NWKEY+int(tcalvar[T_OPTH2O4])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O4]) <= NWKEY )
	                {
                      tem.swy[4] = tem.keyjump( int(tcalvar[T_OPTH2O4]), tem.swy[4] );
                    }        
                    break;
	    	        
    case T_OPTH2O5: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[5] );
   		              tcalvar[T_OPTH2O5] = (int(tcalvar[T_OPTH2O5])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[5] );
   		              tcalvar[T_OPTH2O5] = (NWKEY+int(tcalvar[T_OPTH2O5])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O5]) <= NWKEY )
	                {
                      tem.swy[5] = tem.keyjump( int(tcalvar[T_OPTH2O5]), tem.swy[5] );
                    }        
                    break;
	    	        
    case T_OPTH2O6: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[6] );
   		              tcalvar[T_OPTH2O6] = (int(tcalvar[T_OPTH2O6])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[6] );
   		              tcalvar[T_OPTH2O6] = (NWKEY+int(tcalvar[T_OPTH2O6])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O6]) <= NWKEY )
	                {
                      tem.swy[6] = tem.keyjump( int(tcalvar[T_OPTH2O6]), tem.swy[6] );
                    }        
                    break;
	    	        
    case T_OPTH2O7: if( factor > 1 )
   		            {
   		              tem.next( tem.swy[7] );
   		              tcalvar[T_OPTH2O7] = (int(tcalvar[T_OPTH2O7])+1)%NWKEY;
   		            }
                    if( factor < 1 )
   		            {
   		              tem.prev( tem.swy[7] );
   		              tcalvar[T_OPTH2O7] = (NWKEY+int(tcalvar[T_OPTH2O7])-1)%NWKEY;
   		            }
                    if( int(tcalvar[T_OPTH2O7]) <= NWKEY )
	                {
                      tem.swy[7] = tem.keyjump( int(tcalvar[T_OPTH2O7]), tem.swy[7] );
                    }        
                    break;
    
  //// End of 8 optional h2o fluxes

  }

};

/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************* */

void updateTCLMGridCell( const int& pdyr,
                         const int& CYCLE,
                         const int ndays[] ) 
{

  const double Watts2cal = 1.0 / 0.4845;
  int dyr,day;
  int gisend;
  int transtime = RTIME-1;

  double avgprec, avgtair, avgtaird;

  int CYCLE0[transtime+1];
int year,ndays0[12];
int i,j;
double sumdays;


  avgprec = ZERO;
  avgtair = ZERO;
  avgtaird = ZERO;

  double lat = -999.9;

  if( 0 == pdyr )
  {
   for(j=0; j<transtime+1; j++)
   {
    year = j + clm.startyr;

    ndays0[0] = 31;
    if((year%4) == 0.0)
    {
    ndays0[1] = 29;
    }
    else
    {
    ndays0[1] = 28;
    }
    ndays0[2] = 31;
    ndays0[3] = 30;
    ndays0[4] = 31;
    ndays0[5] = 30;
    ndays0[6] = 31;
    ndays0[7] = 31;
    ndays0[8] = 30;
    ndays0[9] = 31;
    ndays0[10] = 30;
    ndays0[11] = 31;

    sumdays = 0.0;
    for (i=0; i<12; i++)
    {
    sumdays += ndays0[i];
    }
    CYCLE0[j] = sumdays;
    }

    if( 1 == clm.tcldsflag )
    {
      if( 0 == clm.sradflag )
      {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
        for( day = 0; day < CYCLE0[dyr]; ++day )
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


      if( 1 == clm.cldflag )
      {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
        for( day = 0; day < CYCLE0[dyr]; ++day )
        {
          gisend = cldsdat[dyr].getdel( ifclds, day );

          if( -1 == gisend )
          {
            cout << "Ran out of Cloudiness data" << endl << endl;
            flog1 << "Ran out of Cloudiness data" << endl << endl;

            exit( -1 );
          }
        }
        }
      }
      else
      {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
        for( day = 0; day < CYCLE0[dyr]; ++day )
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
      }

      if( 1 == clm.parflag )
      {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
        for( day = 0; day < CYCLE0[dyr]; ++day )
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
    else  // 0 == clm.tcldsflag
    {
      if( 0 == clm.sradflag )
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

      if( 1 == clm.cldflag )
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
      }

      if( 1 == clm.parflag )
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

    // Read in historical monthly air temperatures for grid cell

    if( 1 == clm.ttairflag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
      {
        gisend = tairdat[dyr].getdel( iftair, day );
//        cout << "tairdat = " << dyr << " " << day << " " << tairdat[dyr].mon[day] << endl;

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

    // Read in historical monthly precipitation for grid cell

    if( 1 == clm.tprecflag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
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

	// Read in historical monthly daily temperature range for grid cell

    if( 1 == clm.ttrangeflag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
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

	// Read in historical monthly vapor pressure for grid cell

    if( 1 == clm.tvprflag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
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

        // Read in historical CO2 for grid cell

    if( 1 == clm.tco2flag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
      {
        gisend = co2dat[dyr].getdel( ifco2, day );

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

    // Read in historical monthly ozone data for grid cell

    if( 1 == clm.to3flag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
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

//
//  Nitrogen Deposition
//
    if( 1 == clm.tndepflag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
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

    // Read in wind speed climatology
    if( 1 == clm.tws10flag )
    {
     for( dyr = 0; dyr < (transtime+1); ++dyr )
      {
      for( day = 0; day < CYCLE0[dyr]; ++day )
      {
        gisend = ws10dat[dyr].getdel( ifws10, day );

        if( -1 == gisend )
        {
          cout << "Ran out of Windspeed data" << endl << endl;
          flog1 << "Ran out of Windspeed data" << endl << endl;

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
        cout << "Ran out of Windspeed data" << endl << endl;
        flog1 << "Ran out of Windspeed data" << endl << endl;

        exit( -1 );
      }
     }
    }
}

  dyr = pdyr;
//  cout << "dyr = " << dyr << endl;

 for( day = 0; day < CYCLE; ++day )
  {
  if( 0 == clm.tcldsflag )
  {
    if( 0 == clm.sradflag )
    {
        girrdat[dyr].mon[day] = girrdat[0].mon[day];
    }

    if( 1 == clm.cldflag )
    {
        cldsdat[dyr].mon[day] = cldsdat[0].mon[day];
    }
    else
    {
        nirrdat[dyr].mon[day] = nirrdat[0].mon[day];
    }

    if( 1 == clm.parflag )
    {
        pardat[dyr].mon[day] = pardat[0].mon[day];
    }
  }

  if( 0 == clm.ttairflag )
  {
    tairdat[dyr].max = tairdat[0].max;
    tairdat[dyr].ave = tairdat[0].ave;

      tairdat[dyr].mon[day] = tairdat[0].mon[day];
  }

  if( 0 == clm.tprecflag )
  {
    precdat[dyr].total = precdat[0].total;

      precdat[dyr].mon[day] = precdat[0].mon[day];
  }

  if( 0 == clm.ttrangeflag )
  {
      trangedat[dyr].mon[day] = trangedat[0].mon[day];
  }

  if( 0 == clm.tvprflag )
  {
      vprdat[dyr].mon[day] = vprdat[0].mon[day];
  }
  
  if( 0 == clm.tws10flag )
  {
      ws10dat[dyr].mon[day] = ws10dat[0].mon[day];
  }

  if( 0 == clm.to3flag )
  {
      o3dat[dyr].mon[day] = o3dat[0].mon[day];
  }

  if( 0 == clm.tndepflag )
  {
      ndepdat[dyr] = ndepdat[0];
  }

    climate[clm.I_CO2][day][pdyr] = clm.getCO2LEVEL();
  
 // Assign information from files to telmnt.climate

    // Air temperature

    climate[clm.I_TAIR][day][pdyr] = tairdat[dyr].mon[day];
//    cout << "tair = " << day << " " << pdyr << " " << climate[clm.I_TAIR][day][pdyr] << endl;

//  NDEP
//  
    adep[0][pdyr]= ndepdat[pdyr].adepval;

    // Daylength (hours)
    
    climate[clm.I_DAYL][day][pdyr] = tem.atms.daylength( tairdat[pdyr].row, day+1 );
    
    // Temperature Range

    climate[clm.I_TRANGE][day][pdyr] = trangedat[dyr].mon[day];

    // Daytime air temperature

    climate[clm.I_TAIRD][day][pdyr] = climate[clm.I_TAIR][day][pdyr]
                            + 12.0*climate[clm.I_TRANGE][day][pdyr]/climate[clm.I_DAYL][day][pdyr]
                            * 1.732 / (2.0*3.14159) * sin( 3.14159 * climate[clm.I_DAYL][day][pdyr] / 24.0 );

    // Nighttime air temperature

    climate[clm.I_TAIRN][day][pdyr] = climate[clm.I_TAIR][day][pdyr]
                            - 12.0*climate[clm.I_TRANGE][day][pdyr]/(24.0 - climate[clm.I_DAYL][day][pdyr])
                            * 1.732 / (2.0*3.14159) * sin( 3.14159 * climate[clm.I_DAYL][day][pdyr] / 24.0 );
    
    // Precipitation

    climate[clm.I_PREC][day][pdyr] = precdat[dyr].mon[day];

    // Rain and Snowfall

    clm.precsplt( climate[clm.I_PREC][day][pdyr],
                            climate[clm.I_TAIR][day][pdyr],
                            climate[clm.I_RAIN][day][pdyr],
                            climate[clm.I_SNWFAL][day][pdyr] );

//
//  Calculate vapor pressure deficit from vapor pressure
//

    climate[clm.I_VPR][day][pdyr] = vprdat[dyr].mon[day];

    climate[clm.I_VPDD][day][pdyr] = (10.*0.61078*exp((17.27*climate[clm.I_TAIRD][day][pdyr])
                                                  /(climate[clm.I_TAIRD][day][pdyr]+237.3)))-vprdat[dyr].mon[day];
    if(climate[clm.I_VPDD][day][pdyr] < ZERO) { climate[clm.I_VPDD][day][pdyr] = ZERO; }


    climate[clm.I_VPDN][day][pdyr] = (10.*0.61078*exp((17.27*climate[clm.I_TAIRN][day][pdyr])
                                                  /(climate[clm.I_TAIRN][day][pdyr]+237.3)))-vprdat[dyr].mon[day];
    if(climate[clm.I_VPDN][day][pdyr] < ZERO) { climate[clm.I_VPDN][day][pdyr] = ZERO; }


    // AOT40 ozone index

    climate[clm.I_AOT40][day][pdyr] = o3dat[dyr].mon[day];

    // 10-meter windspeed
    
    climate[clm.I_WS10][day][pdyr] = ws10dat[dyr].mon[day];


  if(tairdat[pdyr].row >= 0.0) { tem.veg.hemisphere = 1; }
  else { tem.veg.hemisphere = 0; }

//**************************************************************


  // Calculate GIRR during first year of simulation
  //   (Note: use same values throughout simulation)

  if( 1 == clm.sradflag )
  {
    lat = tairdat[pdyr].row; 

    clm.yrsumday = ZERO;

      climate[clm.I_GIRR][day][pdyr] = clm.xgirr( lat, day+1, clm.yrsumday, ndays );
  }
  else
  {
      climate[clm.I_GIRR][day][pdyr] = girrdat[dyr].mon[day] * Watts2cal;
  }

  // Calculate NIRR, CLDINESS and PAR or retrieve from earlier calculations

    if( 1 == clm.cldflag )
    {
      climate[clm.I_CLDS][day][pdyr] = cldsdat[dyr].mon[day];

      climate[clm.I_NIRR][day][pdyr] = clm.xnirr( climate[clm.I_CLDS][day][pdyr], climate[clm.I_GIRR][day][pdyr] );
    }
    else
    {
      climate[clm.I_NIRR][day][pdyr] = nirrdat[dyr].mon[day] * Watts2cal;

      climate[clm.I_CLDS][day][pdyr] = clm.mkclds( climate[clm.I_GIRR][day][pdyr], climate[clm.I_NIRR][day][pdyr] );
    }

    if( 0 == clm.parflag )
    {

      climate[clm.I_PAR][day][pdyr]  = clm.xpar( climate[clm.I_CLDS][day][pdyr], climate[clm.I_NIRR][day][pdyr] );
    }
    else
    {
      climate[clm.I_PAR][day][pdyr] = pardat[dyr].mon[day] * Watts2cal;
    }

//  if(pdyr == firstyear || pdyr == 0)
  if(pdyr == 0)
  {
      avgprec += climate[clm.I_PREC][day][pdyr]/365.0;
      avgtair += climate[clm.I_TAIR][day][pdyr]/365.0;
      avgtaird += climate[clm.I_TAIRD][day][pdyr]/365.0;
    tem.veg.setRPREC( avgprec );
    tem.veg.setRTAIR( avgtaird );

//   cout << "diag = " << avgtair << " " << day << " " << pdyr << endl;

    tem.atms.setMXTAIR( tairdat[pdyr].max );
    tem.atms.setAVETAIR( tairdat[pdyr].ave );

    tem.atms.yrprec = precdat[pdyr].total;
  }
}  // end of day loop

//
//    CALCULATE SLOPE OF DAYLENGTH
//
        for( day = 0; day < CYCLE-1; ++day )
        {

      tem.veg.daylslope[day]  = climate[clm.I_DAYL][day+1][pdyr] - climate[clm.I_DAYL][day][pdyr];

        }

};

/* *************************************************************
************************************************************* */
