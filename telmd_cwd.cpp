/* **************************************************************
*****************************************************************
TELMD_CWD.CPP - Runs TEM for a single grid cell

Modifications:

20060127 - DWK created by modifying telm50b5.cpp
20060127 - DWK changed include from telm50b5.h to tel,437.h
20060127 - DWK changed TEMelmnt50:: to TEMelmntflux::
20060127 - DWK added double aot40 to atmswritepred()
20060127 - DWK deleted tem.soil.stm.updateyrCDM(),
           tem.soil.setPREVDST10(), tem.soil.stm.setNEXTTAIR(),
           tem.soil.stm.setNEXTSNOWFALL(),
           tem.soil.setNEXTDST10() and tem.soil.getDST10()
           from equilibrateTEM() and updateTEMmonth()
20060127 - DWK added tem.atms.setAOT40() to equilibrateTEM(),
           setTEMequilState() and updateTEMmonth()
20060127 - DWK deleted double dst10[CYCLE], double prevdst10,
           double STMdx9[MAXNODES], int STMis9, int STMsmass9,
           double STMt9[MAXNODES], double STMwater9[MAXNODES],
           double STMweight9[MAXSNODES], double STMx9[MAXNODES],
           double STMxfa9[MAXNODES], double STMxfb9[MAXNODES]
           to getTEMCohortState(), initializeCohortTEMState(),
           readCohortState(), saveTEMCohortState(),
           setCohortTEMState() and writeCohortState()
20060127 - DWK added I_FOZONE and I_FINDOZONE to
           outputTEMmonth() and temwritepred()
20060127 - DWK deleted Soil thermal model variables from
           outputTEMmonth() and temwritepred()
20070105 - TWC changed name to telm45, versions _allfert,
           _nofert, and _fert provide different fertilization
           scenarios for agriculture


****************************************************************
************************************************************* */

//#define DEBUGT
//#define DEBUGE

#include<cstdio>

  using std::fscanf;
  using std::FILE;
  using std::printf;

#include<iostream>

  using std::cout;
  using std::ios;
  using std::cerr;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setprecision;
  using std::setw;

#include<cstdlib>

  using std::exit;
  using std::atof;
  using std::atoi;

#include<cmath>

  using std::exp;
  using std::fabs;
  using std::pow;

#include<vector>

  using std::vector;

#include<string>

  using std::string;


#include "telmd_cwd.h"

/* *************************************************************
************************************************************* */

Telm45::Telm45()
{

  col = MISSING;
  row = MISSING;
  carea = -999;
  subarea = -999;
  fatalerr = 0;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Telm45::atmswritemiss( ofstream fout[NUMATMS],
                                const vector<string>& predname,
                                const int& pdyr,
                                const int& natmspred,
                                const double value,
                                const int& CYCLE )
{

  int i;
  int dm;
  Clmdat45 atmspred;

  for( i = 0; i < natmspred; ++i )
  {
    for( dm = 0; dm < CYCLE; ++dm )
    {
      atmspred.mon[dm] = value;
    }

	 atmspred.outdel( fout[i],
                          col,
                          row,
                          predname[i],
                          carea,
	                  atmstotyr[pdyr],
                          CYCLE,
                          atmspred.mon,
                          contnent );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Telm45::atmswritepred( ofstream fout[NUMATMS],
                            const int& pdyr,
                                const vector<string>& predname,
                                const int& natmspred,
                                const int& CYCLE )
{

  int i;
  int dm;
  Clmdat45 atmspred;

  // Covert cal/cm2/day to W/m2 (4.186 Joules / calorie)

  const double  cal2Watts = 0.4845;


  for( i = 0; i < natmspred; ++i )
  {
    if( predname.at( i ) == clm.predstr.at( clm.I_GIRR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_GIRR][dm][pdyr] * cal2Watts;
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_NIRR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_NIRR][dm][pdyr] * cal2Watts;
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_PAR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_PAR][dm][pdyr] * cal2Watts;
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_CLDS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_CLDS][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_TAIR][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIRD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_TAIRD][dm][pdyr];
      }
    }
    
    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_TAIRN][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_PREC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_PREC][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_RAIN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_RAIN][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_SNWFAL ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_SNWFAL][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_AOT40 ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_AOT40][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TRANGE ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_TRANGE][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_VPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_VPR][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_VPDD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_VPDD][dm][pdyr];
      }
    }
    
    else if( predname.at( i ) == clm.predstr.at( clm.I_VPDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	atmspred.mon[dm] = climate[clm.I_VPDN][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_DAYL ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_DAYL][dm][pdyr];
      }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_WS10 ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = climate[clm.I_WS10][dm][pdyr];
      }
    }
    
    else
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        atmspred.mon[dm] = MISSING;
      }
    }

    atmspred.outdel( fout[i],
                     col,
                     row,
                     predname[i],
                     carea,
                     year,
                     CYCLE,
                     atmspred.mon,
                     contnent );
  }

};

/* *************************************************************
************************************************************* */

void Telm45::atmswritepredm( ofstream fout[NUMATMS],
                            const int& pdyr,
                                const vector<string>& predname,
                                const int& natmspred,
                                const int& CYCLE )
{

  int i,l;
  int dm,month;
  int ndays[12];
  Clmdat45 atmspred;

  // Covert cal/cm2/day to W/m2 (4.186 Joules / calorie)

  const double  cal2Watts = 0.4845;


  if(CYCLE == 365)
  {
  ndays[0] = 31;
  ndays[1] = 28;
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
  }
  else
  {
  ndays[0] = 31;
  ndays[1] = 29;
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
  }


  for( i = 0; i < natmspred; ++i )
  {
    if( predname.at( i ) == clm.predstr.at( clm.I_GIRR ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] += climate[clm.I_GIRR][l][pdyr] * cal2Watts;
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_NIRR ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] += climate[clm.I_NIRR][l][pdyr] * cal2Watts;
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_PAR ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
//        cout << "PAR = " << dm << " " << month << " " << l << " " << climate[clm.I_PAR][l][pdyr] * cal2Watts  << endl;
        atmspred.mon[month] += climate[clm.I_PAR][l][pdyr] * cal2Watts;
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_CLDS ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] += climate[clm.I_CLDS][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIR ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_TAIR][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIRD ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_TAIRD][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }
    
    else if( predname.at( i ) == clm.predstr.at( clm.I_TAIRN ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_TAIRN][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_PREC ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_PREC][l][pdyr];
        l++;
      }
//        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_RAIN ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_RAIN][l][pdyr];
        l++;
      }
//        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_SNWFAL ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_SNWFAL][l][pdyr];
        l++;
      }
//        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_AOT40 ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_AOT40][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_TRANGE ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_TRANGE][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_VPR ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_VPR][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_VPDD ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_VPDD][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }
    
    else if( predname.at( i ) == clm.predstr.at( clm.I_VPDN ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
      	atmspred.mon[month] += climate[clm.I_VPDN][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_DAYL ) )
    {
     l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] += climate[clm.I_DAYL][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }

    else if( predname.at( i ) == clm.predstr.at( clm.I_WS10 ) )
    {
    l=0;
     for(month=0; month < 12; month ++)
     {
      atmspred.mon[month] = 0.0;
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] += climate[clm.I_WS10][l][pdyr];
        l++;
      }
        atmspred.mon[month] /= ndays[month];
     }
    }
    
    else
    {
      for( dm = 0; dm < ndays[month]; ++dm )
      {
        atmspred.mon[month] = MISSING;
      }
    }

    atmspred.outdelm( fout[i],
                     col,
                     row,
                     predname[i],
                     carea,
                     year,
                     12,
                     atmspred.mon,
                     contnent );
  }

};
/* *************************************************************
************************************************************* */

