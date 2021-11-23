/* *************************************************************
****************************************************************
HUMNACTD_CWD.H - describes human disturbances to natural ecosystems

20060126 - DWK created by modifying humnact50b5.h
20060126 - DWK changed include temconst51.hpp to temconst43.hpp
20060126 - DWK changed include from tprocessXML51.h to
           tprocessXML437.h
20060126 - DWK changed include from tveg50b5.h to tveg437.h
20060126 - DWK changed class Humanact50 to class Humanact43
20060126 - DWK changed inheritance of ProcessXML50 to
           ProcessXML43
20070105 - TWC changed class Humanactflux to class Humnact45
2007 - TWC/BSF summary
   GDDMIN, GDDSEED, GDDHARVST, ISPERENNIAL, tkill: add to public
     functions get/set (ISPREENNIAL only get) and declarations
	 in private variables
   Remove up-front declarations of GDDMIN, GDDSEED, GDDHARVST,
     MAXFRI
   Add distflag and fert1950flag to public vars.
   Remove firendep from private vars. and add to private parameters,
      also add RAP to private parameters

****************************************************************
************************************************************* */

#ifndef HUMNACTD_CWD_H
#define HUMNACTD_CWD_H

#include "temconstsd.hpp"
#include "tprocessXMLd.h"
#include "bioms45.hpp"  // Humnact45 uses Biomass class
#include "tvegd.h"     // Humanact45 uses Tveg45 class

class Humnact45 : public ProcessXML45
{

  public:

     Humnact45();

/* **************************************************************
		 Public Functions
************************************************************** */

     void conversion( const int& pdcmnt,
                      const double& slashc,
                      const double& slashn,
                      const double& vegc,
                      const double& vstrn,
                      const double& vston,
                      const double& solc,
                      const double& soiln,
                      const double& cwdloss,
                      const int& time );

     void createWoodProducts( const int& outyr,
                              const double& stemc,
                              const double& stemn );

     void decayProducts( const int& CYCLE );

     void frostDamage( const double& vegc,
                       const double& vegn );

     void getecd( const string& ecd );

     void grazing( const double& leafc,
                  const double& sapc,
                  const double& heartc,
                  const double& labilec,
                  const double& leafn,
                  const double& sapn,
                  const double& heartn,
                  const double& labilen,
                  const double& rootc );

     void harvest( const int& pdm,
                   const double& seedc,
				   const double& seedn,
                   const double& vegc,
                   const double& vegn );

     void resetMonthlyDisturbFluxes( void );

     void resetMonthlyFluxes( void );

     void resetPROD(  const int& CYCLE );

     void resetYrFluxes( void );

     void setAgricFlags( ofstream& rflog1 );

     void setFireNDEP( void );

     void setNoCrops( const int& pdm );

     void setNoGrazing( void );

     void setNoWoodProducts( const int& pdyr );

     void standingdead( const int& pdcmnt,
                      const double& slashc,
                      const double& slashn,
                      const double& vegc,
                      const double& vstrn,
                      const double& cwdloss );


     void updatestanddead( const int& pdyr,
                           const double& rhmoist,
                           const double& dq10,
                           const double& cnltr);

     void updateCropResidue( void );

     void updateCropResidueFluxes(  const int& CYCLE );

     void updateProducts(  const int& CYCLE );

     void updateTotalProductFormation( void );


     // "Get" and "Set" private variables

     // animalresp *********************************************

     inline double getANIMALRESP( void ) { return animalresp; }

     inline void setANIMALRESP( const double& panimalresp )
     {
       animalresp = panimalresp; }

     // c2n ****************************************************

     inline double getC2N( void ) { return c2n; }

     inline void setC2N( const double& pc2n ) { c2n = pc2n; }


     // cflux **************************************************

     inline double getCFLUX( void ) { return cflux; }

     inline void setCFLUX( const double& pcflx )
     {
       cflux = pcflx;
     }

     // clippings  *****************************************

     inline double getCLIPPINGS( void ) { return clippings; }

     inline void setCLIPPINGS( const double& pclippings )
     {
       clippings = pclippings;
     }


     // convrtflx.carbon ***************************************

     inline double getCONVRTFLXC( void )
     {
       return convrtflx.carbon;
     }

     inline void setCONVRTFLXC( const double& pcnvrtflxc )
     {
       convrtflx.carbon = pcnvrtflxc;
     }


     // convrtflx.nitrogen *************************************

     inline double getCONVRTFLXN( void )
     {
       return convrtflx.nitrogen;
     }

     inline void setCONVRTFLXN( const double& pcnvrtflxn )
     {
       convrtflx.nitrogen = pcnvrtflxn;
     }


     // cropprod.carbon ****************************************

     inline double getCROPPRODC( void )
     {
       return cropprod.carbon;
     }

