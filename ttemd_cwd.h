/* *************************************************************
****************************************************************
TTEMD_CWD.H - Terrestrial Ecosystem Model Version 4.5
****************************************************************

Modifications:

20060126 - DWK created by modifying ttem50b5.h
20060126 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20060126 - DWK deleted global const int STSY
20060126 - DWK changed include from atms50b5.h to atms437.h
20060126 - DWK changed include from tveg50b5.h to tveg437.h
20060126 - DWK changed include from tsoil50b5.h to tsoil437.h
20060126 - DWK changed include from tmcrb50b5.h to tmcrb437.h
20060126 - DWK changed include from humnact50b5.h to
           humnact437.h
20060126 - DWK changed class TTEM50 to class TTEM43
20060126 - DWK added I_FOZONE and I_FINDOZNE to enum temkey
20060126 - DWK deleted I_TSOIL, I_DST0, I_DST5, I_DST10,
           I_DST20, I_DST50, I_DST100, I_DST200, I_FRONTD,
           I_THAWBE, I_THAWEND, I_THAWPCT, and I_ACTLAYER
           from enum temkey
20060126 - DWK added GET_D40 and GET_FOZONE to enum seykey
20060126 - DWK deleted enum sstkey
20060126 - DWK deleted public functions
           void displayOptionalSoilTemp(),
           double getOptionalSoilTemp(), inline sstykey& next()
           and inline sstykey& prev(),
20060126 - DWK deleted ofstream& rflog1 from function call to
           public functions int monthlyTransient() and
           int stepmonth()
20060126 - DWK deleted public string snowfile and
           string slayerfile
20060126 - DWK changed public Humanact50 ag to Humanact43 ag
20060126 - DWK changed public Atmosphere50 to Atmosphere43
20060126 - DWK changed public Tmicrobe50 to Tmicrobe43
20060126 - DWK changed public Tsoil50 to Tsoil43
20060126 - DWK changed public Tveg50 to Tveg43
20060126 - DWK deleted public sstykey ssty[STSY]
20060126 - DWK deleted ofstream& rflog1 from function call to
           private function void getenviron()
20070105 - TWC renamed to ttem45
2007 - TWC/BSF Summary
      temkey: VSM, PCTP, RMLEAF, RMROOT, GC, GS, TRANST, EVAP, INTER
	  seykey: RMLEAF, RMROOT
	  swykey: GC, GS, TRANST, EVAP, INTER
	  Public Vars.: string gcfile
	  Private Vars.: water, pota, pota, swp, eetnew, sm, smnew,
	                 nirrn, eetpet

****************************************************************

References:

VERSION 4.1

Tian, H., J.M. Melillo, D.W. Kicklighter, A.D. McGuire and J.
  Helfrich.  1999. The sensitvity of terrestrial carbon storage to
  historical climate variability and atmospheric CO2 in the United
  States.  Tellus 51B: 414-452.

VERSION 4.2

McGuire, A.D., S. Sitch, J.S. Clein, R. Dargaville, G. Esser, J. Foley,
  M. Heimann, F. Joos, J. Kaplan, D.W. Kicklighter, R.A. Meier, J.M.
  Melillo, B. Moore III, I.C. Prentice, N. Ramankutty, T. Reichenau,
  A. Schloss, H. Tian, L.J. Williams and U. Wittenberg (2001) Carbon
  balance of the terrestrial biosphere in the twentieth century:
  analyses of CO2, climate and land use effects with four process-
  based ecosystem models.  Global Biogeochemical Cycles 15: 183-206.

Tian, H., J.M. Melillo, D.W. Kicklighter, S. Pan, J. Liu, A.D. McGuire
  and B. Moore III (2003) Regional carbon dynamics in monsoon Asia
  and its implications for the global carbon cycle. Global and
  Planetary Change 37: 201-217.

VERSION 4.3

Felzer, B., D. Kicklighter, J. Melillo, C. Wang, Q. Zhuang, and
  R. Prinn (2004) Effects of ozone on net primary production and
  carbon sequestration in the conterminous United States using a
  biogeochemistry model. Tellus 56B: 230-248.

Runge - Kutta - Fehlberg (RKF) adaptive integrator:

Cheney, W., and D. Kincaid.  1985.  Numerical mathematics and
  computing.  Second edition.  Brooks/ Col Publishing Co.  Monterey,
  CA.  pp. 325-328.

****************************************************************
************************************************************** */