int Telm45::coregerr( ofstream& rflog1,
                          const string& varname1,
                          const float& col1,
                          const float& row1,
                          const string& varname2,
                          const float& col2,
                          const float& row2 )
{

  int fatalerr = 0;

  if( col1 != col2 || row1 != row2 )
  {
    fatalerr = 1;

    cout << "ERROR:  " << varname1 << " data and ";
    cout << varname2 << "data are not coregistered." << endl;
    cout << "COL = " << col1 << " and ROW = " << row1;
    cout << " in " << varname1 << " data" << endl;
    cout << "COL = " << col2 << " and ROW = " << row2;
    cout << " in " << varname2 << " data" << endl;

    rflog1 << "ERROR:  " << varname1 << " data and ";
    rflog1 << varname2 << "data are not coregistered." << endl;
    rflog1 << "COL = " << col1 << " and ROW = " << row1;
    rflog1 << " in " << varname1 << " data" << endl;
    rflog1 << "COL = " << col2 << " and ROW = " << row2;
    rflog1 << " in " << varname2 << " data" << endl;
  }

  return fatalerr;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

int Telm45::equilibrateTEM( const double& ptol,
                            const int& pichrt,
                            ofstream ftempred[NUMTEM],
                            const vector<string>& tempredmap,
                            const int& spinoutfg,
                            int& year,
                            const int& CYCLE,
                            const int ndays[] )
{
  // Run TEM until steady state conditions occur (equilibrium)

  int dyr = 0;
  int cdyear = 0;
  int dm;

  tem.totyr = 0;
  tem.endeq = 0;
  tem.intflag = 0;
  tem.initFlag = 0;

  while( (dyr < tem.runsize) && (tem.endeq < 2) )
  {
//    cout << "dyr = " << dyr << " " << tem.runsize << " " << tem.endeq << " " << tem.strteq << endl;
    if( de_startyear > clm.startyr)
    {
      cdyear = (de_startyear + 1 - clm.startyr) + (dyr%de_nyears);
    }
    tem.atms.setNDEP( adep[0][cdyear] );
    for( dm = 0; dm < CYCLE; ++dm )
    {
      // Assign telmnt[0].climate to TEM.atms

      tem.atms.setGIRR( climate[clm.I_GIRR][dm][cdyear] );
      tem.atms.setCLDS( climate[clm.I_CLDS][dm][cdyear] );
      tem.atms.setNIRR( climate[clm.I_NIRR][dm][cdyear] );
      tem.atms.setPAR(  climate[clm.I_PAR][dm][cdyear] );
      tem.atms.setTAIR( climate[clm.I_TAIR][dm][cdyear] );
      tem.atms.setTAIRD( climate[clm.I_TAIRD][dm][cdyear] );
      tem.atms.setTAIRN( climate[clm.I_TAIRN][dm][cdyear] );
      tem.atms.setRAIN( climate[clm.I_RAIN][dm][cdyear] );
      tem.atms.setPREC( climate[clm.I_PREC][dm][cdyear] );
      tem.atms.setSNOWFALL( climate[clm.I_SNWFAL][dm][cdyear] );
      tem.atms.setCO2( climate[clm.I_CO2][dm][0] );
      tem.atms.setAOT40( climate[clm.I_AOT40][dm][cdyear] );
      tem.atms.setVPR( climate[clm.I_VPR][dm][cdyear] );
      tem.atms.setVPDD( climate[clm.I_VPDD][dm][cdyear] );
      tem.atms.setVPDN( climate[clm.I_VPDN][dm][cdyear] );
      tem.atms.setWS10( climate[clm.I_WS10][dm][cdyear] );
      tem.atms.setTRANGE( climate[clm.I_TRANGE][dm][cdyear] );
      tem.atms.setDAYL( climate[clm.I_DAYL][dm][cdyear] );

//cout << "dayl_1 = " << tem.atms.getDAYL() << " " << dm << " " << cdyear << endl;
      #ifdef DEBUGT
        printf( "\nclimate passed to tem in equilibrateTEM, month = %d, cdyear = %d, girr = %6.2lf, clds = %6.2lf, nirr = %6.2lf, par = %5.2lf, prec = %8.21f, taird = %8.21f, co2 = %6.1lf, ws10 = %8.21f", 
                 dm, cdyear, climate[clm.I_GIRR][dm][cdyear], climate[clm.I_CLDS][dm][cdyear], climate[clm.I_NIRR][dm][cdyear], climate[clm.I_PAR][dm][cdyear], climate[clm.I_PREC][dm][cdyear], climate[clm.I_TAIRD][dm][cdyear], climate[clm.I_CO2][dm][0], climate[clm.I_WS10][dm][cdyear]);
        //cout << " equilibrating, simulation year = " << dyr << endl;
      #endif
      
      #ifdef DEBUGT
        printf( "\n before stepmonth, month = %d, dyr = %d, leafc = %5.2lf, soilc = %8.21f, soiln = %8.21f", 
                 dm, dyr, tem.getY(0), tem.getY(6), tem.getY(13));
        //cout << " equilibrating, simulation year = " << dyr << endl;
      #endif


      tem.endeq = tem.stepmonth( dyr,
                                 dm,
                                 tem.intflag,
                                 ptol,
                                 CYCLE,
                                 pichrt );
      
//     cout << "dyr = " << dyr << endl;
      #ifdef DEBUGT
        printf( "\n after stepmonth, month = %d, dyr = %d, rphi = %4.2lf, rtair = %4.2lf, vegc = %8.2lf, gpp = %5.21f, rh = %5.21f", 
                 dm, dyr, tem.veg.getRPHI(), tem.veg.getRTAIR(), tem.veg.getVEGC(), tem.veg.getGPP(), tem.microbe.getRH());
        //cout << " equilibrating, simulation year = " << dyr << endl;
      #endif


      // Save TEM output to telmnt[0].output

    if(intervalflag == 0)
      {
      outputTEMday( dm );
      }
    else if (intervalflag == 1)
      {
      outputTEMmonth( dm, CYCLE);
      } 
    } // end monthly for loop


    if(spinoutfg == 1)
    {
       if(intervalflag == 0)
       {
        temwritepred( ftempred,
                                tempredmap,
                                dyr,
                                pichrt,
                                ntempred,
                                spinoutfg,
                                0,
                                CYCLE,
                                ndays );
       }
       else if(intervalflag == 1 )
       {
        temwritepredm( ftempred,
                                tempredmap,
                                dyr,
                                pichrt,
                                ntempred,
                                spinoutfg,
                                0,
                                12,
                                ndays );
       }
    }


    vegceq[(dyr%de_nyears)] = tem.veg.getVEGC();
    soilceq[(dyr%de_nyears)] = tem.getY(tem.I_SOLC);
    vegneq[(dyr%de_nyears)] = tem.veg.getVEGN();
    soilneq[(dyr%de_nyears)] = tem.getY(tem.I_SOLN);

    ++dyr;
    ++tem.totyr;


// Check to see if steady state conditions have been reached.

    if( dyr >= tem.strteq && 0 == tem.endeq )
    {
//      cout << "equil = " << dyr << " " << de_nyears << " " << vegceq[(dyr%de_nyears)] << " " << soilceq[(dyr%de_nyears)] << " " << vegneq[(dyr%de_nyears)] << " " << soilneq[(dyr%de_nyears)] << endl;
      tem.endeq = tem.testEquilibrium(dyr,
                                     de_nyears,
                                     vegceq[(dyr%de_nyears)],
                                     soilceq[(dyr%de_nyears)],
                                     vegneq[(dyr%de_nyears)],
                                     soilneq[(dyr%de_nyears)] );
    }

  } // end while loop
  
  #ifdef DEBUGE
    cout << endl << " year, vegc, soilc, december prec at equilibrium " 
         << dyr-1 << " " << tem.veg.getVEGC() << " " << tem.soil.yrorgc << " " << climate[clm.I_PREC][11][cdyear] << endl;
  #endif

  if( tem.totyr >= tem.runsize && tem.endeq < 2 )
  {
    tem.nattempt += 1;
    tem.initFlag = 0;
  }
  else { tem.initFlag = 1; }
 
  year = dyr;

  return tem.nattempt;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Telm45::getTEMCohortState( const int& pichrt,
                                const int& CYCLE )
{
  int dm;
  int i;

  tem.veg.setPOTVEG( cohort[pichrt].potveg );

  tem.veg.setCURRENTVEG( cohort[pichrt].currentveg );

  tem.veg.setSUBTYPE( cohort[pichrt].subtype );

  tem.veg.cmnt = cohort[pichrt].cmnt;

//  cout << "cmnt from restart = " << tem.veg.getPOTVEG() << " " << tem.veg.getCURRENTVEG() << " " << tem.veg.getSUBTYPE() << " " << tem.veg.cmnt << endl;

  for( i = 0; i < MAXSTATE; ++i )
  {
    tem.setY( cohort[pichrt].y[i], i );
    tem.setPREVY( cohort[pichrt].prevy[i], i );
  }

  tem.ag.cmnt = cohort[pichrt].agcmnt;

  tem.ag.setGROWDD( cohort[pichrt].aggrowdd );

  tem.ag.setKD( cohort[pichrt].agkd );

  tem.ag.prvstate = cohort[pichrt].agprvstate;

  tem.ag.state = cohort[pichrt].agstate;

  tem.soil.setAVLH2O( cohort[pichrt].avlh2o );

  tem.soil.setAWCAPMM( cohort[pichrt].awcapmm );

  tem.veg.setC2N( cohort[pichrt].c2n );

  tem.veg.setCNEVEN( cohort[pichrt].cneven );

  tem.ag.setCONVRTFLXC( cohort[pichrt].convrtflx.carbon );
  tem.ag.setCONVRTFLXN( cohort[pichrt].convrtflx.nitrogen );
  tem.ag.setSTANDDEADC( cohort[pichrt].standdead.carbon );
  tem.ag.setSTANDDEADN( cohort[pichrt].standdead.nitrogen );
  tem.ag.setVOLAC( cohort[pichrt].vola.carbon );
  tem.ag.setVOLAN( cohort[pichrt].vola.nitrogen );

  tem.ag.setCROPRESIDUEC( cohort[pichrt].cropResidue.carbon );
  tem.ag.setCROPRESIDUEN( cohort[pichrt].cropResidue.nitrogen );

  tem.ag.setCROPTOPT( cohort[pichrt].croptopt );

  tem.distmnthcnt = cohort[pichrt].distmnthcnt;
  tem.disturbflag = cohort[pichrt].disturbflag;
  tem.disturbmonth = cohort[pichrt].disturbmonth;

  tem.ag.fertflag = cohort[pichrt].fertflag;

  tem.firemnthcnt = cohort[pichrt].firemnthcnt;

  tem.ag.setFORMPROD10C( cohort[pichrt].formPROD10.carbon );
  tem.ag.setFORMPROD10N( cohort[pichrt].formPROD10.nitrogen );

  tem.ag.setFORMPROD100C( cohort[pichrt].formPROD100.carbon );
  tem.ag.setFORMPROD100N( cohort[pichrt].formPROD100.nitrogen );

  tem.ag.setFRI( cohort[pichrt].FRI );

//  for( dm = 0; dm < CYCLE; ++dm )
  for( dm = 0; dm < 12; ++dm )
  {
    tem.ag.setINITPROD1C( cohort[pichrt].initPROD1[dm].carbon,
                          dm );

    tem.ag.setINITPROD1N( cohort[pichrt].initPROD1[dm].nitrogen,
                          dm );
  }

  for( i = 0; i < 10; ++i )
  {
    tem.ag.setINITPROD10C( cohort[pichrt].initPROD10[i].carbon, i );
    tem.ag.setINITPROD10N( cohort[pichrt].initPROD10[i].nitrogen, i );
  }

  for( i = 0; i < 100; ++i )
  {
    tem.ag.setINITPROD100C( cohort[pichrt].initPROD100[i].carbon, i );
    tem.ag.setINITPROD100N( cohort[pichrt].initPROD100[i].nitrogen, i );
  }

  tem.ag.irrgflag = cohort[pichrt].irrgflag;

  tem.microbe.setKD( cohort[pichrt].kd );

  tem.ag.setNATSEEDC( cohort[pichrt].natseedC );

  tem.ag.setNATSEEDSTRN( cohort[pichrt].natseedSTRN );

  tem.ag.setNATSEEDSTON( cohort[pichrt].natseedSTON );

  tem.ag.setNATSOIL( cohort[pichrt].natsoil );

  tem.ag.setNATTOPT( cohort[pichrt].nattopt );

  tem.veg.setNEWTOPT( cohort[pichrt].newtopt );

  tem.ag.setNRETENT( cohort[pichrt].nretent );

  tem.ag.setNSRETENT( cohort[pichrt].nsretent );

  tem.ag.setNVRETENT( cohort[pichrt].nvretent );

  tem.atms.setPREV2TAIR( cohort[pichrt].prev2tair );

  tem.atms.setPREVCO2( cohort[pichrt].prevco2 );

  tem.ag.setPREVCROPRESIDUEC( cohort[pichrt].prevCropResidue.carbon );
  tem.ag.setPREVCROPRESIDUEN( cohort[pichrt].prevCropResidue.nitrogen );

  tem.ag.setPREVPROD1C( cohort[pichrt].prevPROD1.carbon );
  tem.ag.setPREVPROD1N( cohort[pichrt].prevPROD1.nitrogen );

  tem.ag.setPREVPROD10C( cohort[pichrt].prevPROD10.carbon );
  tem.ag.setPREVPROD10N( cohort[pichrt].prevPROD10.nitrogen );

  tem.ag.setPREVPROD100C( cohort[pichrt].prevPROD100.carbon );
  tem.ag.setPREVPROD100N( cohort[pichrt].prevPROD100.nitrogen );

  tem.soil.setPREVSPACK( cohort[pichrt].prevspack );

  tem.atms.setPREVTAIR( cohort[pichrt].prevtair );

  tem.ag.setPROD10PAR( cohort[pichrt].prod10par );

  tem.ag.setPROD100PAR( cohort[pichrt].prod100par );

  tem.ag.setPRODUCTYEAR( cohort[pichrt].productYear );

  tem.ag.setPRVCROPNPP( cohort[pichrt].prvcropnpp );

  tem.veg.setRPREC( cohort[pichrt].rprec );
  
  tem.veg.setRDEMANDC( cohort[pichrt].rdemandc );
  
  tem.veg.setRDEMANDN( cohort[pichrt].rdemandn );

  tem.soil.setREET( cohort[pichrt].reet );

  tem.veg.setRLABILEC( cohort[pichrt].rlabilec );
  
  tem.veg.setRLABILEN( cohort[pichrt].rlabilen );
  
  tem.veg.setRGPP( cohort[pichrt].rgpp );
  
  tem.veg.setRINGPP( cohort[pichrt].ringpp );

  tem.veg.setRNPP( cohort[pichrt].rnpp );
  
  tem.veg.setRLTRC( cohort[pichrt].rltrc );

  tem.veg.setRPLEAF( cohort[pichrt].rpleaf );
  
  tem.mcrb.setRRH( cohort[pichrt].rrh );

  tem.veg.setRTAIR( cohort[pichrt].rtair );

  tem.veg.setRTAIRPHI( cohort[pichrt].rtairphi );

  tem.veg.setRPHI( cohort[pichrt].rphi );

  tem.veg.setPHICNT( cohort[pichrt].phicnt );

  tem.ag.setSCONVERT( cohort[pichrt].sconvert );

  tem.ag.setSCONVRTFLXC( cohort[pichrt].sconvrtflx.carbon );
  tem.ag.setSCONVRTFLXN( cohort[pichrt].sconvrtflx.nitrogen );

  tem.ag.setSLASHC( cohort[pichrt].slash.carbon );
  tem.ag.setSLASHN( cohort[pichrt].slash.nitrogen );

  tem.ag.setSLASHPAR( cohort[pichrt].slashpar );

  tem.ag.tillflag = cohort[pichrt].tillflag;

  tem.veg.setTOPT( cohort[pichrt].topt );

  tem.veg.setTOPTMIC( cohort[pichrt].toptmic );

  tem.ag.setVCONVERT( cohort[pichrt].vconvert );

  tem.ag.setVCONVRTFLXC( cohort[pichrt].vconvrtflx.carbon );
  tem.ag.setVCONVRTFLXN( cohort[pichrt].vconvrtflx.nitrogen );

  tem.ag.setVRESPAR( cohort[pichrt].vrespar );

//  tem.soil.setWILTPT( cohort[pichrt].wiltpt );

  tem.veg.yrltrc = cohort[pichrt].yrltrc;
  tem.veg.yrltrn = cohort[pichrt].yrltrn;
  
  tem.veg.yrpleaf = cohort[pichrt].yrpleaf;
  tem.veg.yrpsapwood = cohort[pichrt].yrpsapwood;
  tem.veg.yrproot = cohort[pichrt].yrproot;
  
  tem.veg.yralloclc = cohort[pichrt].yralloclc;
  tem.veg.yrallocsc = cohort[pichrt].yrallocsc;
  tem.veg.yrallocrc = cohort[pichrt].yrallocrc;
  
  tem.veg.yrrmleaf = cohort[pichrt].yrrmleaf;
  tem.veg.yrrmsapwood = cohort[pichrt].yrrmsapwood;
  tem.veg.yrrmroot = cohort[pichrt].yrrmroot;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Telm45::initializeCohortTEMState( const int& pichrt,
                                       const int& CYCLE )
{
  int dm;
  int i;

  for( i = 0; i < MAXSTATE; ++i )
  {
    cohort[pichrt].y[i] = MISSING;
    cohort[pichrt].prevy[i] = MISSING;
  }

  cohort[pichrt].aggrowdd = MISSING;

  cohort[pichrt].agkd = MISSING;

  cohort[pichrt].avlh2o = MISSING;

  cohort[pichrt].awcapmm = MISSING;

  cohort[pichrt].c2n = MISSING;

  cohort[pichrt].cneven = MISSING;

  cohort[pichrt].convrtflx.carbon = MISSING;
  cohort[pichrt].convrtflx.nitrogen = MISSING;
  cohort[pichrt].standdead.carbon = MISSING;
  cohort[pichrt].standdead.nitrogen = MISSING;
  cohort[pichrt].vola.carbon = MISSING;
  cohort[pichrt].vola.nitrogen = MISSING;

  cohort[pichrt].cropResidue.carbon = MISSING;
  cohort[pichrt].cropResidue.nitrogen = MISSING;

  cohort[pichrt].croptopt = MISSING;

  cohort[pichrt].distmnthcnt = -99;
  cohort[pichrt].disturbflag = -99;
  cohort[pichrt].disturbmonth = -99;

  cohort[pichrt].firemnthcnt = -99;

  cohort[pichrt].formPROD10.carbon = MISSING;
  cohort[pichrt].formPROD10.nitrogen = MISSING;

  cohort[pichrt].formPROD100.carbon = MISSING;
  cohort[pichrt].formPROD100.nitrogen = MISSING;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    cohort[pichrt].initPROD1[dm].carbon = MISSING;
    cohort[pichrt].initPROD1[dm].nitrogen = MISSING;
  }

  for( i = 0; i < 10; ++i )
  {
    cohort[pichrt].initPROD10[i].carbon = MISSING;
    cohort[pichrt].initPROD10[i].nitrogen = MISSING;
  }

  for( i = 0; i < 100; ++i )
  {
    cohort[pichrt].initPROD100[i].carbon = MISSING;
    cohort[pichrt].initPROD100[i].nitrogen = MISSING;
  }

  cohort[pichrt].kd = MISSING;

  cohort[pichrt].natseedC = MISSING;

  cohort[pichrt].natseedSTRN = MISSING;

  cohort[pichrt].natseedSTON = MISSING;

  cohort[pichrt].natsoil = MISSING;

  cohort[pichrt].nattopt = MISSING;

  cohort[pichrt].newtopt = MISSING;

  cohort[pichrt].nretent = MISSING;

  cohort[pichrt].nsretent = MISSING;

  cohort[pichrt].nvretent = MISSING;

  cohort[pichrt].prev2tair = MISSING;

  cohort[pichrt].prevco2 = MISSING;

  cohort[pichrt].prevCropResidue.carbon = MISSING;
  cohort[pichrt].prevCropResidue.nitrogen = MISSING;

  cohort[pichrt].prevPROD1.carbon = MISSING;
  cohort[pichrt].prevPROD1.nitrogen = MISSING;

  cohort[pichrt].prevPROD10.carbon = MISSING;
  cohort[pichrt].prevPROD10.nitrogen = MISSING;

  cohort[pichrt].prevPROD100.carbon = MISSING;
  cohort[pichrt].prevPROD100.nitrogen = MISSING;

  cohort[pichrt].prevspack = MISSING;

  cohort[pichrt].prevtair = MISSING;

  cohort[pichrt].productYear = -99;

  cohort[pichrt].prvcropnpp = MISSING;

  cohort[pichrt].rprec = MISSING;
  
  cohort[pichrt].rdemandc = MISSING;
  
  cohort[pichrt].rdemandn = MISSING;

  cohort[pichrt].reet = MISSING;

  cohort[pichrt].rlabilec = MISSING;
  
  cohort[pichrt].rlabilen = MISSING;

  cohort[pichrt].rgpp = MISSING;
  
  cohort[pichrt].ringpp = MISSING;
  
  cohort[pichrt].rnpp = MISSING;

  cohort[pichrt].rltrc = MISSING;

  cohort[pichrt].rpleaf = MISSING;

  cohort[pichrt].rrh = MISSING;

  cohort[pichrt].rtair = MISSING;

  cohort[pichrt].rtairphi = MISSING;

  cohort[pichrt].rphi = MISSING;

  cohort[pichrt].phicnt = MISSING;

  cohort[pichrt].qc = -99;

  cohort[pichrt].sconvrtflx.carbon = MISSING;
  cohort[pichrt].sconvrtflx.nitrogen = MISSING;

  cohort[pichrt].slash.carbon = MISSING;
  cohort[pichrt].slash.nitrogen = MISSING;

  cohort[pichrt].topt = MISSING;

  cohort[pichrt].tqc = -99;

  cohort[pichrt].vconvrtflx.carbon = MISSING;
  cohort[pichrt].vconvrtflx.nitrogen = MISSING;

//  cohort[pichrt].wiltpt = MISSING;

  cohort[pichrt].yrltrc = MISSING;
  cohort[pichrt].yrltrn = MISSING;
  
  cohort[pichrt].yrpleaf = MISSING;
  cohort[pichrt].yrpsapwood = MISSING;
  cohort[pichrt].yrproot = MISSING;
  
  cohort[pichrt].yralloclc = MISSING;
  cohort[pichrt].yrallocsc = MISSING;
  cohort[pichrt].yrallocrc = MISSING;
  
  cohort[pichrt].yrrmleaf = MISSING;
  cohort[pichrt].yrrmsapwood = MISSING;
  cohort[pichrt].yrrmroot = MISSING;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Telm45::outputTEMday( const int& pdm )
{

  // Ecosystem carbon pools determined in integrator

  output[tem.I_LEAFC][pdm] = tem.getY( tem.I_LEAFC );

  output[tem.I_SAPWOODC][pdm] = tem.getY( tem.I_SAPWOODC );

  output[tem.I_HEARTWOODC][pdm] = tem.getY( tem.I_HEARTWOODC );

  output[tem.I_ROOTC][pdm] = tem.getY( tem.I_ROOTC );

  output[tem.I_SEEDC][pdm] = tem.getY( tem.I_SEEDC );

  output[tem.I_LABILEC][pdm] = tem.getY( tem.I_LABILEC );

  output[tem.I_SOLC][pdm] = tem.getY( tem.I_SOLC );

  // Ecosystem nitrogen pools determined in integrator

  output[tem.I_LEAFN][pdm] = tem.getY( tem.I_LEAFN );

  output[tem.I_SAPWOODN][pdm] = tem.getY( tem.I_SAPWOODN );

  output[tem.I_HEARTWOODN][pdm] = tem.getY( tem.I_HEARTWOODN );

  output[tem.I_ROOTN][pdm] = tem.getY( tem.I_ROOTN );

  output[tem.I_SEEDN][pdm] = tem.getY( tem.I_SEEDN );

  output[tem.I_LABILEN][pdm] = tem.getY( tem.I_LABILEN );

  output[tem.I_SOLN][pdm] = tem.getY( tem.I_SOLN );

  output[tem.I_AVLN][pdm] = tem.getY( tem.I_AVLN );


  // Ecosystem water pools determined in integrator

  output[tem.I_AVLW][pdm] = tem.getY( tem.I_SM ) - tem.soil.getWILTPT();

  if( output[tem.I_AVLW][pdm] < ZERO ) {
    output[tem.I_AVLW][pdm] = 0.;
  }

  output[tem.I_SM][pdm] = tem.getY( tem.I_SM );

  output[tem.I_SMLL][pdm] = tem.getY( tem.I_SMLL );

  output[tem.I_VSM][pdm] = tem.getY( tem.I_VSM );

  output[tem.I_VSMLL][pdm] = tem.getY( tem.I_VSMLL );

  output[tem.I_PCTP][pdm] = tem.getY( tem.I_PCTP );

  output[tem.I_RGRW][pdm] = tem.getY( tem.I_RGRW );

  output[tem.I_SGRW][pdm] = tem.getY(tem.I_SGRW );

  // Monthly phenology determined in integrator

  output[tem.I_FPC][pdm] = tem.getY( tem.I_FPC );

  // Monthly carbon fluxes in ecosystems determined in integrator

  output[tem.I_INGPP][pdm] = tem.getY( tem.I_INGPP );

  output[tem.I_GPP][pdm] = tem.getY( tem.I_GPP );

  output[tem.I_FOZONE][pdm] = tem.getY( tem.I_FOZONE );
  output[tem.I_FRDL][pdm] = tem.getY( tem.I_FRDL );
  output[tem.I_FCO2][pdm] = tem.getY( tem.I_FCO2 );
  output[tem.I_TEMP][pdm] = tem.getY( tem.I_TEMP );
  output[tem.I_FH2O][pdm] = tem.getY( tem.I_FH2O );
  output[tem.I_FO3][pdm] = tem.getY( tem.I_FO3 );

  output[tem.I_INNPP][pdm] = tem.getY( tem.I_INNPP );

  output[tem.I_NPP][pdm] = tem.getY( tem.I_NPP );

  output[tem.I_GPR][pdm] = tem.getY( tem.I_GPR );

  output[tem.I_ALLOCLC][pdm] = tem.getY( tem.I_ALLOCLC );

  output[tem.I_ALLOCSC][pdm] = tem.getY( tem.I_ALLOCSC );

  output[tem.I_ALLOCHC][pdm] = tem.getY( tem.I_ALLOCHC );

  output[tem.I_ALLOCRC][pdm] = tem.getY( tem.I_ALLOCRC );

//  cout << "outputdaily = " << tem.getY( tem.I_ALLOCSC ) << endl;

  output[tem.I_ALLOCSEEDC][pdm] = tem.getY( tem.I_ALLOCSEEDC );

  output[tem.I_RVMNT][pdm] = tem.getY( tem.I_RVMNT );

  output[tem.I_RMLEAF][pdm] = tem.getY( tem.I_RMLEAF );

  output[tem.I_RMSAPWOOD][pdm] = tem.getY( tem.I_RMSAPWOOD );

  output[tem.I_RMROOT][pdm] = tem.getY( tem.I_RMROOT );

  output[tem.I_RMSEED][pdm] = tem.getY( tem.I_RMSEED );

  output[tem.I_RMLABILE][pdm] = tem.getY( tem.I_RMLABILE );

  output[tem.I_RVGRW][pdm] = tem.getY( tem.I_RVGRW );

  output[tem.I_LTRLC][pdm] = tem.getY( tem.I_LTRLC );

  output[tem.I_LTRSC][pdm] = tem.getY( tem.I_LTRSC );

  output[tem.I_LTRHC][pdm] = tem.getY( tem.I_LTRHC );

  output[tem.I_LTRRC][pdm] = tem.getY( tem.I_LTRRC );

  output[tem.I_LTRSEEDC][pdm] = tem.getY( tem.I_LTRSEEDC );

  output[tem.I_RH][pdm] = tem.getY( tem.I_RH );

  output[tem.I_DOC][pdm] = tem.getY( tem.I_DOC );

  output[tem.I_DON][pdm] = tem.getY( tem.I_DON );


  // Monthly nitrogen fluxes in ecosystems determined in
  //   integrator

  output[tem.I_NINP][pdm] = tem.getY( tem.I_NINP );

  output[tem.I_AGFRTN][pdm] = tem.getY( tem.I_AGFRTN );

  output[tem.I_INNUP][pdm] = tem.getY( tem.I_INNUP );

  output[tem.I_VNUP][pdm] = tem.getY( tem.I_VNUP );

  output[tem.I_ALLOCLN][pdm] = tem.getY( tem.I_ALLOCLN );

  output[tem.I_ALLOCSN][pdm] = tem.getY( tem.I_ALLOCSN );

  output[tem.I_ALLOCHN][pdm] = tem.getY( tem.I_ALLOCHN );

  output[tem.I_ALLOCRN][pdm] = tem.getY( tem.I_ALLOCRN );

  output[tem.I_ALLOCSEEDN][pdm] = tem.getY( tem.I_ALLOCSEEDN );

  output[tem.I_LTRLN][pdm] = tem.getY( tem.I_LTRLN );

  output[tem.I_LTRSN][pdm] = tem.getY( tem.I_LTRSN );

  output[tem.I_LTRHN][pdm] = tem.getY( tem.I_LTRHN );

  output[tem.I_LTRRN][pdm] = tem.getY( tem.I_LTRRN );

  output[tem.I_LTRSEEDN][pdm] = tem.getY( tem.I_LTRSEEDN );

  output[tem.I_MNUP][pdm] = tem.getY( tem.I_MNUP );

  output[tem.I_NMIN][pdm] = tem.getY( tem.I_NMIN );

  output[tem.I_NLST][pdm] = tem.getY( tem.I_NLST );

  output[tem.I_LCHDON][pdm] = tem.getY( tem.I_LCHDON );

  output[tem.I_LCHDIN][pdm] = tem.getY( tem.I_LCHDIN );

  output[tem.I_LCHDOC][pdm] = tem.getY( tem.I_LCHDOC );

  output[tem.I_DONPROD][pdm] = tem.getY( tem.I_DONPROD );

  output[tem.I_DOCPROD][pdm] = tem.getY( tem.I_DOCPROD );

  output[tem.I_NFIXN][pdm] = tem.getY( tem.I_NFIXN );

  output[tem.I_NFIXS][pdm] = tem.getY( tem.I_NFIXS );

  output[tem.I_NRESORBL][pdm] = tem.getY( tem.I_NRESORBL );

  output[tem.I_NRESORBS][pdm] = tem.getY( tem.I_NRESORBS );

  output[tem.I_NRESORBR][pdm] = tem.getY( tem.I_NRESORBR );

  output[tem.I_NRESORBSEED][pdm] = tem.getY( tem.I_NRESORBSEED );



  // Monthly water fluxes in ecosystems

  output[tem.I_AGIRRIG][pdm] = tem.getY( tem.I_AGIRRIG );

  output[tem.I_INEET][pdm] = tem.getY( tem.I_INEET );

  output[tem.I_INEETLL][pdm] = tem.getY( tem.I_INEETLL );

  output[tem.I_EET][pdm] = tem.getY( tem.I_EET );

  output[tem.I_EETLL][pdm] = tem.getY( tem.I_EETLL );

  output[tem.I_RPERC][pdm] = tem.getY( tem.I_RPERC );

  output[tem.I_SPERC][pdm] = tem.getY( tem.I_SPERC );

  output[tem.I_RRUN][pdm] = tem.getY( tem.I_RRUN );

  output[tem.I_RUNOFFB][pdm] = tem.getY( tem.I_RUNOFFB );

  output[tem.I_SRUN][pdm] = tem.getY( tem.I_SRUN );

  output[tem.I_RUNOFFB][pdm] = tem.getY( tem.I_RUNOFFB );

  output[tem.I_GC][pdm] = tem.getY( tem.I_GC );

  output[tem.I_GS][pdm] = tem.getY( tem.I_GS );

  output[tem.I_PECAN][pdm] = tem.veg.getPECANW();

  output[tem.I_PESOIL][pdm] = tem.veg.getPESOILW();


  // Other ecosystem carbon pools

  output[tem.I_LAI][pdm] = tem.veg.getLAI();

  output[tem.I_TOTEC][pdm] = tem.ag.getTOTEC();

  output[tem.I_TOTC][pdm] = tem.veg.getVEGC();

  // Other ecosystem nitrogen pools

  output[tem.I_VEGN][pdm] = tem.veg.getVEGN();


  // Other ecosystem water pools

  output[tem.I_SNWPCK][pdm] = tem.soil.getSNOWPACK();

  // Other monthly carbon fluxes in ecosystems

  output[tem.I_NEP][pdm] = tem.getNEP();

  output[tem.I_NCE][pdm] = tem.getNCE();


  // Other monthly water fluxes in ecosystems

  output[tem.I_PET][pdm] = tem.veg.getPET();

  output[tem.I_SNWINF][pdm] = tem.soil.getSNOWINF();

  output[tem.I_WYLD][pdm] = tem.soil.getH2OYLD();



  // Carbon in Human product pools

  output[tem.I_AGPRDC][pdm] = tem.ag.getPROD1C();

  output[tem.I_CLIPPINGS][pdm] = tem.ag.getCLIPPINGS();

  output[tem.I_PROD10C][pdm] = tem.ag.getPROD10C();

  output[tem.I_PROD100C][pdm] = tem.ag.getPROD100C();

  output[tem.I_TOTPRDC][pdm] = tem.ag.getTOTPRODC();

  // Carbon in crop residue pool

  output[tem.I_RESIDC][pdm] = tem.ag.getCROPRESIDUEC();

  output[tem.I_AGSTUBC][pdm] = tem.ag.getSTUBBLEC();

  // Nitrogen in Human product pools

  output[tem.I_AGPRDN][pdm] = tem.ag.getPROD1N();

  output[tem.I_PROD10N][pdm] = tem.ag.getPROD10N();

  output[tem.I_PROD100N][pdm] = tem.ag.getPROD100N();

  output[tem.I_TOTPRDN][pdm] = tem.ag.getTOTPRODN();

  // Nitrogen in crop residue pool

  output[tem.I_RESIDN][pdm] = tem.ag.getCROPRESIDUEN();

  output[tem.I_AGSTUBN][pdm] = tem.ag.getSTUBBLEN();

  // Monthly carbon fluxes associated with
  //  agricultural conversion

  output[tem.I_CNVRTC][pdm] = tem.ag.getCONVRTFLXC();

  output[tem.I_VCNVRTC][pdm] = tem.ag.getVCONVRTFLXC();

  output[tem.I_SCNVRTC][pdm] = tem.ag.getSCONVRTFLXC();

  output[tem.I_SLASHC][pdm] = tem.ag.getSLASHC();

  output[tem.I_STANDDEADC][pdm] = tem.ag.getSTANDDEADC();

  output[tem.I_VOLAC][pdm] = tem.ag.getVOLAC();

  output[tem.I_CFLX][pdm] = tem.ag.getCFLUX();

  // Monthly nitrogen fluxes associated with
  //  agricultural conversion

  output[tem.I_CNVRTN][pdm] = tem.ag.getCONVRTFLXN();

  output[tem.I_VCNVRTN][pdm] = tem.ag.getVCONVRTFLXN();

  output[tem.I_SCNVRTN][pdm] = tem.ag.getSCONVRTFLXN();

  output[tem.I_SLASHN][pdm] = tem.ag.getSLASHN();

  output[tem.I_STANDDEADN][pdm] = tem.ag.getSTANDDEADN();

  output[tem.I_VOLAN][pdm] = tem.ag.getVOLAN();

  output[tem.I_NRETNT][pdm] = tem.ag.getNRETENT();

  output[tem.I_NVRTNT][pdm] = tem.ag.getNVRETENT();

  output[tem.I_NSRTNT][pdm] = tem.ag.getNSRETENT();

  // Monthly carbon and nitrogen fluxes from agricultural
  //   ecosystems

  output[tem.I_AGFPRDC][pdm] = tem.ag.getCROPPRODC();
  output[tem.I_AGFPRDN][pdm] = tem.ag.getCROPPRODN();

  output[tem.I_FRESIDC][pdm] = tem.ag.getFORMCROPRESIDUEC();
  output[tem.I_FRESIDN][pdm] = tem.ag.getFORMCROPRESIDUEN();

  output[tem.I_AGPRDFC][pdm] = tem.ag.getPROD1DECAYC();
  output[tem.I_AGPRDFN][pdm] = tem.ag.getPROD1DECAYN();

  output[tem.I_RESIDFC][pdm] = tem.ag.getCROPRESIDUEFLXC();
  output[tem.I_RESIDFN][pdm] = tem.ag.getCROPRESIDUEFLXN();


  // Monthly carbon and nitrogen fluxes from products

  output[tem.I_PRDF10C][pdm] = tem.ag.getFORMPROD10C();
  output[tem.I_PRDF10N][pdm] = tem.ag.getFORMPROD10N();

  output[tem.I_PRD10FC][pdm] = tem.ag.getPROD10DECAYC();
  output[tem.I_PRD10FN][pdm] = tem.ag.getPROD10DECAYN();

  output[tem.I_PRDF100C][pdm] = tem.ag.getFORMPROD100C();
  output[tem.I_PRDF100N][pdm] = tem.ag.getFORMPROD100N();

  output[tem.I_PRD100FC][pdm] = tem.ag.getPROD100DECAYC();
  output[tem.I_PRD100FN][pdm] = tem.ag.getPROD100DECAYN();

  output[tem.I_TOTFPRDC][pdm] = tem.ag.getFORMTOTPRODC();
  output[tem.I_TOTFPRDN][pdm] = tem.ag.getFORMTOTPRODN();

  output[tem.I_TOTPRDFC][pdm] = tem.ag.getTOTPRODDECAYC();
  output[tem.I_TOTPRDFN][pdm] = tem.ag.getTOTPRODDECAYN();

  //  Output agricultural area-specific vs natural area-specific
  //    results

  if( 0 == tem.ag.state )
  {
    output[tem.I_CROPC][pdm] = ZERO;
    output[tem.I_NATVEGC][pdm] = tem.getY( tem.I_LEAFC ) + tem.getY( tem.I_SAPWOODC )
		+ tem.getY( tem.I_HEARTWOODC ) + tem.getY( tem.I_ROOTC ) + tem.getY( tem.I_LABILEC );

    output[tem.I_CROPN][pdm] = ZERO;
    output[tem.I_NATVEGN][pdm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN ) + tem.getY( tem.I_LABILEN );

    output[tem.I_CSTRN][pdm] = ZERO;
    output[tem.I_NATSTRN][pdm] = ZERO;

    output[tem.I_CSTON][pdm] = ZERO;
    output[tem.I_NATSTON][pdm] = tem.getY( tem.I_LABILEN );

    output[tem.I_CROPLAI][pdm] = ZERO;
    output[tem.I_NATLAI][pdm] = tem.veg.getLAI();

    output[tem.I_CROPFPC][pdm] = ZERO;
    output[tem.I_NATFPC][pdm] = tem.getY( tem.I_FPC );

    output[tem.I_AGINGPP][pdm] = ZERO;
    output[tem.I_NATINGPP][pdm] = tem.getY( tem.I_INGPP );

    output[tem.I_AGGPP][pdm] = ZERO;
    output[tem.I_NATGPP][pdm] = tem.getY( tem.I_GPP );

    output[tem.I_AGINNPP][pdm] = ZERO;
    output[tem.I_NATINNPP][pdm] = tem.getY( tem.I_INNPP );

    output[tem.I_AGNPP][pdm] = ZERO;
    output[tem.I_NATNPP][pdm] = tem.getY( tem.I_NPP );

    output[tem.I_AGGPR][pdm] = ZERO;
    output[tem.I_NATGPR][pdm] = tem.getY( tem.I_GPR );

    output[tem.I_AGRVMNT][pdm] = ZERO;
    output[tem.I_NATRVMNT][pdm] = tem.getY( tem.I_RVMNT );

    output[tem.I_AGRVGRW][pdm] = ZERO;
    output[tem.I_NATRVGRW][pdm] = tem.getY( tem.I_RVGRW );

    output[tem.I_AGLTRC][pdm] = ZERO;
    output[tem.I_NATLTRC][pdm] = tem.getY( tem.I_RVGRW );

    output[tem.I_AGINNUP][pdm] = ZERO;
    output[tem.I_NATINNUP][pdm] = tem.getY( tem.I_INNUP );

    output[tem.I_AGVNUP][pdm] = ZERO;
    output[tem.I_NATVNUP][pdm] = tem.getY( tem.I_VNUP );

    output[tem.I_AGLTRN][pdm] = ZERO;
    output[tem.I_NATLTRN][pdm] = tem.getY( tem.I_LTRLN ) + tem.getY( tem.I_LTRSN )
		+ tem.getY( tem.I_LTRHN ) + tem.getY( tem.I_LTRRN );
  }
  else
  {
    output[tem.I_CROPC][pdm] = tem.getY( tem.I_LEAFC ) + tem.getY( tem.I_SAPWOODC )
		+ tem.getY( tem.I_HEARTWOODC ) + tem.getY( tem.I_ROOTC ) + tem.getY( tem.I_LABILEC );
    output[tem.I_NATVEGC][pdm] = ZERO;

    output[tem.I_CROPN][pdm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN ) + tem.getY( tem.I_LABILEN );
    output[tem.I_NATVEGN][pdm] = ZERO;

    output[tem.I_CSTRN][pdm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN );
    output[tem.I_NATSTRN][pdm] = ZERO;

    output[tem.I_CSTON][pdm] = tem.getY( tem.I_LABILEN );
    output[tem.I_NATSTON][pdm] = ZERO;

    output[tem.I_CROPLAI][pdm] = tem.veg.getLAI();
    output[tem.I_NATLAI][pdm] = ZERO;

    output[tem.I_CROPFPC][pdm] = tem.getY( tem.I_FPC );
    output[tem.I_NATFPC][pdm] = ZERO;

    output[tem.I_AGINGPP][pdm] = tem.getY( tem.I_INGPP );
    output[tem.I_NATINGPP][pdm] = ZERO;

    output[tem.I_AGGPP][pdm] = tem.getY( tem.I_GPP );
    output[tem.I_NATGPP][pdm] = ZERO;

    output[tem.I_AGINNPP][pdm] = tem.getY( tem.I_INNPP );
    output[tem.I_NATINNPP][pdm] = ZERO;

    output[tem.I_AGNPP][pdm] = tem.getY( tem.I_NPP );
    output[tem.I_NATNPP][pdm] = ZERO;

    output[tem.I_AGGPR][pdm] = tem.getY( tem.I_GPR );
    output[tem.I_NATGPR][pdm] = ZERO;

    output[tem.I_AGRVMNT][pdm] = tem.getY( tem.I_RVMNT );
    output[tem.I_NATRVMNT][pdm] = ZERO;

    output[tem.I_AGRVGRW][pdm] = tem.getY( tem.I_RVGRW );
    output[tem.I_NATRVGRW][pdm] = ZERO;

    output[tem.I_AGLTRC][pdm] = tem.getY( tem.I_LTRLC ) + tem.getY( tem.I_LTRSC )
		+ tem.getY( tem.I_LTRHC ) + tem.getY( tem.I_LTRRC );
    output[tem.I_NATLTRC][pdm] = ZERO;

    output[tem.I_AGINNUP][pdm] = tem.getY( tem.I_INNUP );
    output[tem.I_NATINNUP][pdm] = ZERO;

    output[tem.I_AGVNUP][pdm] = tem.getY( tem.I_VNUP );
    output[tem.I_NATVNUP][pdm] = ZERO;


    output[tem.I_AGLTRN][pdm] = tem.getY( tem.I_LTRLN ) + tem.getY( tem.I_LTRSN )
		+ tem.getY( tem.I_LTRHN ) + tem.getY( tem.I_LTRRN );
    output[tem.I_NATLTRN][pdm] = ZERO;
  }

};

/* *************************************************************
************************************************************* */


void Telm45::outputTEMmonth( const int& pdm,
                             const int& CYCLE )
{

  int ndays[12],i,dm;

  if(CYCLE == 365)
  {
  ndays[0] = 0;
  ndays[1] = 31;
  ndays[2] = 59;
  ndays[3] = 90;
  ndays[4] = 120;
  ndays[5] = 151;
  ndays[6] = 181;
  ndays[7] = 212;
  ndays[8] = 243;
  ndays[9] = 273;
  ndays[10] = 304;
  ndays[11] = 334;
  }
  else
  {
  ndays[0] = 0;
  ndays[1] = 31;
  ndays[2] = 60;
  ndays[3] = 91;
  ndays[4] = 121;
  ndays[5] = 152;
  ndays[6] = 182;
  ndays[7] = 213;
  ndays[8] = 244;
  ndays[9] = 274;
  ndays[10] = 305;
  ndays[11] = 335;
  }

    for(i = 0; i < NUMTEM; ++i )
    {
     for( dm = 0; dm < 12; ++dm )
      {
        if(pdm == ndays[dm])
        {
        outputm[i][dm] = ZERO;
        }
      }

    }

/*    for(i = 0; i < 5; ++i )
    {
     for( dm = 0; dm < 12; ++dm )
      {
        if(pdm == ndays[dm])
        {
        outpute[i][dm] = ZERO;
        }
      }

    } */

        if(pdm < ndays[1])
        {
         dm = 0; 
        }
        if(pdm >= ndays[1] && pdm < ndays[2])
        {
         dm = 1;
        }
        if(pdm >= ndays[2] && pdm < ndays[3])
        {
         dm = 2;
        }
        if(pdm >= ndays[3] && pdm < ndays[4])
        {
         dm = 3;
        }
        if(pdm >= ndays[4] && pdm < ndays[5])
        {
         dm = 4;
        }
        if(pdm >= ndays[5] && pdm < ndays[6])
        {
         dm = 5;
        }
        if(pdm >= ndays[6] && pdm < ndays[7])
        {
         dm = 6;
        }
        if(pdm >= ndays[7] && pdm < ndays[8])
        {
         dm = 7;
        }
        if(pdm >= ndays[8] && pdm < ndays[9])
        {
         dm = 8;
        }
        if(pdm >= ndays[9] && pdm < ndays[10])
        {
         dm = 9;
        }
        if(pdm >= ndays[10] && pdm < ndays[11])
        {
         dm = 10;
        }
        if(pdm >= ndays[11])
        {
         dm = 11;
        }




  // Ecosystem carbon pools determined in integrator

  outputm[tem.I_LEAFC][dm] = tem.getY( tem.I_LEAFC );

  outputm[tem.I_SAPWOODC][dm] = tem.getY( tem.I_SAPWOODC );

  outputm[tem.I_HEARTWOODC][dm] = tem.getY( tem.I_HEARTWOODC );

  outputm[tem.I_ROOTC][dm] = tem.getY( tem.I_ROOTC );

  outputm[tem.I_SEEDC][dm] = tem.getY( tem.I_SEEDC );

  outputm[tem.I_LABILEC][dm] = tem.getY( tem.I_LABILEC );

  outputm[tem.I_SOLC][dm] = tem.getY( tem.I_SOLC );

  // Ecosystem nitrogen pools determined in integrator

  outputm[tem.I_LEAFN][dm] = tem.getY( tem.I_LEAFN );

  outputm[tem.I_SAPWOODN][dm] = tem.getY( tem.I_SAPWOODN );

  outputm[tem.I_HEARTWOODN][dm] = tem.getY( tem.I_HEARTWOODN );

  outputm[tem.I_ROOTN][dm] = tem.getY( tem.I_ROOTN );

  outputm[tem.I_SEEDN][dm] = tem.getY( tem.I_SEEDN );

  outputm[tem.I_LABILEN][dm] = tem.getY( tem.I_LABILEN );

  outputm[tem.I_SOLN][dm] = tem.getY( tem.I_SOLN );

  outputm[tem.I_AVLN][dm] = tem.getY( tem.I_AVLN );


  // Ecosystem water pools determined in integrator

  outputm[tem.I_AVLW][dm] = tem.getY( tem.I_SM ) - tem.soil.getWILTPT();

//  cout << "output availw = " << outputm[tem.I_AVLW][dm] << endl;
  if( outputm[tem.I_AVLW][dm] < ZERO ) {
    outputm[tem.I_AVLW][dm] = 0.;
  }

  outputm[tem.I_SM][dm] = tem.getY( tem.I_SM );

  outputm[tem.I_SMLL][dm] = tem.getY( tem.I_SMLL );

  outputm[tem.I_VSM][dm] = tem.getY( tem.I_VSM );

  outputm[tem.I_VSMLL][dm] = tem.getY( tem.I_VSMLL );

  outputm[tem.I_PCTP][dm] = tem.getY( tem.I_PCTP );

  outputm[tem.I_RGRW][dm] = tem.getY( tem.I_RGRW );

  outputm[tem.I_SGRW][dm] = tem.getY(tem.I_SGRW );

  // Monthly phenology determined in integrator

  outputm[tem.I_FPC][dm] = tem.getY( tem.I_FPC );

  // Monthly carbon fluxes in ecosystems determined in integrator

  outputm[tem.I_INGPP][dm] += tem.getY( tem.I_INGPP );

  outputm[tem.I_GPP][dm] += tem.getY( tem.I_GPP );

  outputm[tem.I_FOZONE][dm] += tem.getY( tem.I_FOZONE );
  outputm[tem.I_FRDL][dm] += tem.getY( tem.I_FRDL );
  outputm[tem.I_FCO2][dm] += tem.getY( tem.I_FCO2 );
  outputm[tem.I_TEMP][dm] += tem.getY( tem.I_TEMP );
  outputm[tem.I_FH2O][dm] += tem.getY( tem.I_FH2O );
  outputm[tem.I_FO3][dm] += tem.getY( tem.I_FO3 );

  outputm[tem.I_INNPP][dm] += tem.getY( tem.I_INNPP );

  outputm[tem.I_NPP][dm] += tem.getY( tem.I_NPP );

  outputm[tem.I_GPR][dm] += tem.getY( tem.I_GPR );

  outputm[tem.I_ALLOCLC][dm] += tem.getY( tem.I_ALLOCLC );

  outputm[tem.I_ALLOCSC][dm] += tem.getY( tem.I_ALLOCSC );

  outputm[tem.I_ALLOCHC][dm] += tem.getY( tem.I_ALLOCHC );

  outputm[tem.I_ALLOCRC][dm] += tem.getY( tem.I_ALLOCRC );

// cout << "outmonth allocrc = " << tem.getY( tem.I_ALLOCRC ) << " " << outputm[tem.I_ALLOCRC][dm] << endl;

  outputm[tem.I_ALLOCSEEDC][dm] += tem.getY( tem.I_ALLOCSEEDC );

  outputm[tem.I_RVMNT][dm] += tem.getY( tem.I_RVMNT );

  outputm[tem.I_RMLEAF][dm] += tem.getY( tem.I_RMLEAF );

  outputm[tem.I_RMSAPWOOD][dm] += tem.getY( tem.I_RMSAPWOOD );

  outputm[tem.I_RMROOT][dm] += tem.getY( tem.I_RMROOT );

  outputm[tem.I_RMSEED][dm] += tem.getY( tem.I_RMSEED );

  outputm[tem.I_RMLABILE][dm] += tem.getY( tem.I_RMLABILE );

  outputm[tem.I_RVGRW][dm] += tem.getY( tem.I_RVGRW );

  outputm[tem.I_LTRLC][dm] += tem.getY( tem.I_LTRLC );

  outputm[tem.I_LTRSC][dm] += tem.getY( tem.I_LTRSC );

  outputm[tem.I_LTRHC][dm] += tem.getY( tem.I_LTRHC );

  outputm[tem.I_LTRRC][dm] += tem.getY( tem.I_LTRRC );

  outputm[tem.I_LTRSEEDC][dm] += tem.getY( tem.I_LTRSEEDC );

  outputm[tem.I_RH][dm] += tem.getY( tem.I_RH );

  outputm[tem.I_DOC][dm] += tem.getY( tem.I_DOC );

  outputm[tem.I_DON][dm] += tem.getY( tem.I_DON );


  // Monthly nitrogen fluxes in ecosystems determined in
  //   integrator

  outputm[tem.I_NINP][dm] += tem.getY( tem.I_NINP );

  outputm[tem.I_AGFRTN][dm] += tem.getY( tem.I_AGFRTN );

  outputm[tem.I_INNUP][dm] += tem.getY( tem.I_INNUP );

  outputm[tem.I_VNUP][dm] += tem.getY( tem.I_VNUP );

  outputm[tem.I_ALLOCLN][dm] += tem.getY( tem.I_ALLOCLN );

  outputm[tem.I_ALLOCSN][dm] += tem.getY( tem.I_ALLOCSN );

  outputm[tem.I_ALLOCHN][dm] += tem.getY( tem.I_ALLOCHN );

  outputm[tem.I_ALLOCRN][dm] += tem.getY( tem.I_ALLOCRN );

  outputm[tem.I_ALLOCSEEDN][dm] += tem.getY( tem.I_ALLOCSEEDN );

  outputm[tem.I_LTRLN][dm] += tem.getY( tem.I_LTRLN );

  outputm[tem.I_LTRSN][dm] += tem.getY( tem.I_LTRSN );

  outputm[tem.I_LTRHN][dm] += tem.getY( tem.I_LTRHN );

  outputm[tem.I_LTRRN][dm] += tem.getY( tem.I_LTRRN );

  outputm[tem.I_LTRSEEDN][dm] += tem.getY( tem.I_LTRSEEDN );

  outputm[tem.I_MNUP][dm] += tem.getY( tem.I_MNUP );

  outputm[tem.I_NMIN][dm] += tem.getY( tem.I_NMIN );

  outputm[tem.I_NLST][dm] += tem.getY( tem.I_NLST );

  outputm[tem.I_LCHDON][dm] += tem.getY( tem.I_LCHDON );

  outputm[tem.I_LCHDIN][dm] += tem.getY( tem.I_LCHDIN );

  outputm[tem.I_LCHDOC][dm] += tem.getY( tem.I_LCHDOC );

  outputm[tem.I_DONPROD][dm] += tem.getY( tem.I_DONPROD );

  outputm[tem.I_DOCPROD][dm] += tem.getY( tem.I_DOCPROD );

  outputm[tem.I_NFIXN][dm] += tem.getY( tem.I_NFIXN );

  outputm[tem.I_NFIXS][dm] += tem.getY( tem.I_NFIXS );

  outputm[tem.I_NRESORBL][dm] += tem.getY( tem.I_NRESORBL );

  outputm[tem.I_NRESORBS][dm] += tem.getY( tem.I_NRESORBS );

  outputm[tem.I_NRESORBR][dm] += tem.getY( tem.I_NRESORBR );

  outputm[tem.I_NRESORBSEED][dm] += tem.getY( tem.I_NRESORBSEED );



  // Monthly water fluxes in ecosystems

  outputm[tem.I_AGIRRIG][dm] += tem.getY( tem.I_AGIRRIG );

  outputm[tem.I_INEET][dm] += tem.getY( tem.I_INEET );

  outputm[tem.I_INEETLL][dm] += tem.getY( tem.I_INEETLL );

  outputm[tem.I_EET][dm] += tem.getY( tem.I_EET );

  outputm[tem.I_EETLL][dm] += tem.getY( tem.I_EETLL );

  outputm[tem.I_RPERC][dm] += tem.getY( tem.I_RPERC );

  outputm[tem.I_SPERC][dm] += tem.getY( tem.I_SPERC );

  outputm[tem.I_RUNOFFS][dm] += tem.getY( tem.I_RUNOFFS );

  outputm[tem.I_RUNOFFB][dm] += tem.getY( tem.I_RUNOFFB );

  outputm[tem.I_SRUN][dm] += tem.getY( tem.I_SRUN );

  outputm[tem.I_RRUN][dm] += tem.getY( tem.I_RRUN );

  outputm[tem.I_GC][dm] += tem.getY( tem.I_GC );

  outputm[tem.I_GS][dm] += tem.getY( tem.I_GS );

  outputm[tem.I_PECAN][dm] += tem.veg.getPECANW();

  outputm[tem.I_PESOIL][dm] += tem.veg.getPESOILW();


  // Other ecosystem carbon pools

  outputm[tem.I_LAI][dm] = tem.veg.getLAI();

  outputm[tem.I_TOTEC][dm] = tem.ag.getTOTEC();

  outputm[tem.I_TOTC][dm] = tem.veg.getVEGC();

  // Other ecosystem nitrogen pools

  outputm[tem.I_VEGN][dm] = tem.veg.getVEGN();


  // Other ecosystem water pools

  outputm[tem.I_SNWPCK][dm] = tem.soil.getSNOWPACK();

  // Other monthly carbon fluxes in ecosystems

  outputm[tem.I_NEP][dm] += tem.getNEP();

  outputm[tem.I_NCE][dm] += tem.getNCE();


  // Other monthly water fluxes in ecosystems

  outputm[tem.I_PET][dm] += tem.veg.getPET();

  outputm[tem.I_SNWINF][dm] += tem.soil.getSNOWINF();

  outputm[tem.I_WYLD][dm] += tem.soil.getH2OYLD();



  // Carbon in Human product pools

  outputm[tem.I_AGPRDC][dm] = tem.ag.getPROD1C();

  outputm[tem.I_CLIPPINGS][dm] = tem.ag.getCLIPPINGS();

  outputm[tem.I_PROD10C][dm] = tem.ag.getPROD10C();

  outputm[tem.I_PROD100C][dm] = tem.ag.getPROD100C();

  outputm[tem.I_TOTPRDC][dm] = tem.ag.getTOTPRODC();

  // Carbon in crop residue pool

  outputm[tem.I_RESIDC][dm] = tem.ag.getCROPRESIDUEC();

  outputm[tem.I_AGSTUBC][dm] = tem.ag.getSTUBBLEC();

  // Nitrogen in Human product pools

  outputm[tem.I_AGPRDN][dm] = tem.ag.getPROD1N();

  outputm[tem.I_PROD10N][dm] = tem.ag.getPROD10N();

  outputm[tem.I_PROD100N][dm] = tem.ag.getPROD100N();

  outputm[tem.I_TOTPRDN][dm] = tem.ag.getTOTPRODN();

  // Nitrogen in crop residue pool

  outputm[tem.I_RESIDN][dm] = tem.ag.getCROPRESIDUEN();

  outputm[tem.I_AGSTUBN][dm] = tem.ag.getSTUBBLEN();

  // Monthly carbon fluxes associated with
  //  agricultural conversion

  outputm[tem.I_CNVRTC][dm] += tem.ag.getCONVRTFLXC();

  outputm[tem.I_VCNVRTC][dm] += tem.ag.getVCONVRTFLXC();

  outputm[tem.I_SCNVRTC][dm] += tem.ag.getSCONVRTFLXC();

  outputm[tem.I_SLASHC][dm] += tem.ag.getSLASHC();

  outputm[tem.I_STANDDEADC][dm] += tem.ag.getSTANDDEADC();

  outputm[tem.I_VOLAC][dm] += tem.ag.getVOLAC();

  outputm[tem.I_CFLX][dm] += tem.ag.getCFLUX();

  // Monthly nitrogen fluxes associated with
  //  agricultural conversion

  outputm[tem.I_CNVRTN][dm] += tem.ag.getCONVRTFLXN();

  outputm[tem.I_VCNVRTN][dm] += tem.ag.getVCONVRTFLXN();

  outputm[tem.I_SCNVRTN][dm] += tem.ag.getSCONVRTFLXN();

  outputm[tem.I_SLASHN][dm] += tem.ag.getSLASHN();

  outputm[tem.I_STANDDEADN][dm] += tem.ag.getSTANDDEADN();

  outputm[tem.I_VOLAN][dm] += tem.ag.getVOLAN();

  outputm[tem.I_NRETNT][dm] += tem.ag.getNRETENT();

  outputm[tem.I_NVRTNT][dm] += tem.ag.getNVRETENT();

  outputm[tem.I_NSRTNT][dm] += tem.ag.getNSRETENT();

  // Monthly carbon and nitrogen fluxes from agricultural
  //   ecosystems

  outputm[tem.I_AGFPRDC][dm] += tem.ag.getCROPPRODC();
  outputm[tem.I_AGFPRDN][dm] += tem.ag.getCROPPRODN();

  outputm[tem.I_FRESIDC][dm] += tem.ag.getFORMCROPRESIDUEC();
  outputm[tem.I_FRESIDN][dm] += tem.ag.getFORMCROPRESIDUEN();

  outputm[tem.I_AGPRDFC][dm] += tem.ag.getPROD1DECAYC();
  outputm[tem.I_AGPRDFN][dm] += tem.ag.getPROD1DECAYN();

  outputm[tem.I_RESIDFC][dm] += tem.ag.getCROPRESIDUEFLXC();
  outputm[tem.I_RESIDFN][dm] += tem.ag.getCROPRESIDUEFLXN();


  // Monthly carbon and nitrogen fluxes from products

  outputm[tem.I_PRDF10C][dm] += tem.ag.getFORMPROD10C();
  outputm[tem.I_PRDF10N][dm] += tem.ag.getFORMPROD10N();

  outputm[tem.I_PRD10FC][dm] += tem.ag.getPROD10DECAYC();
  outputm[tem.I_PRD10FN][dm] += tem.ag.getPROD10DECAYN();

  outputm[tem.I_PRDF100C][dm] += tem.ag.getFORMPROD100C();
  outputm[tem.I_PRDF100N][dm] += tem.ag.getFORMPROD100N();

  outputm[tem.I_PRD100FC][dm] += tem.ag.getPROD100DECAYC();
  outputm[tem.I_PRD100FN][dm] += tem.ag.getPROD100DECAYN();

  outputm[tem.I_TOTFPRDC][dm] += tem.ag.getFORMTOTPRODC();
  outputm[tem.I_TOTFPRDN][dm] += tem.ag.getFORMTOTPRODN();

  outputm[tem.I_TOTPRDFC][dm] += tem.ag.getTOTPRODDECAYC();
  outputm[tem.I_TOTPRDFN][dm] += tem.ag.getTOTPRODDECAYN();

  //  Output agricultural area-specific vs natural area-specific
  //    results

  if( 0 == tem.ag.state )
  {
    outputm[tem.I_CROPC][dm] = ZERO;
    outputm[tem.I_NATVEGC][dm] = tem.getY( tem.I_LEAFC ) + tem.getY( tem.I_SAPWOODC )
		+ tem.getY( tem.I_HEARTWOODC ) + tem.getY( tem.I_ROOTC ) + tem.getY( tem.I_LABILEC );

    outputm[tem.I_CROPN][dm] = ZERO;
    outputm[tem.I_NATVEGN][dm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN ) + tem.getY( tem.I_LABILEN );

    outputm[tem.I_CSTRN][dm] = ZERO;
    outputm[tem.I_NATSTRN][dm] = ZERO;

    outputm[tem.I_CSTON][dm] = ZERO;
    outputm[tem.I_NATSTON][dm] = tem.getY( tem.I_LABILEN );

    outputm[tem.I_CROPLAI][dm] = ZERO;
    outputm[tem.I_NATLAI][dm] = tem.veg.getLAI();

    outputm[tem.I_CROPFPC][dm] = ZERO;
    outputm[tem.I_NATFPC][dm] = tem.getY( tem.I_FPC );

    outputm[tem.I_AGINGPP][dm] = ZERO;
    outputm[tem.I_NATINGPP][dm] += tem.getY( tem.I_INGPP );

    outputm[tem.I_AGGPP][dm] = ZERO;
    outputm[tem.I_NATGPP][dm] += tem.getY( tem.I_GPP );

    outputm[tem.I_AGINNPP][dm] = ZERO;
    outputm[tem.I_NATINNPP][dm] += tem.getY( tem.I_INNPP );

    outputm[tem.I_AGNPP][dm] = ZERO;
    outputm[tem.I_NATNPP][dm] += tem.getY( tem.I_NPP );

    outputm[tem.I_AGGPR][dm] = ZERO;
    outputm[tem.I_NATGPR][dm] += tem.getY( tem.I_GPR );

    outputm[tem.I_AGRVMNT][dm] = ZERO;
    outputm[tem.I_NATRVMNT][dm] += tem.getY( tem.I_RVMNT );

    outputm[tem.I_AGRVGRW][dm] = ZERO;
    outputm[tem.I_NATRVGRW][dm] += tem.getY( tem.I_RVGRW );

    outputm[tem.I_AGLTRC][dm] = ZERO;
    outputm[tem.I_NATLTRC][dm] += tem.getY( tem.I_RVGRW );

    outputm[tem.I_AGINNUP][dm] = ZERO;
    outputm[tem.I_NATINNUP][dm] += tem.getY( tem.I_INNUP );

    outputm[tem.I_AGVNUP][dm] = ZERO;
    outputm[tem.I_NATVNUP][dm] += tem.getY( tem.I_VNUP );

    outputm[tem.I_AGLTRN][dm] = ZERO;
    outputm[tem.I_NATLTRN][dm] += tem.getY( tem.I_LTRLN ) + tem.getY( tem.I_LTRSN )
		+ tem.getY( tem.I_LTRHN ) + tem.getY( tem.I_LTRRN );
  }
  else
  {
    outputm[tem.I_CROPC][dm] = tem.getY( tem.I_LEAFC ) + tem.getY( tem.I_SAPWOODC )
		+ tem.getY( tem.I_HEARTWOODC ) + tem.getY( tem.I_ROOTC ) + tem.getY( tem.I_LABILEC );
    outputm[tem.I_NATVEGC][dm] = ZERO;

    outputm[tem.I_CROPN][dm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN ) + tem.getY( tem.I_LABILEN );
    outputm[tem.I_NATVEGN][dm] = ZERO;

    outputm[tem.I_CSTRN][dm] = tem.getY( tem.I_LEAFN ) + tem.getY( tem.I_SAPWOODN )
		+ tem.getY( tem.I_HEARTWOODN ) + tem.getY( tem.I_ROOTN );
    outputm[tem.I_NATSTRN][dm] = ZERO;

    outputm[tem.I_CSTON][dm] = tem.getY( tem.I_LABILEN );
    outputm[tem.I_NATSTON][dm] = ZERO;

    outputm[tem.I_CROPLAI][dm] = tem.veg.getLAI();
    outputm[tem.I_NATLAI][dm] = ZERO;

    outputm[tem.I_CROPFPC][dm] = tem.getY( tem.I_FPC );
    outputm[tem.I_NATFPC][dm] = ZERO;

    outputm[tem.I_AGINGPP][dm] += tem.getY( tem.I_INGPP );
    outputm[tem.I_NATINGPP][dm] = ZERO;

    outputm[tem.I_AGGPP][dm] += tem.getY( tem.I_GPP );
    outputm[tem.I_NATGPP][dm] = ZERO;

    outputm[tem.I_AGINNPP][dm] += tem.getY( tem.I_INNPP );
    outputm[tem.I_NATINNPP][dm] = ZERO;

    outputm[tem.I_AGNPP][dm] += tem.getY( tem.I_NPP );
    outputm[tem.I_NATNPP][dm] = ZERO;

    outputm[tem.I_AGGPR][dm] += tem.getY( tem.I_GPR );
    outputm[tem.I_NATGPR][dm] = ZERO;

    outputm[tem.I_AGRVMNT][dm] += tem.getY( tem.I_RVMNT );
    outputm[tem.I_NATRVMNT][dm] = ZERO;

    outputm[tem.I_AGRVGRW][dm] += tem.getY( tem.I_RVGRW );
    outputm[tem.I_NATRVGRW][dm] = ZERO;

    outputm[tem.I_AGLTRC][dm] += tem.getY( tem.I_LTRLC ) + tem.getY( tem.I_LTRSC )
		+ tem.getY( tem.I_LTRHC ) + tem.getY( tem.I_LTRRC );
    outputm[tem.I_NATLTRC][dm] = ZERO;

    outputm[tem.I_AGINNUP][dm] += tem.getY( tem.I_INNUP );
    outputm[tem.I_NATINNUP][dm] = ZERO;

    outputm[tem.I_AGVNUP][dm] += tem.getY( tem.I_VNUP );
    outputm[tem.I_NATVNUP][dm] = ZERO;


    outputm[tem.I_AGLTRN][dm] += tem.getY( tem.I_LTRLN ) + tem.getY( tem.I_LTRSN )
		+ tem.getY( tem.I_LTRHN ) + tem.getY( tem.I_LTRRN );
    outputm[tem.I_NATLTRN][dm] = ZERO;
  }
//
//  Extra variables
//
/*outpute[0][dm] += tem.veg.getFRDL();
outpute[1][dm] += tem.veg.getFCO2();
outpute[2][dm] += tem.veg.getTEMP();
outpute[3][dm] += tem.veg.getFOZONE();
outpute[4][dm] += tem.veg.getFH2O(); */
};

/* *************************************************************
************************************************************** */

void Telm45::readCohortState( ifstream& ifstate,
                              const int& pichrt,
                              const int& CYCLE )
{
  int i;
  int dm;
  float dumflt;
  int dumint;

  ifstate >> dumflt;    // Longitude of element
cout << "longitude = " << dumflt << endl;
  ifstate >> dumflt;    // Latitude of element

cout << "latitude = " << dumflt << endl;
  ifstate >> dumint;   // ichrt+1

  ifstate >> cohort[pichrt].srcCohort;
  ifstate >> cohort[pichrt].standage;
  ifstate >> cohort[pichrt].chrtarea;
  ifstate >> cohort[pichrt].potveg;
  ifstate >> cohort[pichrt].currentveg;
  ifstate >> cohort[pichrt].subtype;
  ifstate >> cohort[pichrt].cmnt;

  for( i = 0; i < MAXSTATE; ++i )
  {
    ifstate >> cohort[pichrt].y[i];
    ifstate >> cohort[pichrt].prevy[i];
  }

  ifstate >> cohort[pichrt].agcmnt;

  ifstate >> cohort[pichrt].aggrowdd;

  ifstate >> cohort[pichrt].agkd;

  ifstate >> cohort[pichrt].agprvstate;

  ifstate >> cohort[pichrt].agstate;

  ifstate >> cohort[pichrt].avlh2o;

  ifstate >> cohort[pichrt].awcapmm;

  ifstate >> cohort[pichrt].c2n;

  ifstate >> cohort[pichrt].cneven;

  ifstate >> cohort[pichrt].convrtflx.carbon;
  ifstate >> cohort[pichrt].convrtflx.nitrogen;
  ifstate >> cohort[pichrt].standdead.carbon;
  ifstate >> cohort[pichrt].standdead.nitrogen;
  ifstate >> cohort[pichrt].vola.carbon;
  ifstate >> cohort[pichrt].vola.nitrogen;

  ifstate >> cohort[pichrt].cropResidue.carbon;
  ifstate >> cohort[pichrt].cropResidue.nitrogen;

  ifstate >> cohort[pichrt].croptopt;

  ifstate >> cohort[pichrt].distmnthcnt;
  ifstate >> cohort[pichrt].disturbflag;
  ifstate >> cohort[pichrt].disturbmonth;

  ifstate >> cohort[pichrt].fertflag;

  ifstate >> cohort[pichrt].firemnthcnt;

  ifstate >> cohort[pichrt].formPROD10.carbon;
  ifstate >> cohort[pichrt].formPROD10.nitrogen;

  ifstate >> cohort[pichrt].formPROD100.carbon;
  ifstate >> cohort[pichrt].formPROD100.nitrogen;

  ifstate >> cohort[pichrt].FRI;

//  for( dm = 0; dm < CYCLE; ++dm )
  for( dm = 0; dm < 12; ++dm )
  {
    ifstate >> cohort[pichrt].initPROD1[dm].carbon;
    ifstate >> cohort[pichrt].initPROD1[dm].nitrogen;
  }

  for( i = 0; i < 10; ++i )
  {
    ifstate >> cohort[pichrt].initPROD10[i].carbon;
    ifstate >> cohort[pichrt].initPROD10[i].nitrogen;
  }

  for( i = 0; i < 100; ++i )
  {
    ifstate >> cohort[pichrt].initPROD100[i].carbon;
    ifstate >> cohort[pichrt].initPROD100[i].nitrogen;
  }

  ifstate >> cohort[pichrt].irrgflag;

  ifstate >> cohort[pichrt].kd;

  ifstate >> cohort[pichrt].natseedC;

  ifstate >> cohort[pichrt].natseedSTRN;

  ifstate >> cohort[pichrt].natseedSTON;

  ifstate >> cohort[pichrt].natsoil;

  ifstate >> cohort[pichrt].nattopt;

  ifstate >> cohort[pichrt].newtopt;

  ifstate >> cohort[pichrt].nretent;

  ifstate >> cohort[pichrt].nsretent;

  ifstate >> cohort[pichrt].nvretent;

  ifstate >> cohort[pichrt].prev2tair;

  ifstate >> cohort[pichrt].prevco2;

  ifstate >> cohort[pichrt].prevCropResidue.carbon;
  ifstate >> cohort[pichrt].prevCropResidue.nitrogen;

  ifstate >> cohort[pichrt].prevPROD1.carbon;
  ifstate >> cohort[pichrt].prevPROD1.nitrogen;

  ifstate >> cohort[pichrt].prevPROD10.carbon;
  ifstate >> cohort[pichrt].prevPROD10.nitrogen;

  ifstate >> cohort[pichrt].prevPROD100.carbon;
  ifstate >> cohort[pichrt].prevPROD100.nitrogen;

  ifstate >> cohort[pichrt].prevspack;

  ifstate >> cohort[pichrt].prevtair;

  ifstate >> cohort[pichrt].prod10par;

  ifstate >> cohort[pichrt].prod100par;

  ifstate >> cohort[pichrt].productYear;

  ifstate >> cohort[pichrt].prvchrtarea;

  ifstate >> cohort[pichrt].prvcropnpp;

  ifstate >> cohort[pichrt].rprec;
  
  ifstate >> cohort[pichrt].rdemandc;
  
  ifstate >> cohort[pichrt].rdemandn;

  ifstate >> cohort[pichrt].reet;

  ifstate >> cohort[pichrt].rlabilec;
  
  ifstate >> cohort[pichrt].rlabilen;
  
  ifstate >> cohort[pichrt].rgpp;
  
  ifstate >> cohort[pichrt].ringpp;

  ifstate >> cohort[pichrt].rnpp;

  ifstate >> cohort[pichrt].rltrc;

  ifstate >> cohort[pichrt].rpleaf;
  
  ifstate >> cohort[pichrt].rrh;

  ifstate >> cohort[pichrt].rtair;

  ifstate >> cohort[pichrt].rtairphi;

  ifstate >> cohort[pichrt].rphi;

  ifstate >> cohort[pichrt].phicnt;

  ifstate >> cohort[pichrt].qc;

  ifstate >> cohort[pichrt].sconvert;

  ifstate >> cohort[pichrt].sconvrtflx.carbon;
  ifstate >> cohort[pichrt].sconvrtflx.nitrogen;

  ifstate >> cohort[pichrt].slash.carbon;
  ifstate >> cohort[pichrt].slash.nitrogen;

  ifstate >> cohort[pichrt].slashpar;

  ifstate >> cohort[pichrt].tillflag;

  ifstate >> cohort[pichrt].topt;

  ifstate >> cohort[pichrt].tqc;

  ifstate >> cohort[pichrt].vconvert;

  ifstate >> cohort[pichrt].vconvrtflx.carbon;
  ifstate >> cohort[pichrt].vconvrtflx.nitrogen;

  ifstate >> cohort[pichrt].vrespar;

//  ifstate >> cohort[pichrt].wiltpt;

  ifstate >> cohort[pichrt].yrltrc;
  ifstate >> cohort[pichrt].yrltrn;
  
  ifstate >> cohort[pichrt].yrpleaf;
  ifstate >> cohort[pichrt].yrpsapwood;
  ifstate >> cohort[pichrt].yrproot;
  
  ifstate >> cohort[pichrt].yralloclc;
  ifstate >> cohort[pichrt].yrallocsc;
  ifstate >> cohort[pichrt].yrallocrc;
  
  ifstate >> cohort[pichrt].yrrmleaf;
  ifstate >> cohort[pichrt].yrrmsapwood;
  ifstate >> cohort[pichrt].yrrmroot;

  cout << "yrmroot = " << cohort[pichrt].yrrmroot << endl;
  ifstate.seekg( 0, ios::cur );

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Telm45::saveTEMCohortState( const int& pichrt,
                                 const int& CYCLE )
{
  int dm;
  int i;


  cohort[pichrt].potveg = tem.veg.getPOTVEG();

  cohort[pichrt].currentveg = tem.veg.getCURRENTVEG();

  cohort[pichrt].subtype = tem.veg.getSUBTYPE();

  cohort[pichrt].cmnt = tem.veg.cmnt;

  for( i = 0; i < MAXSTATE; ++i )
  {
    cohort[pichrt].y[i] = tem.getY( i );
    cohort[pichrt].prevy[i] = tem.getPREVY( i );
  }

  cohort[pichrt].agcmnt = tem.ag.cmnt;

  cohort[pichrt].aggrowdd = tem.ag.getGROWDD();

  cohort[pichrt].agkd = tem.ag.getKD();

  cohort[pichrt].agprvstate = tem.ag.prvstate;

  cohort[pichrt].agstate = tem.ag.state;

  cohort[pichrt].avlh2o = tem.soil.getAVLH2O();

  cohort[pichrt].awcapmm = tem.soil.getAWCAPMM();

  cohort[pichrt].c2n = tem.veg.getC2N();

  cohort[pichrt].cneven = tem.veg.getCNEVEN();

  cohort[pichrt].convrtflx.carbon = tem.ag.getCONVRTFLXC();
  cohort[pichrt].convrtflx.nitrogen = tem.ag.getCONVRTFLXN();
  cohort[pichrt].standdead.carbon = tem.ag.getSTANDDEADC();
  cohort[pichrt].standdead.nitrogen = tem.ag.getSTANDDEADN();
  cohort[pichrt].vola.carbon = tem.ag.getVOLAC();
  cohort[pichrt].vola.nitrogen = tem.ag.getVOLAN();

  cohort[pichrt].cropResidue.carbon = tem.ag.getCROPRESIDUEC();
  cohort[pichrt].cropResidue.nitrogen = tem.ag.getCROPRESIDUEN();

  cohort[pichrt].croptopt = tem.ag.getCROPTOPT();

  cohort[pichrt].distmnthcnt = tem.distmnthcnt;
  cohort[pichrt].disturbflag = tem.disturbflag;
  cohort[pichrt].disturbmonth = tem.disturbmonth;

  cohort[pichrt].fertflag = tem.ag.fertflag;

  cohort[pichrt].firemnthcnt = tem.firemnthcnt;

  cohort[pichrt].formPROD10.carbon = tem.ag.getFORMPROD10C();
  cohort[pichrt].formPROD10.nitrogen = tem.ag.getFORMPROD10N();

  cohort[pichrt].formPROD100.carbon = tem.ag.getFORMPROD100C();
  cohort[pichrt].formPROD100.nitrogen = tem.ag.getFORMPROD100N();

  cohort[pichrt].FRI = tem.ag.getFRI();

  for( dm = 0; dm < CYCLE; ++dm )
  {
    cohort[pichrt].initPROD1[dm].carbon = tem.ag.getINITPROD1C( dm );
    cohort[pichrt].initPROD1[dm].nitrogen = tem.ag.getINITPROD1N( dm );
  }

  for( i = 0; i < 10; ++i )
  {
    cohort[pichrt].initPROD10[i].carbon = tem.ag.getINITPROD10C( i );
    cohort[pichrt].initPROD10[i].nitrogen = tem.ag.getINITPROD10N( i );
  }

  for( i = 0; i < 100; ++i )
  {
    cohort[pichrt].initPROD100[i].carbon = tem.ag.getINITPROD100C( i );
    cohort[pichrt].initPROD100[i].nitrogen = tem.ag.getINITPROD100N( i );
  }

  cohort[pichrt].irrgflag = tem.ag.irrgflag;

  cohort[pichrt].kd = tem.microbe.getKD();

  cohort[pichrt].natseedC = tem.ag.getNATSEEDC();

  cohort[pichrt].natseedSTRN = tem.ag.getNATSEEDSTRN();

  cohort[pichrt].natseedSTON = tem.ag.getNATSEEDSTON();

  cohort[pichrt].natsoil = tem.ag.getNATSOIL();

  cohort[pichrt].nattopt = tem.ag.getNATTOPT();

  cohort[pichrt].newtopt = tem.veg.getNEWTOPT();

  cohort[pichrt].nretent = tem.ag.getNRETENT();

  cohort[pichrt].nsretent = tem.ag.getNSRETENT();

  cohort[pichrt].nvretent = tem.ag.getNVRETENT();

  cohort[pichrt].prev2tair = tem.atms.getPREV2TAIR();

  cohort[pichrt].prevco2 = tem.atms.getPREVCO2();

  cohort[pichrt].prevCropResidue.carbon = tem.ag.getPREVCROPRESIDUEC();
  cohort[pichrt].prevCropResidue.nitrogen = tem.ag.getPREVCROPRESIDUEN();

  cohort[pichrt].prevPROD1.carbon = tem.ag.getPREVPROD1C();
  cohort[pichrt].prevPROD1.nitrogen = tem.ag.getPREVPROD1N();

  cohort[pichrt].prevPROD10.carbon = tem.ag.getPREVPROD10C();
  cohort[pichrt].prevPROD10.nitrogen = tem.ag.getPREVPROD10N();

  cohort[pichrt].prevPROD100.carbon = tem.ag.getPREVPROD100C();
  cohort[pichrt].prevPROD100.nitrogen = tem.ag.getPREVPROD100N();

  cohort[pichrt].prevspack = tem.soil.getPREVSPACK();

  cohort[pichrt].prevtair = tem.atms.getPREVTAIR();

  cohort[pichrt].prod10par = tem.ag.getPROD10PAR();

  cohort[pichrt].prod100par = tem.ag.getPROD100PAR();

  cohort[pichrt].productYear = tem.ag.getPRODUCTYEAR();

  cohort[pichrt].prvcropnpp = tem.ag.getPRVCROPNPP();

  cohort[pichrt].rprec = tem.veg.getRPREC();
  
  cohort[pichrt].rdemandc = tem.veg.getRDEMANDC();
  
  cohort[pichrt].rdemandn = tem.veg.getRDEMANDN();

  cohort[pichrt].reet = tem.soil.getREET();

  cohort[pichrt].rlabilec = tem.veg.getRLABILEC();
  
  cohort[pichrt].rlabilen = tem.veg.getRLABILEN();
  
  cohort[pichrt].rgpp = tem.veg.getRGPP();
  
  cohort[pichrt].ringpp = tem.veg.getRINGPP();

  cohort[pichrt].rnpp = tem.veg.getRNPP();

  cohort[pichrt].rltrc = tem.veg.getRLTRC();

  cohort[pichrt].rpleaf = tem.veg.getRPLEAF();

  cohort[pichrt].rrh = tem.mcrb.getRRH();

  cohort[pichrt].rtair = tem.veg.getRTAIR();

  cohort[pichrt].rtairphi = tem.veg.getRTAIRPHI();

  cohort[pichrt].rphi = tem.veg.getRPHI();

  cohort[pichrt].phicnt = tem.veg.getPHICNT();

  cohort[pichrt].sconvert = tem.ag.getSCONVERT();

  cohort[pichrt].sconvrtflx.carbon = tem.ag.getSCONVRTFLXC();
  cohort[pichrt].sconvrtflx.nitrogen = tem.ag.getSCONVRTFLXN();

  cohort[pichrt].slash.carbon = tem.ag.getSLASHC();
  cohort[pichrt].slash.nitrogen = tem.ag.getSLASHN();

  cohort[pichrt].slashpar = tem.ag.getSLASHPAR();

  cohort[pichrt].tillflag = tem.ag.tillflag;

  cohort[pichrt].topt = tem.veg.getTOPT();

  cohort[pichrt].toptmic = tem.veg.getTOPTMIC();

  cohort[pichrt].vconvert = tem.ag.getVCONVERT();

  cohort[pichrt].vconvrtflx.carbon = tem.ag.getVCONVRTFLXC();
  cohort[pichrt].vconvrtflx.nitrogen = tem.ag.getVCONVRTFLXN();

  cohort[pichrt].vrespar = tem.ag.getVRESPAR();

//  cohort[pichrt].wiltpt = tem.soil.getWILTPT();

  cohort[pichrt].yrltrc = tem.veg.yrltrc;
  cohort[pichrt].yrltrn = tem.veg.yrltrn;
  
  cohort[pichrt].yrpleaf = tem.veg.yrpleaf;
  cohort[pichrt].yrpsapwood = tem.veg.yrpsapwood;
  cohort[pichrt].yrproot = tem.veg.yrproot;
  
  cohort[pichrt].yralloclc = tem.veg.yralloclc;
  cohort[pichrt].yrallocsc = tem.veg.yrallocsc;
  cohort[pichrt].yrallocrc = tem.veg.yrallocrc;
  
  cohort[pichrt].yrrmleaf = tem.veg.yrrmleaf;
  cohort[pichrt].yrrmsapwood = tem.veg.yrrmsapwood;
  cohort[pichrt].yrrmroot = tem.veg.yrrmroot; 

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Telm45::setCohortTEMState( const ElmntCohort45& firstchrt,
                                    ElmntCohort45& targetchrt,
                                const int& CYCLE )
{
  int dm;
  int i;


  for( i = 0; i < MAXSTATE; ++i )
  {
    targetchrt.y[i] = firstchrt.y[i];
    targetchrt.prevy[i] = firstchrt.prevy[i];
  }

  targetchrt.aggrowdd = firstchrt.aggrowdd;

  targetchrt.agkd = firstchrt.agkd;

  targetchrt.avlh2o = firstchrt.avlh2o;

  targetchrt.awcapmm = firstchrt.awcapmm;

  targetchrt.c2n = firstchrt.c2n;

  targetchrt.cneven = firstchrt.cneven;

  targetchrt.convrtflx.carbon = firstchrt.convrtflx.carbon;
  targetchrt.convrtflx.nitrogen = firstchrt.convrtflx.nitrogen;
  targetchrt.standdead.carbon = firstchrt.standdead.carbon;
  targetchrt.standdead.nitrogen = firstchrt.standdead.nitrogen;
  targetchrt.vola.carbon = firstchrt.vola.carbon;
  targetchrt.vola.nitrogen = firstchrt.vola.nitrogen;

  targetchrt.cropResidue.carbon = firstchrt.cropResidue.carbon;
  targetchrt.cropResidue.nitrogen = firstchrt.cropResidue.nitrogen;

  targetchrt.croptopt = firstchrt.croptopt;

  targetchrt.distmnthcnt = firstchrt.distmnthcnt;

  targetchrt.firemnthcnt = firstchrt.firemnthcnt;

  targetchrt.formPROD10.carbon = firstchrt.formPROD10.carbon;
  targetchrt.formPROD10.nitrogen = firstchrt.formPROD10.nitrogen;

  targetchrt.formPROD100.carbon = firstchrt.formPROD100.carbon;
  targetchrt.formPROD100.nitrogen = firstchrt.formPROD100.nitrogen;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    targetchrt.initPROD1[dm].carbon = firstchrt.initPROD1[dm].carbon;
    targetchrt.initPROD1[dm].nitrogen = firstchrt.initPROD1[dm].nitrogen;
  }

  for( i = 0; i < 10; ++i )
  {
    targetchrt.initPROD10[i].carbon = firstchrt.initPROD10[i].carbon;
    targetchrt.initPROD10[i].nitrogen = firstchrt.initPROD10[i].nitrogen;
  }

  for( i = 0; i < 100; ++i )
  {
    targetchrt.initPROD100[i].carbon = firstchrt.initPROD100[i].carbon;
    targetchrt.initPROD100[i].nitrogen = firstchrt.initPROD100[i].nitrogen;
  }

  targetchrt.kd = firstchrt.kd;

  targetchrt.natseedC = firstchrt.natseedC;

  targetchrt.natseedSTRN = firstchrt.natseedSTRN;

  targetchrt.natseedSTON = firstchrt.natseedSTON;

  targetchrt.natsoil = firstchrt.natsoil;

  targetchrt.nattopt = firstchrt.nattopt;

  targetchrt.newtopt = firstchrt.newtopt;

  targetchrt.nretent = firstchrt.nretent;

  targetchrt.nsretent = firstchrt.nsretent;

  targetchrt.nvretent = firstchrt.nvretent;

  targetchrt.prev2tair = firstchrt.prev2tair;

  targetchrt.prevco2 = firstchrt.prevco2;

  targetchrt.prevCropResidue.carbon = firstchrt.prevCropResidue.carbon;
  targetchrt.prevCropResidue.nitrogen = firstchrt.prevCropResidue.nitrogen;

  targetchrt.prevPROD1.carbon = firstchrt.prevPROD1.carbon;
  targetchrt.prevPROD1.nitrogen = firstchrt.prevPROD1.nitrogen;

  targetchrt.prevPROD10.carbon = firstchrt.prevPROD10.carbon;
  targetchrt.prevPROD10.nitrogen = firstchrt.prevPROD10.nitrogen;

  targetchrt.prevPROD100.carbon = firstchrt.prevPROD100.carbon;
  targetchrt.prevPROD100.nitrogen = firstchrt.prevPROD100.nitrogen;

  targetchrt.prevspack = firstchrt.prevspack;

  targetchrt.prevtair = firstchrt.prevtair;

  targetchrt.productYear = firstchrt.productYear;

  targetchrt.prvcropnpp = firstchrt.prvcropnpp;

  targetchrt.rprec = firstchrt.rprec;
  
  targetchrt.rdemandc = firstchrt.rdemandc;
  
  targetchrt.rdemandn = firstchrt.rdemandn;

  targetchrt.reet = firstchrt.reet;
  
  targetchrt.rlabilec = firstchrt.rlabilec;
  
  targetchrt.rlabilen = firstchrt.rlabilen;
  
  targetchrt.rgpp = firstchrt.rgpp;
  
  targetchrt.ringpp = firstchrt.ringpp;

  targetchrt.rnpp = firstchrt.rnpp;

  targetchrt.rltrc = firstchrt.rltrc;

  targetchrt.rpleaf = firstchrt.rpleaf;

  targetchrt.rrh = firstchrt.rrh;

  targetchrt.rtair = firstchrt.rtair;

  targetchrt.rtairphi = firstchrt.rtairphi;

  targetchrt.rphi = firstchrt.rphi;

  targetchrt.phicnt = firstchrt.phicnt;

  targetchrt.qc = firstchrt.qc;

  targetchrt.sconvrtflx.carbon = firstchrt.sconvrtflx.carbon;
  targetchrt.sconvrtflx.nitrogen = firstchrt.sconvrtflx.nitrogen;

  targetchrt.slash.carbon = firstchrt.slash.carbon;
  targetchrt.slash.nitrogen = firstchrt.slash.nitrogen;

  targetchrt.topt = firstchrt.topt;

  targetchrt.tqc = firstchrt.tqc;

  targetchrt.vconvrtflx.carbon = firstchrt.vconvrtflx.carbon;
  targetchrt.vconvrtflx.nitrogen = firstchrt.vconvrtflx.nitrogen;

//  targetchrt.wiltpt = firstchrt.wiltpt;

  targetchrt.yrltrc = firstchrt.yrltrc;
  targetchrt.yrltrn = firstchrt.yrltrn;
  
  targetchrt.yrpleaf = firstchrt.yrpleaf;
  targetchrt.yrpsapwood = firstchrt.yrpsapwood;
  targetchrt.yrproot = firstchrt.yrproot;
  
  targetchrt.yralloclc = firstchrt.yralloclc;
  targetchrt.yrallocsc = firstchrt.yrallocsc;
  targetchrt.yrallocrc = firstchrt.yrallocrc;
  
  targetchrt.yrrmleaf = firstchrt.yrrmleaf;
  targetchrt.yrrmsapwood = firstchrt.yrrmsapwood;
  targetchrt.yrrmroot = firstchrt.yrrmroot;


};

/* *************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

int Telm45::setGIStopography( ofstream& rflog1,
                                  int& ftlerr,
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

  ftlerr = coregerr( rflog1,
                     "Climate",
                     col,
                     row,
                     "TEXTURE",
                     fao.col,
                     fao.row );

  tem.soil.setPCTSAND( fao.pctsand );
  tem.soil.setPCTSILT( fao.pctsilt );
  tem.soil.setPCTCLAY( fao.pctclay );

  gisend = elv.getdel( felev );

  if( gisend == -1 )
  {
    rflog1 << "Ran out of Elevation data" << endl << endl;

    exit( -1 );
  }

  ftlerr = coregerr( rflog1,
                     "Climate",
                     col,
                     row,
                     "ELEV",
                     elv.col,
                     elv.row );

  tem.elev = elv.elev;

  return gisend;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Telm45::setTEMequilState( ofstream& rflog1,
                                   const int& equil,
                                   const int& totsptime,
                                   const int& pichrt,
                                   ofstream fout[NUMTEM],
                                   const vector<string>& predname,
                                   const int& CYCLE,
                                   const int& spinoutfg,
                                   const int ndays[] )
{
  int dyr = 0;
  int dm;
  double avgphi = ZERO;
  double avgphish = ZERO;
  double avgtair = ZERO;


  // Set all TEM state-related variables in cohort to MISSING
  //   (i.e. start with "clean slate" in cohort)

  initializeCohortTEMState( pichrt,
                            CYCLE );

  // Assign cohort data to TEM (i.e. transfer information from
  //   the land cover/land use module to TEM and start with
  //   "clean slate" for TEM cohort data)

  getTEMCohortState( pichrt,
                     CYCLE );

  cohort[pichrt].qc = ACCEPT;
  cohort[pichrt].tqc = ACCEPT;
  
//cout << "cohort[pichrt] = veryfirst" << cohort[pichrt].tqc << " " << cohort[pichrt].qc <<   endl;
  if( de_startyear > clm.startyr ) { dyr = de_startyear + 1 - clm.startyr; }

  tem.totyr = 0;

//
//  BSF Allow fertilization on during equilibrium
//
//  tem.ag.fert1950flag = 0;

  tem.atms.setMXTAIR( mxtair[dyr] );
  tem.atms.yrprec = yrprec[dyr];

  tem.atms.setNDEP( adep[0][dyr] );

  for( dm = 0; dm < CYCLE; ++dm )
  {
//cout << "month = " << dm << " " << CYCLE << endl;
    // Pass climate data for particular month to TEM

    tem.atms.setGIRR( climate[clm.I_GIRR][dm][dyr] );
    tem.atms.setCLDS( climate[clm.I_CLDS][dm][dyr] );
    tem.atms.setNIRR( climate[clm.I_NIRR][dm][dyr] );
    tem.atms.setPAR(  climate[clm.I_PAR][dm][dyr] );
    tem.atms.setTAIR( climate[clm.I_TAIR][dm][dyr] );
    tem.atms.setTAIRD( climate[clm.I_TAIRD][dm][dyr] );
    tem.atms.setTAIRN( climate[clm.I_TAIRN][dm][dyr] );
    tem.atms.setRAIN( climate[clm.I_RAIN][dm][dyr] );
    tem.atms.setPREC( climate[clm.I_PREC][dm][dyr] );
    tem.atms.setSNOWFALL( climate[clm.I_SNWFAL][dm][dyr] );
    tem.atms.setCO2( climate[clm.I_CO2][dm][0] );
    tem.atms.setAOT40( climate[clm.I_AOT40][dm][dyr] );
    tem.atms.setTRANGE( climate[clm.I_TRANGE][dm][dyr] );
    tem.atms.setVPR( climate[clm.I_VPR][dm][dyr] );
    tem.atms.setVPDD( climate[clm.I_VPDD][dm][dyr] );
    tem.atms.setVPDN( climate[clm.I_VPDN][dm][dyr] );
    tem.atms.setWS10( climate[clm.I_WS10][dm][dyr] );
    tem.atms.setDAYL( climate[clm.I_DAYL][dm][dyr] );

//cout << "dayl_2 = " << tem.atms.getDAYL() << endl;
//cout << "hello1" << " " << tem.atms.yrprec << endl;

    // Check TEM climate input for valid data

    cohort[pichrt].qc = temgisqc(  cohort[pichrt].chrtarea,
		                           tem.soil.getPCTSAND(),
                                   tem.soil.getPCTSILT(),
                                   tem.soil.getPCTCLAY(),
                                   tem.veg.cmnt,
                                   tem.elev,
                                   tem.atms.getNIRR(),
                                   tem.atms.getPAR(),
                                   tem.atms.getTAIR(),
                                   tem.atms.getMXTAIR(),
                                   avetair[dyr],
                                   tem.atms.yrprec,
                                   tem.atms.getRAIN(),
                                   tem.atms.getSNOWFALL(),
                                   tem.atms.getCO2(),
                                   tem.atms.getAOT40(),
                                   tem.atms.getNDEP(),
								   tem.atms.getTRANGE(),
								   tem.atms.getVPR() );

//cout << "here I am " << endl;
    if( cohort[pichrt].qc != ACCEPT )
    {
      rflog1 << "temgisqc = " << cohort[pichrt].qc;
      rflog1 << " during month " << (dm+1) << endl;
      break;
    }


//cout << "hello2" << endl;
    // determine avg. tair of initial year
      
//    avgtair += tem.atms.getTAIRD()/12.0;
    avgtair += tem.atms.getTAIRD()/365.0;

    if( dm == 0 ) { tem.veg.setTOPT( tem.veg.getTOPTMIN( tem.veg.cmnt ) ); }
    if( tem.atms.getTAIRD() > tem.veg.getTOPT() ) 
    { 
      tem.veg.setTOPT( tem.atms.getTAIRD() ); 
    }

    if( dm == 0 ) { tem.veg.setTOPTMIC( tem.veg.getTOPTMIN( tem.veg.cmnt ) ); }
    if( tem.atms.getTAIRD() > tem.veg.getTOPTMIC() )
    {
      tem.veg.setTOPTMIC( tem.atms.getTAIRD() );
    }


  }


  // Check TEM parameters for specific vegetation types

  if( ACCEPT == cohort[pichrt].qc )
  {
    cohort[pichrt].qc = tem.ecdqc( tem.veg.cmnt );

    if( cohort[pichrt].qc != ACCEPT )
    {
      // Note: If a TEM parameter is invalid,
      //   cohort[pichrt].qc will have a value greater than
      //   100

      rflog1 << "temecdqc = " << cohort[pichrt].qc << endl;
    }
  }

//cout << "hello3" << endl;
  if( cohort[pichrt].qc != ACCEPT )
  {
    // If environmental conditions are too extreme for the
    //   existence of vegetation (e.g., no precipitation or
    //   constant freezing air temperatures), assign zero to
    //   all TEM variables if the plant community is anything
    //   besides ice and open water; and all TEM parameters
    //   are valid (i.e. cohort[pichrt].qc < 100 )

    if( cohort[pichrt].qc < 100
        && tem.veg.cmnt > 1
        && (mxtair[dyr] < -1.0 || yrprec[dyr] <= ZERO) )
    {
      // Set tqc flag to assign zero to all TEM variables
      //   during simulation

      cohort[pichrt].tqc = TQCZEROFLAG;
    }
    else { cohort[pichrt].tqc = REJECT; }

    // Set missing values to telmnt[0].output

    setTEMmiss( dyr,
                equil,
                totsptime,
                pichrt,
                CYCLE  );
  }
  else // "cohort[pichrt].qc == ACCEPT"
  {

/* *************************************************************
                   Start Equilibrium Conditions
************************************************************* */

    // Determine soil properties of element based on
    //   soil texture

    tem.soil.xtext( tem.veg.cmnt,
                    tem.soil.getPCTSILT(),
                    tem.soil.getPCTCLAY() );


    // Initialize tem.atms.prevco2
    
    tem.atms.setPREVTAIR( climate[clm.I_TAIR][CYCLE-1][dyr] );
    tem.atms.setPREV2TAIR( climate[clm.I_TAIR][CYCLE-2][dyr] );
    tem.soil.setPREVSPACK( ZERO );
    tem.atms.setPREVCO2( climate[clm.I_CO2][CYCLE-1][dyr] );  
    
    tem.veg.setRTAIR( avgtair );
    
    // need to set rtair before call to phenology
    for( dm = 0; dm < CYCLE; ++dm )
    {
      tem.veg.phenology( tem.veg.cmnt,
                         dm,
                         climate[clm.I_TAIR][dm][dyr],
                         climate[clm.I_TAIRD][dm][dyr],
                         climate[clm.I_TAIRN][dm][dyr],
                         1.0,
                         1.0,
                         0.0,
                         climate[clm.I_DAYL][dm][dyr]);
    
//      avgphi += tem.veg.getPHI()/12.0;
      avgphi += tem.veg.getPHI()/365.0;
      avgphish += tem.veg.getPHI()/30.0;
    }
    
    tem.veg.setRPHI( avgphi );
    tem.veg.setRPHISH( avgphish );

    // Assume potential vegetation when determining
    //   equilibrium conditions
//
//  BSF  Allow management during equilibration
//
//    tem.ag.state = 0;
//    tem.ag.prvstate = 0;

//    tem.ag.tillflag = 0;
//    tem.ag.fertflag = 0;
//    tem.ag.irrgflag = 0;

    tem.disturbflag = 0;
    tem.distmnthcnt = 0;
    // Initialize agricultural growing degree days to zero

    tem.ag.setGROWDD( ZERO );

    // "While" loop to allow adaptive integrator tolerance
    //   (i.e. tem.tol) to be reduced if chaotic behavior
    //   occurs

    tem.qualcon[dyr] = 0;


    // Try up to "tem.maxnrun" times to equilibrate TEM.  If
    //   TEM does not equilibrate within "tem.runsize"
    //   iterations, decrease tem.tol by an order of magnitude
    //   and try again

    tem.nattempt = 0;
    tem.tol = tem.inittol;
    tem.baseline = tem.initbase;
    tem.initFlag = 0;
    
    while( tem.nattempt < tem.maxnrun
           && 0 == tem.initFlag )
    {
      tem.initializeState( CYCLE );
      
      tem.nattempt = equilibrateTEM( tem.tol, pichrt, fout, predname, spinoutfg, year, CYCLE, ndays );

      if( tem.nattempt < tem.maxnrun
          && 0 == tem.initFlag )
      {
      	tem.tol /= 10.0;
      }
    }


rflog1 << "final equilibration year = " << year << endl;

    // Save quality control information about the simulation
    //   conditions when the equilibrium portion ended
    //   (i.e. did the carbon and nitrogen fluxes really come
    //         to equilibrium or was the run terminated after
    //         running chaotically for a specified maximum
    //         number of years?)

    tem.qualcon[dyr] += (tem.nattempt + 1);


    // If simulation is part of a transient simulation, reset
    //   tem.totyr to represent an actual year rather than
    //   the number of iterations required to reach equilibrum

    if( 0 == equil )
    {
      tem.totyr = tem.startyr - totsptime - 1;
      ttotyr[dyr] = tem.totyr;

    if(intervalflag == 0)
     {
      cohort[pichrt].tqc = transqc( tem.maxyears,
	                            tem.totyr,
                                    CYCLE,
//                                    tem.veg.getVEGC() );
	                            output[tem.I_LABILEC] );
     }
    else
    {
      cohort[pichrt].tqc = transqc( tem.maxyears,
                                    tem.totyr,
                                    CYCLE,
//                                    tem.veg.getVEGC() );
                                    outputm[tem.I_LABILEC] );
    }
//
    }
    else { ttotyr[dyr] = tem.totyr; }
  } // End of "cohort.qc == ACCEPT"


  // Save TEM state of cohort to telmnt[0].cohort

  saveTEMCohortState( pichrt,
                      CYCLE );

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Telm45::setTEMmiss( const int& pdyr,
                             const int& equil,
                             const int& totsptime,
                             const int& pichrt,
                             const int& CYCLE )
{
  int dm;
  int i;

  if( 0 == equil )
  {
    ttotyr[pdyr] = tem.startyr
                   - totsptime - 1
                   + (pdyr * tem.diffyr);
  }
  else
  {
    ttotyr[pdyr] = -999;
  }

  tem.totyr = ttotyr[pdyr];

  if( TQCZEROFLAG == cohort[pichrt].tqc )
  {
    if( 1 == equil ) { ttotyr[pdyr] = 1; }

    // Assign zero to all TEM state variables

    for( i = 0; i < MAXSTATE; ++i )
    {
      tem.setY( ZERO, i );
      tem.setPREVY(ZERO, i );
    }

    for( i = MAXSTATE; i < NUMEQ; ++i )
    {
      tem.setY( ZERO, i );
    }

    // Assign zero to all TEM ouput variables

    for(i = 0; i < NUMTEM; ++i )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        output[i][dm] = ZERO;
      }
     for( dm = 0; dm < 12; ++dm )
      {
        outputm[i][dm] = ZERO;
      }

    }
  }
  else
  {
    // Assign missing values to grid cells that are covered by ice or open
    // water, or where TEM did not converge on a solution

    for( i = 0; i < MAXSTATE; ++i )
    {
      tem.setY( MISSING, i );
      tem.setPREVY( MISSING, i );
    }

    for( i = MAXSTATE; i < NUMEQ; ++i )
    {
      tem.setY( MISSING, i );
    }

    for(i = 0; i < NUMTEM; ++i )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        output[i][dm] = MISSING;
      }
      for( dm = 0; dm < CYCLE; ++dm )
      {
        outputm[i][dm] = MISSING;
      }

    }
  }

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

