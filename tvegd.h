/** **************************************************************
*****************************************************************
TVEGD.H -  Vegetation characteristics used in the Terrestrial
                Ecosystem Model (TEM)

****************************************************************
************************************************************* */

#ifndef TVEG45_equil_H
#define TVEG45_equil_H

/**
    tveg45: uses constants declared in temconsts.hpp, uses biomass structure
    established by bioms.hpp, inherits xml reading methods from tprocessxml, and
    declares an object of the penmon class, which is used for the calculation of
    water and energy fluxes 
*/

#include "temconstsd.hpp"
#include "bioms45.hpp"
#include "tprocessXMLd.h"
#include "penmond.h"

class Tveg45 : public ProcessXML45
{

  public:

     Tveg45();

/** **************************************************************
		 Public Functions
************************************************************** */

     void aerodynamics(const int& pdcmnt,
		               const double& windspeed); 
	 
	 void allocate( const int& pdcmnt,
                    const int& pndays,
                    const int& nfeed,
                    const int& pdm,
                    const double& gdd,
                    const int& agstate,
                    const int& agharvest );

     void boundTOPT( const int& pcmnt );

     void deltafo3( const int& pdcmnt,
                    const double& d40 );

     void getecd( const string& ecd );

     void gppxclm( const int& pdcmnt,
                     const int& pndays,
                     const double& co2,
                     const double& par,
                     const double& vpr,
                     const double& vpd,
                     const double& pdayl,
                     const double& kext,
                     const double& rhmcrb,
                     const double& prec,
                     ofstream& temdiagfile );
     
     void litterresp( const int& pdcmnt,
                      const int& pndays );

     void nupxclm( const int& pdcmnt,
                    const double& nmax_grow,
                     const double& soilh2o,
                     const double& availn,
                     const double& rofT,
                     const double& ksoil,
                     const double& foliage,
                     const double& rootmass );

     void petsw  ( const int& pdcmnt,
                        const int& pdm,
                        const int& pndays,
                        const double& pdayl,
                        const double& taird,
                        const double& tairn,
                        const double& co2,
                        const double& nirrn,
                        const double& lwoutd,
                        const double& lwoutn,
                        const double& vpr,
                        const double& vpdd,
                        const double& vpdn,
                        const double& snowcover,
                        const double& precip,
                        const double& esoilmmmo, 
                        const double& avlh2o,
                        const double& elev );

     void phenology(const int& pdcmnt,
                    const int& pdm,
                    const double& tair,
                    const double& ptaird,
                    const double& ptairn,
                    const double& avlh2o,
                    const double& awcapmm,
                    const double& prec,
                    const double& pdayl);


     void resetEcds( const int& pcmnt, const double& psiplusc );

     void resetMonthlyFluxes( void );

     void resetNEWTOPT( const int& pcmnt,
                        const double& tair,
                        const double& prtairphi,
                        const double& prphi );

     void resetYrFluxes( void );

     void rxTLaRS( const int& pdcmnt,
                     const double& ptair,
                     const double& ptref );

     double setCI( const int& pdcmnt,
                   const int& pndays,
                   const double& fp,
                   const double& fd,
                   const double& ca,
                   const double& fsoil,
                   const double& pdayl );

     void setRESPQ10( const int& pdcmnt, const double& tair );

     void setTEMP( const int& pdcmnt, const double& tair );


      void updateDynamics( const int& pdcmnt,
                             const double& ninput,
                             const double& availn,
                             const int& nfeed,
                             const int& agstate,
                             const int& agperennial,
                             const int& fertflag,
                             const double& netnmin,
                             double& agfertn,
                             const double& soninp );



     // "Get" and "Set" private variables and parameters

     // adjc2n *************************************************

     inline double getADJC2N( void ) { return adjc2n; }

     inline void setADJC2N( const double& padjc2n )
     {
       adjc2n = padjc2n;
     };

// allocation get and set methods
     // alloclc ****************************************************

     inline double getALLOCLC( void ) { return leaf.allocc; };

     inline void setALLOCLC( const double& palloclc ) { leaf.allocc = palloclc; };

     // allocsc ****************************************************

     inline double getALLOCSC( void ) { return sapwood.allocc; };

     inline void setALLOCSC( const double& pallocsc ) { sapwood.allocc = pallocsc; };

     // allochc ****************************************************

     inline double getALLOCHC( void ) { return heartwood.allocc; };

     inline void setALLOCHC( const double& pallochc ) { heartwood.allocc = pallochc; };

     // allocrc ****************************************************

     inline double getALLOCRC( void ) { return root.allocc; };

     inline void setALLOCRC( const double& pallocrc ) { root.allocc = pallocrc; };

     // allocseedc ****************************************************