#ifndef TTEMD_CWD_H
#define TTEMD_CWD_H

#include "temconstsd.hpp"
#include "tprocessXMLd.h"

#ifdef CALIBRATE_TEM
  // Additional global constants
  const int DEBUG_ROW = 50;

  const int WSY = 8;
  const int ESY = 10;
  const int NEKEY = 140; // number of choices for output as "optional E fluxes"
  const int NWKEY = 45; // number of choices for output as "optional W fluxes"
#endif


// Objects describing basic components of the ecosystem

#include "bioms45.hpp"   // TTEM45 uses Biomass class


// Objects describing the structure of the ecosystem

#include "atmsd.h"    // TTEM45 uses Atms45 class
#include "tvegd.h"    // TTEM45 uses Tveg45 class
#include "tsoild.h"   // TTEM45 uses Tsoil45 class
#include "tmcrbd.h"  // TTEM45 uses Tmcrb45 class
#include "penmond.h"


// Objects describing the effects of human activities on the
//   ecosystem

#include "humnactd_cwd.h" // TTEM45 uses Humnact45 class


class Ttem45
{

  public:

     Ttem45();

     enum temkey
     {
       I_LEAFC,    I_SAPWOODC, I_HEARTWOODC, I_ROOTC,
       I_SEEDC,    I_LABILEC,   I_SOLC,

       I_LEAFN,    I_SAPWOODN, I_HEARTWOODN, I_ROOTN,
       I_SEEDN,    I_LABILEN,     I_SOLN,     I_AVLN,

       I_FOZONE,   I_DOC,      I_DON, 
// 18 C&N Pools (MAXESTAT)

       I_SM,       I_VSM,      I_SMLL,        I_VSMLL,    I_PCTP,
       I_RGRW,      I_SGRW,
// 7 Water Pools (MAXWSTAT)

       I_FPC,

       I_ALLOCLC,  I_ALLOCSC,  I_ALLOCHC,  I_ALLOCRC,  I_ALLOCSEEDC,
       I_ALLOCILC, I_ALLOCISC, I_ALLOCIHC, I_ALLOCIRC, I_ALLOCISEEDC,

       I_INGPP,    I_GPP,      I_INNPP,
       I_NPP,      I_GPR,      I_RVMNT,    I_RMLEAF,
       I_RMSAPWOOD,   I_RMROOT,I_RMSEED,   I_RMLABILE,
       I_RVGRW,    I_LTRLC,    I_LTRSC,    I_LTRHC, I_LTRRC,
       I_LTRSEEDC,    I_RH, 

       I_ALLOCLN,  I_ALLOCSN,  I_ALLOCHN,  I_ALLOCRN,  I_ALLOCSEEDN,
       I_ALLOCILN, I_ALLOCISN, I_ALLOCIHN, I_ALLOCIRN, I_ALLOCISEEDN,
       I_NINP,     I_AGFRTN,   I_INNUP,    I_VNUP,
       I_NRESORBL, I_NRESORBS, I_NRESORBR, I_NRESORBSEED,
       I_LTRLN,    I_LTRSN,   I_LTRHN,     I_LTRRN,  I_LTRSEEDN,
       I_MNUP,     I_NMIN,     I_NLST,     I_DOCPROD,  I_LCHDOC,
       I_DONPROD,  I_LCHDON,   I_NFIXS,    I_NFIXN,    I_FRDL,
       I_FCO2,     I_FH2O,     I_TEMP,     I_FO3,      I_LCHDIN,   
// 67 C&N Fluxes (NUMEEQ-MAXESTAT)

       I_AGIRRIG,  I_INEET,    I_EET,      I_RPERC,     I_SPERC,
       I_RRUN,     I_SRUN,     I_INEETLL,  I_EETLL,     I_RUNOFFS, 
       I_RUNOFFB, 

       I_GC,       I_GS,       I_PECAN,    I_PESOIL, 
// 13 Water Fluxes (NUMWEQ-MAXWSTAT)

// 18+5+67+11 = 101 = NUMEQ; no variables below this point should appear in
//   the tem.y[NUMEQ] variable

