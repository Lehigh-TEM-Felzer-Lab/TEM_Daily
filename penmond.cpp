/* **************************************************************
*****************************************************************
PENMOND.CPP - object computes EET using Penman-Monteith

Modifications:

20040330 - BSF created
20070105 - TWC renamed penmonflux to penmon45
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
  using std::atoi;

#include<cmath>

  using std::exp;
  using std::pow;
  using std::sqrt;
  using std::atan;
  using std::min;
  using std::log;

#include<string>

  using std::string;

#include "penmond.h"


Penmon45::Penmon45() : ProcessXML45()
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

  CPAIR = 1010.0; // air specific heat (J kg^-1)
  RHOAIR = 1.292; // air density (kg m^-3)
  PSY = 0.662; // psychrometric constant
  LAMBDA = 2442000.0; // latent heat of vaporization of water, J kg^-1
  DAYtoS = 86400.0; // conversion factor from days to seconds
  CO2REF = 340.0; // reference CO2 value
  SCALEH = 8000.0; // atmospheric scale height in meters


};


/* *************************************************************
************************************************************* */

double Penmon45::gcpet( const int& pdcmnt,
                         const int& dm,
                         const double& lai,
                         const double& gpp,
                         const double& rcanmm,
                         const double& vpr,
                         const double& vpd,
                         const double& co2,
                         const double& pdayl )