int Telm45::temgisqc( const long& subarea,
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
						  const double& vpd )


{

  int qc;

  qc = ACCEPT;

  if( subarea < 1 ) { return qc = 1; }
  if( pctsand < ZERO ) { return qc = 2; }
  if( pctsilt < ZERO ) { return qc = 3; }
  if( pctclay < ZERO ) { return qc = 4; }
  if( cmnt < 2 || cmnt > NUMVEG ) { return qc = 5; }
  if( elev <= -999.0 ) { return qc = 6;}

  if( nirr <= -1.0 ) { return qc = 7; }
  if( par <= -1.0 ) { return qc = 8; }
  if( tair <= -99.0 ) { return qc = 9; }
  if( mxtair < -1.0 ) { return qc = 10; }
  if( avtair <= -99.0 ) { return qc = 11; }
//  if( yrprec <= ZERO ) { return qc = 12; }
  if( yrprec < ZERO ) { return qc = 12; }
  if( rain <= -1.0 ) { return qc = 13; }
  if( snowfall <= -1.0 ) { return qc = 14; }
  if( co2 <= -1.0 ) { return qc = 15; }
  if( aot40 <= -1.0 ) { return qc = 16; }
  if( trange <= -1.0 ) { return qc = 17; }
  if( vpd <= -1.0 ) { return qc = 18; }
  if( ndep<= -1.0 ) { return qc = 19; }

  return qc;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Telm45::temwritepred( ofstream fout[NUMTEM],
                               const vector<string>& predname,
                               const int& pdyr,
                               const int& pichrt,
                               const int& ntempred,
                               const int& spinoutfg,
                               const int& equilfg,
                               const int& CYCLE,
                               const int ndays[12] )
{
  // Units conversion from grams to milligrams
  const double GRAMS2MG = 1000.0;

  // Units conversion from proportion to percent
  const double PROP2PCT = 100.0;

  int i;
  int dm;
  Temdat45 tempred;


  for( i = 0; i < ntempred; ++i )
  {
    // ************** Carbon stocks in ecosystems  *************


    if( predname.at( i ) == tem.predstr.at( tem.I_LEAFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LEAFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SAPWOODC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SAPWOODC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_HEARTWOODC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_HEARTWOODC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ROOTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ROOTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LABILEC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LABILEC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SOLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SOLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTEC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTEC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTC][dm];
      }
    }


    // *************** Nitrogen stocks in ecosystems ***********

    else if( predname.at( i ) == tem.predstr.at( tem.I_LEAFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LEAFN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SAPWOODN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SAPWOODN][dm];
//        tempred.mon[dm] = output[tem.I_SAPWOODN][dm] * GRAMS2MG;

      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_HEARTWOODN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_HEARTWOODN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ROOTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ROOTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SEEDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LABILEN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LABILEN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SOLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SOLN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AVLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AVLN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VEGN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VEGN][dm];
      }
    }


    // *****************Water stocks in ecosystems *************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AVLW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AVLW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SM ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SM][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VSM ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VSM][dm] * PROP2PCT;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PCTP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SNWPCK ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SNWPCK][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SGRW][dm];
      }
    }


   // ******************** Phenology ***************************


    else if( predname.at( i ) == tem.predstr.at( tem.I_LAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_FPC][dm] * PROP2PCT;
      }
    }


    // *************** Carbon fluxes in ecosystems *************


    else if( predname.at( i ) == tem.predstr.at( tem.I_INGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_INGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_GPP][dm];
      }
    }

    // *********************** Ozone Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FOZONE ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_FOZONE][dm] * PROP2PCT;

      }
    }

    // *********************** Radiation Effects *******************
    
         else if( predname.at( i ) == tem.predstr.at( tem.I_FRDL ) )
         {
            for( dm = 0; dm < CYCLE; ++dm )
            {
               tempred.mon[dm] = output[tem.I_FRDL][dm] * PROP2PCT;

            }
         }
    
    // *********************** CO2 Effects *******************
    
       else if( predname.at( i ) == tem.predstr.at( tem.I_FCO2 ) )
       {
           for( dm = 0; dm < CYCLE; ++dm )
           {
              tempred.mon[dm] = output[tem.I_FCO2][dm] * PROP2PCT;
    
           }
       }
   