       I_TOTEC,    I_TOTC,     I_VEGN,

       I_SNWPCK,   I_AVLW,     I_AVLWLL,

       I_NEP,      I_NCE,      I_LAI,

       I_PET,      I_SNWINF,   I_WYLD,

       I_AGPRDC,   I_PROD10C,  I_PROD100C, I_TOTPRDC,

       I_RESIDC,   I_AGSTUBC,

       I_AGPRDN,   I_PROD10N,  I_PROD100N, I_TOTPRDN,

       I_RESIDN,   I_AGSTUBN,

       I_CNVRTC,   I_VCNVRTC,  I_SCNVRTC,  I_SLASHC,    I_CFLX,

       I_CNVRTN,   I_VCNVRTN,  I_SCNVRTN,  I_SLASHN,    I_NRETNT,
       I_NVRTNT,   I_NSRTNT,   I_STANDDEADC,   I_STANDDEADN,
       I_VOLAC,    I_VOLAN,

  //ok to here
       I_AGFPRDC,  I_AGFPRDN,  I_FRESIDC,  I_FRESIDN,   I_AGPRDFC,
       I_AGPRDFN,  I_RESIDFC,  I_RESIDFN,

       I_PRDF10C,  I_PRDF10N,  I_PRD10FC,  I_PRD10FN,   I_PRDF100C,
       I_PRDF100N, I_PRD100FC, I_PRD100FN, I_TOTFPRDC,  I_TOTFPRDN,
       I_TOTPRDFC, I_TOTPRDFN,

       I_CROPC,    I_NATVEGC,  I_CROPN,    I_NATVEGN,   I_CSTRN,
       I_NATSTRN,  I_CSTON,    I_NATSTON,

       I_CROPLAI,  I_NATLAI,   I_CROPFPC,  I_NATFPC,

       I_AGINGPP,  I_NATINGPP, I_AGGPP,    I_NATGPP,    I_AGINNPP,
       I_NATINNPP, I_AGNPP,    I_NATNPP,   I_AGGPR,     I_NATGPR,
       I_AGRVMNT,  I_NATRVMNT, I_AGRVGRW,  I_NATRVGRW,  I_AGLTRC,
       I_NATLTRC,

       I_AGINNUP,  I_NATINNUP, I_AGVNUP,   I_NATVNUP,
       I_AGVNMBL,  I_NATVNMBL,
       I_AGVNRSRB, I_NVNRSRB,  I_AGLTRN,   I_NATLTRN,   I_CLIPPINGS
//  99 extra variables
     };