     inline void setCROPPRODC( const double& pcrpprdc )
     {
       cropprod.carbon = pcrpprdc;
     }


     // cropprod.nitrogen **************************************

     inline double getCROPPRODN( void )
     {
       return cropprod.nitrogen;
     }

     inline void setCROPPRODN( const double& pcrpprdn )
     {
       cropprod.nitrogen = pcrpprdn;
     }

     // cropResidue.carbon *************************************

     inline double getCROPRESIDUEC( void )
     {
       return cropResidue.carbon;
     }

     inline void setCROPRESIDUEC( const double& pcrpresiduec )
     {
       cropResidue.carbon = pcrpresiduec;
     }


     // cropResidue.nitrogen ***********************************

     inline double getCROPRESIDUEN( void )
     {
       return cropResidue.nitrogen;
     }

     inline void setCROPRESIDUEN( const double& pcrpresiduen )
     {
       cropResidue.nitrogen = pcrpresiduen;
     }


     // cropResidueFlux.carbon *********************************

     inline double getCROPRESIDUEFLXC( void )
     {
       return cropResidueFlux.carbon;
     }



     // cropResidueFlux.nitrogen *******************************

     inline double getCROPRESIDUEFLXN( void )
     {
       return cropResidueFlux.nitrogen;
     }


     // cropseedC **********************************************

     inline double getCROPSEEDC( const int& pcmnt )
     {
       return cropseedC[pcmnt];
     }

     inline void setCROPSEEDC( const double& pcropseedc,
                               const int& pcmnt )
     {
       cropseedC[pcmnt] = pcropseedc;
     }


     // cropseedSTON *******************************************

     inline double getCROPSEEDSTON( const int& pcmnt )
     {
       return cropseedSTON[pcmnt];
     }

     inline void setCROPSEEDSTON( const double& pcropseedston,
                                  const int& pcmnt )
     {
       cropseedSTON[pcmnt] = pcropseedston;
     }



     // croptopt ***********************************************

     inline double getCROPTOPT( void ) { return croptopt; }

     inline void setCROPTOPT( const double& pcrptopt )
     {
       croptopt = pcrptopt;
     }


     // fertn **************************************************

//     inline double getFERTN( void ) { return fertn; }

//     inline void setFERTN( const double& pfertn )
//     {
//       fertn = pfertn;
//     }

//     FIRENDEP **************************************************

       inline double getFIRENDEP( void ) { return firendep; }

       inline double setFIRENDEP( const double& FIRENDEP )
       {
         return firendep = FIRENDEP;
       }

     // forage.cleaf *********************************

     inline double getFORAGECLEAF( void )
     {
       return forage.cleaf;
     }

     inline void setFORAGECLEAF( const double& pforagecleaf )
     {
       forage.cleaf = pforagecleaf;
     }
     // forage.cleaf *********************************

     inline double getFORAGECSAP( void )
     {
       return forage.csap;
     }

     inline void setFORAGECSAP( const double& pforagecsap )
     {
       forage.csap = pforagecsap;
     }
     // forage.cleaf *********************************

     inline double getFORAGECHEART( void )
     {
       return forage.cheart;
     }

     inline void setFORAGECHEART( const double& pforagecheart )
     {
       forage.cheart = pforagecheart;
     }
     // forage.cleaf *********************************

     inline double getFORAGECLABILE( void )
     {
       return forage.clabile;
     }

     inline void setFORAGECLABILE( const double& pforageclabile )
     {
       forage.clabile = pforageclabile;
     }


     // forage.nleaf *********************************

     inline double getFORAGENLEAF( void )
     {
       return forage.nleaf;
     }

     inline void setFORAGENLEAF( const double& pforagenleaf )
     {
       forage.nleaf = pforagenleaf;
     }

     // forage.nsap *********************************

     inline double getFORAGENSAP( void )
     {
       return forage.nsap;
     }

     inline void setFORAGENSAP( const double& pforagensap )
     {
       forage.nsap = pforagensap;
     }
     // forage.nheart *********************************

     inline double getFORAGENHEART( void )
     {
       return forage.nheart;
     }

     inline void setFORAGENHEART( const double& pforagenheart )
     {
       forage.nheart = pforagenheart;
     }
     // forage.nleaf *********************************

     inline double getFORAGENLABILE( void )
     {
       return forage.nlabile;
     }

     inline void setFORAGENLABILE( const double& pforagenlabile )
     {
       forage.nlabile = pforagenlabile;
     }

     // formCropResidue.carbon *********************************

     inline double getFORMCROPRESIDUEC( void )
     {
       return formCropResidue.carbon;
     }

     inline void setFORMCROPRESIDUEC( const double& pfrmcrpresiduec )
     {
       formCropResidue.carbon = pfrmcrpresiduec;
     }


