/* *************************************************************
****************************************************************
TSOILD.CPP - object describing general characteristics of
                soil

Modifications:

20060106 - DWK created by modifying Tsoilflux2b.cpp
20070105 - TWC changed name to tsoil45

2007:  TWC/BSF
    resetMonthlyFluxes: evap = 0
	resetYrFluxes: yrevap = 0
	setSWP function
	updateHydrology: avlh2o instead of sh2o; adapt function
	   to allow ineet = eet instead of from xeet function


****************************************************************
************************************************************* */

#include<cstdio>

  using std::printf;

#include<iostream>

  using std::cout;
  using std::ios;
  using std::cerr;
  using std::endl;

#include<fstream>

  using std::ifstream;
  using std::ofstream;

#include<cstdlib>

  using std::exit;
  using std::atof;
  using std:: atoi;

#include<cmath>

  using std::exp;
  using std::pow;
  using std::log;

#include<string>

  using std::string;


#include "tsoild.h"

/* *************************************************************
************************************************************* */

Tsoil45::Tsoil45( void ) : ProcessXML45()
{

  text  = -99;
  wsoil = -99;

  pctsand = MISSING;
  pctsilt = MISSING;
  pctclay = MISSING;
  psiplusc = MISSING;


  awcapmm = MISSING;
  fldcap = MISSING;
  wiltpt = MISSING;
  totpor = MISSING;

  snowpack = MISSING;

  avlh2o = MISSING;
  moist = MISSING;
  pcfc = MISSING;
  pctp = MISSING;
  vsm = MISSING;

  rgrndh2o = MISSING;
  sgrndh2o = MISSING;

  snowinf = MISSING;
  rperc = MISSING;
  sperc = MISSING;
  rrun = MISSING;
  srun = MISSING;
  h2oyld = MISSING;

  org.carbon = MISSING;
  org.nitrogen = MISSING;

  availn = MISSING;

  yrsnowpack = MISSING;

  yravlh2o = MISSING;
  yrsmoist = MISSING;
  yrpctp = MISSING;
  yrvsm = MISSING;
  meanvsm = MISSING;

  yrrgrndh2o = MISSING;
  yrsgrndh2o = MISSING;

  yrsnowinf = MISSING;
  yrrperc = MISSING;
  yrsperc = MISSING;
  yrrrun = MISSING;
  yrsrun = MISSING;
  yrh2oyld = MISSING;

  yrorgc = MISSING;
  yrorgn = MISSING;
  yrc2n = MISSING;

  yravln = MISSING;

  ninput = MISSING;
  yrnin = MISSING;

  nlost = MISSING;
  yrnlost = MISSING;
  yrlchdin = MISSING;

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

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::getrootz( const string& ecd )
{
  ifstream infile;
  int dcmnt;
  int comtype;


  infile.open( ecd.c_str(), ios::in );

  if( !infile )
  {
    cerr << endl;
    cerr << "Cannot open " << ecd << " for root ECD input";
    cerr << endl;
    exit( -1 );
  }

  getXMLrootNode( infile, "rootzECD" );

  for( dcmnt = 1; dcmnt < MAXCMNT; ++dcmnt )
  {

    comtype = getXMLcommunityNode( infile, "rootzECD" );

    if( comtype >= MAXCMNT )
    {
      cerr << endl << "comtype is >= MAXCMNT" << endl;
      cerr << "comtype cannot be greater than " << (MAXCMNT-1);
      cerr << " in leafECD" << endl;
      exit( -1 );
    }


    rootzc[comtype] = getXMLcmntArrayDouble( infile,
                                             "rootzECD",
                                             "rootzc",
                                             comtype );

    minrootz[comtype] = getXMLcmntArrayDouble( infile,
                                               "rootzECD",
                                               "minrootz",
                                               comtype );

    endXMLcommunityNode( infile );
  }

  if ( dcmnt < MAXCMNT )
  {
    cerr << endl << " Parameters found for only " << dcmnt;
    cerr << " community types out of a maximum of ";
    cerr << (MAXCMNT-1) << " types in rootzECD" << endl;
    exit( -1 );
  }

  infile.close();

};

/* *************************************************************
************************************************************* */
/* *************************************************************
 * ************************************************************* */

void Tsoil45::infilt( const double& sm )
{


  infiltrate = 6.44 * pow((sm - 0.01)/(0.25 - 0.01),(2.0/0.16)+3);


};


/* *************************************************************
************************************************************* */

void Tsoil45::lake( const double& tair,
                    const double& prec,
                    double& rain,
                    double& snowfall,
       		    const double& pet,
                    double& eet )
{

  rgrndh2o = ZERO;
  sperc = ZERO;
  snowpack = ZERO;
  sgrndh2o = ZERO;
  moist = ZERO;

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

  eet = pet;
  h2oyld = prec - pet;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::percol( const double& rain)
{

  double extra;
  double recharge;
  sperc = ZERO;
  rperc = ZERO;

  if( avlh2o > awcapmm )
  {
//    extra = 10.0*(avlh2o - awcapmm);
    extra = (avlh2o - awcapmm);
    recharge = rain + snowinf;
    if( recharge <= ZERO ) { recharge = 0.001; }
    sperc = extra*snowinf/recharge;
    rperc = extra*rain/recharge;
//    cout << "rperc = " << rperc << endl;
    // moisture loss above field capacity with a characteristic time of a 3 days
  } 

/*  recharge = rain + snowinf;
  if( recharge <= ZERO ) { recharge = 0.001; }
  if( (avlh2o + rain + snowinf - eet) > awcapmm )
  {
    extra = rain + snowinf + avlh2o - awcapmm - eet;

    sperc = snowinf * extra / recharge;

    rperc = rain * extra / recharge;
  } */

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::resetMonthlyFluxes( void )
{
  // Reset monthly fluxes to zero

  // Nitrogen fluxes

  ninput = ZERO;
  nlost = ZERO;

  // Water fluxes

// Comment out next two lines in MITTEM as these values come
//   from CLM rather than TEM

  ineet = ZERO;
  eet = ZERO;

  rperc = ZERO;
  sperc = ZERO;
  rrun = ZERO;
  srun = ZERO;

  snowinf = ZERO;
  h2oyld = ZERO;


};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::resetYrFluxes( void )
{
  // Reset annual fluxes and summary variables to zero

  // Annual carbon storage

  yrorgc = ZERO;

  // Annual nitrogen storage

  yrorgn = ZERO;
  yravln = ZERO;

  // Annual water storage

  yravlh2o = ZERO;
  yrsmoist = ZERO;
  yrvsm = ZERO;
  yrpctp = ZERO;
  yrsnowpack = ZERO;
  yrrgrndh2o = ZERO;
  yrsgrndh2o = ZERO;


  // Annual nitrogen fluxes

  yrnin = ZERO;
  yrnlost = ZERO;
  yrlchdin = ZERO;

  // Annual water fluxes

  yrineet = ZERO;
  yreet = ZERO;
  yrrperc = ZERO;
  yrsperc = ZERO;
  yrrrun = ZERO;
  yrsrun = ZERO;

  yrsnowinf = ZERO;
  yrh2oyld = ZERO;

  yrnin = ZERO;
  yrnlost = ZERO;
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tsoil45::rrunoff( const double& rgrndh2o )
{

  double rrunof;

  rrunof = 0.5 * (rgrndh2o + rperc);

  return rrunof;

};

/* *************************************************************
************************************************************* */

void Tsoil45::runu( const double& prec,
                    const double& sm, 
                    const double& vsm)
{
//runoff from the upper soil layer
  double i0;

  i0 = 252 * (1-pow((1-vsm),1/0.008));
 
  if (i0 + prec >= 252) { runoffs = prec - 250 + sm;}
  else { runoffs = prec - 250 + sm + 250 * pow((1-(i0 + prec)/250),(1+0.008));}

};


/* *************************************************************
 * ************************************************************* */

void Tsoil45::runl( const double& sm )
{
//runoff from the lower soil layer(baseflow)
  if (sm >= 0 && sm <= 1200) { runoffb = ((0.96 * 0.34)/(0.96 * 1250)) *sm;}
  else { runoffb = ((0.96 * 0.34)/(0.96 * 1250)) *sm + (0.34*24 - 0.000077*0.34*24/0.96)* 
                        pow((sm - 0.96*1250)/(1250-1200),2);}

};


/* *************************************************************
************************************************************* */

void Tsoil45::setSWP( const double& vsm )
{
  double pota, potb;
  
  pota = 100.0*exp(-4.396-0.0715*pctclay-0.000488*pctsand*pctsand-
		0.00004285*pctsand*pctsand*pctclay);
  
  potb = -3.14 -0.00222*pctclay*pctclay -
		0.00003484*pctsand*pctsand*pctclay;
		
  if( vsm > 0.01) { swp = -0.001 * pota * pow(vsm,potb);}
  else { swp = -0.001 * pota * pow( 0.01 , potb);}
  
  gm = (1.0 + exp(0.5*(-1.5)))/(1.0 + exp(0.5*(-1.5 - swp)));
  
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::setKH2O( const double& vsm,
                       const int& moistlim )
{
  double vfc;

  if( 0 == moistlim )
  {
    vfc = pcfldcap * 0.01;

    kh2o = pow( vfc, 3.0 );
//    kh2o = 1.0;
  }
  else
  {
    if( vsm > 1.0 ) { kh2o = 1.0; }
    else { kh2o = pow( vsm, 3.0 ); }
//    kh2o = 1.0 + 0.5*exp( 20.0*((pcwiltpt+pcfldcap)/200.0-vsm));
//    kh2o = 1.0/kh2o;
  }


};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Tsoil45::showecd( void )
{

  cout << endl << "                   SOIL CHARACTERISTICS OF SITE";
  cout << endl << endl;
  printf( "PSAND    = %5.2lf      PSILT = %5.2lf      PCLAY = %5.2lf\n",
          pctsand,
          pctsilt,
          pctclay );

  printf( "POROSITY = %5.2lf   PCFLDCAP = %5.2lf   PCWILTPT = %5.2lf\n",
          pctpor,
          pcfldcap,
          pcwiltpt );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tsoil45::snowmelt( const double& elev,
                          const double& tair,
                          const double& prevtair,
                          const double& psnowpack )
{

  double snowflux = ZERO;

  if( tair >= -1.0 )
  {
    if( elev <= 500.0 ) { snowflux = psnowpack;}
    else
    {
      if( prevtair < -1.0 ) { snowflux = 0.5 * psnowpack; }
      else { snowflux = psnowpack; }
    }
  }

  if( snowflux < ZERO ) { snowflux = ZERO; }

  return snowflux;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tsoil45::srunoff( const double& elev,
                         const double& tair,
                         const double& prevtair,
                         const double& prev2tair,
                         const double& sgrndh2o )
{

  double srunof = ZERO;

  if( tair >= -1.0 )
  {
    if( prevtair < -1.0 )
    {
      srunof = 0.1 * (sgrndh2o + sperc);
    }
    else
    {
      if( prev2tair < -1.0 )
      {
	    if( elev <= 500.0 )
        {
          srunof = 0.5 * (sgrndh2o + sperc);
        }
	    else { srunof = 0.25 * (sgrndh2o + sperc); }
      }
      else { srunof = 0.5 * (sgrndh2o + sperc); }
    }
  }

  return srunof;

};


/* *************************************************************
************************************************************* */

void Tsoil45::updateHydrology( const double& elev,
                               const double& tair,
                               const double& prevtair,
                               const double& prev2tair,
                               const double& rain,
                               const double& pet,
                               const double& avlh2o,
                               const double& rgrndh2o,
                               const double& sgrndh2o,
                               const int& irrgflag,
                               double& irrigate,
                               const int& pdm )
{
  // veg.pet is actually eet, so just set ineet and eet to pet

  ineet = pet;
  eet = ineet;

  // Determine percolation of rain water (rperc) and snow melt
  //   water (sperc) through the soil profile

  percol( rain );

  // Determine runoff derived from rain (soil.rrun) and/or
  //   snow (soil.srun)

  rrun = rrunoff( rgrndh2o );

  srun = srunoff( elev,
                  tair,
                  prevtair,
                  prev2tair,
                  sgrndh2o );

};


/* *************************************************************
************************************************************* */
void Tsoil45::updateDOCLEACH( const double& doc,
                              const double& sh2o )

{

   lchdoc = (doc/(sh2o+rrun+srun))*(rrun+srun);

};


/* *************************************************************
************************************************************* */

void Tsoil45::updateNLosses( const int& pdcmnt,
                             const double& h2oloss,
                             const double& availn,
                             const double& soilh2o )
{
   nlost = (availn/(soilh2o+rrun+srun))*(rrun+srun);
/*  if( soilh2o > ZERO )
  {
    nlost = availn / (soilh2o+h2oloss);

    nlost *= (h2oloss + (rootz * 1000.0))
             / (rootz * 1000.0);

    nlost *= nloss[pdcmnt];
  } 
  else { nlost = ZERO; } */ 
nlost *= nloss[pdcmnt];
//cout << "nlost = " << nlost << " " << availn << " " << soilh2o << " " << rrun << " " << srun << " " << nloss[pdcmnt] << endl;
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tsoil45::updateRootZ( const int& pdcmnt,
                             const double& sh2o,
                             const double& finerootc )

{

  double pota, potb;
  
  //Saxton Equations 
  pota = 100.0*exp(-4.396-0.0715*pctclay-0.000488*pctsand*pctsand-
		0.00004285*pctsand*pctsand*pctclay);
  
  potb = -3.14 -0.00222*pctclay*pctclay -
		0.00003484*pctsand*pctsand*pctclay;

  rootz = rootzc[pdcmnt];
  if( rootz < minrootz[pdcmnt] ) { rootz = minrootz[pdcmnt]; }

  pctpor = 100.0*(0.332 - 0.0007251*pctsand + 0.1276*log10(pctclay));
  if(pdcmnt == 8)
  {
    pcfldcap = 100.0*pow(15.0/pota, 1.0/potb);
    pcwiltpt = 100.0*pow(3000.0/pota, 1.0/potb);
  }
  else
  {
  pcfldcap = 100.0*pow(33.0/pota, 1.0/potb);
  pcwiltpt = 100.0*pow(1500.0/pota, 1.0/potb);
  }
  //pcwiltpt = 100.0*pow(10000.0/pota, 1.0/potb);
// Inverting the saxton equation psi = -0.001*pota*vsm^potb
// And using the definition of fldcap = -0.033 MPa; wiltpt = -1.5 MPa

  totpor  = rootz * pctpor * 10.0;
  fldcap  = rootz * pcfldcap * 10.0;
  wiltpt  = rootz * pcwiltpt * 10.0;

  awcapmm = fldcap - wiltpt;

  return (sh2o / (rootz * 1000.0));

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tsoil45::xtext( const int& pdcmnt,
                     const double& pctsilt,
                     const double& pctclay )
{

  double pota, potb;
  
  totpor = fldcap = wiltpt = MISSING;
  awcapmm =  MISSING;

  psiplusc = (pctsilt + pctclay) * 0.01;
  if( psiplusc < 0.01 ) { psiplusc = 0.01; }
  
  //Saxton Equations  
  pota = 100.0*exp(-4.396-0.0715*pctclay-0.000488*pctsand*pctsand-
		0.00004285*pctsand*pctsand*pctclay);
  
  potb = -3.14 -0.00222*pctclay*pctclay -
		0.00003484*pctsand*pctsand*pctclay;

  rootz = rootzc[pdcmnt];
  if( rootz < minrootz[pdcmnt] ) { rootz = minrootz[pdcmnt]; }

  pctpor = 100.0*(0.332 - 0.0007251*pctsand + 0.1276*log10(pctclay));
  pcfldcap = 100.0*pow(33.0/pota, 1.0/potb);
  pcwiltpt = 100.0*pow(1500.0/pota, 1.0/potb);
// Inverting the saxton equation psi = -0.001*pota*vsm^potb
// And using the definition of fldcap = -0.033 MPa; wiltpt = -1.5 MPa

  totpor  = rootz * pctpor * 10.0;
  fldcap  = rootz * pcfldcap * 10.0;
  wiltpt  = rootz * pcwiltpt * 10.0;

  awcapmm = fldcap - wiltpt;

};