     #ifdef CALIBRATE_TEM
       enum seykey { NOEKEY,               //0        
                     GET_VEGC,             //1
                     GET_STRN,             //2
                     GET_SOILC,            //3
                     GET_SOILN,            //4
                     GET_AVALN,            //5
                     GET_LEAFC,            //6      
                     GET_SAPWOODC,         //7  
                     GET_HEARTWOODC,       //8
                     GET_ROOTC,            //9
                     GET_SEEDC,            //10   
                     GET_LABILEC,          //11
                     GET_LEAFN,            //12
                     GET_SAPWOODN,         //13
                     GET_HEARTWOODN,       //14
                     GET_ROOTN,            //15
                     GET_SEEDN,            //16          
                     GET_LABILEN,          //17
                     GET_FPC,              //18         
                     GET_ALLOCLC,          //19
                     GET_ALLOCSC,          //20
                     GET_ALLOCHC,          //21
                     GET_ALLOCRC,          //22
                     GET_ALLOCSEEDC,       //23
                     GET_ALLOCILC,         //24
                     GET_ALLOCISC,         //25
                     GET_ALLOCIHC,         //26
                     GET_ALLOCIRC,         //27
                     GET_ALLOCISEEDC,      //28
                     GET_INGPP,            //29
                     GET_GPP,              //30
                     GET_KPLANT,           //31
                     GET_D40,              //32
                     GET_FOZONE,           //33
                     GET_INNPP,            //34
                     GET_NPP,              //35
                     GET_GPR,              //36
                     GET_RVMNT,            //37
                     GET_RMLEAF,           //38
                     GET_RMSAPWOOD,        //39
                     GET_RMROOT,           //40
                     GET_RMSEED,           //41
                     GET_RMLABILE,         //42
                     GET_RVGRW,            //43
                     GET_LTRLC,            //44        
                     GET_LTRSC,            //45
                     GET_LTRHC,            //46
                     GET_LTRRC,            //47
                     GET_LTRSEEDC,         //48
                     GET_AGSTUBC,          //49
                     GET_RH,               //50
                     GET_NEP,              //51
                     GET_LAI,              //52
                     GET_ALLOCLN,          //53
                     GET_ALLOCSN,          //54
                     GET_ALLOCHN,          //55
                     GET_ALLOCRN,          //56
                     GET_ALLOCSEEDN,       //57
                     GET_ALLOCILN,         //58
                     GET_ALLOCISN,         //59
                     GET_ALLOCIHN,         //60
                     GET_ALLOCIRN,         //61
                     GET_ALLOCISEEDN,      //62
                     GET_NINP,             //63
                     GET_AGFRTN,           //64
                     GET_INNUP,            //65
                     GET_VNUP,             //66
                     GET_NRESORBL,         //67
                     GET_NRESORBS,         //68
                     GET_NRESORBR,         //69
                     GET_NRESORBSEED,      //70
                     GET_LTRLN,            //71
                     GET_LTRSN,            //72
                     GET_LTRHN,            //73
                     GET_LTRRN,            //74
                     GET_LTRSEEDN,         //75
                     GET_AGSTUBN,          //76
                     GET_MNUP,             //77
                     GET_NMIN,             //78
                     GET_NLST,             //79
                     GET_CNVRTC,           //80
                     GET_VCNVRTC,          //81
                     GET_SCNVRTC,          //82
                     GET_SLASHC,           //83
                     GET_CFLX,             //84
                     GET_NCE,              //85
                     GET_CNVRTN,           //86
                     GET_VCNVRTN,          //87
                     GET_SCNVRTN,          //88
                     GET_SLASHN,           //89
                     GET_NRETNT,           //90
                     GET_NVRTNT,           //91
                     GET_NSRTNT,           //92
                     GET_AGPRDC,           //93
                     GET_PROD10C,          //94
                     GET_PROD100C,         //95
                     GET_RESIDC,           //96
                     GET_AGPRDN,           //97
                     GET_PROD10N,          //98
                     GET_PROD100N,         //99
                     GET_RESIDN,           //100
                     GET_AGFPRDC,          //101
                     GET_PRDF10C,          //102
                     GET_PRDF100C,         //103
                     GET_FRESIDC,          //104
                     GET_AGPRDFC,          //105
                     GET_PRD10FC,          //106
                     GET_PRD100FC,         //107
                     GET_TOTPRDFC,         //108
                     GET_RESIDFC,          //109
                     GET_AGFPRDN,          //110
                     GET_PRDF10N,          //111
                     GET_PRDF100N,         //112
                     GET_FRESIDN,          //113
                     GET_AGPRDFN,          //114
                     GET_PRD10FN,          //115
                     GET_PRD100FN,         //116
                     GET_TOTPRDFN,         //117
                     GET_RESIDFN,          //118
                     GET_L2SN,             //119
                     GET_DOC,                //120
                     GET_CLIPPINGS,          //121
                     GET_DON,               //122
                     GET_DOCPROD,            //123
                     GET_LCHDOC,               //124
                     GET_DONPROD,             //125
                     GET_LCHDON,             //126
                     GET_LCHDIN,             //127
                     GET_NFIXS,               //128
                     GET_NFIXN,               //129
                     GET_NDEP,               //130
                     GET_STANDDEADC,          //131
                     GET_STANDDEADN,           //132
                     GET_VOLAC,               //133
                     GET_VOLAN,               //134
                     GET_FRDL,                //135
                     GET_FCO2,               //136
                     GET_FH2O,               //137
                     GET_TEMP,               //138
                     GET_FO3                 //139           
                     };