     inline double getALLOCSEEDC( void ) { return seed.allocc; };

     inline void setALLOCSEEDC( const double& pallocseedc ) { seed.allocc = pallocseedc; };

     // allocilc ****************************************************

     inline double getALLOCILC( void ) { return leaf.allocic; };

     inline void setALLOCILC( const double& pallocilc ) { leaf.allocic = pallocilc; };

     // allocisc ****************************************************

     inline double getALLOCISC( void ) { return sapwood.allocic; };

     inline void setALLOCISC( const double& pallocisc ) { sapwood.allocic = pallocisc; };

     // allocihc ****************************************************

     inline double getALLOCIHC( void ) { return heartwood.allocic; };

     inline void setALLOCIHC( const double& pallocihc ) { heartwood.allocic = pallocihc; };

     // allocirc ****************************************************

     inline double getALLOCIRC( void ) { return root.allocic; };

     inline void setALLOCIRC( const double& pallocirc ) { root.allocic = pallocirc; };

     // allociseedc ****************************************************

     inline double getALLOCISEEDC( void ) { return seed.allocic; };

     inline void setALLOCISEEDC( const double& pallociseedc ) { seed.allocic = pallociseedc; };

     
     // allocln ****************************************************

     inline double getALLOCLN( void ) { return leaf.allocn; };

     inline void setALLOCLN( const double& pallocln ) { leaf.allocn = pallocln; };

     // allocsn ****************************************************

     inline double getALLOCSN( void ) { return sapwood.allocn; };

     inline void setALLOCSN( const double& pallocsn ) { sapwood.allocn = pallocsn; };

     // allochn ****************************************************

     inline double getALLOCHN( void ) { return heartwood.allocn; };

     inline void setALLOCHN( const double& pallochn ) { heartwood.allocn = pallochn; };

     // allocrn ****************************************************

     inline double getALLOCRN( void ) { return root.allocn; };

     inline void setALLOCRN( const double& pallocrn ) { root.allocn = pallocrn; };

     // allocseedn ****************************************************

     inline double getALLOCSEEDN( void ) { return seed.allocn; };

     inline void setALLOCSEEDN( const double& pallocseedn ) { seed.allocn = pallocseedn; };
     
     // allociln ****************************************************

     inline double getALLOCILN( void ) { return leaf.allocin; };

     inline void setALLOCILN( const double& pallociln ) { leaf.allocin = pallociln; };

     // allocsn ****************************************************

     inline double getALLOCISN( void ) { return sapwood.allocin; };

     inline void setALLOCISN( const double& pallocisn ) { sapwood.allocin = pallocisn; };

     // allochn ****************************************************

     inline double getALLOCIHN( void ) { return heartwood.allocin; };

     inline void setALLOCIHN( const double& pallocihn ) { heartwood.allocin = pallocihn; };

     // allocrn ****************************************************

     inline double getALLOCIRN( void ) { return root.allocin; };

     inline void setALLOCIRN( const double& pallocirn ) { root.allocn = pallocirn; };

     // allocseedn ****************************************************

     inline double getALLOCISEEDN( void ) { return seed.allocin; };

     inline void setALLOCISEEDN( const double& pallociseedn ) { seed.allocin = pallociseedn; };


// end of allocation get and set methods

     // c2n ****************************************************

     inline double getC2N( void ) { return c2n; };

     inline void setC2N( const double& pc2n ) { c2n = pc2n; };



     // c2nb ***************************************************

     inline double getC2NB( const int& pcmnt )
     {
       return c2nb[pcmnt];
     };

     inline void setC2NB( const double& pc2nb,
                          const int& pcmnt )
     {
       c2nb[pcmnt] = pc2nb;
     };


     // c2nmin *************************************************

     inline double getC2NMIN( const int& pcmnt )
     {
       return c2nmin[pcmnt];
     };

     inline void setC2NMIN( const double& pc2nmin,
                            const int& pcmnt )
     {
       c2nmin[pcmnt] = pc2nmin;
     };


     // cmax ***************************************************

     inline double getCMAX( void ) { return cmax; };

     inline void setCMAX( const double& pcmax )
     {
       cmax = pcmax;
     };



     // cmax1b *************************************************

     inline double getCMAX1B( const int& pcmnt )
     {
       return cmax1b[pcmnt];
     };

     inline void setCMAX1B( const double& pcmax1b,
                            const int& pcmnt )
     {
       cmax1b[pcmnt] = pcmax1b;
     };



     // cneven *************************************************

     inline double getCNEVEN( void ) { return cneven; };

     inline void setCNEVEN( const double& pcneven )
     {
       cneven = pcneven;
     };

     // cnltr **************************************************

      inline double getCNLTR( const int& pcmnt )
      {
        return cnltr[pcmnt];
      };