// *********************** TEMP Effects *******************

      else if( predname.at( i ) == tem.predstr.at( tem.I_TEMP ) )
      {
          for( dm = 0; dm < CYCLE; ++dm )
          {
             tempred.mon[dm] = output[tem.I_TEMP][dm] * PROP2PCT;

          }
      }

// *********************** FH2O Effects *******************

     else if( predname.at( i ) == tem.predstr.at( tem.I_FH2O ) )
      {
         for( dm = 0; dm < CYCLE; ++dm )
         {
             tempred.mon[dm] = output[tem.I_FH2O][dm] * PROP2PCT;

         }
      }

// *********************** Ozone Effects *******************

     else if( predname.at( i ) == tem.predstr.at( tem.I_FO3 ) )
     {
         for( dm = 0; dm < CYCLE; ++dm )
         {
             tempred.mon[dm] = output[tem.I_FO3][dm] * PROP2PCT;

         }
     }


    else if( predname.at( i ) == tem.predstr.at( tem.I_INNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_INNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_GPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCSC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCSEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMLEAF ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RMLEAF][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMSAPWOOD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RMSAPWOOD][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMROOT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RMROOT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMSEED ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RMSEED][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMLABILE) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RMLABILE][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRSC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRSEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RH ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RH][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DOC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_DOC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_DON][dm];
      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_NEP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NEP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NCE ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NCE][dm];
      }
    }


    // ************** Nitrogen fluxes in ecosystems ************

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCLN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCSN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCHN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_ALLOCSEEDN][dm];
      }
    }

	else if( predname.at( i ) == tem.predstr.at( tem.I_NINP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NINP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGFRTN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_INNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_INNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRLN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRSN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRHN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRRN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LTRSEEDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_MNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_MNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NMIN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NMIN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBL ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NRESORBL][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NRESORBS][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NRESORBR][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBSEED ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NRESORBSEED][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NLST ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NLST][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LCHDON][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDIN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LCHDIN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDOC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_LCHDOC][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DONPROD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_DONPROD][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DOCPROD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_DOCPROD][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NFIXN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NFIXN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NFIXS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NFIXS][dm] * GRAMS2MG;
      }
    }


    // *****************Water fluxes in ecosystems *************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGIRRIG ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGIRRIG][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_INEET ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_INEET][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_EET ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_EET][dm];
      }
    }

   else if( predname.at( i ) == tem.predstr.at( tem.I_RPERC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RPERC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SPERC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SPERC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RRUN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_RRUN][dm] - tem.rfrac[dm]*((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = output[tem.I_RRUN][dm];
      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_SRUN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_SRUN][dm] - (1-tem.rfrac[dm])*((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = output[tem.I_SRUN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_GC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_GS][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PET ) )
    {
      for ( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PET][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SNWINF ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SNWINF][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_WYLD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_WYLD][dm] - ((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = output[tem.I_WYLD][dm];
//        cout << "wyld = " << dm << " " << output[tem.I_WYLD][dm] << " " << tem.wevapd[dm] << " " << tem.wevapn[dm] << endl;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PECAN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PECAN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PESOIL ) )
   {
     for( dm = 0; dm < CYCLE; ++dm )
     {
       tempred.mon[dm] = output[tem.I_PESOIL][dm];
     }
   }



// ************** Carbon stocks in products ********************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CLIPPINGS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CLIPPINGS][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD10C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PROD10C][dm];      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD100C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PROD100C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RESIDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGSTUBC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGSTUBC][dm];
      }
    }

    // ************** Nitrogen stocks in products **************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD10N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PROD10N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD100N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PROD100N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RESIDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGSTUBN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGSTUBN][dm];
      }
    }


    // *** Carbon fluxes during agricultural conversion ********


    else if( predname.at( i ) == tem.predstr.at( tem.I_CNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VCNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VCNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SCNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SCNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SLASHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SLASHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_STANDDEADC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_STANDDEADC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VOLAC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VOLAC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CFLX ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CFLX][dm];
      }
    }


    // *** Nitrogen fluxes during agricultural conversion ******


    else if( predname.at( i ) == tem.predstr.at( tem.I_CNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CNVRTN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VCNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_VCNVRTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SCNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_SCNVRTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SLASHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	 tempred.mon[dm] = output[tem.I_SLASHN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_STANDDEADN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = output[tem.I_STANDDEADN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VOLAN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = output[tem.I_VOLAN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRETNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NRETNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NVRTNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NVRTNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NSRTNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NSRTNT][dm];
      }
    }


    // ************** Carbon fluxes to/from products ***********

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGFPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF10C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRDF10C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF100C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRDF100C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTFPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTFPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FRESIDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_FRESIDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGPRDFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD10FC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRD10FC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD100FC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRD100FC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTPRDFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RESIDFC][dm];
      }
    }

    // ************** Nitrogen fluxes to/from products *********


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGFPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF10N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRDF10N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF100N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRDF100N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTFPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTFPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FRESIDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_FRESIDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGPRDFN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD10FN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRD10FN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD100FN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_PRD100FN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_TOTPRDFN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_RESIDFN][dm];
      }
    }

    // ************** Carbon stocks in crops   *****************


    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CROPC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVEGC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATVEGC][dm];
      }
    }


    // ************** Nitrogen stocks in crops *****************


    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CROPN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVEGN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATVEGN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CSTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CSTRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATSTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATSTRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CSTON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CSTON][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATSTON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATSTON][dm];
      }
    }

    // ******************** Crop Phenology *********************

    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPLAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CROPLAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATLAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPFPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_CROPFPC][dm] * PROP2PCT;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATFPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATFPC][dm] * PROP2PCT;
      }
    }

    // ************** Carbon fluxes in croplands ***************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGINGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATINGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGINNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATINNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGGPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGGPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATGPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATGPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGRVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGRVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATRVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATRVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGRVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGRVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATRVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATRVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGLTRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGLTRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLTRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATLTRC][dm];
      }
    }

    // ************** Nitrogen fluxes in croplands *************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGINNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATINNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGVNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGVNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATVNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGLTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_AGLTRN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = output[tem.I_NATLTRN][dm] * GRAMS2MG;
      }
    }

    else
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = MISSING;
      }
    }


    // Write output data to files

    if(equilfg == 1)
    {
    if( predname.at( i ) == tem.predstr.at( tem.I_VSM )
        || predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      tempred.poutdel( fout[i],
                       col,
                       row,
                       predname.at( i ),
                       (pichrt+1),
                       cohort[pichrt].standage,
                       tem.veg.getPOTVEG(),
                       tem.veg.getCURRENTVEG(),
                       tem.veg.getSUBTYPE(),
                       tem.veg.cmnt,
                       (PROP2PCT * tem.soil.getPSIPLUSC()),
                       tem.qualcon[pdyr],
                       carea,
                       cohort[pichrt].chrtarea,
                       ttotyr[pdyr],
                       CYCLE,
                       tempred.mon,
                       region );
    }
    else
    {
      tempred.outdel( fout[i],
                      col,
                      row,
                      predname.at( i ),
                      (pichrt+1),
                      cohort[pichrt].standage,
                      tem.veg.getPOTVEG(),
                      tem.veg.getCURRENTVEG(),
                      tem.veg.getSUBTYPE(),
                      tem.veg.cmnt,
                      (PROP2PCT * tem.soil.getPSIPLUSC()),
                      tem.qualcon[pdyr],
                      carea,
                      cohort[pichrt].chrtarea,
                      ttotyr[pdyr],
                      CYCLE,
                      tempred.mon,
                      region );
    }
  }