       enum swykey { NOWKEY,               //0    
                     GET_SH2O,             //1
                     GET_PCTP,             //2
                     GET_VSM,              //3
                     GET_VSMLL,            
                     GET_RAIN,             //4
                     GET_SNWFAL,           //5
                     GET_SNWINF,           //6
                     GET_AGIRRIG,          //7
                     GET_PET,              //8
                     GET_TAIR,             //9
                     GET_TAIRD,            //10
                     GET_TAIRN,            //11
                     GET_INEET,            //12
                     GET_EET,              //13
                     GET_INEETLL,
                     GET_EETLL,
                     GET_GC,               //14
                     GET_GS,               //15
                     GET_RPERC,            //16
                     GET_SPERC,            //17
                     GET_RRUN,             //18
                     GET_SRUN,             //19
                     GET_WYLD,             //20
                     GET_PESOIL,           //21
                     GET_PECAN,            //22
                     GET_SHFLUX,           //23
                     GET_SWP,              //24
                     GET_VEGH,             //25
                     GET_USTAR,            //26
                     GET_ZD,               //27
                     GET_ZO,               //28
                     GET_R_AA,             //29
                     GET_R_AC,             //30
                     GET_R_AS,             //31
                     GET_R_SS,             //32
                     GET_VAPR,             //33
                     GET_VPDD,             //34
                     GET_VPDN,             //35
                     GET_AVLH2O,           //36
                     GET_RGRNDW,           //37
                     GET_SGRNDW,           //38
                     GET_GDD,              //39
                     GET_SNWPCK,           //40
                     GET_NIRR,           //41
                     GET_PAR,             //42
                     GET_GIRR,             //43
                     GET_CLDS             //44
                     };

     #endif

/* **************************************************************
			Public Functions
************************************************************** */

     #ifdef CALIBRATE_TEM
       void displayOptionalEflx( const seykey& s );
       void displayOptionalWflx( const swykey& s );
       void coutputTEMmonth( const int& pdm,
                     const int& CYCLE );
     #endif

     int ecdqc( const int& dcmnt );

     void ECDsetODEstate( const int& pdcmnt,
                          const double& psiplusc );

     void getco2( void );

     #ifdef CALIBRATE_TEM
       double getOptionalEflx( const int& optflx,
                               const int& dm );

       double getOptionalWflx( const int& optflx,
                               const int& dm );

       void pcdisplayMonth( const int& dyr,
                            const int& dm );
     #endif

     void getsitecd( const int& dv, const string& ecd );
     
     void initializeState( const int& CYCLE );
     void initializecalibState( const int& CYCLE );

     int monthlyTransient( const int& outyr,
                           const int& pdm,
                           const double& outtol,
                           const int& CYCLE,
                           const int& ichrt );

     #ifdef CALIBRATE_TEM
       inline seykey& keyjump( const int& keyloc, seykey& s )
       {
         return s = seykey( keyloc );
       }

       inline swykey& keyjump( const int& keyloc, swykey& s )
       {
         return s = swykey( keyloc );
       }
       
       inline seykey& next( seykey& s )
       {
         //return s = (GET_L2SN == s) ? GET_LEAFC : seykey( s+1 );
         return s = seykey( (s+1)%NEKEY );
       }

       inline swykey& next( swykey& s )
       {
         //return s = (GET_R_SS == s) ? GET_SH2O : swykey( s+1 );
         return s = swykey( (s+1)%NWKEY );
       }

       inline seykey& prev( seykey& s )
       {
         //return s = (GET_LEAFC == s) ? GET_L2SN : seykey( s-1 );
         return s = seykey( (NEKEY+s-1)%NEKEY );
       }

       inline swykey& prev( swykey& s )
       {
         //return s = (GET_SH2O == s) ? GET_R_SS : swykey( s-1 );
         return s = swykey( (NWKEY+s-1)%NWKEY );
       }
     #endif

     void resetMonthlyELMNTFluxes( void );

     void resetYrFluxes( void );

     void setELMNTecd( const int& pdcmnt,
                       const double& psiplusc );

     void setPrevState( void );

     int stepmonth( const int& pdyr,
                    const int& pdm,
                    int& intflag,
                    const double& ptol,
                    const int& CYCLE,
                    const int& icohort );

     int testEquilibrium( const int& pdyr,
                          const int& nyears,
                          const double& vegceq,
                          const double& soilceq,
                          const double& vegneq,
                          const double& soilneq );
                          

     void updateYearSummary( const int& pdm );

     void yearSummaryExtrapolate( void );

     // "Get" and "Set" private variables and parameters


