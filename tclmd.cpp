/* *************************************************************
****************************************************************
TCLMD.CPP - describes climate module used by TEM

20060126 - DWK created by modifying tclm50b5.cpp
20060126 - DWK changed include from tclm50b5.h to tclm437.h
20060126 - DWK changed TEMclm50:: to TEMclmflux::
20060126 - DWK changed inheritance from Atmosphere50 to
           Atmosphere43 in TEMclmflux()
20060126 - DWK added I_AOT to TEMclmflux()
20060126 - DWK added void initO3(), double mkd40() and
           void setO3Flags()
20070105 - TWC changed name to tclm45
2007 - TWC/BSF summary
  predstr array: add VPD, TRANGE
  inito3: add basline ozone file
  initTrange
  initVPD

****************************************************************
************************************************************* */

#include<cstdio>

  using std::FILE;
  using std::fscanf;

#include<iostream>

  using std::cout;
  using std::ios;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<iomanip>

  using std::setprecision;

#include<cstdlib>

  using std::exit;

#include<cmath>

  using std::cos;
  using std::pow;
  using std::sin;

#include<vector>
  using std::vector;

#include<string>

  using std::string;


#include "tclmd.h"

/* *************************************************************
************************************************************* */

Tclm45::Tclm45() : Atms45(), predstr( NUMATMS )
{

// Initialize predstr array

  predstr.at( I_GIRR ) = "GIRR";
  predstr.at( I_NIRR ) = "NIRR";
  predstr.at( I_PAR ) = "PAR";
  predstr.at( I_CLDS ) = "CLDINESS";
  predstr.at( I_TAIR ) = "TAIR";
  predstr.at( I_PREC ) = "PREC";
  predstr.at( I_RAIN ) = "RAIN";
  predstr.at( I_SNWFAL ) = "SNOWFALL";
  predstr.at( I_CO2 ) = "CO2";
  predstr.at( I_VPR ) = "VPR";
  predstr.at( I_TRANGE ) = "TRANGE";
  predstr.at( I_TAIRD ) = "TAIRD";
  predstr.at( I_TAIRN ) = "TAIRN";
  predstr.at( I_VPDD ) = "VPDD";
  predstr.at( I_VPDN ) = "VPDN";
  predstr.at( I_WS10 ) = "WS10";
  predstr.at( I_AOT40 ) = "AOT40";
  predstr.at( I_DAYL ) = "DAYL";

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tclm45::mkclds( const double& girr,
                         const double& nirr )
{

  double clouds;

  if( nirr >= (0.76 * girr) ) { return clouds = ZERO; }
  else
  {
    clouds = 1.0 - (((nirr/girr) - 0.251)/0.509);
    clouds *= 100.0;
  }
  if ( clouds > 100.0 ) { clouds = 100.0; }

  return clouds;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tclm45::mkd40( const double& lon,
                        const double& lat,
		        const string& contnent,
                        const double& o3,
		        const int& pdm )
{
  int k;
  int itmp;
  double d40;
  double tozone;
  double tmin;
  double tmax;
  double y;
  double tdum1;
  double tdum2;
  double slope1;
  double slope2;

  if ( lon > -90.0
       && lon < -55.0
       && lat > 10.0
       && lat < 90.0
       && "NAMERICA" == contnent )
  {
    //  Eastern North America
    tmin = 6.;
    tmax = 15.;
    y = 12.8;
  }
  else if ( lon > -124.0
            && lon < -112.0
            && lat > 47.0
            && lat < 55.0
            && "NAMERICA" == contnent )
  {
    //  Northwest North America
    tmin = 5.;
    tmax = 15.;
    y = 11.0;
  }
  else if ( lon > -180.0
            && lon < -123.0
            && lat > 53.0
            && lat < 90.0
            && "NAMERICA" == contnent )
  {
    //  Alaska and North
    tmin = 6.;
    tmax = 17.;
    y = 4.3;
  }
  else if ( lon > -180.0
            && lon < -90.0
            && lat > 10.0
            && lat < 90.0
            && "NAMERICA" == contnent )
  {
    //  West North America
    tmin = 6.;
    tmax = 16.;
    y = 7.9;
  }
  else if ( lon > -15.0
            && lon < 180.0
            && lat > 10.0
            && lat < 90.0
            && ("EUROPE" == contnent
            || "ASIA" == contnent) )
  {
    //  Eur-Asia
    tmin = 5.;
    tmax = 15.;
    y = -1.04 * lat + 75.72;
    if ( y < 0.0 ) { y = 0.0; }
  }
  else
  {
    itmp=-999;
  }

  //  Next loop

  if ( 0 == pdm
       || 1 == pdm
       || 2 == pdm
       || 3 == pdm
       || 10 == pdm
       || 11 == pdm
       || -999 == itmp )
  {
    d40 = 0.0;
  }
  else
  {
    tdum1 = tmin+(tmax-tmin)/2.0;
    tdum2 = ((24.-tmax)+tmin)/2.0;
    slope1 = (2*y)/(tmax-tmin);
    slope2 = (-1*2*y)/((24-tmax)+tmin);

    for ( k = 0; k < 24; ++k )
    {
      if ( k <= (tmin-1) )
      {
        tozone = ((slope2)*k+(-1*slope2*(tmin-tdum2)))+ o3;
      }

      if ( k > (tmin-1) && k <= (tmax-1) )
      {
        tozone = ((slope1)*k+(-1*slope1*tdum1))+ o3;
      }

      if ( k > (tmax-1) )
      {
        tozone = ((slope2)*k+(-1*slope2*(tmax+tdum2)))+ o3;
      }

      if ( tozone >= 40.0 && k >= 6 && k <= 18 )
      {
        d40 += (tozone-40.0);
      }
    }

    d40 *= 31.0;
  }

  return d40;

};

/* *************************************************************
************************************************************* */


/* **************************************************************
************************************************************** */

double Tclm45::xgirr( const float& plat,
                        const int& dayin,
                        double& psumday,
                        const int ndays[] )
{

  const double pi = 3.141592654;                // Greek "pi"
  const double sp = 1368.0 * 3600.0 / 41860.0;  // solar constant

  double lambda;
  double sumd;
  double sig;
  double eta;
  double sinbeta;
  double sb;
  double sotd;
  int day;
  int dm;
  int hour;
  int ndayst[12];
  double gross;
  double sumdaymonth;
  int numday;


  ndayst[0] = 0;
  ndayst[1] = 31;
  ndayst[2] = 59;
  ndayst[3] = 90;
  ndayst[4] = 120;
  ndayst[5] = 151;
  ndayst[6] = 181;
  ndayst[7] = 212;
  ndayst[8] = 243;
  ndayst[9] = 273;
  ndayst[10] = 304;
  ndayst[11] = 334;  

  lambda = plat * pi / 180.0;

for(dm=0;dm<12;dm++)
{
if(dayin >= ndayst[0] && dayin  < ndayst[1]) { numday = ndays[0]; }
if(dayin >= ndayst[1] && dayin  < ndayst[2]) { numday = ndays[1]; }
if(dayin >= ndayst[2] && dayin  < ndayst[3]) { numday = ndays[2]; }
if(dayin >= ndayst[3] && dayin  < ndayst[4]) { numday = ndays[3]; }
if(dayin >= ndayst[4] && dayin  < ndayst[5]) { numday = ndays[4]; }
if(dayin >= ndayst[5] && dayin  < ndayst[6]) { numday = ndays[5]; }
if(dayin >= ndayst[6] && dayin  < ndayst[7]) { numday = ndays[6]; }
if(dayin >= ndayst[7] && dayin  < ndayst[8]) { numday = ndays[7]; }
if(dayin >= ndayst[8] && dayin  < ndayst[9]) { numday = ndays[8]; }
if(dayin >= ndayst[9] && dayin  < ndayst[10]) { numday = ndays[9]; }
if(dayin >= ndayst[10] && dayin  < ndayst[11]) { numday = ndays[10]; }
else if (dayin >= ndayst[11]) {numday = ndays[11]; }

if(dayin == ndayst[dm])
  {
  gross = ZERO;
  sumdaymonth = ZERO;
  }
}

//  ++psumday;
//  for( day = 0; day < ndays[pdm]; ++day )
//  for( day = 0; day < numday; ++day )
//  {
//    ++psumday;
//    sumdaymonth += 1.0;
    
    sumd = 0;
    sig = -23.4856*cos(2 * pi * ((dayin) + 10.0)/365.25);
    sig *= pi / 180.0;

    for( hour = 0; hour < 24; ++hour )
    {
      eta = (double) ((hour+1) - 12) * pi / 12.0;
      sinbeta = sin( lambda ) * sin( sig )
                + cos( lambda ) * cos( sig ) * cos( eta );

      sotd = 1 - (0.016729
             * cos( 0.9856 * ((dayin) - 4.0) * pi / 180.0) );

      sb = sp * sinbeta / pow( sotd, 2.0 );
      if( sb >= ZERO ) { sumd += sb; }
    }

//    gross += sumd;
    gross = sumd;
//  }

//  gross /= sumdaymonth;

//  cout << "xgirr = " << gross << " " <<  dayin << " " << numday << endl;
  return gross;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tclm45::xnirr( const double& clds, const double& girr )
{

  double pnirr;

//  if( clds >= ZERO )
//  {
    pnirr = girr * (0.251 + (0.509*(1.0 - clds/100.0)));
//  }
//  else { nirr = MISSING; }

  return pnirr;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tclm45::xpar( const double& clds, const double& nirr )
{

  double ppar;
  
  if( clds >= ZERO )
  {
      ppar = nirr * ((0.2 * clds / 100.0) + 0.45);
  }
//  else { par = MISSING; }
  else {ppar = 0.45 * nirr; }

  return ppar;

};