      inline void setCNLTR( const double& pcnltr,
                      const int& pcmnt )
      {
        cnltr[pcmnt] = pcnltr;
      };



     // cnmin **************************************************

     inline double getCNMIN( const int& pcmnt )
     {
       return cnmin[pcmnt];
     };

     inline void setCNMIN( const double& pcnmin,
                           const int& pcmnt )
     {
       cnmin[pcmnt] = pcnmin;
     };


     // currentveg *********************************************

     inline int getCURRENTVEG( void ) { return currentveg; };

     inline void setCURRENTVEG( const int& ptveg )
     {
       currentveg = ptveg;
     };


     // dc2n ***************************************************

     inline double getDC2N( void ) { return dc2n; };

     inline void setDC2N( const double& pdc2n )
     {
       dc2n = pdc2n;
     };

     // dfo3 *********************************************

     inline double getDFO3( void ) { return dfo3; };

     // errcnt *************************************************

     inline double getERRCNT( void ) { return errcnt; };

     inline void setERRCNT( const double& perrcnt )
     {
       errcnt = perrcnt;
     };

     // fozone *************************************************

     inline double getFOZONE( void ) { return fozone; };

     inline void setFOZONE( const double& pfozone )
     {
       fozone = pfozone;
     };

     // frdl *************************************************

     inline double getFRDL( void ) { return frdl; };

     // fco2 *************************************************

     inline double getFCO2( void ) { return fco2; };

     // fh2o *************************************************
  
     inline double getFH2O( void ) { return fh2o; };

     // fo3 *************************************************

     inline double getFO3( void ) { return fo3; };



     // fpc ****************************************************

     inline double getFPC( void ) { return fpc; };

     inline void setFPC( const double& pfpc )
     {
       fpc = pfpc;
     };


     // gc *************************************************

     inline double getGC( void ) { return gc; };

     inline void setGC( const double& pgc )
     {
       gc = pgc;
     };

     // gpp ****************************************************

     inline double getGPP( void ) { return gpp; };
     
     
     // gpr ****************************************************

     inline double getGPR( void ) { return gpr; };

     // gs *************************************************

     inline double getGS( void ) { return gs; };

     inline void setGS( const double& pgs )
     {
       gs = pgs;
     };

     // ingpp **************************************************

     inline double getINGPP( void ) { return ingpp; };

     // initcneven *********************************************

     inline double getINITCNEVEN( const int& pcmnt )
     {
       return initcneven[pcmnt];
     };

     inline void setINITCNEVEN( const double& pincneven,
                                const int& pcmnt )
     {
       initcneven[pcmnt] = pincneven;
     };

     // innpp **************************************************

     inline double getINNPP( void ) { return innpp; };

     // inuptake ***********************************************

     inline double getINUPTAKE( void )
     {
       return inuptake;
     };


     // kc *****************************************************

     inline double getKC( const int& pcmnt )
     {
       return kc[pcmnt];
     };

     inline void setKC( const double& pkc, const int& pcmnt )
     {
       kc[pcmnt] = pkc;
     };


     // ki *****************************************************

     inline double getKI( const int& pcmnt )
     {
       return ki[pcmnt];
     };

     inline void setKI( const double& pki, const int& pcmnt )
     {
       ki[pcmnt] = pki;
     };


     // kn1 ****************************************************

     inline double getKN1( const int& pcmnt )
     {
       return kn1[pcmnt];
     };

     inline void setKN1( const double& pkn1, const int& pcmnt )
     {
       kn1[pcmnt] = pkn1;
     };


     // kra ****************************************************

     inline double getKRA( const int& pcmnt )
     {
       return kra[pcmnt];
     };

     inline void setKRA( const double& pkra, const int& pcmnt )
     {
       kra[pcmnt] = pkra;
     };



     // lai ****************************************************

     inline double getLAI( void ) { return lai; };
     inline void setLAI( const double& plai ) { lai = plai; };

     // laiopt ****************************************************

     inline double getLAIOPT( void ) { return laiopt; };



     // plant compartment carbon stocks **********************************
     inline double getLEAFC( void ) { return leaf.carbon; };
     inline void setLEAFC( const double& rleafc ) { leaf.carbon = rleafc; };

     inline double getSAPWOODC( void ) { return sapwood.carbon; };
     inline void setSAPWOODC( const double& psapwoodc ) { sapwood.carbon = psapwoodc; };

     inline double getHEARTWOODC( void ) { return heartwood.carbon; };
     inline void setHEARTWOODC( const double& pheartwoodc ) { heartwood.carbon = pheartwoodc; };

     inline double getROOTC( void ) { return root.carbon; };
     inline void setROOTC( const double& prootc ) { root.carbon = prootc; };