     // avlnb **************************************************

     inline double getAVLNB( const int& pcmnt )
     {
       return avlnb[pcmnt];
     }

     inline void setAVLNB( const double& pavlnb,
                           const int& pcmnt )
     {
       avlnb[pcmnt] = pavlnb;
     }


     // nce ****************************************************

     inline double getNCE( void ) { return nce; }

    // nmax_grow *************************************************
     inline double getNMAX_GROW( const int& pchrt )
     {
       return nmax_grow[pchrt];
     }

     inline void setNMAX_GROW( const double& pnmax_grow,
                           const int& pchrt )
     {
       nmax_grow[pchrt] = pnmax_grow;
     }


     // nep ****************************************************

     inline double getNEP( void ) { return nep; }


     // prevy **************************************************

     inline double getPREVY( const int& i ) { return prevy[i]; }

     inline void setPREVY( const double& pprevy, const int& i )
     {
       prevy[i] = pprevy;
     }


     // solcb **************************************************

     inline double getSOLCB( const int& pcmnt )
     {
       return solcb[pcmnt];
     }

     inline void setSOLCB( const double& psolcb,
                           const int& pcmnt )
     {
       solcb[pcmnt] = psolcb;
     }


     // solnb **************************************************

     inline double getSOLNB( const int& pcmnt )
     {
       return solnb[pcmnt];
     }

     inline void setSOLNB( const double& psolnb,
                           const int& pcmnt )
     {
       solnb[pcmnt] = psolnb;
     }

//    Vegetation Nitrogen Initial Pools

     // leafnb **************************************************

     inline double getLEAFNB( const int& pcmnt )
     {
       return leafnb[pcmnt];
     }

     inline void setLEAFNB( const double& pleafnb,
                           const int& pcmnt )
     {
       leafnb[pcmnt] = pleafnb;
     }


     // sapwoodnb **************************************************

     inline double getSAPWOODNB( const int& pcmnt )
     {
       return sapwoodnb[pcmnt];
     }

     inline void setSAPWOODNB( const double& psapwoodnb,
                           const int& pcmnt )
     {
       sapwoodnb[pcmnt] = psapwoodnb;
     }

     // heartwoodnb **************************************************

     inline double getHEARTWOODNB( const int& pcmnt )
     {
       return heartwoodnb[pcmnt];
     }

     inline void setHEARTWOODNB( const double& pheartwoodnb,
                           const int& pcmnt )
     {
       heartwoodnb[pcmnt] = pheartwoodnb;
     }


     // rootnb **************************************************

     inline double getROOTNB( const int& pcmnt )
     {
       return rootnb[pcmnt];
     }

     inline void setROOTNB( const double& prootnb,
                           const int& pcmnt )
     {
       rootnb[pcmnt] = prootnb;
     }


     // labilenb **************************************************

     inline double getLABILENB( const int& pcmnt )
     {
       return labilenb[pcmnt];
     }

     inline void setLABILENB( const double& plabilenb,
                           const int& pcmnt )
     {
       labilenb[pcmnt] = plabilenb;
     }

   // seednb **************************************************

    inline double getSEEDNB( const int& pcmnt )
    {
      return seednb[pcmnt];
    }

    inline void setSEEDNB( const double& pseednb,
                      const int& pcmnt )
   {
      seednb[pcmnt] = pseednb;
   }

     // totalc *************************************************

     inline double getTOTALC( void ) { return totalc; }

//   Vegetation Parameters


     // leafcb **************************************************

     inline double getLEAFCB( const int& pcmnt )
     {
       return leafcb[pcmnt];
     }

     inline void setLEAFCB( const double& pleafcb,
                           const int& pcmnt )
     {
       leafcb[pcmnt] = pleafcb;
     }


     // sapwoodcb **************************************************

     inline double getSAPWOODCB( const int& pcmnt )
     {
       return sapwoodcb[pcmnt];
     }

     inline void setSAPWOODCB( const double& psapwoodcb,
                           const int& pcmnt )
     {
       sapwoodcb[pcmnt] = psapwoodcb;
     }

    // heartwoodcb **************************************************

    inline double getHEARTWOODCB( const int& pcmnt )
    {
      return heartwoodcb[pcmnt];
    }

