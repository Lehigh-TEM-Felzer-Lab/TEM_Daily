/* **************************************************************
*****************************************************************
PENMOND.H - object computes EET using Penmon Monteith

Modifications:

20040330 - BSF created
*****************************************************************
************************************************************** */

// Class uses global constants CYCLE and MAXRTIME

#ifndef PENMOND_H
#define PENMOND_H

#include "temconstsd.hpp"

// Penmon45 inherits ProcessXML45 Class
#include "tprocessXMLd.h"

class Penmon45 : public ProcessXML45

{

  public:

     Penmon45();

/* **************************************************************
		 Public Functions
************************************************************** */



     double gcpet( const int& pdcmnt,
                   const int& dm,
                   const double& lai,
                   const double& gpp,
                   const double& rcanmm,
                   const double& vpr,
                   const double& vpd,
                   const double& co2,
                   const double& pdayl );
                   
     void hydraulics( const int& pdcmnt,
                      const int& phen,
                      const double& lai,
                      const double& psla,
                      const double& stemc,
                      const double& rootc,
                      const double& availh2o,
                      const double& awcapmm,
                      const double& prec,
                      ofstream& temdiagfile );

     void getecd( const string& ecd );

     double shuttlewallace( const int& pdcmnt,
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
                       const int& phen );

     double shuttlewallacell( const int& pdcmnt,
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
                       const int& phen );

     double watev ( const double& nirr,
                    const double& lwout,
                    const double& tmean,
                    const double& vpd,
                    const double& wind,
                    const double& rrun,
                    const double& rsun,
                    const int& dm );

   // albedo *************************************************

   inline double getALBEDO( const int& pcmnt )
   {
     return albedo[pcmnt];
   };

   inline void setALBEDO( const double& palbedo,
                     const int& pcmnt )
   {
     albedo[pcmnt] = palbedo;
   };


     // cdleaf *************************************************

     inline double getCDLEAF( const int& pcmnt )
     {
       return cdleaf[pcmnt];
     };

     inline void setCDLEAF( const double& pcdleaf,
                            const int& pcmnt )
     {
       cdleaf[pcmnt] = pcdleaf;
     };

     // wleaf *************************************************

     inline double getWLEAF( const int& pcmnt )
     {
       return wleaf[pcmnt];
     };

     inline void setWLEAF( const double& pwleaf,
                            const int& pcmnt )
     {
       wleaf[pcmnt] = pwleaf;
     };



     // gsa *************************************************

     inline double getGSA( const int& pcmnt )
     {
       return gsa[pcmnt];
     };

     inline void setGSA( const double& pgsa,
                            const int& pcmnt )
     {
       gsa[pcmnt] = pgsa;
     };


     // gsmin *************************************************

     inline double getGSMIN( const int& pcmnt )
     {
       return gsmin[pcmnt];
     };

     inline void setGSMIN( const double& pgsmin,
                          const int& pcmnt )
     {
       gsmin[pcmnt] = pgsmin;
     };


     // kext *************************************************

     inline double getKEXT( const int& pcmnt )
     {
       return kext[pcmnt];
     };

     inline void setKEXT( const double& pkext,
                          const int& pcmnt )
     {
       kext[pcmnt] = pkext;
     };
     
     // kplant *************************************************
     
     inline double getKPLANT( void ) 
     { 
       return kplant; 
     };

     // fws ****************************************************
     
     inline double getFWS( void ) 
     { 
       return fws; 
     };
     
     // dfws_dleafc  ********************************************
     
     inline double getDFWS_DLEAFC( void ) 
     { 
       return dfws_dleafc; 
     };
     
     // dfws_dstemc  ********************************************
     
     inline double getDFWS_DSTEMC( void ) 
     { 
       return dfws_dstemc; 
     };

     // dfws_drootc  ********************************************
     
     inline double getDFWS_DROOTC( void ) 
     { 
       return dfws_drootc; 
     };
  
     // lscmin *************************************************

     inline double getLSCMIN( const int& pcmnt )
     {
       return lscmin[pcmnt];
     };

     inline void setLSCMIN( const double& plscmin,
                            const int& pcmnt )
     {
       lscmin[pcmnt] = plscmin;
     };
     
     // groot *************************************************

     inline double getGROOT( const int& pcmnt )
     {
       return groot[pcmnt];
     };

     inline void setGROOT( const double& pgroot,
                           const int& pcmnt )
     {
       groot[pcmnt] = pgroot;
     };
     