{

  double gc, gcmmol;
  double fd;
  double anetmmol;


//  gppmmol = gpp / (12.0e-3 * 24.0 * 3600. * ndays[dm]);

//  anetmmol = gpp / (12.0e-3 * pdayl * 3600. * ndays[dm]) - rcanmm;

  anetmmol = gpp / (12.0 * pdayl * 3600. /1000.0 ) - rcanmm;
  
  if(anetmmol <= ZERO) { anetmmol = ZERO; }

  fd = vpr/(vpr+vpd); // using relative humidity now

  gcmmol = gsmin[pdcmnt]*lai+ gsa[pdcmnt]* anetmmol * fd / (co2/1.0e6);

// gc in m/s -- see Tyree, 2005 or Nobel, 1991 for conversion factor

  gc = gcmmol / 41550.0;

  if(gc < ZERO) {gc = ZERO;}


  return gc;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Penmon45::hydraulics( const int& pdcmnt,
                           const int& phen,
                           const double& lai,
                           const double& psla,
                           const double& stemc,
                           const double& rootc,
                           const double& availh2o,
                           const double& awcapmm,
                           const double& prec,
                           ofstream& temdiagfile )

{                        
  /* hydraulics: 
  calculates water stress as a function of soil water status
  and leaf specific conductivity (kplant/lai)

  also calculates partials of water stress function
  with respect to stemc and rootc 
  */
  
  double gstem, kha, khb, lsc;
  double dlsc_dleafc, dlsc_drootc, dlsc_dstemc, dfws_dlsc;

  double wfrac;
  double power;

//  wfrac = (availh2o+prec)/awcapmm;
//  wfrac = availh2o/awcapmm;
//  BSF COMBO START
  power = 1.0;
  wfrac = availh2o/awcapmm;
if(phen == 2 &&  (pdcmnt != 8 || pdcmnt != 7))
//if(phen == 2 )
{
  wfrac = wfrac *pow((wfrac/(wfrac+0.1)),2.0);
  power = 2.0;
//  power = 1.3;
}
//BSF COMBO END

  if( wfrac > 1.0 ) { wfrac = 1.0; }
  if( wfrac < 0.001 ) { wfrac = 0.001; }
  
  // determine above- and below-ground hydraulic conductances: mmol m^-2 s^-1 MPa^-1
  gstem = kstem[pdcmnt] / (1000.0 * rhostem[pdcmnt] * vegh * vegh );
  kha = gstem * stemc;
  if( stemc < 0.01 ) { kha = gstem * 0.01; }  
  khb = groot[pdcmnt] * rootc;
  if( rootc < 0.01 ) { khb = groot[pdcmnt] * 0.01; } 
  
  kplant = kha * khb / (kha + khb); // series sum of conductances

  if(lai > 0.01) 
  { 
    lsc = kplant/lai; 
    dlsc_drootc = 1.0/lai*groot[pdcmnt]*kha*kha/((kha+khb)*(kha+khb));
    dlsc_dstemc = 1.0/lai*gstem*khb*khb/((kha+khb)*(kha+khb));
    dlsc_dleafc = -lsc*psla/lai;
  }
  else 
  { 
    lsc = kplant/0.01;
    dlsc_drootc = 100.0*groot[pdcmnt]*kha*kha/((kha+khb)*(kha+khb));
    dlsc_dstemc = 100.0*gstem*khb*khb/((kha+khb)*(kha+khb));
    dlsc_dleafc = -100.0*lsc*psla; 
  }

  if( lsc < lscmin[pdcmnt] ) { lsc = lscmin[pdcmnt]; }
  // not accurate, but prevents unwanted cases from arising, 
  // and provides allocation incentives

//  fws = 1.0 - exp(-(lsc/lscmin[pdcmnt])*wfrac);

if(pdcmnt == 8 || pdcmnt == 7)
{
  if (wfrac < 0.4 && wfrac > 0.3) {fws = 1.0 - exp(-(lsc/lscmin[pdcmnt])*pow(wfrac,power))+0.1 ;}
  if (wfrac < 0.3 ) {fws = 1.0 - exp(-(lsc/lscmin[pdcmnt])*pow(wfrac,power))+0.2 ;}
  else {fws = 1.0 - exp(-(lsc/lscmin[pdcmnt])*pow(wfrac,power));}  // BSF COMBO
}
else
{
  fws = 1.0 - exp(-(lsc/lscmin[pdcmnt])*pow(wfrac,power)); // BSF COMBO
}
//temdiagfile << "fws = " << fws << " " << wfrac << " " << lsc << " " << lscmin[pdcmnt] << endl;
//cout << "fws = " << fws << " " << wfrac << " " << lsc << " " << lscmin[pdcmnt] << " " << availh2o << " " << awcapmm << endl;
  dfws_dlsc = wfrac/lscmin[pdcmnt]*exp(-(lsc/lscmin[pdcmnt])*wfrac);
  
  dfws_dleafc = dfws_dlsc*dlsc_dleafc;
  
  dfws_drootc = dfws_dlsc*dlsc_drootc;
  
  dfws_dstemc = dfws_dlsc*dlsc_dstemc;

  //fws = pow( wfrac, lscmin[pdcmnt]/lsc );

  //dfws_drootc = -lscmin[pdcmnt]*log(wfrac)*fws/(lsc*lsc)*
  //             groot[pdcmnt]*kha*kha/((kha+khb)*(kha+khb));
               
  //dfws_dstemc = -lscmin[pdcmnt]*log(wfrac)*fws/(lsc*lsc)*
  //             gstem*khb*khb/((kha+khb)*(kha+khb));
               
  if( dfws_dstemc < ZERO ) { dfws_dstemc = ZERO; }
  if( dfws_drootc < ZERO ) { dfws_drootc = ZERO; }


};



/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Penmon45::shuttlewallace( const int& pdcmnt,
                  const double& nirr,
                  const double& lwout,
                  const double& lai,
                  const double& tmean,
                  const double& vpd,
                  const int& dm,
                  const double& gc,
                  const double& snowcover,
                  const double& etimes,
                  const double& precip,
                  const double& esoilmmmo,
                  const double& elev,
                  const int& phen )

{

/*    Description of Shuttleworth & Wallace (1985) interactive model:

  Source height, where temperature (tmean) and vapor pressure deficit (vpd)
are known, is assumed coupled to a canopy airspace (via r_aa), which interacts
with both the plant canopy (via r_ac), and the soil surface (via r_as).  Leaves
and soil each release latent energy at a rate given by the penman monteith
equation, but based on the canopy airspace vapor pressure deficit (vpdo)
instead of the source height vpd.
*/

  double npmc, dpmc, npms, dpms, pms, pmc;
  double wc, ws;
  double vpdo; // vapor pressure deficit of canopy airspace
  
  double ecanmmmo; // evaporation from canopy interception
  double maxh2ocanopy = 0.1; // maximum canopy water storage per unit leaf area
  double lwcor; // lwout corrected for canopy evaporation energy flux 

  double rn, rnsoil; // net radiation - total and soil
  double totalbedo;

  double r_sc; // stomatal resistance, s m^-1
  double resc, resa, ress; // intermediate calculation
  double etsw;  //final ET

//  double num,den;
  double exp_h;
  
//cout << "nirr in penmond = " << nirr << endl;

  setBETA( tmean );
  exp_h = exp(-elev/SCALEH);

  if(gc < 0.00001) {r_sc = 1.0/0.00001;}
  else { r_sc = 1.0/gc;}
//  else { r_sc = 1.0/(gc/2.0);}
  
//    if(esoilmmmo != 0.0 && nirr > 0.0)
//    {
//    cout << "diag1 = " << esoilmmmo << " " << r_ss <<  endl;
//    num = beta*rnsoil + CPAIR*RHOAIR*vpdo/r_as;
//    den = esoilmmmo*LAMBDA/(etimes*ndays[dm]);
//    r_ss = ((((num/den)-beta)*(1/PSY)-1)*r_as);
//    cout << "diag2 = " << r_ss << endl;
//    }

  resa = (beta + PSY*exp_h) * r_aa;
  ress = (beta + PSY*exp_h) * r_as + PSY * exp_h * r_ss;
  resc = (beta + PSY*exp_h) * r_ac + PSY * exp_h * r_sc;


  if(esoilmmmo != 0.0) {
   wc = 1.0;
  }
  else {
  wc = (1.0 + resc * resa / (ress * (resc + resa)));
  }
  wc = 1.0/wc;


  if(esoilmmmo != 0.0) {
   ws = (1.0 + resa)/resc;
  }
  else {
  ws = (1.0 + ress * resa / (resc * (ress + resa)));
  }
  ws = 1.0/ws;
  
//
// calculate evaporation from canopy interception, first in mm mo^-1, 
// then convert to W m^-2 and deduct from net radiation prior to other calculations  
//

//  ecanmmmo = ndays[dm]*(1.0 - exp(-0.005*precip))
//  ecanmmmo = (1.0 - exp(-0.005*precip))
//   BSF  Adapt daily to monthly
//                      *(1.0 - exp(-kext[pdcmnt]*lai))*maxh2ocanopy*lai;
  
ecanmmmo = 0.2 * precip * (1 - expl(-kext[pdcmnt]*lai));
if(pdcmnt == 5 ) 
{
  ecanmmmo = 0.06 * precip * (1 - expl(-kext[pdcmnt]*lai));
}
if(pdcmnt == 7 )
{
  ecanmmmo = 0.06 * precip * (1 - expl(-kext[pdcmnt]*lai));
//  ecanmmmo = 0.11 * precip * (1 - expl(-kext[pdcmnt]*lai));
}
if(pdcmnt == 8 )
{
  ecanmmmo = 0.06 * precip * (1 - expl(-kext[pdcmnt]*lai));
//  ecanmmmo = 0.2 * precip * (1 - expl(-kext[pdcmnt]*lai));
}

if(pdcmnt == 15)
{
ecanmmmo = 0.2 * precip * (1 - expl(-kext[pdcmnt]*lai));
//ecanmmmo = 0.7 * precip * (1 - expl(-kext[pdcmnt]*lai));
}
if(ecanmmmo < 0.0) { ecanmmmo = 0.0;}
//cout << "diag = " << ecanmmmo << " " << precip << " " << throughfall << " " << dm << endl;
//  lwcor = lwout + (ecanmmmo)*LAMBDA/(etimes*ndays[dm]);   
  lwcor = lwout + (ecanmmmo)*LAMBDA/(etimes);   

//cout << "pen = " << resa << " " << ress << " " << resc << " " << wc << " " << ws << " " << ecanmmmo << " " << lwcor << endl;
//cout << "lwcor = " << lwout << " " << ecanmmmo << " " << etimes << " " << ndays[dm] << endl;
//
//  snowcover is considered to have a constant albedo of 0.5
//  soil/snow/substrate assumed to store 20% of rnsoil as per p. 845
//  Shuttleworth and Wallace (1985)
//
  if(snowcover > 0.0)
  {
    totalbedo = albedo[pdcmnt] * (1.0 - exp(-kext[pdcmnt] * lai)) +
       0.5 * exp(-kext[pdcmnt] * lai);
    //rnsoil = 0.8 * (0.5 * nirr - lwout) * exp(-kext[pdcmnt] * lai);
    rnsoil = 0.8 * (0.5 * nirr - lwcor) * exp(-kext[pdcmnt] * lai);
  }
  else
  {
    totalbedo = albedo[pdcmnt];
    //rnsoil = 0.8 * ((1.0 - totalbedo) * nirr - lwout) * exp(-kext[pdcmnt] * lai);
    rnsoil = 0.8 * ((1.0 - totalbedo) * nirr - lwcor) * exp(-kext[pdcmnt] * lai);
  }
//  rnsoil = rnsoil - esoilmmmo*LAMBDA/(etimes*ndays[dm]);
  rnsoil = rnsoil - esoilmmmo*LAMBDA/(etimes);

//cout << "diag = " << rnsoil << " " << nirr << " " << lwcor << " " << lai << " " << esoilmmmo << endl;
    //rn = nirr * (1.0 - totalbedo) - lwout;
//    rn = nirr * (1.0 - totalbedo) - lwcor - esoilmmmo*LAMBDA/(etimes*ndays[dm]);
    rn = nirr * (1.0 - totalbedo) - lwcor - esoilmmmo*LAMBDA/(etimes);

    npmc = beta*rn+(CPAIR*RHOAIR*exp_h*vpd-beta*r_ac*rnsoil)/(r_aa+r_ac);
    dpmc = beta+PSY*exp_h*(1.0+r_sc/(r_aa+r_ac));
    npms = beta*rn+(CPAIR*RHOAIR*exp_h*vpd-beta*r_as*(rn-rnsoil))/(r_aa+r_as);
    dpms = beta+PSY*exp_h*(1.0+r_ss/(r_aa+r_as));

    pmc = (npmc/dpmc);
    if(esoilmmmo != 0.0) {
      pms = 0.0;
    }
    else {
    pms = (npms/dpms);
    }

    etsw = (wc*pmc + ws*pms); // W m^-2

//cout << "etsw = " << etsw << " " << wc << " " << pmc << " " << ws << " " << pms << endl;
    // having obtained the overall latent energy flux, we now can determine vpdo,
    //   and thus the respective contributions of canopy and soil
    
    if(etsw > ZERO) // sensible heat flux: residual 
    {
      shflux = rn - 0.25 * rnsoil - etsw;
    }
    
    vpdo = vpd + r_aa*(beta*rn - (beta + PSY*exp_h)*etsw)/(CPAIR*RHOAIR*exp_h);

//  cout << "diag = " << vpdo << " " << vpd << " " << r_aa << " " << beta << " " << rn << " " << exp_h << " " << etsw << endl;

//    etsw *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    etsw *= (etimes)/LAMBDA; // convert to mm h2o month^-1
    etsw += ecanmmmo+esoilmmmo;

    pecan = beta*(rn - rnsoil) + CPAIR*RHOAIR*exp_h*vpdo/r_ac;
    pecan /= (beta + PSY*exp_h*(1.0 + r_sc/r_ac)); // W m^-2
//    pecan *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    pecan *= (etimes)/LAMBDA; // convert to mm h2o month^-1
//    if(pecan < 0.0) {pecan = 0.0;}

    pesoil = beta*rnsoil + CPAIR*RHOAIR*exp_h*vpdo/r_as;
    pesoil /= (beta + PSY*exp_h*(1.0 + r_ss/r_as)); // W m^-2
//    pesoil *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    pesoil *= (etimes)/LAMBDA; // convert to mm h2o month^-1
    if(esoilmmmo != 0.0) {pesoil = esoilmmmo;}
//    if(pesoil < 0.0) {pesoil = 0.0;}

//    if(etsw < 0.0) {etsw = 0.0;}
//    cout << "diag = " << pesoil << " " << beta << " " << vpdo << " "<< r_as << " " << rnsoil << " " << etimes << endl;
//    cout << "diag = " << pecan << " " << beta << " " << vpdo << " "<< r_ac << " " << r_sc << " " << rn << " " << rnsoil << " " << etimes << endl;
//      cout << "diag = " << etsw << " " << pecan << " " << pesoil << " " << ecanmmmo << endl;
    return etsw;

};

/* *************************************************************
************************************************************* */
double Penmon45::shuttlewallacell( const int& pdcmnt,
                  const double& nirr,
                  const double& lwout,
                  const double& lai,
                  const double& tmean,
                  const double& vpd,
                  const int& dm,
                  const double& gc,
                  const double& etimes,
                  const double& esoilmmmo,
                  const double& elev,
                  const int& phen )

{

/*    Description of Shuttleworth & Wallace (1985) interactive model:

  Source height, where temperature (tmean) and vapor pressure deficit (vpd)
are known, is assumed coupled to a canopy airspace (via r_aa), which interacts
with both the plant canopy (via r_ac), and the soil surface (via r_as).  Leaves
and soil each release latent energy at a rate given by the penman monteith
equation, but based on the canopy airspace vapor pressure deficit (vpdo)
instead of the source height vpd.
*/

  double npmc, dpmc, npms, dpms, pms, pmc;
  double wc, ws;
  double vpdo; // vapor pressure deficit of canopy airspace
  

  double rn, rnsoil; // net radiation - total and soil
  double totalbedo;

  double r_sc; // stomatal resistance, s m^-1
  double resc, resa, ress; // intermediate calculation
  double etsw;  //final ET

//  double num,den;
  double exp_h;
  
//cout << "nirr in penmond = " << nirr << endl;

  setBETA( tmean );
  exp_h = exp(-elev/SCALEH);

  if(gc < 0.00001) {r_sc = 1.0/0.00001;}
  else { r_sc = 1.0/gc;}
//  else { r_sc = 1.0/(gc/2.0);}
  
//    if(esoilmmmo != 0.0 && nirr > 0.0)
//    {
//    cout << "diag1 = " << esoilmmmo << " " << r_ss <<  endl;
//    num = beta*rnsoil + CPAIR*RHOAIR*vpdo/r_as;
//    den = esoilmmmo*LAMBDA/(etimes*ndays[dm]);
//    r_ss = ((((num/den)-beta)*(1/PSY)-1)*r_as);
//    cout << "diag2 = " << r_ss << endl;
//    }

  resa = (beta + PSY*exp_h) * r_aa;
  ress = (beta + PSY*exp_h) * r_as + PSY * exp_h * r_ssll;
  resc = (beta + PSY*exp_h) * r_ac + PSY * exp_h * r_sc;


  if(esoilmmmo != 0.0) {
   wc = 1.0;
  }
  else {
  wc = (1.0 + resc * resa / (ress * (resc + resa)));
  }
  wc = 1.0/wc;


  if(esoilmmmo != 0.0) {
   ws = (1.0 + resa)/resc;
  }
  else {
  ws = (1.0 + ress * resa / (resc * (ress + resa)));
  }
  ws = 1.0/ws;
  
//
// calculate evaporation from canopy interception, first in mm mo^-1, 
// then convert to W m^-2 and deduct from net radiation prior to other calculations  
//

//  ecanmmmo = ndays[dm]*(1.0 - exp(-0.005*precip))
//  ecanmmmo = (1.0 - exp(-0.005*precip))
//   BSF  Adapt daily to monthly
//                      *(1.0 - exp(-kext[pdcmnt]*lai))*maxh2ocanopy*lai;
  
//cout << "diag = " << ecanmmmo << " " << precip << " " << throughfall << " " << dm << endl;
//  lwcor = lwout + (ecanmmmo)*LAMBDA/(etimes*ndays[dm]);   

//cout << "pen = " << resa << " " << ress << " " << resc << " " << wc << " " << ws << " " << ecanmmmo << " " << lwcor << endl;
//cout << "lwcor = " << lwout << " " << ecanmmmo << " " << etimes << " " << ndays[dm] << endl;
//
//  snowcover is considered to have a constant albedo of 0.5
//  soil/snow/substrate assumed to store 20% of rnsoil as per p. 845
//  Shuttleworth and Wallace (1985)
//
    totalbedo = albedo[pdcmnt];
    //rnsoil = 0.8 * ((1.0 - totalbedo) * nirr - lwout) * exp(-kext[pdcmnt] * lai);
    rnsoil = 0.8 * ((1.0 - totalbedo) * nirr - lwout) * exp(-kext[pdcmnt] * lai);
//  rnsoil = rnsoil - esoilmmmo*LAMBDA/(etimes*ndays[dm]);
  rnsoil = rnsoil - esoilmmmo*LAMBDA/(etimes);

//cout << "diag = " << rnsoil << " " << nirr << " " << lwcor << " " << lai << " " << esoilmmmo << endl;
    //rn = nirr * (1.0 - totalbedo) - lwout;
//    rn = nirr * (1.0 - totalbedo) - lwcor - esoilmmmo*LAMBDA/(etimes*ndays[dm]);
    rn = nirr * (1.0 - totalbedo) - lwout - esoilmmmo*LAMBDA/(etimes);

    npmc = beta*rn+(CPAIR*RHOAIR*exp_h*vpd-beta*r_ac*rnsoil)/(r_aa+r_ac);
    dpmc = beta+PSY*exp_h*(1.0+r_sc/(r_aa+r_ac));
    npms = beta*rn+(CPAIR*RHOAIR*exp_h*vpd-beta*r_as*(rn-rnsoil))/(r_aa+r_as);
    dpms = beta+PSY*exp_h*(1.0+r_ssll/(r_aa+r_as));

    pmc = (npmc/dpmc);
    if(esoilmmmo != 0.0) {
      pms = 0.0;
    }
    else {
    pms = (npms/dpms);
    }

    etsw = (wc*pmc + ws*pms); // W m^-2

//cout << "etsw = " << etsw << " " << wc << " " << pmc << " " << ws << " " << pms << endl;
    // having obtained the overall latent energy flux, we now can determine vpdo,
    //   and thus the respective contributions of canopy and soil
    
    if(etsw > ZERO) // sensible heat flux: residual 
    {
      shflux = rn - 0.25 * rnsoil - etsw;
    }
    
    vpdo = vpd + r_aa*(beta*rn - (beta + PSY*exp_h)*etsw)/(CPAIR*RHOAIR*exp_h);

//  cout << "diag = " << vpdo << " " << vpd << " " << r_aa << " " << beta << " " << rn << " " << exp_h << " " << etsw << endl;

//    etsw *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    etsw *= (etimes)/LAMBDA; // convert to mm h2o month^-1
    etsw += esoilmmmo;

    pecan = beta*(rn - rnsoil) + CPAIR*RHOAIR*exp_h*vpdo/r_ac;
    pecan /= (beta + PSY*exp_h*(1.0 + r_sc/r_ac)); // W m^-2
//    pecan *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    pecan *= (etimes)/LAMBDA; // convert to mm h2o month^-1
//    if(pecan < 0.0) {pecan = 0.0;}

    pesoil = beta*rnsoil + CPAIR*RHOAIR*exp_h*vpdo/r_as;
    pesoil /= (beta + PSY*exp_h*(1.0 + r_ssll/r_as)); // W m^-2
//    pesoil *= (etimes*ndays[dm])/LAMBDA; // convert to mm h2o month^-1
    pesoil *= (etimes)/LAMBDA; // convert to mm h2o month^-1
    if(esoilmmmo != 0.0) {pesoil = esoilmmmo;}
//    if(pesoil < 0.0) {pesoil = 0.0;}

//    if(etsw < 0.0) {etsw = 0.0;}
//    cout << "diag = " << pesoil << " " << beta << " " << vpdo << " "<< r_as << " " << rnsoil << " " << etimes << endl;
//    cout << "diag = " << pecan << " " << beta << " " << vpdo << " "<< r_ac << " " << r_sc << " " << rn << " " << rnsoil << " " << etimes << endl;
//      cout << "diag = " << etsw << " " << pecan << " " << pesoil << " " << ecanmmmo << endl;
    return etsw;

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Penmon45::setBETA( const double& tmean )

{
  double svp = 6.108*exp( 17.27*tmean / ( 237.3+tmean ));

  beta = svp * (4098.0+34.5*tmean)/(pow((237.3+tmean),2));

  beta = svp * 4098.0/(pow((237.3+tmean),2));

};

/* *************************************************************
************************************************************* */
double Penmon45::watev(  const double& nirr,
                         const double& lwout,
                         const double& tmean,
                         const double& vpd,
                         const double& wind,
                         const double& rrun,
                         const double& srun,
                         const int& dm )
                        

{

//  double pevap;
  double wevap;
//  double dsm;
//  double ep;
//  double prob;
//  double rbar;
  double netrad;
  double es, e, tdew;
//  double ra = 200.0/wind;
//  double rs = 280.0/wind;


    setBETA( tmean );

    es = 6.112 * exp((17.97*tmean)/(tmean+243.5));
    e = es - vpd;
    tdew = (243.5*log(e/6.112))/(17.67-log(e/6.112));

//    cout << "inter " << es << " " << e << " " << tdew << " " << tmean << endl;

//    netrad = 0.8*nirr-40;

    netrad = nirr-lwout;

    if(netrad < 0.0) {netrad = ZERO;}

//    wevap = ((netrad+(CPAIR*RHOAIR)*(tmean-tdew)/ra)/
//          (1+PSY*(ra+rs)/(beta*ra)))*
//          (DAYtoS*ndays[dm])/LAMBDA;

//    wevap = ndays[dm]*0.408*(beta*netrad + 2.6*PSY*(0.5+wind/161)*(es-e))/(beta+PSY);

      wevap = (beta /(beta+PSY))* netrad + ((PSY/(beta+PSY)))*6.43*(1.0+0.536*wind)*vpd/100.;    
      wevap /= LAMBDA;
//      wevap *= (DAYtoS*ndays[dm]);
      wevap *= (DAYtoS);

//   cout << "wevap = " << wevap << " " << netrad << endl;

/*  if(wevap < ZERO) {wevap = ZERO;}

  if( (rrun+srun) >= pevap )
  {
   wevap = pevap;
  }
  else
  {
  ep = pevap / ndays[dm];
  prob = 1.0 - exp( -0.005*(rrun+srun) );
  if( prob != ZERO )
  {
   rbar = (rrun+srun) / (ndays[dm] * prob);
  }
  else { rbar = ZERO; }

  if ( rbar != ZERO)
  {
    dsm = rbar*prob*(1 - exp( -pevap/rbar )) - ep;
  }
  else { dsm = -ep; }

  dsm *= ndays[dm];

  wevap = rrun + srun - dsm;

  if( wevap > pevap ) {wevap = pevap; }

  } */

  return wevap;

};


/*/ **************************************************************
************************************************************** */

void Penmon45::getecd( const string& ecd )
{
  ifstream infile;
  int dcmnt;
  int comtype;


  infile.open( ecd.c_str(), ios::in );

  if ( !infile )
  {
    cerr << "\nCannot open " << ecd << " for gc ECD input" << endl;
    exit( -1 );
  }

  getXMLrootNode( infile, "gcECD" );

  for ( dcmnt = 1; dcmnt < MAXCMNT; ++dcmnt )
  {  
    comtype = getXMLcommunityNode( infile, "gcECD" );

    if( comtype >= MAXCMNT )
    {
      cerr << endl << "comtype is >= MAXCMNT" << endl;
      cerr << "comtype cannot be greater than ";
      cerr << (MAXCMNT-1) << endl;
      cerr << " in gcECD" << endl;

      exit( -1 );
    }

    cdleaf[comtype] = getXMLcmntArrayDouble( infile,
                                             "gcECD",
                                             "cdleaf",
                                             comtype );

    wleaf[comtype] = getXMLcmntArrayDouble( infile,
                                         "gcECD",
                                         "wleaf",
                                         comtype );


    gsa[comtype] = getXMLcmntArrayDouble( infile,
                                           "gcECD",
                                           "gsa",
                                           comtype );

    gsmin[comtype] = getXMLcmntArrayDouble( infile,
                                           "gcECD",
                                           "gsmin",
                                           comtype );

    kext[comtype]= getXMLcmntArrayDouble( infile,
                                            "gcECD",
                                            "kext",
                                            comtype );

    albedo[comtype]= getXMLcmntArrayDouble( infile,
                                        "gcECD",
                                        "albedo",
                                        comtype );
                                        
    lscmin[comtype]= getXMLcmntArrayDouble( infile,
                                        "gcECD",
                                        "lscmin",
                                        comtype );
                                        
    groot[comtype]= getXMLcmntArrayDouble( infile,
                                        "gcECD",
                                        "groot",
                                        comtype );
                                        
    kstem[comtype]= getXMLcmntArrayDouble( infile,
                                        "gcECD",
                                        "kstem",
                                        comtype );
    
    rhostem[comtype]= getXMLcmntArrayDouble( infile,
                                        "gcECD",
                                        "rhostem",
                                        comtype );


    endXMLcommunityNode( infile );

  }


  if ( dcmnt < MAXCMNT )
  {
    cerr << endl << " Parameters found for only " << dcmnt;
    cerr << " community types out of a maximum of ";
    cerr << (MAXCMNT-1) << " types in gcECD" << endl;
    exit( -1 );
  }

  infile.close();

};
/* *************************************************************
************************************************************* */


