/* **************************************************************
*****************************************************************
TMCRBD.H - object describing characteristics of soil microbes
	        used in the Terrestrial Ecosystem Model (TEM)

Modifications:

20060126 - DWK created by modifying tmcrb50b5.h
20060126 - DWK changed include from tprocessXML51.h
           to tprocessXML437.h
20060126 - DWK changed class Tmicrobe50 to class Tmicrobeflux
20060126 - DWK changed inheritance from ProcessXML50 to
           ProcessXML43
20060126 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20060126 - DWK deleted const double& tsoil and
           const int& tsoilflg from function call to setDQ10()
20070105 - TWC changed name to tmcrb45
2007 - TWC/BSF
   New function setDQ10LaRS (remove setNEWDQ10)
   Public Functions and Private Parameters: remove alpha, beta,
        gamma, qref, tref
   Private Parameters: add rhalpha, rhbeta, rhgamma, rhqref,
        rhtref

*****************************************************************
************************************************************** */


#ifndef TMCRBD_H
#define TMCRBD_H

// Tmcrb45 uses the global constants NUMMSAC and MAXCMNT
#include "temconstsd.hpp"

#include "tprocessXMLd.h"
#include "bioms45.hpp"

class Tmcrb45: public ProcessXML45
{

   public:

     Tmcrb45();

/* **************************************************************
		 Public Functions
************************************************************** */

     void getvegecd( const string& ecd );

     void resetEcds( const int& pcmnt, const double& psiplusc );

     void resetMonthlyFluxes( void );

     void resetYrFluxes( void );

     void setDQ10LaRS( const int& pdcmnt,
                       const double& ptair );
                       
     double setDQ10LT( const int& pdcmnt,
                     const double& ptair,
                     const double& ptopt );

     double setRHMOIST( const int& pdcmnt,
                        const double& pctfldcap,
                        const double& pctwiltpt,
                        const double& pctpor,
                        const double& vsm,
                        const int& moistlim );

     void showecd( const int& pdcmnt );

     void updateDynamics( const int& pcmnt,
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
                          const double& volan );

     double yrkd( const int& nfeed,
                  const double& yrltrc,
                  const double& yrltrn,
                  const int& pdcmnt );


     // "Get" and "Set" private variables and parameters

     // cnsoil *************************************************

     inline double getCNSOIL( const int& pcmnt )
     {
       return cnsoil[pcmnt];
     }

     inline void setCNSOIL( const double& pcnsoil,
                            const int& pcmnt )
     {
       cnsoil[pcmnt] = pcnsoil;
     }


     // decay **************************************************

     inline double getDECAY( void ) { return decay; }

     // docdon **************************************************

     inline double getDOCDON( const int& pcmnt ) { return docdon[pcmnt]; }

     // gmin **************************************************
     
          inline double getGMIN( void ) { return gmin; }
     

     // kd *****************************************************

     inline double getKD( void ) { return kd; }

     inline void setKD( const double& pkd ) { kd = pkd; }

      // decomp **********************************************

     inline double getDECOMP( void ) { return decomp; };


      // docprod **********************************************

     inline double getDOCPROD( void ) { return docprod; };

      // donprod **********************************************

     inline double getDONPROD( void ) { return donprod; };



     // kdb ****************************************************

     inline double getKDB( const int& pcmnt )
     {
       return kdb[pcmnt];
     }

     inline void setKDB( const double& pkdb,
                         const int& pcmnt )
     {
       kdb[pcmnt] = pkdb;
     }


     // kdc ****************************************************

     inline double getKDC( void ) { return kdc; }

     inline void setKDC( const double& pkdc ) { kdc = pkdc; }


     // kn2 ****************************************************

     inline double getKN2( const int& pcmnt )
     {
       return kn2[pcmnt];
     }

     inline void setKN2( const double& pkn2,
                         const int& pcmnt )
     {
       kn2[pcmnt] = pkn2;
     }


     // lcclnc *************************************************

     inline double getLCCLNC( const int& pcmnt )
     {
       return lcclnc[pcmnt];
     }

     inline void setLCCLNC( const double& plcclnc,
                            const int& pcmnt )
     {
       lcclnc[pcmnt] = plcclnc;
     }


     // moistmax ***********************************************

     inline double getMOISTMAX( const int& pcmnt )
     {
       return moistmax[pcmnt];
     }

     inline void setMOISTMAX( const double& pmoistmax,
                              const int& pcmnt )
     {
       moistmax[pcmnt] = pmoistmax;
     }


     // moistmin ***********************************************

     inline double getMOISTMIN( const int& pcmnt )
     {
       return moistmin[pcmnt];
     }

     inline void setMOISTMIN( const double& pmoistmin,
                              const int& pcmnt )
     {
       moistmin[pcmnt] = pmoistmin;
     }


     // moistopt ***********************************************

     inline double getMOISTOPT( const int& pcmnt )
     {
       return moistopt[pcmnt];
     }