if(equilfg == 0)
{
    if( predname.at( i ) == tem.predstr.at( tem.I_VSM )
        || predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      tempred.poutdel( fout[i],
                       col,
                       row,
                       predname.at( i ),
                       (pichrt+1),
                       cohort[pichrt].standage,
                       tem.veg.getPOTVEG(),
                       tem.veg.getCURRENTVEG(),
                       tem.veg.getSUBTYPE(),
                       tem.veg.cmnt,
                       (PROP2PCT * tem.soil.getPSIPLUSC()),
                       tem.qualcon[pdyr],
                       carea,
                       cohort[pichrt].chrtarea,
                       pdyr,
                       CYCLE,
                       tempred.mon,
                       region );
    }
    else
    {
      tempred.outdel( fout[i],
                      col,
                      row,
                      predname.at( i ),
                      (pichrt+1),
                      cohort[pichrt].standage,
                      tem.veg.getPOTVEG(),
                      tem.veg.getCURRENTVEG(),
                      tem.veg.getSUBTYPE(),
                      tem.veg.cmnt,
                      (PROP2PCT * tem.soil.getPSIPLUSC()),
                      tem.qualcon[pdyr],
                      carea,
                      cohort[pichrt].chrtarea,
                      pdyr,
                      CYCLE,
                      tempred.mon,
                      region );
    }
  }
 }

};