     inline double getSEEDC( void ) { return seed.carbon; };
     inline void setSEEDC( const double& pseedc ) { seed.carbon = pseedc; };

     inline double getLABILEC( void ) { return labile.carbon; };
     inline void setLABILEC( const double& plabilec ) { labile.carbon = plabilec; };

     inline double getSTRC( void ) { return strctrl.carbon; };
     inline void setSTRC( const double& pstrc ) { strctrl.carbon = pstrc; };

     inline double getVEGC( void ) { return plant.carbon; };
     inline void setVEGC( const double& pvegc ) { plant.carbon = pvegc; };


     // plant compartment nitrogen stocks **********************************
     inline double getLEAFN( void ) { return leaf.nitrogen; };
     inline void setLEAFN( const double& pleafn ) { leaf.nitrogen = pleafn; };

     inline double getSAPWOODN( void ) { return sapwood.nitrogen; };
     inline void setSAPWOODN( const double& psapwoodn ) { sapwood.nitrogen = psapwoodn; };

     inline double getHEARTWOODN( void ) { return heartwood.nitrogen; };
     inline void setHEARTWOODN( const double& pheartwoodn ) { heartwood.nitrogen = pheartwoodn; };

     inline double getROOTN( void ) { return root.nitrogen; };
     inline void setROOTN( const double& prootn ) { root.nitrogen = prootn; };

     inline double getSEEDN( void ) { return seed.nitrogen; };
     inline void setSEEDN( const double& pseedn ) { seed.nitrogen = pseedn; };

     inline double getLABILEN( void ) { return labile.nitrogen; };
     inline void setLABILEN( const double& plabilen ) { labile.nitrogen = plabilen; };

     inline double getSTRN( void ) { return strctrl.nitrogen; };
     inline void setSTRN( const double& pstrn) { strctrl.nitrogen = pstrn; };

     inline double getVEGN( void ) { return plant.nitrogen; };
     inline void setVEGN( const double& pvegn ) { plant.nitrogen = pvegn; };

     // leaf litterfall ******************************************
     inline double getLTRLC( void ) { return leaf.ltrc; };

     inline double getLTRLN( void ) { return leaf.ltrn; };

     // sapwoodlitterfall ******************************************
     inline double getLTRSC( void ) { return sapwood.ltrc; };

     inline double getLTRSN( void ) { return sapwood.ltrn; };

     // heartwoodlitterfall ******************************************
     inline double getLTRHC( void ) { return heartwood.ltrc; };

     inline double getLTRHN( void ) { return heartwood.ltrn; };

     // root litterfall ******************************************
     inline double getLTRRC( void ) { return root.ltrc; };

     inline double getLTRRN( void ) { return root.ltrn; };

     // seed litterfall ******************************************
     inline double getLTRSEEDC( void ) { return seed.ltrc; };

     inline double getLTRSEEDN( void ) { return seed.ltrn; };


     // newtopt ************************************************

     inline double getNEWTOPT( void ) { return newtopt; };

     inline void setNEWTOPT( const double& pnewtopt )
     {
       newtopt = pnewtopt;
     };


     // nmax ***************************************************

     inline double getNMAX( void ) { return nmax; };

     inline void setNMAX( const double& pnmax )
     {
       nmax = pnmax;
     };


     // nmax1b *************************************************

     inline double getNMAX1B( const int& pcmnt )
     {
       return nmax1b[pcmnt];
     };

     inline void setNMAX1B( const double& pnmax1b,
                            const int& pcmnt )
     {
       nmax1b[pcmnt] = pnmax1b;
     };


     // NNF *************************************************

     inline double getNNF( const int& pcmnt )
     {
       return nnf[pcmnt];
     };

     inline void setNNF( const double& pnnf,
                            const int& pcmnt )
     {
       nnf[pcmnt] = pnnf;
     };


     // npp ****************************************************

     inline double getNPP( void ) { return npp; };


     // nresorb ************************************************

     inline double getNRESORBL( void ) { return leaf.nresorb; };
     inline double getNRESORBS( void ) { return sapwood.nresorb; };
     inline double getNRESORBR( void ) { return root.nresorb; };
     inline double getNRESORBSEED( void ) { return seed.nresorb; };



     // nuptake ************************************************

     inline double getNUPTAKE( void ) { return nuptake; };


     // o3para *************************************************

     inline double getO3PARA( const int& pcmnt )
     {
       return o3para[pcmnt];
     };

     inline void setO3PARA( const double& po3para,
                            const int& pcmnt )
     {
       o3para[pcmnt] = po3para;
     };


     // o3parb *************************************************

     inline double getO3PARB( const int& pcmnt )
     {
       return o3parb[pcmnt];
     };