     // formCropResidue.nitrogen *******************************

     inline double getFORMCROPRESIDUEN( void )
     {
       return formCropResidue.nitrogen;
     }

     inline void setFORMCROPRESIDUEN( const double& pfrmcrpresiduen )
     {
       formCropResidue.nitrogen = pfrmcrpresiduen;
     }


     // formPROD10.carbon **************************************

     inline double getFORMPROD10C( void )
     {
       return formPROD10.carbon;
     }

     inline void setFORMPROD10C( const double& pfrmprod10c )
     {
       formPROD10.carbon = pfrmprod10c;
     }


     // formPROD10.nitrogen ************************************

     inline double getFORMPROD10N( void )
     {
       return formPROD10.nitrogen;
     }

     inline void setFORMPROD10N( const double& pfrmprod10n )
     {
       formPROD10.nitrogen = pfrmprod10n;
     }


     // formPROD100.carbon *************************************

     inline double getFORMPROD100C( void )
     {
       return formPROD100.carbon;
     }

     inline void setFORMPROD100C( const double& pfrmprod100c )
     {
       formPROD100.carbon = pfrmprod100c;
     }


     // formPROD100.nitrogen ***********************************

     inline double getFORMPROD100N( void )
     {
       return formPROD100.nitrogen;
     }

     inline void setFORMPROD100N( const double& pfrmprod100n )
     {
       formPROD100.nitrogen = pfrmprod100n;
     }


     // formTOTPROD.carbon *************************************

     inline double getFORMTOTPRODC( void )
     {
       return formTOTPROD.carbon;
     }

     inline void setFORMTOTPRODC( const double& pfrmtotprodc )
     {
       formTOTPROD.carbon = pfrmtotprodc;
     }


     // formTOTPROD.nitrogen ***********************************

     inline double getFORMTOTPRODN( void )
     {
       return formTOTPROD.nitrogen;
     }

     inline void setFORMTOTPRODN( const double& pfrmtotprodn )
     {
       formTOTPROD.nitrogen = pfrmtotprodn;
     }


     // FRI ****************************************************

     inline int getFRI( void ) { return FRI; }

     inline void setFRI( const int& pfri ) { FRI = pfri; }
     
     // FROSTFLAG ****************************************************

     inline int getFROSTFLAG( void ) { return frostflag; }

     inline void setFROSTFLAG( const int& pfrostflag ) { frostflag = pfrostflag; }

     // GDDMIN *************************************************

     inline double getGDDMIN( const int& pcmnt )
     {
       return GDDMIN[pcmnt];
     }
     inline void setGDDMIN( const double& pgddmin,
                            const int& pcmnt )
     {
       GDDMIN[pcmnt] = pgddmin;
     }


     // GDDSEED *************************************************

     inline double getGDDSEED( const int& pcmnt )
     {
       return GDDSEED[pcmnt];
     }
     inline void setGDDSEED( const double& pgddseed,
                             const int& pcmnt )
     {
       GDDSEED[pcmnt] = pgddseed;
     }


     // GDDHARVST ***********************************************

     inline double getGDDHARVST( const int& pcmnt )
     {
       return GDDHARVST[pcmnt];
     }
     inline void setGDDHARVST( const double& pgddharvst,
                               const int& pcmnt )
     {
       GDDHARVST[pcmnt] = pgddharvst;
     }


     // growdd *************************************************

     inline double getGROWDD( void ) { return growdd; }

     inline void setGROWDD( const double& pgrowdd )
     {
       growdd = pgrowdd;
     }


     // harvstC *********************************************

     inline double getHARVSTC( const int& pcmnt )
     {
       return harvstC[pcmnt];
     }

     inline void setHARVSTC( const double& pharvstC,
                             const int& pcmnt )
     {
       harvstC[pcmnt] = pharvstC;
     }


     // harvstN *********************************************

     inline double getHARVSTN( const int& pcmnt )
     {
       return harvstN[pcmnt];
     }

     inline void setHARVSTN( const double& pharvstN,
                                const int& pcmnt )
     {
       harvstN[pcmnt] = pharvstN;
     }

     // IMMBADD *********************************************
     
     inline double getIMMBADD( void ) { return immbadd; }

     inline void setIMMBADD( const double& pimmbadd ) { immbadd = pimmbadd; }

     // initPROD1.carbon ***************************************

     inline double getINITPROD1C( const int& pdm )
     {
       return initPROD1[pdm].carbon;
     }

     inline void setINITPROD1C( const double& pinprod1c,
                                const int& pdm )
     {
       initPROD1[pdm].carbon = pinprod1c;
     }


     // initPROD1.nitrogen *************************************

     inline double getINITPROD1N( const int& pdm )
     {
       return initPROD1[pdm].nitrogen;
     }