/* *************************************************************
************************************************************* */

void Telm45::temwritepredm( ofstream fout[NUMTEM],
                               const vector<string>& predname,
                               const int& pdyr,
                               const int& pichrt,
                               const int& ntempred,
                               const int& spinoutfg,
                               const int& equilfg,
                               const int& CYCLE,
                               const int ndays[12] )
{
  // Units conversion from grams to milligrams
  const double GRAMS2MG = 1000.0;

  // Units conversion from proportion to percent
  const double PROP2PCT = 100.0;

  int i;
  int dm;
  Temdat45 tempred;

//
//  print extras
//
//     cout << "ndays = " << ndays[0] << " " << ndays[1] << " " << ndays[2] << " " << ndays[3] << " " << ndays[4] << " " << ndays[5] << " " << ndays[6] << " " << ndays[7] << " " << ndays[8] << " " << ndays[9] << " " << ndays[10] << " " << ndays[11] << endl;
//     cout << "gpp frdl " << outpute[0][0]/ndays[0] << " " << outpute[0][1]/ndays[1] << " " << outpute[0][2]/ndays[2] << " " << outpute[0][3]/ndays[3] << " " << outpute[0][4]/ndays[4] << " " << outpute[0][5]/ndays[5] << " " << outpute[0][6]/ndays[6] << " " << outpute[0][7]/ndays[7] << " " << outpute[0][8]/ndays[8] << " " << outpute[0][9]/ndays[9] << " " << outpute[0][10]/ndays[10] << " " << outpute[0][11]/ndays[11] << endl;
//     cout << "gpp fco2 " << outpute[1][0]/ndays[0] << " " << outpute[1][1]/ndays[1] << " " << outpute[1][2]/ndays[2] << " " << outpute[1][3]/ndays[3] << " " << outpute[1][4]/ndays[4] << " " << outpute[1][5]/ndays[5] << " " << outpute[1][6]/ndays[6] << " " << outpute[1][7]/ndays[7] << " " << outpute[1][8]/ndays[8] << " " << outpute[1][9]/ndays[9] << " " << outpute[1][10]/ndays[10] << " " << outpute[1][11]/ndays[11] << endl;
//     cout << "gpp temp " << outpute[2][0]/ndays[0] << " " << outpute[2][1]/ndays[1] << " " << outpute[2][2]/ndays[2] << " " << outpute[2][3]/ndays[3] << " " << outpute[2][4]/ndays[4] << " " << outpute[2][5]/ndays[5] << " " << outpute[2][6]/ndays[6] << " " << outpute[2][7]/ndays[7] << " " << outpute[2][8]/ndays[8] << " " << outpute[2][9]/ndays[9] << " " << outpute[2][10]/ndays[10] << " " << outpute[2][11]/ndays[11] << endl;
//     cout << "gpp fozone " << outpute[3][0]/ndays[0] << " " << outpute[3][1]/ndays[1] << " " << outpute[3][2]/ndays[2] << " " << outpute[3][3]/ndays[3] << " " << outpute[3][4]/ndays[4] << " " << outpute[3][5]/ndays[5] << " " << outpute[3][6]/ndays[6] << " " << outpute[3][7]/ndays[7] << " " << outpute[3][8]/ndays[8] << " " << outpute[3][9]/ndays[9] << " " << outpute[3][10]/ndays[10] << " " << outpute[3][11]/ndays[11] << endl;
//     cout << "gpp fh2o " << outpute[4][0]/ndays[0] << " " << outpute[4][1]/ndays[1] << " " << outpute[4][2]/ndays[2] << " " << outpute[4][3]/ndays[3] << " " << outpute[4][4]/ndays[4] << " " << outpute[4][5]/ndays[5] << " " << outpute[4][6]/ndays[6] << " " << outpute[4][7]/ndays[7] << " " << outpute[4][8]/ndays[8] << " " << outpute[4][9]/ndays[9] << " " << outpute[4][10]/ndays[10] << " " << outpute[4][11]/ndays[11] << endl;


  for( i = 0; i < ntempred; ++i )
  {
    // ************** Carbon stocks in ecosystems  *************


    if( predname.at( i ) == tem.predstr.at( tem.I_LEAFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LEAFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SAPWOODC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SAPWOODC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_HEARTWOODC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_HEARTWOODC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ROOTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ROOTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LABILEC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LABILEC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SOLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SOLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTEC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTEC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTC][dm];
      }
    }


    // *************** Nitrogen stocks in ecosystems ***********

    else if( predname.at( i ) == tem.predstr.at( tem.I_LEAFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LEAFN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SAPWOODN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SAPWOODN][dm];
//        tempred.mon[dm] = output[tem.I_SAPWOODN][dm] * GRAMS2MG;

      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_HEARTWOODN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_HEARTWOODN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ROOTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ROOTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SEEDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LABILEN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LABILEN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SOLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SOLN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AVLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AVLN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VEGN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VEGN][dm];
      }
    }


    // *****************Water stocks in ecosystems *************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AVLW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AVLW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SM ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SM][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VSM ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VSM][dm] * PROP2PCT;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PCTP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SNWPCK ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SNWPCK][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SGRW][dm];
      }
    }


   // ******************** Phenology ***************************


    else if( predname.at( i ) == tem.predstr.at( tem.I_LAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_FPC][dm] * PROP2PCT;
      }
    }


    // *************** Carbon fluxes in ecosystems *************


    else if( predname.at( i ) == tem.predstr.at( tem.I_INGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_INGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_GPP][dm];
      }
    }

    // *********************** Ozone Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FOZONE ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_FOZONE][dm] * PROP2PCT;

      }
    }