     inline void setO3PARB( const double& po3parb,
                            const int& pcmnt )
     {
       o3parb[pcmnt] = po3parb;
     };


     // o3parc *************************************************

     inline double getO3PARC( const int& pcmnt )
     {
       return o3parc[pcmnt];
     };

     inline void setO3PARC( const double& po3parc,
                            const int& pcmnt )
     {
       o3parc[pcmnt] = po3parc;
     };

   // pecanw *************************************************

   inline double getPECANW( void ) { return pecanw; };

   // pesoilw *************************************************

   inline double getPESOILW( void ) { return pesoilw; };
   
   // esoilmmmo *************************************************

   inline double getESOILMMMO( void ) { return esoilmmmo; };
   inline void setESOILMMMO( const double& pesoilmmmo )
     {
       esoilmmmo = pesoilmmmo;
     };


   // shfluxw *************************************************

   inline double getSHFLUXW( void ) { return shfluxw; };

  // soninput ************************************************

   inline double getSONINPUT( void ) { return soninput; };

  // denitr ************************************************
  
   inline double getDENITR( void ) { return denitr; };
 
    // pet *************************************************

     inline double getPET( void ) { return pet; };

     inline void setPET( const double& ppet )
     {
       pet = ppet;
     };

    // petll *************************************************
     inline double getPETLL( void ) { return petll; };

     inline void setPETLL( const double& ppetll )
     {
       petll = ppetll;
     };

     // phen *************************************************
         //  BSF COMBO

    inline int getPHEN( const int& pcmnt ) { return phen[pcmnt]; };

     // phi *************************************************

    inline double getPHI( void ) { return phi; };

    inline void setPHI( const double& pphi )
    {
      phi = pphi;
    };
     // prvphi *************************************************

    inline double getPRVPHI( void ) { return prvphi; };

    inline void setPRVPHI( const double& pprvphi )
    {
      prvphi = pprvphi;
    };


    // phicnt *************************************************

   inline double getPHICNT( void ) { return phicnt; };

   inline void setPHICNT( const double& pphicnt )
   {
     phicnt = pphicnt;
   };



     // potveg *************************************************

     inline int getPOTVEG( void ) { return potveg; };

     inline void setPOTVEG( const int& ptveg )
     {
       potveg = ptveg;
     };

     // ratref***************************************************

     inline double getRATREF( const int& pcmnt )
     {
       return ratref[pcmnt];
     };


      // href ****************************************************

     inline double getHREF( const int& pcmnt )
     {
       return href[pcmnt];
     };

     // rg *****************************************************
     inline double getRGRWTH( void ) { return plant.rg; };

     // rmleaf *****************************************************
     inline double getRMLEAF( void ) { return leaf.rm; };

     // rmsapwood *****************************************************
     inline double getRMSAPWOOD( void ) { return sapwood.rm; };

     // rmroot *****************************************************
     inline double getRMROOT( void ) { return root.rm; };

     // rmlabile *****************************************************
     inline double getRMLABILE( void ) { return labile.rm; };

    // rmseed *****************************************************
    inline double getRMSEED( void ) { return seed.rm; };


    // temperature multiplier for rm: rmt
    inline double getRMT( void ) { return rmt; };

     // rltrc **********************************************

     inline double getRLTRC( void ) { return rltrc; };

     inline void setRLTRC( const double& prltrc )
     {
       rltrc = prltrc;
     };


     // rnpp **********************************************

     inline double getRNPP( void ) { return rnpp; };

     inline void setRNPP( const double& prnpp )
     {
       rnpp = prnpp;
     };
     
     // rgpp **********************************************

     inline double getRGPP( void ) { return rgpp; };

     inline void setRGPP( const double& prgpp )
     {
       rgpp = prgpp;
     };
     
     // ringpp **********************************************

     inline double getRINGPP( void ) { return ringpp; };

     inline void setRINGPP( const double& pringpp )
     {
       ringpp = pringpp;
     };
     
     // rlabilec **********************************************

     inline double getRLABILEC( void ) { return rlabilec; };

     inline void setRLABILEC( const double& prlabilec )
     {
       rlabilec = prlabilec;
     };
     
     // rlabilen **********************************************

     inline double getRLABILEN( void ) { return rlabilen; };

     inline void setRLABILEN( const double& prlabilen )
     {
       rlabilen = prlabilen;
     };
     
     // rdemandc **********************************************

     inline double getRDEMANDC( void ) { return rdemandc; };

     inline void setRDEMANDC( const double& prdemandc )
     {
       rdemandc = prdemandc;
     };
     
     // rdemandc **********************************************

     inline double getRDEMANDN( void ) { return rdemandn; };