     inline void setINITPROD1N( const double& pinprod1n,
                                const int& pdm )
     {
       initPROD1[pdm].nitrogen = pinprod1n;
     }


     // initPROD10[].carbon ************************************

     inline double getINITPROD10C( const int& i )
     {
       return initPROD10[i].carbon;
     }

     inline void setINITPROD10C( const double& pinprod10c,
                                 const int& i )
     {
       initPROD10[i].carbon = pinprod10c;
     }


     // initPROD10[].nitrogen **********************************

     inline double getINITPROD10N( const int& i )
     {
       return initPROD10[i].nitrogen;
     }

     inline void setINITPROD10N( const double& pinprod10n,
                                 const int& i )
     {
       initPROD10[i].nitrogen = pinprod10n;
     }


     // initPROD100[].carbon ***********************************

     inline double getINITPROD100C( const int& i )
     {
       return initPROD100[i].carbon;
     }

     inline void setINITPROD100C( const double& pinprod100c,
                                  const int& i )
     {
       initPROD100[i].carbon = pinprod100c;
     }


     // initPROD100[].nitrogen *********************************

     inline double getINITPROD100N( const int& i )
     {
       return initPROD100[i].nitrogen;
     }

     inline void setINITPROD100N( const double& pinprod100n,
                                  const int& i )
     {
       initPROD100[i].nitrogen = pinprod100n;
     }

     // isPerennial *********************************

     inline int getISPERENNIAL( const int& pcmnt )
     {
       return isPerennial[pcmnt];
     }

     // irrigate ***********************************************

//     inline double getIRRIGATE( void ) { return irrigate; }

     //manure.carbon *********************************

     inline double getMANUREC( void )
     {
       return manure.carbon;
     }

     inline void setMANUREC( const double& pmanurec )
     {
       manure.carbon = pmanurec;
     }

     // manure.nitrogen *********************************

     inline double getMANUREN( void )
     {
       return manure.nitrogen;
     }

     inline void setMANUREN( const double& pmanuren )
     {
       manure.nitrogen = pmanuren;
     }

     // kd *****************************************************

     inline double getKD( void ) { return kd; }

     inline void setKD( const double& pkd ) { kd = pkd; }

     // natseedC ***********************************************

     inline double getNATSEEDC( void ) { return natseedC; }

     inline void setNATSEEDC( const double& pnatseedc )
     {
       natseedC = pnatseedc;
     }


     // natseedSTRN ********************************************

     inline double getNATSEEDSTRN( void )
     {
       return natseedSTRN;
     }

     inline void setNATSEEDSTRN( const double& pnatseedstrn )
     {
       natseedSTRN = pnatseedstrn;
     }


     // natseedSTON ********************************************

     inline double getNATSEEDSTON( void )
     {
       return natseedSTON;
     }

     inline void setNATSEEDSTON( const double& pnatseedston )
     {
       natseedSTON = pnatseedston;
     }


     // natsoil ************************************************

     inline double getNATSOIL( void ) { return natsoil; }

     inline void setNATSOIL( const double& pnatsoil )
     {
       natsoil = pnatsoil;
     }


     // nattopt ************************************************

     inline double getNATTOPT( void ) { return nattopt; }

     inline void setNATTOPT( const double& pnattopt )
     {
       nattopt = pnattopt;
     }


     // nretent ************************************************

     inline double getNRETENT( void ) { return nretent; }

     inline void setNRETENT( const double& pnretent )
     {
       nretent = pnretent;
     }


     // nsretconv **********************************************

     inline double getNSRETCONV( const int& pcmnt )
     {
       return nsretconv[pcmnt];
     }

     inline void setNSRETCONV( const double& pnsretconv,
                               const int& pcmnt )
     {
       nsretconv[pcmnt] = pnsretconv;
     }


     // nsretent ***********************************************

     inline double getNSRETENT( void ) { return nsretent; }

     inline void setNSRETENT( const double& pnsretent )
     {
       nsretent = pnsretent;
     }


     // nvretconv **********************************************

     inline double getNVRETCONV( const int& pcmnt )
     {
       return nvretconv[pcmnt];
     }

     inline void setNVRETCONV( const double& pnvretconv,
                               const int& pcmnt )
     {
       nvretconv[pcmnt] = pnvretconv;
     }


     // nvretent ***********************************************

     inline double getNVRETENT( void ) { return nvretent; }

     inline void setNVRETENT( const double& pnvretent )
     {
       nvretent = pnvretent;
     }


     // prevCropResidue.carbon *********************************

     inline double getPREVCROPRESIDUEC( void )
     {
       return prevCropResidue.carbon;
     }

     inline void setPREVCROPRESIDUEC( const double& pcrpresc )
     {
       prevCropResidue.carbon = pcrpresc;
     }


     // prevCropResidue.nitrogen *******************************