// *********************** Radiation Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FRDL ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_FRDL][dm];

      }
    }

// *********************** CO2 Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FCO2 ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_FCO2][dm];

      }
    }

// *********************** TEMP Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_TEMP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_TEMP][dm];

      }
    }

// *********************** FH2O Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FH2O ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_FH2O][dm];

      }
    }

// *********************** Ozone Effects *******************

    else if( predname.at( i ) == tem.predstr.at( tem.I_FO3 ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_FO3][dm];

      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_INNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_INNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_GPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCSC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCSEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMLEAF ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RMLEAF][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMSAPWOOD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RMSAPWOOD][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMROOT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RMROOT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMSEED ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RMSEED][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RMLABILE) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RMLABILE][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRLC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRLC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRSC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSEEDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRSEEDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RH ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RH][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DOC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_DOC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_DON][dm];
      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_NEP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NEP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NCE ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NCE][dm];
      }
    }


    // ************** Nitrogen fluxes in ecosystems ************

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCLN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCSN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCHN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_ALLOCSEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_ALLOCSEEDN][dm];
      }
    }

	else if( predname.at( i ) == tem.predstr.at( tem.I_NINP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NINP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGFRTN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_INNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_INNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRLN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRLN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRSN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRHN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRRN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LTRSEEDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LTRSEEDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_MNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_MNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NMIN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NMIN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBL ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NRESORBL][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NRESORBS][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NRESORBR][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRESORBSEED ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NRESORBSEED][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NLST ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NLST][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LCHDON][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDIN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LCHDIN][dm] * GRAMS2MG;
      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_LCHDOC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_LCHDOC][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DONPROD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_DONPROD][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_DOCPROD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_DOCPROD][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NFIXN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NFIXN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NFIXS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NFIXS][dm] * GRAMS2MG;
      }
    }


    // *****************Water fluxes in ecosystems *************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGIRRIG ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGIRRIG][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_INEET ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_INEET][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_EET ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_EET][dm];
      }
    }

   else if( predname.at( i ) == tem.predstr.at( tem.I_RPERC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RPERC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SPERC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SPERC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RRUN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_RRUN][dm] - tem.rfrac[dm]*((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = outputm[tem.I_RRUN][dm];
      }
    }


    else if( predname.at( i ) == tem.predstr.at( tem.I_SRUN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_SRUN][dm] - (1-tem.rfrac[dm])*((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = outputm[tem.I_SRUN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_GC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_GS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_GS][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PET ) )
    {
      for ( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PET][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SNWINF ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SNWINF][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_WYLD ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
//        tempred.mon[dm] = output[tem.I_WYLD][dm] - ((tem.wevapd[dm]+tem.wevapn[dm])/2);
        tempred.mon[dm] = outputm[tem.I_WYLD][dm];
//        cout << "wyld = " << dm << " " << output[tem.I_WYLD][dm] << " " << tem.wevapd[dm] << " " << tem.wevapn[dm] << endl;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PECAN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PECAN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PESOIL ) )
   {
     for( dm = 0; dm < CYCLE; ++dm )
     {
       tempred.mon[dm] = outputm[tem.I_PESOIL][dm];
     }
   }



// ************** Carbon stocks in products ********************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CLIPPINGS ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CLIPPINGS][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD10C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PROD10C][dm];      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD100C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PROD100C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RESIDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGSTUBC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGSTUBC][dm];
      }
    }

    // ************** Nitrogen stocks in products **************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD10N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PROD10N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PROD100N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PROD100N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RESIDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGSTUBN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGSTUBN][dm];
      }
    }


    // *** Carbon fluxes during agricultural conversion ********


    else if( predname.at( i ) == tem.predstr.at( tem.I_CNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VCNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VCNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SCNVRTC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SCNVRTC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SLASHC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SLASHC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_STANDDEADC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_STANDDEADC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VOLAC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VOLAC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CFLX ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CFLX][dm];
      }
    }


    // *** Nitrogen fluxes during agricultural conversion ******


    else if( predname.at( i ) == tem.predstr.at( tem.I_CNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CNVRTN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VCNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_VCNVRTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SCNVRTN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_SCNVRTN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_SLASHN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
      	 tempred.mon[dm] = outputm[tem.I_SLASHN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_STANDDEADN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_STANDDEADN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_VOLAN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
         tempred.mon[dm] = outputm[tem.I_VOLAN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NRETNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NRETNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NVRTNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NVRTNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NSRTNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NSRTNT][dm];
      }
    }


    // ************** Carbon fluxes to/from products ***********

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGFPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF10C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRDF10C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF100C ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRDF100C][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTFPRDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTFPRDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FRESIDC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_FRESIDC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGPRDFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD10FC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRD10FC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD100FC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRD100FC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTPRDFC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDFC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RESIDFC][dm];
      }
    }

    // ************** Nitrogen fluxes to/from products *********


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGFPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGFPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF10N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRDF10N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRDF100N ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRDF100N][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTFPRDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTFPRDN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_FRESIDN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_FRESIDN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGPRDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGPRDFN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD10FN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRD10FN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_PRD100FN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_PRD100FN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_TOTPRDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_TOTPRDFN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_RESIDFN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_RESIDFN][dm];
      }
    }

    // ************** Carbon stocks in crops   *****************


    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CROPC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVEGC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATVEGC][dm];
      }
    }


    // ************** Nitrogen stocks in crops *****************


    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CROPN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVEGN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATVEGN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CSTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CSTRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATSTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATSTRN][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CSTON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CSTON][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATSTON ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATSTON][dm];
      }
    }

    // ******************** Crop Phenology *********************

    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPLAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CROPLAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLAI ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATLAI][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_CROPFPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_CROPFPC][dm] * PROP2PCT;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATFPC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATFPC][dm] * PROP2PCT;
      }
    }

    // ************** Carbon fluxes in croplands ***************

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGINGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATINGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATGPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATGPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGINNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATINNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATNPP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATNPP][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGGPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGGPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATGPR ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATGPR][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGRVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGRVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATRVMNT ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATRVMNT][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGRVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGRVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATRVGRW ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATRVGRW][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGLTRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGLTRC][dm];
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLTRC ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATLTRC][dm];
      }
    }

    // ************** Nitrogen fluxes in croplands *************


    else if( predname.at( i ) == tem.predstr.at( tem.I_AGINNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGINNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATINNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATINNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGVNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGVNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATVNUP ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATVNUP][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_AGLTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_AGLTRN][dm] * GRAMS2MG;
      }
    }

    else if( predname.at( i ) == tem.predstr.at( tem.I_NATLTRN ) )
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = outputm[tem.I_NATLTRN][dm] * GRAMS2MG;
      }
    }

    else
    {
      for( dm = 0; dm < CYCLE; ++dm )
      {
        tempred.mon[dm] = MISSING;
      }
    }


    // Write output data to files