     inline void setRDEMANDN( const double& prdemandn )
     {
       rdemandn = prdemandn;
     };

     // rprec **********************************************

     inline double getRPREC( void ) { return rprec; };

     inline void setRPREC( const double& prprec )
     {
       rprec = prprec;
     };

    // rphi **********************************************

    inline double getRPHI( void ) { return rphi; };

    inline void setRPHI( const double& prphi )
    {
      rphi = prphi;
    };
    
    // rphish **********************************************

    inline double getRPHISH( void ) { return rphish; };

    inline void setRPHISH( const double& prphish )
    {
      rphish = prphish;
    };

    // rpleaf **********************************************

    inline double getRPLEAF( void ) { return rpleaf; };

    inline void setRPLEAF( const double& prpleaf )
    {
      rpleaf = prpleaf;
    };


    // rtair **********************************************

    inline double getRTAIR( void ) { return rtair; };

    inline void setRTAIR( const double& prtair )
    {
      rtair = prtair;
    };

    // rtairphi **********************************************

    inline double getRTAIRPHI( void ) { return rtairphi; };

    inline void setRTAIRPHI( const double& prtairphi )
    {
      rtairphi = prtairphi;
    };


     // sla ****************************************************

     inline double getSLA( const int& pcmnt )
     {
       return sla[pcmnt];
     };

     inline void setSLA( const double& psla, const int& pcmnt )
     {
       sla[pcmnt] = psla;
     };

     // subtype ************************************************

     inline int getSUBTYPE( void ) { return subtype; };

     inline void setSUBTYPE( const int& psubtype )
     {
       subtype = psubtype;
     };

     // tauleaf **************************************************

     inline double getTAULEAF( const int& pcmnt )
     {
       return tauleaf[pcmnt];
     };

     inline void setTAULEAF( const double& ptauleaf,
                           const int& pcmnt )
     {
       tauleaf[pcmnt] = ptauleaf;
     };

     // tausapwood **************************************************

     inline double getTAUSAPWOOD( const int& pcmnt )
     {
       return tausapwood[pcmnt];
     };

     inline void setTAUSAPWOOD( const double& ptausapwood,
                           const int& pcmnt )
     {
       tausapwood[pcmnt] = ptausapwood;
     };

     // tauheartwood **************************************************

     inline double getTAUHEARTWOOD( const int& pcmnt )
     {
       return tauheartwood[pcmnt];
     };

     inline void setTAUHEARTWOOD( const double& ptauheartwood,
                           const int& pcmnt )
     {
       tauheartwood[pcmnt] = ptauheartwood;
     };

     // tauroot **************************************************

    inline double getTAUROOT( const int& pcmnt )
    {
      return tauroot[pcmnt];
    };

    inline void setTAUROOT( const double& ptauroot,
                      const int& pcmnt )
    {
      tauroot[pcmnt] = ptauroot;
    };

    // tauseed **************************************************

    inline double getTAUSEED( const int& pcmnt )
    {
     return tauseed[pcmnt];
    };

    inline void setTAUSEED( const double& ptauseed,
                 const int& pcmnt )
    {
     tauseed[pcmnt] = ptauseed;
    };

    // temp ***************************************************

    inline double getTEMP( void ) { return temp; };

    inline void setTEMP( const double& ptemp )
    {
      temp = ptemp;
    };


     // tmax ***************************************************

     inline double getTMAX( const int& pcmnt )
     {
       return tmax[pcmnt];
     };

     inline void setTMAX( const double& ptmax,
                          const int& pcmnt )
     {
       tmax[pcmnt] = ptmax;
     };


     // tmin ***************************************************

     inline double getTMIN( const int& pcmnt )
     {
       return tmin[pcmnt];
     };

     inline void setTMIN( const double& ptmin,
                          const int& pcmnt )
     {
       tmin[pcmnt] = ptmin;
     };


     // topt ***************************************************

     inline double getTOPT( void ) { return topt; };

     inline void setTOPT( const double& ptopt )
     {
       topt = ptopt;
     };

     // toptmic ***************************************************
     inline double getTOPTMIC( void ) { return toptmic; };

     inline void setTOPTMIC( const double& ptoptmic )
     {
       toptmic = ptoptmic;
     };


     // toptmax ************************************************

     inline double getTOPTMAX( const int& pcmnt )
     {
       return toptmax[pcmnt];
     };

     inline void setTOPTMAX( const double& ptoptmax,
                             const int& pcmnt )
     {
       toptmax[pcmnt] = ptoptmax;
     };


     // toptmin ************************************************

     inline double getTOPTMIN( const int& pcmnt )
     {
       return toptmin[pcmnt];
     };