     inline double getPREVCROPRESIDUEN( void )
     {
       return prevCropResidue.nitrogen;
     }

     inline void setPREVCROPRESIDUEN( const double& pcrpresn )
     {
       prevCropResidue.nitrogen = pcrpresn;
     }


     // prevPROD1.carbon ***************************************

     inline double getPREVPROD1C( void )
     {
       return prevPROD1.carbon;
     }

     inline void setPREVPROD1C( const double& pprod1c )
     {
       prevPROD1.carbon = pprod1c;
     }


     // prev.PROD1.nitrogen ************************************

     inline double getPREVPROD1N( void )
     {
       return prevPROD1.nitrogen;
     }

     inline void setPREVPROD1N( const double& pprod1n )
     {
       prevPROD1.nitrogen = pprod1n;
     }


     // prevPROD10.carbon **************************************

     inline double getPREVPROD10C( void )
     {
       return prevPROD10.carbon;
     }

     inline void setPREVPROD10C( const double& pprod10c )
     {
       prevPROD10.carbon = pprod10c;
     }


     // prevPROD10.nitrogen ************************************

     inline double getPREVPROD10N( void )
     {
       return prevPROD10.nitrogen;
     }

     inline void setPREVPROD10N( const double& pprod10n )
     {
       prevPROD10.nitrogen = pprod10n;
     }


     // prevPROD100.carbon *************************************

     inline double getPREVPROD100C( void )
     {
       return prevPROD100.carbon;
     }

     inline void setPREVPROD100C( const double& pprod100c )
     {
       prevPROD100.carbon = pprod100c;
     }


     // prevPROD100.nitrogen ***********************************

     inline double getPREVPROD100N( void )
     {
       return prevPROD100.nitrogen;
     }

     inline void setPREVPROD100N( const double& pprod100n )
     {
       prevPROD100.nitrogen = pprod100n;
     }


     // PROD1.carbon *******************************************

     inline double getPROD1C( void ) { return PROD1.carbon; }


     // PROD1.nitrogen *****************************************

     inline double getPROD1N( void ) { return PROD1.nitrogen; }


     // PROD10.carbon ******************************************

     inline double getPROD10C( void ) { return PROD10.carbon; }


     // PROD10.nitrogen ****************************************

     inline double getPROD10N( void )
     {
       return PROD10.nitrogen;
     }


     // PROD100.carbon *****************************************

     inline double getPROD100C( void )
     {
       return PROD100.carbon;
     }


     // PROD100.nitrogen ***************************************

     inline double getPROD100N( void )
     {
       return PROD100.nitrogen;
     }


     // PROD1decay.carbon **************************************

     inline double getPROD1DECAYC( void )
     {
       return PROD1decay.carbon;
     }


     // PROD1decay.nitrogen ************************************

     inline double getPROD1DECAYN( void )
     {
       return PROD1decay.nitrogen;
     }


     // PROD10decay.carbon *************************************

     inline double getPROD10DECAYC( void )
     {
       return PROD10decay.carbon;
     }


     // PROD10decay.nitrogen ***********************************

     inline double getPROD10DECAYN( void )
     {
       return PROD10decay.nitrogen;
     }


     // PROD100decay.carbon ************************************

     inline double getPROD100DECAYC( void )
     {
       return PROD100decay.carbon;
     }


     // PROD100decay.nitrogen **********************************

     inline double getPROD100DECAYN( void )
     {
       return PROD100decay.nitrogen;
     }


     // prod10par **********************************************

     inline double getPROD10PAR( void ) { return prod10par; }

     inline void setPROD10PAR( const double& pprod10par )
     {
       prod10par = pprod10par;
     }


     // prod100par *********************************************

     inline double getPROD100PAR( void ) { return prod100par; }

     inline void setPROD100PAR( const double& pprod100par )
     {
       prod100par = pprod100par;
     }


     // productYear ********************************************

     inline int getPRODUCTYEAR( void ) { return productYear; }

     inline void setPRODUCTYEAR( const int& pprodyr )
     {
       productYear = pprodyr;
     }


     // prvcropnpp *********************************************

     inline double getPRVCROPNPP( void ) { return prvcropnpp; }

     inline void setPRVCROPNPP( const double& pprvcropnpp )
     {
       prvcropnpp = pprvcropnpp;
     }


     // residueC ***********************************************

     inline double getRESIDUEC( const int& pcmnt )
     {
       return residueC[pcmnt];
     }

     inline void setRESIDUEC( const double& presidueC,
                              const int& pcmnt )
     {
       residueC[pcmnt] = presidueC;
     }


     // residueN ***********************************************

     inline double getRESIDUEN( const int& pcmnt )
     {
       return residueN[pcmnt];
     }