    inline void setHEARTWOODCB( const double& pheartwoodcb,
                      const int& pcmnt )
    {
      heartwoodcb[pcmnt] = pheartwoodcb;
    }

     // rootcb **************************************************

     inline double getROOTCB( const int& pcmnt )
     {
       return rootcb[pcmnt];
     }

     inline void setROOTCB( const double& prootcb,
                           const int& pcmnt )
     {
       rootcb[pcmnt] = prootcb;
     }


     // labilecb **************************************************

     inline double getLABILECB( const int& pcmnt )
     {
       return labilecb[pcmnt];
     }

     inline void setLABILECB( const double& plabilecb,
                           const int& pcmnt )
     {
       labilecb[pcmnt] = plabilecb;
     }

   // seedcb **************************************************

   inline double getSEEDCB( const int& pcmnt )
   {
    return seedcb[pcmnt];
   }

   inline void setSEEDCB( const double& pseedcb,
                      const int& pcmnt )
   {
   seedcb[pcmnt] = pseedcb;
   }

     // y ******************************************************

     inline double getY( const int& i ) { return y[i]; }

     inline void setY( const double& py, const int& i )
     {
       y[i] = py;
     }


/* **************************************************************
			 Public Variables
************************************************************** */

     // Input Ecd files with calibration data

     #ifdef CALIBRATE_TEM
       string soilfile;
       string rootfile;
       string vegfile;
       string mcrvfile;
       string agfile;
       string gcfile;
     #endif
       
     ifstream gofile;
     ifstream teminfile;
     ofstream temdiagfile;
     string goname;
     ProcessXML45 goxml;
       
     #ifdef CALIBRATE_TEM
       int adapttol;
     #endif

     Humnact45 ag;

     Atms45 atms;

     Penmon45 pen;

     static int avlnflag;

     static int baseline;

     #ifdef CALIBRATE_TEM
       int calwind;
     #endif

     static double ctol;

     static int diffyr;

     int distmnthcnt;

     int disturbflag;

     int disturbmonth;

     double elev;              // elevation (m)

     static int endeq;

     static int endyr;

     static int equil;

     // index for vegetation type (ez = veg.temveg-1)
     int ez;

     int firemnthcnt;
     int fireoccur;
     int stormoccur;

     static int initbase;

     int initFlag;

     static double inittol;

     #ifdef CALIBRATE_TEM
       int intbomb;
     #endif

     static int intflag;
     
     static int maxit;
     
     static int maxitmon;

     static int maxnrun;

     static int maxyears;

     Tmcrb45 microbe;

     static int moistlim;

     int nattempt;

     static int nfeed;

     static double ntol;

     static int o3flag;

     int predflag;

     vector<string> predstr;

     int qualcon[MAXRTIME];

     double outputm[NUMTEM][12];

     double outputmcl[24][12];

     int retry;

     static int rheqflag;

     static int runsize;

     #ifdef CALIBRATE_TEM
       seykey sey[ESY];
       swykey swy[WSY];
     #endif

     Tsoil45 soil;

     static int startyr;

     static int strteq;

	 double tauavg;

     double tol;

     #ifdef CALIBRATE_TEM
       int tolbomb;
       int topwind;
     #endif

     int totyr;

     Tveg45 veg;

     Tmcrb45 mcrb;

     static int wrtyr;

     static double wtol;

     double yrnce;

     double yrnep;    // (g C / (sq. meter * year))

     double yrrsoil;  // (g C / (sq. meter * year))

     double yrtotalc;

     double mxeet;

     // Site ECD variables

     string version;
     string sitename;
     string developer;
     string sitecol;
     string siterow;
     string updated;
     string description;

     double wevapd[366];
     double wevapn[366];
     double rfrac[366];
     
     int harcnt;
/* **************************************************************
		     Protected Variables
************************************************************** */

  protected:

     int dbugflg;


  private:

/* **************************************************************
		 Private Functions
************************************************************** */

     int adapt( const int& numeq,
                double pstate[],
                const double& ptol,
                const int& pdm,
                const double& nmax_grow );

     int boundcon( double ptstate[],
                   double err[],
                   const double& ptol );

     void delta( const int& dm,
                 const double& nmax_grow,
                 double pstate[],
                 double pdstate[] );