     inline void setTOPTMIN( const double& ptoptmin,
                             const int& pcmnt )
     {
       toptmin[pcmnt] = ptoptmin;
     };

// totc **************************************************

    inline double getTOTC( void ) { return totc; }

    inline void setTOTC( const double& ptotc )
    {
      totc = ptotc;
    }

// vegninp **************************************************

    inline double getVEGNINP( void ) { return vegninp; }

    inline void setVEGNINP( const double& pvegninp )
    {
      vegninp = pvegninp;
    }


/** *************************************************************
		 Public Variables
************************************************************* */


     /// int cmnt: Index for community type
     int cmnt;

     /// int hemisphere: flag marking grid cell as belonging to northern (1) or southern (0) hemisphere
     int hemisphere;

     /// penmon45 instantiation of variable pen: deals with water and energy budgets from surface
     Penmon45 pen;

     /// double yralloc{l,s,r}c: annual total-allocations to leaf, stem, root
     double yralloclc;
     double yrallocsc;
     double yrallocrc;
     
     /// double yralloci{l,s,r}c: annual investment-allocations to leaf, stem, root
     double yrallocilc;
     double yrallocisc;
     double yrallocirc;
     
     /// double yrallocseed{c,n}: annual allocations to seed
     double yrallocseedc;
     double yrallociseedc;
     double yrallocseedn;

     /// double yrc2n: ratio of yrcarbon to yrnitrogen
     double yrc2n;

     /// double yrcarbon: Annual sum of plant.carbon
     double yrcarbon;

     /// double yrfpc: Sum of monthly FPC
     double yrfpc;

	 /// double yrgc: annual canopy conductance
	 double yrgc;

     /// double yrgpp: annual sum of monthly GPP
     double yrgpp;

     /// double yrgpr: annual sum of plant respiration
     double yrgpr;

	 /// double yrgs: annual stomatal conductance
	 double yrgs;

     /// double yringpp: annual sum of non-downregulated gpp
     double yringpp;

     /// double yrinnpp: annual sum of non-downregulated npp
     double yrinnpp;

     /// double yrinnup: annual sum of non-downregulated nup
     double yrinnup;

     /// double yrlai: sum of monthly LAI
     double yrlai;

     /// double yrltrc: annual sum of plant.ltrc
     double yrltrc;

     /// double yrltrn: annual sum of plant.ltrn
     double yrltrn;

     /// double yrnitrogen: annual sum of plant.nitrogen
     double yrnitrogen;

     /// double yrnpp: annual sum of npp
     double yrnpp;

     /// double yrnresorb: annual sum of nresorb
     double yrnrsorb;

     /// double yrnup: annual sum of n uptake
     double yrnup;

     /// double yrp{leaf, sapwood, root}: annual sum of fractional total-allocations
     double yrpleaf;
     double yrpsapwood;
     double yrproot;
     double yrpseed;
     
     /// double yrp{leaf, sapwood, root}i: annual sum of fractional investment-allocations
     double yrpleafi;
     double yrpsapwoodi;
     double yrprooti;
     double yrpseedi;

     /// double yrrgrowth: annual sum of growth respiration
     double yrrgrowth;

     /// double yrrmaint: annual sum of maintenance respiration
     double yrrmaint;

     /// double yrrm{leaf, sapwood, root}: annual sum of maintenance respiration
     double yrrmleaf;
     double yrrmsapwood;
     double yrrmroot;

     /// double yrstoren: Annual sum of labile.nitrogen
     double yrstoren;

     /// double yrstructn: Annual sum of strctrl.nitrogen
     double yrstructn;

     ///  slope of daylength
     double daylslope[366];

  private:

/** **************************************************************
		 Private Functions
************************************************************** */

     /// void checkBiomassFluxes(): ensures no biomass fluxes are negative (see .cpp file)                                                           
     void checkBiomassFluxes( Biomass& pveg );


/* **************************************************************
		 Private Variables
************************************************************** */

     ///  Biomass objects for different plant compartments: leaf, sapwood, heartwood, root, seed, labile, structural, plant

     Biomass leaf;
     Biomass sapwood;
     Biomass heartwood;
     Biomass root;
     Biomass seed;
     Biomass plant;
     Biomass labile;
     Biomass strctrl;

     /// Index for current vegetation type
     int currentveg;

     // C:N demand from the labile comartment in the given timestep
     double cnprod;

     // rate of change of ozone effect
     double dfo3;

     // partial derivatives of gpp with respect to leafc, stemc, rootc
     double dgppdleafc;
     double dgppdstemc;
     double dgppdrootc;
     
     double dvnupdrootc;

     double errcnt;

     double folmass;

     // Multiplier of direct ozone effects on GPP
     double fozone;
     double fco2;
     double frdl;
     double fh2o;
     double fo3;