     inline void setRESIDUEN( const double& presidueN,
                              const int& pcmnt )
     {
       residueN[pcmnt] = presidueN;
     }


     // sconvert ***********************************************

     inline double getSCONVERT( void ) { return sconvert; }

     inline void setSCONVERT( const double& pscnvrt )
     {
       sconvert = pscnvrt;
     }


     // sconvrtflx.carbon **************************************

     inline double getSCONVRTFLXC( void )
     {
       return sconvrtflx.carbon;
     }

     inline void setSCONVRTFLXC( const double& pscnvrtflxc )
     {
       sconvrtflx.carbon = pscnvrtflxc;
     }


     // sconvrtflx.nitrogen ************************************

     inline double getSCONVRTFLXN( void )
     {
       return sconvrtflx.nitrogen;
     }

     inline void setSCONVRTFLXN( const double& pscnvrtflxn )
     {
       sconvrtflx.nitrogen = pscnvrtflxn;
     }


     // slash.carbon *******************************************

     inline double getSLASHC( void ) { return slash.carbon; }

     inline void setSLASHC( const double& pslashc )
     {
       slash.carbon = pslashc;
     }


     // slash.nitrogen *****************************************

     inline double getSLASHN( void ) { return slash.nitrogen; }

     inline void setSLASHN( const double& pslashn )
     {
       slash.nitrogen = pslashn;
     }


     // slashpar ***********************************************

     inline double getSLASHPAR( void ) { return slashpar; }

     inline void setSLASHPAR( const double& pslashpar )
     {
       slashpar = pslashpar;
     }

     // Standingdead.carbon ************************************
     
     inline double getSTANDDEADC( void )
     {
        return standdead.carbon;
     }
     inline void setSTANDDEADC( const double& pstanddeadc )
     {
       standdead.carbon = pstanddeadc;
     }

   
     // Standingdead.nitrogen ************************************

     inline double getSTANDDEADN( void )
     {
        return standdead.nitrogen;
     }
     inline void setSTANDDEADN( const double& pstanddeadn )
     {
       standdead.nitrogen = pstanddeadn;
     }


     // Volatized.carbon ************************************

     inline double getVOLAC( void )
     {
        return vola.carbon;
     }
     inline void setVOLAC( const double& pvolac )
     {
       vola.carbon = pvolac;
     }

     // Volatolized.nitrogen ************************************

     inline double getVOLAN( void )
     {
        return vola.nitrogen;
     }
     inline void setVOLAN( const double& pvolan )
     {
       vola.nitrogen = pvolan;
     }


     // stubble.carbon *****************************************

     inline double getSTUBBLEC( void )
     {
       return stubble.carbon;
     }

     inline void setSTUBBLEC( const double& pstubc )
     {
       stubble.carbon = pstubc;
     }


     // stubble.nitrogen ***************************************

     inline double getSTUBBLEN( void )
     {
       return stubble.nitrogen;
     }

     inline void setSTUBBLEN( const double& pstubn )
     {
       stubble.nitrogen = pstubn;
     }


     // tillfactor *********************************************

     inline double getTILLFACTOR( const int& pcmnt )
     {
       return tillfactor[pcmnt];
     }

     inline void setTILLFACTOR( const double& ptillfact,
                                const int& pcmnt )
     {
       tillfactor[pcmnt] = ptillfact;
     }


     // tkill *********************************************

     inline double getTKILL( const int& pcmnt )
     {
       return tkill[pcmnt];
     }

     inline void setTKILL( const double& ptkill,
                           const int& pcmnt )
     {
       tkill[pcmnt] = ptkill;
     }


     // totec **************************************************

     inline double getTOTEC( void ) { return totec; }

     inline void setTOTEC( const double& ptotec )
     {
       totec = ptotec;
     }


     // TOTPROD.carbon *****************************************

     inline double getTOTPRODC( void )
     {
       return TOTPROD.carbon;
     }


     // TOTPROD.nitrogen ***************************************

     inline double getTOTPRODN( void )
     {
       return TOTPROD.nitrogen;
     }


     // TOTPRODdecay.carbon ************************************

     inline double getTOTPRODDECAYC( void )
     {
       return TOTPRODdecay.carbon;
     }


     // TOTPRODdecay.nitrogen **********************************

     inline double getTOTPRODDECAYN( void )
     {
       return TOTPRODdecay.nitrogen;
     }

     // urine **************************************************

     inline double getURINE( void )
     {
       return urine;
     }

     inline void setURINE( const double& purine )
     {
       urine = purine;
     }


     // vconvert ***********************************************

     inline double getVCONVERT( void ) { return vconvert; }

     inline void setVCONVERT( const double& pvcnvrt )
     {
       vconvert = pvcnvrt;
     }


     // vconvrtflx.carbon **************************************

     inline double getVCONVRTFLXC( void )
     {
       return vconvrtflx.carbon;
     }