if(equilfg == 1)
{
    if( predname.at( i ) == tem.predstr.at( tem.I_VSM )
        || predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      tempred.poutdelm( fout[i],
                       col,
                       row,
                       predname.at( i ),
                       (pichrt+1),
                       cohort[pichrt].standage,
                       tem.veg.getPOTVEG(),
                       tem.veg.getCURRENTVEG(),
                       tem.veg.getSUBTYPE(),
                       tem.veg.cmnt,
                       (PROP2PCT * tem.soil.getPSIPLUSC()),
                       tem.qualcon[pdyr],
                       carea,
                       cohort[pichrt].chrtarea,
                       ttotyr[pdyr],
                       CYCLE,
                       tempred.mon,
                       region );
    }
    else
    {
      tempred.outdelm( fout[i],
                      col,
                      row,
                      predname.at( i ),
                      (pichrt+1),
                      cohort[pichrt].standage,
                      tem.veg.getPOTVEG(),
                      tem.veg.getCURRENTVEG(),
                      tem.veg.getSUBTYPE(),
                      tem.veg.cmnt,
                      (PROP2PCT * tem.soil.getPSIPLUSC()),
                      tem.qualcon[pdyr],
                      carea,
                      cohort[pichrt].chrtarea,
                      ttotyr[pdyr],
                      CYCLE,
                      tempred.mon,
                      region );
    }
  }

if(equilfg == 0)
    {
    if( predname.at( i ) == tem.predstr.at( tem.I_VSM )
        || predname.at( i ) == tem.predstr.at( tem.I_PCTP ) )
    {
      tempred.poutdelm( fout[i],
                       col,
                       row,
                       predname.at( i ),
                       (pichrt+1),
                       cohort[pichrt].standage,
                       tem.veg.getPOTVEG(),
                       tem.veg.getCURRENTVEG(),
                       tem.veg.getSUBTYPE(),
                       tem.veg.cmnt,
                       (PROP2PCT * tem.soil.getPSIPLUSC()),
                       tem.qualcon[pdyr],
                       carea,
                       cohort[pichrt].chrtarea,
                       pdyr,
                       CYCLE,
                       tempred.mon,
                       region );
    }
    else
    {
      tempred.outdelm( fout[i],
                      col,
                      row,
                      predname.at( i ),
                      (pichrt+1),
                      cohort[pichrt].standage,
                      tem.veg.getPOTVEG(),
                      tem.veg.getCURRENTVEG(),
                      tem.veg.getSUBTYPE(),
                      tem.veg.cmnt,
                      (PROP2PCT * tem.soil.getPSIPLUSC()),
                      tem.qualcon[pdyr],
                      carea,
                      cohort[pichrt].chrtarea,
                      pdyr,
                      CYCLE,
                      tempred.mon,
                      region );
    }
   }
 } 

};

/* *************************************************************
************************************************************* */

int Telm45::transqc( int& maxyears,
                         int& totyr,
                         const int& CYCLE,
                         double plantc[366] )
{

  int dm;
  int qc;
  double sumcarbon = ZERO;
  qc = ACCEPT;

  if( totyr < 0 || totyr >= maxyears ) { return qc = 30; }
  for( dm = 0; dm < CYCLE; ++dm ) { sumcarbon += plantc[dm]; }
  if( sumcarbon <= 0.1 ) { return qc = TQCZEROFLAG; }

  return qc;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Telm45::updateTEMmonth( const int& equil,
                                 const int& totsptime,
                                 const int& pdyr,
                                 const int& pdm,
                                 const int& pichrt,
                                 const int& CYCLE )
{
  // Pass cohort characteristics information to TEM

  getTEMCohortState( pichrt,
                     CYCLE );

  if( ACCEPT == cohort[pichrt].qc
      && ACCEPT == cohort[pichrt].tqc )
  {
//   cout << "inside if loop" << endl;
    // Pass monthly climate information to TEM

    tem.atms.setGIRR( climate[clm.I_GIRR][pdm][pdyr] );
    tem.atms.setCLDS( climate[clm.I_CLDS][pdm][pdyr] );
    tem.atms.setNIRR( climate[clm.I_NIRR][pdm][pdyr] );
    tem.atms.setPAR(  climate[clm.I_PAR][pdm][pdyr] );
    tem.atms.setTAIR( climate[clm.I_TAIR][pdm][pdyr] );
    tem.atms.setTAIRD( climate[clm.I_TAIRD][pdm][pdyr] );
    tem.atms.setTAIRN( climate[clm.I_TAIRN][pdm][pdyr] );
    tem.atms.setRAIN( climate[clm.I_RAIN][pdm][pdyr] );
    tem.atms.setPREC( climate[clm.I_PREC][pdm][pdyr] );
    tem.atms.setSNOWFALL( climate[clm.I_SNWFAL][pdm][pdyr] );
    tem.atms.setCO2( climate[clm.I_CO2][pdm][pdyr] );
    tem.atms.setAOT40( climate[clm.I_AOT40][pdm][pdyr] );
    tem.atms.setNDEP( adep[0][pdyr] );
    tem.atms.setTRANGE( climate[clm.I_TRANGE][pdm][pdyr] );
    tem.atms.setVPR( climate[clm.I_VPR][pdm][pdyr] );
    tem.atms.setVPDD( climate[clm.I_VPDD][pdm][pdyr] );
    tem.atms.setVPDN( climate[clm.I_VPDN][pdm][pdyr] );
    tem.atms.setWS10( climate[clm.I_WS10][pdm][pdyr] );
    tem.atms.setDAYL( climate[clm.I_DAYL][pdm][pdyr] );
//    cout << "dayl_3 = " << tem.atms.getDAYL() << endl;
    tem.atms.setMXTAIR( mxtair[pdyr] );
    tem.atms.yrprec = yrprec[pdyr];



//   cout << "climate = " << tem.atms.getGIRR() << " " << tem.atms.getCLDS() << " " << tem.atms.getNIRR() <<
//        " " << tem.atms.getPAR() << " " << tem.atms.getTAIR() << " " << tem.atms.getRAIN() << " " <<
//      tem.atms.getSNOWFALL() << " " << tem.atms.getCO2() << " " << tem.atms.getAOT40() << " " <<
//       tem.atms.getAOT40B() << " " << tem.atms.getTRANGE() << " " << tem.atms.getVPD() << " " <<
//       tem.atms.getDAYL() << " " <<
//       tem.atms.getMXTAIR() << " " << 
//       tem.atms.yrprec << endl;
    tem.baseline = 0;
    tem.wrtyr = -99;
//
//  BSF CHANGE
    tem.totyr = clm.startyr
//                - totsptime - 1
                - totsptime
                + (pdyr * tem.diffyr);

//    cout << "totyr = " << clm.startyr << " " << totsptime << " " << pdyr << " " << tem.diffyr << " " << tem.totyr << endl;

    // Allow optimum N fertilization of crops after 1950
/*    if(tem.ag.cmnt == 17 || tem.ag.cmnt == 19) {
    tem.ag.fertflag = 1;
    }
    else {
    tem.ag.fertflag = 0;
    } */
//  cout << "fertflag = " << tem.ag.fertflag << endl;
//    tem.ag.fertflag = 0;
//    if( 1 == tem.ag.fertflag && tem.totyr >= 1950 )
    if( 1 == tem.ag.fertflag )
    {
      tem.ag.fert1950flag = 1;
    }
    else
    {
      tem.ag.fert1950flag = 0;
    }


//    if( 1 == tem.ag.irrgflag && tem.totyr >= 1950 )
    if( 1 == tem.ag.irrgflag )
    {
      tem.ag.irrg1950flag = 1;
    }
    else
    {
      tem.ag.irrg1950flag = 0;
    }


    // Run the Terrestrial Ecosystem Model (TEM) under
    //   transient conditions


    wrtyr = tem.monthlyTransient( pdyr,
                                  pdm,
                                  tem.tol,
                                  CYCLE,
                                  pichrt );

//    if( 1901 == tem.totyr ) { exit( -1 ); }
    // Save TEM output to telmnt[0].output

    if(intervalflag == 0)
      {
      outputTEMday( pdm );
      }
    else if (intervalflag == 1)
      {
      outputTEMmonth( pdm, CYCLE);
      }


    ttotyr[pdyr] = tem.totyr;
//    cout << "totyr1 = " << ttotyr[pdyr] << " " << pdm << endl;
  } // End of qc == ACCEPT and tqc = ACCEPT
  else
  {
    if( (CYCLE-1) == pdm )
    {
      // Set missing values to telmnt[0].output

      setTEMmiss( pdyr,
                  equil,
                  totsptime,
                  pichrt,
                  CYCLE );
//      cout << "totyr2 = " << ttotyr[pdyr] << " " << pdm << endl;
    }
  }

  // Save TEM state for cohort

  saveTEMCohortState( pichrt,
                      CYCLE );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Telm45::writeCohortState( ofstream& ofstate,
                                   const int& pichrt,
                                   const int& CYCLE )
{
  int i;
  int dm;

  ofstate << col << " ";
  ofstate << row << " ";
  ofstate << (pichrt+1) << " ";

  ofstate << cohort[pichrt].srcCohort << " ";
  ofstate << cohort[pichrt].standage << " ";
  ofstate << cohort[pichrt].chrtarea << " ";
  ofstate << cohort[pichrt].potveg << " ";
  ofstate << cohort[pichrt].currentveg << " ";
  ofstate << cohort[pichrt].subtype << " ";
  ofstate << cohort[pichrt].cmnt << " ";


  for( i = 0; i < MAXSTATE; ++i )
  {
    ofstate << cohort[pichrt].y[i] << " ";
    ofstate << cohort[pichrt].prevy[i] << " ";
  }

  ofstate << cohort[pichrt].agcmnt << " ";

  ofstate << cohort[pichrt].aggrowdd << " ";

  ofstate << cohort[pichrt].agkd << " ";

  ofstate << cohort[pichrt].agprvstate << " ";

  ofstate << cohort[pichrt].agstate << " ";

  ofstate << cohort[pichrt].avlh2o << " ";

  ofstate << cohort[pichrt].awcapmm << " ";

  ofstate << cohort[pichrt].c2n << " ";

  ofstate << cohort[pichrt].cneven << " ";

  ofstate << cohort[pichrt].convrtflx.carbon << " ";
  ofstate << cohort[pichrt].convrtflx.nitrogen << " ";
  ofstate << cohort[pichrt].standdead.carbon << " ";
  ofstate << cohort[pichrt].standdead.nitrogen << " ";
  ofstate << cohort[pichrt].vola.carbon << " ";
  ofstate << cohort[pichrt].vola.nitrogen << " ";

  ofstate << cohort[pichrt].cropResidue.carbon << " ";
  ofstate << cohort[pichrt].cropResidue.nitrogen << " ";

  ofstate << cohort[pichrt].croptopt << " ";

  ofstate << cohort[pichrt].distmnthcnt << " ";
  ofstate << cohort[pichrt].disturbflag << " ";
  ofstate << cohort[pichrt].disturbmonth << " ";

  ofstate << cohort[pichrt].fertflag << " ";

  ofstate << cohort[pichrt].firemnthcnt << " ";

  ofstate << cohort[pichrt].formPROD10.carbon << " ";
  ofstate << cohort[pichrt].formPROD10.nitrogen << " ";

  ofstate << cohort[pichrt].formPROD100.carbon << " ";
  ofstate << cohort[pichrt].formPROD100.nitrogen << " ";

  ofstate << cohort[pichrt].FRI << " ";

  for( dm = 0; dm < CYCLE; ++dm )
  {
    ofstate << cohort[pichrt].initPROD1[dm].carbon << " ";
    ofstate << cohort[pichrt].initPROD1[dm].nitrogen << " ";
  }

  for( i = 0; i < 10; ++i )
  {
    ofstate << cohort[pichrt].initPROD10[i].carbon << " ";
    ofstate << cohort[pichrt].initPROD10[i].nitrogen << " ";
  }

  for( i = 0; i < 100; ++i )
  {
    ofstate << cohort[pichrt].initPROD100[i].carbon << " ";
    ofstate << cohort[pichrt].initPROD100[i].nitrogen << " ";
  }

  ofstate << cohort[pichrt].irrgflag << " ";

  ofstate << cohort[pichrt].kd << " ";

  ofstate << cohort[pichrt].natseedC << " ";

  ofstate << cohort[pichrt].natseedSTRN << " ";

  ofstate << cohort[pichrt].natseedSTON << " ";

  ofstate << cohort[pichrt].natsoil << " ";

  ofstate << cohort[pichrt].nattopt << " ";

  ofstate << cohort[pichrt].newtopt << " ";

  ofstate << cohort[pichrt].nretent << " ";

  ofstate << cohort[pichrt].nsretent << " ";

  ofstate << cohort[pichrt].nvretent << " ";

  ofstate << cohort[pichrt].prev2tair << " ";

  ofstate << cohort[pichrt].prevco2 << " ";

  ofstate << cohort[pichrt].prevCropResidue.carbon << " ";
  ofstate << cohort[pichrt].prevCropResidue.nitrogen << " ";

  ofstate << cohort[pichrt].prevPROD1.carbon << " ";
  ofstate << cohort[pichrt].prevPROD1.nitrogen << " ";

  ofstate << cohort[pichrt].prevPROD10.carbon << " ";
  ofstate << cohort[pichrt].prevPROD10.nitrogen << " ";

  ofstate << cohort[pichrt].prevPROD100.carbon << " ";
  ofstate << cohort[pichrt].prevPROD100.nitrogen << " ";

  ofstate << cohort[pichrt].prevspack << " ";

  ofstate << cohort[pichrt].prevtair << " ";

  ofstate << cohort[pichrt].prod10par << " ";

  ofstate << cohort[pichrt].prod100par << " ";

  ofstate << cohort[pichrt].productYear << " ";

  ofstate << cohort[pichrt].prvchrtarea << " ";

  ofstate << cohort[pichrt].prvcropnpp << " ";

  ofstate << cohort[pichrt].rprec << " ";
  
  ofstate << cohort[pichrt].rdemandc << " ";
  
  ofstate << cohort[pichrt].rdemandn << " ";

  ofstate << cohort[pichrt].reet << " ";
  
  ofstate << cohort[pichrt].rlabilec << " ";
  
  ofstate << cohort[pichrt].rlabilen << " ";
  
  ofstate << cohort[pichrt].rgpp << " ";
  
  ofstate << cohort[pichrt].ringpp << " ";

  ofstate << cohort[pichrt].rnpp << " ";

  ofstate << cohort[pichrt].rltrc << " ";

  ofstate << cohort[pichrt].rpleaf << " ";

  ofstate << cohort[pichrt].rrh << " ";

  ofstate << cohort[pichrt].rtair << " ";

  ofstate << cohort[pichrt].rtairphi << " ";

  ofstate << cohort[pichrt].rphi << " ";

  ofstate << cohort[pichrt].phicnt << " ";

  ofstate << cohort[pichrt].qc << " ";

  ofstate << cohort[pichrt].sconvert << " ";

  ofstate << cohort[pichrt].sconvrtflx.carbon << " ";
  ofstate << cohort[pichrt].sconvrtflx.nitrogen << " ";

  ofstate << cohort[pichrt].slash.carbon << " ";
  ofstate << cohort[pichrt].slash.nitrogen << " ";

  ofstate << cohort[pichrt].slashpar << " ";

  ofstate << cohort[pichrt].tillflag << " ";

  ofstate << cohort[pichrt].topt << " ";

  ofstate << cohort[pichrt].tqc << " ";

  ofstate << cohort[pichrt].vconvert << " ";

  ofstate << cohort[pichrt].vconvrtflx.carbon << " ";
  ofstate << cohort[pichrt].vconvrtflx.nitrogen << " ";

  ofstate << cohort[pichrt].vrespar << " ";

//  ofstate << cohort[pichrt].wiltpt << " ";

  ofstate << cohort[pichrt].yrltrc << " ";
  ofstate << cohort[pichrt].yrltrn << " ";
  
  ofstate << cohort[pichrt].yrpleaf << " ";
  ofstate << cohort[pichrt].yrpsapwood << " ";
  ofstate << cohort[pichrt].yrproot << " ";
  
  ofstate << cohort[pichrt].yralloclc << " ";
  ofstate << cohort[pichrt].yrallocsc << " ";
  ofstate << cohort[pichrt].yrallocrc << " ";
  
  ofstate << cohort[pichrt].yrrmleaf << " ";
  ofstate << cohort[pichrt].yrrmsapwood << " ";
  ofstate << cohort[pichrt].yrrmroot << " ";
  
  ofstate << endl;

};