     inline void setMOISTOPT( const double& pmoistopt,
                              const int& pcmnt )
     {
       moistopt[pcmnt] = pmoistopt;
     }


     // netnmin ************************************************

     inline double getNETNMIN( void ) { return netnmin; }

     inline void setNETNMIN( const double& pnetnmin )
     {
       netnmin = pnetnmin;
     }


     // nup ****************************************************

     inline double getNUP( void ) { return nup; }

     inline void setNUP( const double& pnup )
     {
       nup = pnup;
     }


     // nupa ***********************************************

     inline double getNUPA( const int& pcmnt )
     {
       return nupa[pcmnt];
     }

     inline void setNUPA( const double& pnupa,
                          const int& pcmnt )
     {
       nupa[pcmnt] = pnupa;
     }


     // nupb ***************************************************

     inline double getNUPB( const int& pcmnt )
     {
       return nupb[pcmnt];
     }

     inline void setNUPB( const double& pnupb,
                          const int& pcmnt )
     {
       nupb[pcmnt] = pnupb;
     }


     // nuptake ************************************************

     inline double getNUPTAKE( void ) { return nuptake; }


     // propftos ***********************************************

     inline double getPROPFTOS( const int& pcmnt )
     {
       return propftos[pcmnt];
     }

     inline void setPROPFTOS( const double& ppropftos,
                              const int& pcmnt )
     {
       propftos[pcmnt] = ppropftos;
     }


     // rh *****************************************************

     inline double getRH( void ) { return rh; }

     inline void setRH( const double& prh)
     {
       rh = prh;
     };


     // rrh **********************************************

     inline double getRRH( void ) { return rrh; };

     inline void setRRH( const double& prrh)
     {
       rrh = prrh;
     };


     // rhq10 **************************************************

     inline double getRHQ10( const int& pcmnt )
     {
       return rhq10[pcmnt];
     }

     inline void setRHQ10( const double& prhq10,
                           const int& pcmnt )
     {
       rhq10[pcmnt] = prhq10;
     }


/* **************************************************************
		 Public Variables
************************************************************** */


     // Annual sum of netnmin
     double yrnmin;         // (g N / (sq. meter * year))

     // Annual sum of nuptake
     double yrnuptake;      // (g N / (sq. meter * year))

     // Annual sum of rh
     double yrrh;       // (g C / (sq. meter * year))

     // Running mean of rh
     double rrh;       // (g C / (sq. meter * year))

     // docproc
     double docproc;


   private:

/* **************************************************************
		 Private Functions
************************************************************** */

     double nminxclm( const int& pdcmnt,
                      const double& soilh2o,
                      const double& soilorgc,
                      const double& soilorgn,
                      const double& availn,
                      const double& ksoil,
                      const int& nopen,
                      const double& immbadd,
                      const double& volac,
                      const double& volan );

     double rhxclm( const double& soilorgc,
                    const double& pdq10,
                    const double& moist,
                    const double& rltrc,
                    const double& volac );


/* **************************************************************
		 Private Variables
************************************************************** */

     // Effect of temperature on decomposition
     double dq10;

     // Net nitrogen mineralization
     double netnmin; // (g N / (sq. meter * month))

     // Total nitrogen uptake or "immobilzation" by microbes
     double nuptake;  // (g N / (sq. meter * month))

     // Heterotrophic respiration
     double rh;  // (g C / (sq. meter * month))
     double docprod;
     double donprod;
     double decomp;

/* *************************************************************
		 Private Parameters
************************************************************* */

     double cnsoil[MAXCMNT];

     // Parameter representing the quality of soil organic matter

     double decay;

     // Gross Mineralization
     double gmin;

     // Biome-specific decomposition parameters for function rhxclm

     double kd;
     double kdb[MAXCMNT];
     double kdc;

     double kdin[NUMMSAC];     // kd values read in from file
     double kdsave[NUMMSAC];   // kd values saved to a file

     // Biome-specific half saturation parameter for function
     //   nminxclm describing the effect of available nitrogen
     //   on microbial nitrogen uptake

     double kn2[MAXCMNT];


     double lcclnc[MAXCMNT];

     // Biome-specific parameters describing the influence of
     //   soil moisture on decomposition (i.e., moist)

     double moistmin[MAXCMNT];
     double moistopt[MAXCMNT];
     double moistmax[MAXCMNT];

    // N fixation parameter


     double nup;
     double nupa[MAXCMNT];
     double nupb[MAXCMNT];

     double propftos[MAXCMNT];

     double rhq10[MAXCMNT];

  double rhalpha[MAXCMNT];
  double rhbeta[MAXCMNT];
  double rhgamma[MAXCMNT];
  double rhqref[MAXCMNT];
  double rhtref[MAXCMNT];

  double docfr[MAXCMNT];
  double docdon[MAXCMNT];
};

#endif