     inline void setVCONVRTFLXC( const double& pvcnvrtflxc )
     {
       vconvrtflx.carbon = pvcnvrtflxc;
     }


     // vconvrtflx.nitrogen ************************************

     inline double getVCONVRTFLXN( void )
     {
       return vconvrtflx.nitrogen;
     }

     inline void setVCONVRTFLXN( const double& pvcnvrtflxn )
     {
       vconvrtflx.nitrogen = pvcnvrtflxn;
     }


     // vrespar ************************************************

     inline double getVRESPAR( void ) { return vrespar; }

     inline void setVRESPAR( const double& pvrespar )
     {
       vrespar = pvrespar;
     }


/* *************************************************************
		 Public Variables
************************************************************* */

     // Index for disturbed community type
    int cmnt;

    // Flag indicating a disturbance occurrence
    int distflag;

     // Flag to indicate start of optimum fertilization of
     //   croplands after 1950 (and irrigation)
    int fert1950flag;
    int irrg1950flag;

     // Flag to indicate whether or not optimum
     //   fertilization of croplands occurs
    int fertflag;

    // Monthly fertilizer application
    double fertn;

    // Flag to indicate whether or not optimum irrigation of
    //   croplands occur
    int irrgflag;

    // Monthly irrigation (mm / month)
    double irrigate;

    int lulcyear;

    int massbalflg;

    int mez;

    // Flag to indicate that element is in agriculture during
    //   previous year
    int prvstate;

    // Flag to indicate that element is in agriculture during
    //   current year
    int state;

     // Flag to indicate whether or not croplands are tilled
    int tillflag;

     // Flag to indicate whether or not simulation is conducted
     //   with transient land use data
    int tlulcflag;

    // Annual sum of cflux
    double yrcflux;

    // Annual sum of convrtflx.carbon
    double yrconvrtC;

    // Annual sum of convrtflx.nitrogen
    double yrconvrtN;

    // Annual sum of PROD1decay.carbon
    double yrdecayPROD1C;

    // Annual sum of PROD10decay.carbon
    double yrdecayPROD10C;

    // Annual sum of PROD100decay.carbon
    double yrdecayPROD100C;

    // Annual sum of PROD1decay.nitrogen
    double yrdecayPROD1N;

    // Annual sum of PROD10decay.nitrogen
    double yrdecayPROD10N;

    // Annual sum of PROD100decay.nitrogen
    double yrdecayPROD100N;

    // Annual sum of TOTPRODdecay.carbon
    double yrdecayTOTPRODC;

    // Annual sum of TOTPRODdecay.nitrogen
    double yrdecayTOTPRODN;

    // Annual sum of growing degree days
    double yrgrowdd;

    // Annual sum of fertn
    double yrfertn;

    // Annual sum of cropResidueFlux.carbon
    double yrfluxResidueC;

    // Annual sum of cropResidueFlux.nitrogen
    double yrfluxResidueN;

    // Annual sum of cropprod.carbon
    double yrformPROD1C;

    // Annual sum of formPROD10.carbon
    double yrformPROD10C;

    // Annual sum of formPROD100.carbon
    double yrformPROD100C;

    // Annual sum of cropprod.nitrogen
    double yrformPROD1N;

    // Annual sum of formPROD10.nitrogen
    double yrformPROD10N;

    // Annual sum of formPROD100.nitrogen
    double yrformPROD100N;

    // Annual sum of formCropResidue.carbon
    double yrformResidueC;

    // Annual sum of formCropResidue.nitrogen
    double yrformResidueN;

    // Annual sum of formTOTPROD.carbon
    double yrformTOTPRODC;

    // Annual sum of formTOTPROD.nitrogen
    double yrformTOTPRODN;
    
    //Annual sum of frostflag
    double yrfrost;

    // Annual sum of irrigate
    double yrirrig;

    // Annual sum of nretent
    double yrnrent;

    // Annual sum of nsretent
    double yrnsrent;

    // Annual sum of nvretent
    double yrnvrent;

    // Annual sum of sconvrtflx.carbon
    double yrsconvrtC;

    // Annual sum of sconvrtflx.nitrogen
    double yrsconvrtN;

    // Annual sum of slash.carbon
    double yrslashC;

    // Annual sum of slash.nitrogen
    double yrslashN;

    // Annual sum of stubble.carbon
    double yrstubC;

    // Annual sum of stubble.nitrogen
    double yrstubN;

    // Annual sum of vconvrtflx.carbon
    double yrvconvrtC;

    // Annual sum of vconvrtflx.nitrogen
    double yrvconvrtN;


   private:

/* *************************************************************
		 Private Variables
************************************************************* */

    // Monthly carbon flux to the atmosphere as a result of
    // livestock respiration
    double animalresp;