     // Monthly foliar projective cover
     double fpc;

     // Ratio of indirect metabolic cost of leaves to direct metabolic cost
     double frm;

	 // canopy conductance

	 double gc;

     // Monthly gross primary productivity (GPP)
     double gpp;
     
     // Monthly gross plant respiration (rm + rg)
     double gpr;

	 // stomatal conductance (based on gc/lai)

	 double gs;

      // Initial monthly gross primary productivity
     double ingpp;

     // Initial net primary productivity
     double innpp;

     // Initial C/N of biomass production
     double inprodcn;

     // Initial N uptake by plants
     double inuptake;

	 // Maintenance Respiration half saturation
	 double krx;

     // Monthly leaf area index
     double lai;

     // Monthly optimal leaf area index
     double laiopt;

	 // Maximum Biome LAI
	 double laisitemax;

     // Updated optimum air temperature for current year
     double newtopt;

     // Monthly net primary productivity (NPP)
     double npp;

     // Monthly N uptake by plants
     double nuptake;

     // soil evaporation, transpiration, and sensible heat flux in W m^-2
     double pesoilw;
     double esoilmmmo;
     double pecanw;
     double shfluxw;

     // PET
	 double pet;

     // PET Lower
         double petll;

     // Phenology term
         double phi,prvphi;

     // number of months since budburst
        double phicnt;

     // Index for potential vegetation biome type
     int potveg;

     // Effect of air temperature on plant respiration
     double respq10;

     // Monthly growth respiration
     double rg;

     double rmt; //temperature multiplier for maint. resp.

     // running mean of rltrc
     double rltrc;

     // running mean of rnpp
     double rnpp;
     
     // running mean of gpp
     double rgpp;
     
     // running mean of non-n-limited gpp
     double ringpp;
     
     // running means of labilec, labilen
     double rlabilec;
     double rlabilen;
     
     // running means of demand for c, n from labile pool
     double rdemandc;
     double rdemandn;

     // running mean of phi
     double rphi;
     double rphish;
     
     // 5-year running mean of year-average leaf allocation fraction
     double rpleaf;

     // running mean of precipitation
     double rprec;

     // running mean of tair
     double rtair;

     // running mean of tair times phi
     double rtairphi;

     // Temperature function for plant respiration
     double rTLaRS;

     //  SONINPUT
    double soninput; 
    
     // DENITRIFICATION
    double denitr;

     // Index for vegetation subtype
     int subtype;

     // Effect of air temperature on GPP
     double temp;

     // Veg C
     double totc;

     // Symbiotic N fixation
     double vegninp;


/* *************************************************************
		 Private Parameters
************************************************************* */
     
     /// PFT-specific vegetation parameters, found in .dat file

     double cmax;
     double cmax1b[MAXCMNT];
     
     double tauleaf[MAXCMNT];  // leaf turnover time
     double tausapwood[MAXCMNT];  // sapwood turnover time
     double tauheartwood[MAXCMNT];  // sapwood turnover time
     double tauroot[MAXCMNT];  // root turnover time
     double tauseed[MAXCMNT];  // seed turnover time

     double kr;
     double kra[MAXCMNT];
     
     double nmax;
     double nmax1b[MAXCMNT];
     
     double adjc2n;
     double c2n;
     double c2nb[MAXCMNT];
     double c2nmin[MAXCMNT];
     double cnmin[MAXCMNT];
     double cnltr[MAXCMNT];
     double dc2n;

     double cneven;
     double initcneven[MAXCMNT];
     
     double nnf[MAXCMNT];

     double o3para[MAXCMNT];
     double o3parb[MAXCMNT];
     double o3parc[MAXCMNT]; 

     // PFT-specific vegetation parameters, found in veg.ecd file

     double kc[MAXCMNT];
     double ki[MAXCMNT];
     
     double tmax[MAXCMNT];
     double tmin[MAXCMNT];
     double topt;
     double toptmic;
     double toptmax[MAXCMNT];
     double toptmin[MAXCMNT];
     
     double raalpha[MAXCMNT];
     double rabeta[MAXCMNT];
     double ragamma[MAXCMNT];
     double raqref[MAXCMNT];
     double ratref[MAXCMNT];

     double kn1[MAXCMNT];
     double sla[MAXCMNT];
     
     double href[MAXCMNT];
     double krnup[MAXCMNT];
     double kallom[MAXCMNT];

     double cnleaf[MAXCMNT];
     double cnsapwood[MAXCMNT];
     double cnheartwood[MAXCMNT];
     double cnroot[MAXCMNT];
     double cnseed[MAXCMNT];

     double pseedlc[MAXCMNT];
     double fsaplive[MAXCMNT];
     int phen[MAXCMNT];

};

#endif

