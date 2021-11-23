//
/* **************************************************************
*****************************************************************
ATMSD.CPP - object describes physical characteristics of the
	         atmosphere

Modifications:

20060126 - DWK created by modifying atms50b5.cpp
20060126 - DWK changed include from atms50b5.h to atms437.h
20060126 - DWK changed Atmosphere50:: to class Atmsflux::
20070105 - TWC changed Atmsflux:: to class Atms45
2007 - TWC/BSF: Summary
  using std:sin
  Add three new function: daylength

****************************************************************
************************************************************* */

#include<cmath>
  using std::exp;
  using std::sin;
  using std::cos;
  using std::pow;
  using std::acos;
  using std::tan;
  using std::fabs;

#include<iostream>

    using std::cout;
    using std::ios;
    using std::cerr;
    using std::endl;


/* *************************************************************
************************************************************* */

#include "atmsd.h"

Atms45::Atms45()
{
  // Number of days per month

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

};

/* **************************************************************
************************************************************** */

double Atms45::daylength( const float& lat, const int& day )
{

  double m,pi,astrday;

  pi = 3.14159;

  if(day >= 354) {astrday = day-354;}
  else
  { astrday = day + 11;}

  m = 1-tan(pi*lat/180.0)*tan(((pi/180.0)*23.439)*cos((pi/182.625)*astrday));
  dayl = (acos(1-m)*(180.0/pi)/180.0)*24.0;
//  cout << "diag = " << day << " " << dayl << " " << lat << " " << m << endl;
  return dayl;

};

/* *************************************************************
************************************************************* */
/* **************************************************************
************************************************************** */
double Atms45::lwrad( const double& tair,
                      const double& vpr,
                      const double& nirr,
                      const double& girr )

{

    double epsc, ccor;
    double lwout;
    
    epsc = 1.24 * pow((vpr/(tair+273.15)),1.0/7.0);

    ccor = 1.6 * (nirr/girr) - 0.2;

//cout << "nirr, girr = " << nirr << " " << girr << endl;
    if (ccor < 0.2) {ccor = 0.2;}
    if (ccor > 1.0) {ccor = 1.0;}

    lwout = (1.0 - epsc) * ccor * 5.67e-8 * pow((tair+273.15),4.);
    
    return lwout;

};

/* **************************************************************
************************************************************** */

/* *************************************************************
************************************************************* */

void Atms45::petjh( const double& nirr,
                          const double& tair,
                          const int& pdm )
{

  double f;
  double rt;

  f = ((9.0/5.0) * tair) + 32.0;
  rt = nirr * 0.016742;
//  pet = ((0.014*f) - 0.37) * rt * ndays[pdm];
  pet = ((0.014*f) - 0.37) * rt;

  if ( pet <= ZERO ) { pet = 0.001; }

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Atms45::precsplt( const double& prec,
                             const double& tair,
                             double& rain,
                             double& snowfall )
{


/* *************************************************************
	Willmott's assumptions on snow/rain split:
************************************************************** */

  if ( tair >= -1.0 )
  {
    rain = prec;
    snowfall = ZERO;
  }
  else
  {
    rain = ZERO;
    snowfall = prec;
  }

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Atms45::resetMonthlyFluxes( void )
{
  // Reset monthly fluxes to zero

  pet = ZERO;

};
/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Atms45::resetYrFluxes( void )
{
  // Reset annual fluxes and summary variables to zero

  yrrain = ZERO;
  yrsnowfall = ZERO;
  yrpet = ZERO;

};


