/* **************************************************************
*****************************************************************
TMCRBd.CPP - object describing characteristics of soil
                 microbes used in the Terrestrial Ecosystem Model
                 (TEM)

Modifications:

20060126 - DWK created by modifying tmcrb50b5.cpp
20060126 - DWK changed include from tmcrb50b5.h to tmcrb437.h
20060126 - DWK changed Tmicrobe50:: to Tmicrobeflux::
20060126 - DWK changed inheritance from ProcessXML50 to
           ProcessXML43 in Tmicrobeflux()
20060126 - DWK deleted double tsoil and int tsoilflg from
           setDQ10()
20070105 - TWC changed name to tmcrb45
2007 - TWC/BSF
     getvegecd: add rhq10, rhalpha, rhbeta, rhgamma,
	      rhqref, rhtref (instead of tref, qref, alpha,
		  beta, gamma)
     nminxclm: add condt: if(nmin+availn < 0) nmin = -availn
	 rhxclm: replace dq10 with pdq10
	 setDQ10LaRS (setNew DQ10 in TEM44): new LARS function

*****************************************************************
************************************************************** */

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
  using std::log;
  using std::pow;

#include<vector>

  using std::vector;

#include<string>

  using std::string;
  
//#define RHMOIST_SECH
#define RHMOIST_WFPS

#include "tmcrbd.h"

/* **************************************************************
************************************************************** */