     // kstem *************************************************

     inline double getKSTEM( const int& pcmnt )
     {
       return kstem[pcmnt];
     };

     inline void setKSTEM( const double& pkstem,
                           const int& pcmnt )
     {
       kstem[pcmnt] = pkstem;
     };
     
     // rhostem *************************************************

     inline double getRHOSTEM( const int& pcmnt )
     {
       return rhostem[pcmnt];
     };

     inline void setRHOSTEM( const double& prhostem,
                             const int& pcmnt )
     {
       rhostem[pcmnt] = prhostem;
     };

	 // r_aa *************************************************

     inline double getR_AA( void ) 
     {
       return r_aa;
     };

     inline void setR_AA( const double& pr_aa )
     {
       r_aa = pr_aa;
     };

	 // r_ac *************************************************

     inline double getR_AC( void )
     {
       return r_ac;
     };

     inline void setR_AC( const double& pr_ac )
     {
       r_ac = pr_ac;
     };

	 // r_as *************************************************

     inline double getR_AS( void )
     {
       return r_as;
     };

     inline void setR_AS( const double& pr_as )
     {
       r_as = pr_as;
     };
     
     // r_ss *************************************************

     inline double getR_SS( void )
     {
       return r_ss;
     };

     inline void setR_SS( const double& pr_ss )
     {
       r_ss = pr_ss;
     };

     // r_ssll *************************************************
     inline double getR_SSLL( void )
     {
       return r_ssll;
     };

     inline void setR_SSLL( const double& pr_ssll )
     {
       r_ssll = pr_ssll;
     };

	 // ustar *************************************************

     inline double getUSTAR( void ) 
     {
       return ustar;
     };

     inline void setUSTAR( const double& pustar )
     {
       ustar = pustar;
     };

	 // zd *************************************************

     inline double getZD( void )
     {
       return zd;
     };

     inline void setZD( const double& pzd )
     {
       zd = pzd;
     };

	 // zo *************************************************

     inline double getZO( void )
     {
       return zo;
     };

     inline void setZO( const double& pzo )
     {
       zo = pzo;
     };

	 // vegh *************************************************

     inline double getVEGH( void )
     {
       return vegh;
     };

     inline void setVEGH( const double& pvegh )
     {
       vegh = pvegh;
     };
     
     // ndays *************************************************

     inline double getNDAYS( const int& pdm ) { return ndays[pdm]; };

     // pecan *************************************************

     inline double getPECAN( void ) { return pecan; };

     // pesoil *************************************************

     inline double getPESOIL( void ) { return pesoil; };
     
     // sensible heat flux *************************************************
     
     inline double getSHFLUX( void ) { return shflux; };




/* **************************************************************
		 Private Functions
************************************************************** */

private:

void setBETA( const double& tmean );

/* **************************************************************
		 Private Variables
************************************************************** */


 // Evapotranspiration Variables

     double beta;
     double ndays[12];

     double CPAIR;
     double DAYtoS;
     double LAMBDA;
     double PSY;
     double RHOAIR;
     double CO2REF;
     double SCALEH;
     
     double kplant; // plant hydraulic conductance, mmol m^-2 s^-1 MPa^-1
     double fws; // water stress multiplier, 0-1
     double dfws_dleafc; // derivatives of fws with respect to leafc, stemc, rootc
     double dfws_dstemc; 
     double dfws_drootc; 

     double pecan; // transpiration
     double pesoil; // soil evaporation
     double shflux; // sensible heat flux

	 double r_aa; // canopy space - ref height resistance
	 double r_ac; // canopy airspace - leaves resistance
	 double r_as; // canopy airspace - soil surface resistance
	 double r_ss; // soil internal resistance to water loss
	 double r_ssll; // soil internal resistance to water loss lower layer

	 double ustar; // friction velocity
	 double zd; // displacement height
	 double zo; // roughness length
	 double vegh; // vegetation height

/* **************************************************************
		 Private Parameters
************************************************************** */

     double cdleaf[MAXCMNT];
     double wleaf[MAXCMNT];
     double gsa[MAXCMNT];
     double gsmin[MAXCMNT];
     double kext[MAXCMNT];
     double albedo[MAXCMNT];
     double lscmin[MAXCMNT];
     double groot[MAXCMNT];
     double kstem[MAXCMNT];
     double rhostem[MAXCMNT];


};

#endif