     void cropDynamics( const int& dm, const double& nmax_grow, double pstate[] );

     void getenviron( void );

     void massbal( void );

     void natvegDynamics( const int& dm, const double& nmax_grow, double pstate[] );

     #ifdef CALIBRATE_TEM
       void pcdisplayDT( const double& tottime,
                         const double& deltat );

//       void pcdisplayMonth( const int& dyr,
//                            const int& dm );

       void pcdisplayODEerr( const int& test,
                             double pstate[] );
     #endif

     void resetODEflux( void );
     
     void rkbs( const int& numeq,
               double pstate[],
               double& pdt,
               const int& pdm,
               const double& nmax_grow );

     void rkf( const int& numeq,
               double pstate[],
               double& pdt,
               const int& pdm,
               const double& nmax_grow );

     void step( const int& numeq,
                double pstate[],
                double pdstate[],
                double ptstate[],
                double& pdt );

     void updateVegBiomass( double pstate[] );

/* **************************************************************
		     Private Variables
************************************************************** */

     ifstream fecd[MAXCMNT];
     
     long delta_count;

     // Net Carbon Exchange (NCE)
     double nce;

     // Net Ecosystem Production
     double nep;      // (g C / (sq. meter * month))

     double nmax_grow[MAXCHRTS];

     // NINPUT needed for calibration
     double ninput;
     // Values of ODE state variables for carbon, nitrogen and
     //   water pools during the previous month
     double prevy[MAXSTATE];

     // Total carbon storage (veg.plant + soil.org)
     double totalc;   // (g C/sq. meter)

     // Values of ODE state variables for current month
     double y[NUMEQ];
     
     
     // Variables for displaying "optional fluxes" at the end of a year: sum, max, avg, min      
     #ifdef CALIBRATE_TEM
       double outevarsum[ESY], outevaravg[ESY], outevarmax[ESY], outevarmin[ESY]; 
       double outwvarsum[WSY], outwvaravg[WSY], outwvarmax[WSY], outwvarmin[WSY];
     #endif

     // Adaptive integrator variables

     int blackhol;

     int syint;
     int test;

     double dum4[NUMEQ];
     double error[NUMEQ];

     double dum5[NUMEQ];
     double dumy[NUMEQ];

     double ydum[NUMEQ];
     double dy[NUMEQ];
     double yprime[NUMEQ];
     double rk45[NUMEQ];

     double f11[NUMEQ];
     double f2[NUMEQ];
     double f13[NUMEQ];
     double f3[NUMEQ];
     double f14[NUMEQ];
     double f4[NUMEQ];
     double f15[NUMEQ];
     double f5[NUMEQ];
     double f16[NUMEQ];
     double f6[NUMEQ];
     
     double zstate[NUMEQ];
     double dum2[NUMEQ];
     double dum3[NUMEQ];
     double dp2[NUMEQ]; 
     double dp3[NUMEQ];
     double dz1[NUMEQ]; 
     double dz2[NUMEQ]; 
     double dz3[NUMEQ]; 
     double dz4[NUMEQ];

     static double  a1;

     static double  a3;
     static double a31;
     static double a32;

     static double  a4;
     static double a41;
     static double a42;
     static double a43;

     static double  a5;
     static double a51;
     static double a52;
     static double a53;
     static double a54;

     static double  b1;
     static double  b3;
     static double  b4;
     static double  b5;

     static double  b6;
     static double b61;
     static double b62;
     static double b63;
     static double b64;
     static double b65;

     double dummy;

     double nirrn;
     int dumcnt;




/* **************************************************************
			Private Parameters
************************************************************** */

     double leafcb[MAXCMNT];

     double sapwoodcb[MAXCMNT];

     double heartwoodcb[MAXCMNT];

     double rootcb[MAXCMNT];

     double labilecb[MAXCMNT];

     double seedcb[MAXCMNT];

     double leafnb[MAXCMNT];

     double sapwoodnb[MAXCMNT];

     double heartwoodnb[MAXCMNT];

     double rootnb[MAXCMNT];

     double labilenb[MAXCMNT];

     double seednb[MAXCMNT];

     double solcb[MAXCMNT];

     double solnb[MAXCMNT];

     double avlnb[MAXCMNT];

 };

#endif