Tmcrb45::Tmcrb45() : ProcessXML45()
{

  // Initialize variables to MISSING values

  dq10 = MISSING;

  nuptake = MISSING;
  netnmin = MISSING;

  rh = MISSING;

  yrnmin = MISSING;
  yrnuptake = MISSING;

  yrrh = MISSING;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::getvegecd( const string& ecd )
{
  ifstream infile;
  int dcmnt;
  int comtype;


  infile.open( ecd.c_str(), ios::in );

  if( !infile )
  {
    cerr << endl << "Cannot open " << ecd;
    cerr << " for microbe ECD input" << endl;

    exit( -1 );
  }

  getXMLrootNode( infile, "microbeECD" );

  for( dcmnt = 1; dcmnt < MAXCMNT; ++dcmnt )
  { 
    comtype = getXMLcommunityNode( infile, "microbeECD" );
       

    if( comtype >= MAXCMNT )
    {
      cerr << endl << "comtype is >= MAXCMNT" << endl;
      cerr << "comtype cannot be greater than " << (MAXCMNT-1);
      cerr << " in microbeECD" << endl;
      exit( -1 );
    }


    rhalpha[comtype]= getXMLcmntArrayDouble( infile,
                                             "microbeECD",
                                             "rhalpha",
                                             comtype );

    rhbeta[comtype]= getXMLcmntArrayDouble( infile,
                                            "microbeECD",
                                            "rhbeta",
                                            comtype );

    rhgamma[comtype]= getXMLcmntArrayDouble( infile,
                                             "microbeECD",
                                             "rhgamma",
                                             comtype );

    rhqref[comtype]= getXMLcmntArrayDouble( infile,
                                            "microbeECD",
                                            "rhqref",
                                            comtype );

    rhtref[comtype]= getXMLcmntArrayDouble( infile,
                                            "microbeECD",
                                            "rhtref",
                                            comtype );

    kn2[comtype] = getXMLcmntArrayDouble( infile,
                                          "microbeECD",
                                          "kn2",
                                          comtype );

    moistmin[comtype] = getXMLcmntArrayDouble( infile,
                                               "microbeECD",
                                               "moistmin",
                                               comtype );

    moistopt[comtype] = getXMLcmntArrayDouble( infile,
                                               "microbeECD",
                                               "moistopt",
                                               comtype );

    moistmax[comtype] = getXMLcmntArrayDouble( infile,
                                               "microbeECD",
                                               "moistmax",
                                               comtype );

    docfr[comtype] = getXMLcmntArrayDouble( infile,
                                               "microbeECD",
                                               "docfr",
                                               comtype );

    docdon[comtype] = getXMLcmntArrayDouble( infile,
                                               "microbeECD",
                                               "docdon",
                                               comtype );


    endXMLcommunityNode( infile );
  }

  if( dcmnt < MAXCMNT )
  {
    cerr << endl << " Parameters found for only " << dcmnt;
    cerr << " community types out of a maximum of ";
    cerr << (MAXCMNT-1) << " types in microbeECD" << endl;

    exit( -1 );
  }

  infile.close();

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tmcrb45::nminxclm( const int& pdcmnt,
                             const double& soilh2o,
                             const double& soilorgc,
                             const double& soilorgn,
                             const double& availn,
                             const double& ksoil,
                             const int& nopen,
                             const double& immbadd,
                             const double& volac,
                             const double& volan )
{
//  double gmin;
  double immb;
  double nmin;
  double tcnsoil;

//cout << "nterms = " << immbadd << " " << volac << " " << volan << endl;

  tcnsoil = cnsoil[pdcmnt];


  nuptake = ZERO;
  nmin = ZERO;
//  if ( soilorgc > ZERO && soilorgn > ZERO )
  if ( soilorgc > 0.01 && soilorgn > 0.01 ) //BSF COMBO
  {
    immb  = (availn * ksoil) / soilh2o;
//    cout << "immb ori = " << immb << endl;
    immb  = ((availn + immbadd) * ksoil) / soilh2o;
//    cout << "immb after = " << immb << " " << availn << " " << immbadd << " " << soilorgn << endl;
    immb /= (kn2[pdcmnt] + immb);

//    cout << "immb before = " << immb << endl;
//    immb += immbadd;
//    cout << "immb after = " << immb << endl;

    nuptake = nup * immb * decay * rh;

//    if (nuptake > availn)
//    {
//       nuptake = availn;
//    }

    gmin = (soilorgn / soilorgc) * rh;

    if(nopen == 1)
    {
     nmin = 0.99 * gmin - nuptake;
    }
    else
    {
    nmin   = gmin - nuptake;
    }

    if ( nmin >= ZERO )
    {
      nmin *= (soilorgn/soilorgc) * tcnsoil;
    }
    else
    {
      nmin *= (soilorgc/soilorgn) / tcnsoil;
      if(nmin+availn < ZERO)
      {
        nmin = -availn;
      }
    }

  }

  return nmin;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::resetEcds( const int& pcmnt,
                            const double& psiplusc )
{
  kdc =  kdb[pcmnt];

  if( kdc < ZERO ) { kdc = ZERO; }


  nup =  nupb[pcmnt];

  if( nup < ZERO ) { nup = ZERO; }


  // Determine the "decay" parameter

  decay = 0.26299
          + (1.14757 * propftos[pcmnt])
          - (0.42956 * pow( propftos[pcmnt], 2.0 ));

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::resetMonthlyFluxes( void )
{
  // Reset monthly fluxes to zero

  // Carbon fluxes

  rh = ZERO;

  // Nitrogen fluxes

  nuptake = ZERO;
  netnmin = ZERO;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::resetYrFluxes( void )
{
  // Reset annual fluxes and summary variables to zero

  // Carbon fluxes

  yrrh = ZERO;

  // Nitrogen fluxes

  yrnuptake = ZERO;
  yrnmin = ZERO;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tmcrb45::rhxclm( const double& soilorgc,
                           const double& pdq10,
                           const double& moist,
                           const double& rltrc,
                           const double& volac )
{

//
//  Change to increase time to decomposition
//
//  Shree Update
//  double fsoil = 0.5;
  double fsoil = 0.0;

  rh = (kd/30.5) * soilorgc * moist * pdq10;
//  cout << "rh = " << rh << " " << kd << " " << soilorgc << " " << moist << " " << pdq10 << endl;
//  cout << "rh = " << rh << " " << rltrc << " " << rrh << endl;
  return rh += fsoil*(rltrc-rrh);

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::setDQ10LaRS( const int& pdcmnt,
                          const double& ptair )
{
  // setDQ10LaRS: effect of temperature on decomposition,
  //              as per LaRS respiration formula

  double rhbase;
  double rhexp;
  double rhdenm;
  double rhxT;

  rhbase = rhqref[pdcmnt]*exp(-rhalpha[pdcmnt]*(ptair - rhtref[pdcmnt]));
  rhexp = (ptair - rhtref[pdcmnt])/10.0;
  rhdenm = 1.0 + exp(rhbeta[pdcmnt] - ptair) + exp(ptair - rhgamma[pdcmnt]);

  rhxT = pow(rhbase, rhexp)/rhdenm;

  dq10 = rhxT;
//  rhTLaRS = pow( rhq10[pdcmnt], ptair / 10.0 );

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tmcrb45::setDQ10LT( const int& pdcmnt,
                         const double& ptair,
                         const double& ptopt )
{
  // setDQ10LT: effect of temperature on decomposition,
  //              as per Lloyd & Taylor (1994) respiration formula

  double rhxT,rhxtopt;

  rhxtopt = 1.0;

  rhxT = exp(308.56*(1.0/(273.15+25.0-227.13)-1.0/(273.15+ptair-227.13)));

//  rhxtopt = exp(308.56*(1.0/(273.15+25.0-227.13)-1.0/(273.15+ptopt-227.13)));

  dq10 = rhxT;

  dq10 /=  rhxtopt;

  return dq10;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tmcrb45::setRHMOIST( const int& pdcmnt,
                               const double& pctfldcap,
                               const double& pctwiltpt,
                               const double& pctpor,
                               const double& vsm,
                               const int& moistlim )
{

/* rhxh2o: effect of moisture on decomposition */

  double rhxh2o;
  double vfc;
  double wfps;
  
  if( 0 == moistlim )
  {
    vfc = pctfldcap*0.01;

    rhxh2o = (vfc - moistmin[pdcmnt])
             * (vfc - moistmax[pdcmnt]);

    rhxh2o /= rhxh2o - pow( (vfc - moistopt[pdcmnt]), 2.0 );
  }
  else
  {
    rhxh2o = (vsm - moistmin[pdcmnt])
             * (vsm - moistmax[pdcmnt]);

    rhxh2o /= rhxh2o - pow( (vsm - moistopt[pdcmnt]), 2.0 );
    
    #ifdef RHMOIST_WFPS
      wfps = 100.0*vsm/pctpor;
      rhxh2o = 0.2+0.8*wfps*(1.0-wfps)/(wfps*(1.0-wfps) + (0.6-wfps)*(0.6-wfps));
    #endif  
    
    #ifdef RHMOIST_SECH
//      vsmopt = (pctpor - 15.0)/100.0;
//      vsmdenm = (pctpor - 15.0 - pctwiltpt)/100.0;
//      rhxh2o = 0.2 + 0.8 * exp( -((vsm-vsmopt)*(vsm-vsmopt))/(0.4809*vsmdenm*vsmdenm) );
      
      rhxh2o = 1.0 + 0.5*exp( 20.0*((pctwiltpt+pctfldcap)/200.0-vsm)) + 0.5*exp(20.0*(vsm-0.008*pctpor));
      rhxh2o = 1.0/rhxh2o;
//      cout << " vsmopt = " << vsmopt << " " << rhxh2o << " " << vsm << " " << pctwiltpt << " " << pctpor << endl;
//      exit(-1); 
    #endif  
  }

  if( rhxh2o < ZERO ) { rhxh2o = ZERO; }

  return rhxh2o;

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::showecd( const int& pdcmnt )
{

  cout << endl << "   MICROBIAL PARAMETERS INFLUENCED BY CLIMATE";
  cout << endl << endl;
  printf( "          KN2 = %6.4lf\n", kn2[pdcmnt] );
  printf( "     MOISTMIN = %8.6lf\n", moistmin[pdcmnt] );
  printf( "     MOISTOPT = %8.6lf\n", moistopt[pdcmnt] );
  printf( "     MOISTMAX = %8.6lf\n", moistmax[pdcmnt] );
  printf( "       CNSOIL = %5.2lf\n", cnsoil[pdcmnt] );

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

void Tmcrb45::updateDynamics( const int& pdcmnt,
                                 const double& pctfldcap,
                                 const double& pctwiltpt,
                                 const double& pctpor,
                                 const double& soilorgc,
                                 const double& soilorgn,
                                 const double& soilh2o,
                                 const double& vsm,
                                 const double& availn,
                                 const int& moistlim,
                                 const int& tillflag,
                                 const double& tillfactor,
                                 const double& ksoil,
                                 const double& rltrc,
                                 const int& nopen,
                                 const double& immbadd,
                                 const double& volac,
                                 const double& volan )
{
  double rhmoist;

  // rhmoist: effect of moisture on decomposition

  rhmoist = setRHMOIST( pdcmnt, pctfldcap, pctwiltpt, pctpor, vsm, moistlim );

  decomp = rhxclm( soilorgc, dq10, rhmoist, rltrc, volac );


  if( decomp < ZERO ) { decomp = ZERO; }

//  DOC production term

   docprod = docfr[pdcmnt] * decomp;
   rh = decomp - docprod;

  // Adjust decomposition rate due to tillage effects

  if( 1 == tillflag )
  {
    rh *= tillfactor;
  }

   donprod = docprod * soilorgn/soilorgc;
//  cout << "finalrh = " << tillflag << " " << rh << endl;

  // Determine Net N Mineralization (microbe.netnmin)

  netnmin = nminxclm( pdcmnt,
                      soilh2o,
                      soilorgc,
                      soilorgn,
                      availn,
                      ksoil,
                      nopen,
                      immbadd,
                      volac,
                      volan );

};

/* **************************************************************
************************************************************** */


/* **************************************************************
************************************************************** */

double Tmcrb45::yrkd( const int& nfeed,
                         const double& yrltrc,
                         const double& yrltrn,
                         const int& pdcmnt )
{

  double yrkd;

  if ( yrltrn <= 0.000000000000001 ) { return  yrkd = ZERO; }
  if ( yrltrc < ZERO )
  {
    cout << "YRLTRC is < 0.0 in microbe.yrkd()" << endl;

    exit( -1 );
  }
  if ( 0 == nfeed ) { yrkd = kdc; }
  else
  {
    yrkd = kdc * pow( (yrltrc/yrltrn), -0.784 )
           / pow( lcclnc[pdcmnt], -0.784 );
  }

  return yrkd;
};