    // Monthly carbon flux to the atmosphere as a result of
    //   NEP plus agricultural conversion fluxes
    double cflux;

    double clippings;

    // Monthly loss of carbon and nitrogen to the atmosphere as a
    //   result of agricultural conversion
    Biomass convrtflx;

    // Monthly carbon and nitrogen fluxes associated with crop
    //   yield
    Biomass cropprod;

    // Stock of crop residue left in crop fields after harvest
    Biomass cropResidue;

    // Monthly loss of carbon from burning crop residue
    Biomass cropResidueFlux;

    double croptopt;


    // Monthly forage rates
    Biomass forage;

    // Monthly creation of crop residue from harvest
    Biomass formCropResidue;

    // Monthly formation of 10-year wood products
    Biomass formPROD10;

    // Monthly formation of 100-year wood products
    Biomass formPROD100;

    // Monthly formation of all "products"
    Biomass formTOTPROD;

     // Fire return interval
     int FRI;

     // Flag to indicate frost damage
     int frostflag;

     // Growing degree day thresholds for different crops
     double GDDMIN[MAXCMNT];
     double GDDSEED[MAXCMNT];
     double GDDHARVST[MAXCMNT];

     // Growing degree days ( degrees C )
    double growdd;

    double immbadd;

    Biomass initPROD1[366];

    Biomass initPROD10[10];

    Biomass initPROD100[100];

    // Perennial crop flag: does crop survive harvest? ( 1=yes, 0=no )
    int isPerennial[MAXCMNT];

     // Intrinsic decomposition rate
    double kd;

    // Monthly manure inputs
    Biomass manure;

    double natsoil;

    double natseedC;

    double natseedSTRN;

    double natseedSTON;

    double nattopt;

    // Total nitrogen retained by ecosystem after conversion
    double nretent;

    // Nitrogen from soil organic matter retained after
    //   conversion
    double nsretent;

    // Nitrogen from vegetation biomass retained after
    //   conversion
    double nvretent;

    // Carbon and nitrogen stocks in agricultural products
    //   during previous month
    Biomass prevPROD1;

    // Carbon and nitrogen stocks in 10-year wood products
    //   during previous month
    Biomass prevPROD10;

    // Carbon and nitrogen stocks in 100-year wood products
    //   during previous month
    Biomass prevPROD100;

    // Carbon and nitrogen stocks in agricultural products
    //   during current month
    Biomass PROD1;

    // Carbon and nitrogen stocks in 10-year wood products
    //   during current month
    Biomass PROD10;

    // Carbon and nitrogen stocks in 100-year wood products
    //   during current month
    Biomass PROD100;

    // Loss of carbon and nitrogen from "decay" of agricultural
    //   products
    Biomass PROD1decay;

    // Loss of carbon and nitrogen from "decay" of 10-year wood
    //   products
    Biomass PROD10decay;

    // Loss of carbon and nitrogen from "decay" of 100-year wood
    //   products
    Biomass PROD100decay;

    int productYear;

    // Crop net primary production during previous month
    double prvcropnpp;

    // Stock of crop residue from previous month
    Biomass prevCropResidue;

    // Soil organic matter lost during conversion to agriculture
    Biomass sconvrtflx;

    // Stock of slash resulting from conversion to agriculture
    Biomass slash;

    // Stock of crop residue that is actually added to soil
    //   (i.e. residue that is not burned off after harvest)
    Biomass stubble;

    // Stock of standing dead added to slash and volatilized resulting from storms
    Biomass standdead;
    Biomass vola;

    // Crop-specific "killing temperature" below which frost damage occurs
    double tkill[MAXCMNT];

    // Total carbon storage in ecosystem (i.e. VEGC + TOTSOLC,
    //   does not include carbon in agricultural and wood
    //   products
    double totec;

    // Carbon and nitrogen stocks in all agricultural and wood
    //   products
    Biomass TOTPROD;

    // Loss of carbon and nitrogen from "decay" of all
    //   agricultural and wood products
    Biomass TOTPRODdecay;

    // Monthly urine inputs
    double urine;

    // Vegetation biomass lost during conversion to agriculture
    Biomass vconvrtflx;


/* *************************************************************
                   Private Parameters
************************************************************* */

    double c2n;

    double cropseedC[MAXCMNT];
    double cropseedSTON[MAXCMNT];

    double firendep;

    double harvstC[MAXCMNT];
    double harvstN[MAXCMNT];

    double nsretconv[MAXCMNT];
    double nvretconv[MAXCMNT];

    double prod10par;
    double prod100par;

    double RAP;

    double residueC[MAXCMNT];
    double residueN[MAXCMNT];

    double sconvert;

    double slashpar;

    double tillfactor[MAXCMNT];

    double vconvert;

    double vrespar;

};

#endif

