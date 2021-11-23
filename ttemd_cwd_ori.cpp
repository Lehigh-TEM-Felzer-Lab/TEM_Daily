/* *************************************************************
****************************************************************
TTEMD_CWD.CPP - Terrestrial Ecosystem Model Version 4.5
****************************************************************

Modifications:

20060126 - DWK created by modifying ttem50b5.cpp
20060126 - DWK changed include from ttem50b5.h to ttem437.h
20060126 - DWK changed TTEM50:: to TTEMFLUX::
20060126 - DWK deleted initialization of snowfile and
           slayerfile int TTEMFLUX()
20060126 - DWK added I_FOZONE and I_FINDOZNE to TTEMFLUX()
20060126 - DWK deleted I_TSOIL, I_DST0, I_DST5, I_DST10,
           I_DST20, I_DST50, I_DST100, I_DST200, I_FRONTD,
           I_THAWBE, I_THAWEND, I_THAWPCT, and I_ACTLAYER from
           TTEMFLUX()
20060126 - DWK deleted displayOptionalSoilTemp() and
           getOptionalSoilTemp()
20060126 - DWK deleted rflog1 from monthlyTransient(),
           int stepmonth() and getenviron()
20070105 - TWC changed name to ttem45
2007 - TWC/BSF Summary of Changes
       RMLEAF, RMROOT: Delta, displayoptionsEflx, getoptionalEflx,
                       pcdisplayODEerr, boundcon, and pools at top
       GC, GS, TRANST, EVAP, INTER: Delta, displayoptionsWflx,
                       getoptionalWflx, updateyrsummary, and
                       pools at top
       KRA, KRB, (remove RMMAX): ECDsetODEstate, getcropecd, getsiteecd,
                                 writeecd
       VSM, PCTP: Delta, Massbal, pcdisplayODEerr
       include tconduct45.ecd
       boundcon and pcdisplayODEerr also include GC
       CropDynamics and NatVegDynamics:
	     Call to SWP routine in soil
		 Call to EVAP, EET, AVLH2O in soil
		 Changes to veg.updatedynamics
		 soil.updatehydrology: replace SM with AVLH2O
		 soil.updateNLosses: replaces RAIN with RPERC + SPERC
		 define nirrn, pme, lwout, dum
	     Cropdynamics: ag.getGDDSEED, ag.getISPERENNIAL
		 NatVegDynamics: perennial = 1
	   getenviron: comment out petjh
	   soil.getNINPUT()massbal:
	     vnrsrb conditional to keep from getting negative
		 NINP and NLST calculations
	   monthlyTransient: VSM and resetEcds
	   pcdisplayMonth: replace AVLH2O with SM-WILTPT
	   stepmonth:
	     seed natural vegetation following disturbance
		 seed natural vegetation following crop abandonment
		 kill crops at cold temperatures and allow for perennial crops
		 changes to crop harvesting: allow for seeding and perennial crops
		 Define I_VSM
		 Initialize PRVLAI, LEAFADD, LEAFDROP
		 ozone effect: no longer use 50% compounding (also baseline ozone
		   but no longer used)
		 Remove setRESPQ10
		 Add soil.setsoil.getNINPUT()NINPUT
		 setPREVPAR, PRVLAI, LEAFADD, LEAFDROP (and PME, LEAFEFCY no longer used)
		 yrgc, yrgs, yrtranst, yrevap, yrinter

****************************************************************
************************************************************** */

//#define BORLAND_CPP

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

#ifdef BORLAND_CPP
  #include<stdlib>
#else
  #include<cstdlib>
#endif

  using std::exit;
  using std::atof;
  using std::atoi;

#include<cmath>

  using std::exp;
  using std::fabs;
  using std::modf;
  using std::pow;
  using std::max;

#include<vector>

  using std::vector;

#include<string>

  using std::string;

#include<iomanip>

  using std::setprecision;

#define OPENN

#define CALIBRATE_TEM

//#define STEP_DAILY

//#define TIM_RSOIL
#define SIB_RSOIL

//#define DEBUG_CTEM

//#define DEBUG_XTEM

#ifdef CALIBRATE_TEM
  #include<curses.h>
  #include<cctype>
    using std::toupper;
#endif

#include "ttemd_cwd.h"

/* **************************************************************
************************************************************** */

// Initialization of static members

int Ttem45::avlnflag = 0;
int Ttem45::nfeed = 0;
int Ttem45::rheqflag = 0;
int Ttem45::moistlim = 0;
int Ttem45::o3flag = 0;
int Ttem45::initbase = 0;
int Ttem45::baseline = 0;
int Ttem45::intflag = 0;

int Ttem45::maxnrun = 0;
int Ttem45::equil = 0;
int Ttem45::runsize = 0;
int Ttem45::maxyears = 0;
int Ttem45::strteq = 0;
int Ttem45::endeq = 0;
int Ttem45::startyr = 0;
int Ttem45::endyr = 0;
int Ttem45::diffyr = 0;
int Ttem45::wrtyr = 0;

double Ttem45::ctol = 1.0;
double Ttem45::ntol = 0.02;
double Ttem45::wtol = 0.5;

// Initialization of adaptive integrator variables

double Ttem45::inittol = 0.01;
int Ttem45::maxit = 20;
int Ttem45::maxitmon = 100;

double Ttem45::a1 =   0.115740741;

double   Ttem45::a3 =   0.548927875;
double  Ttem45::a31 =   0.09375;
double  Ttem45::a32 =   0.28125;

double   Ttem45::a4 =   0.535331384;
double  Ttem45::a41 =   0.879380974;
double  Ttem45::a42 =  -3.277196177;
double  Ttem45::a43 =   3.320892126;

double   Ttem45::a5 =  -0.20;
double  Ttem45::a51 =   2.032407407;
double  Ttem45::a52 =  -8.0;
double  Ttem45::a53 =   7.173489279;
double  Ttem45::a54 =  -0.2058966866;

double   Ttem45::b1 =   0.118518519;
double   Ttem45::b3 =   0.518986355;
double   Ttem45::b4 =   0.50613149;
double   Ttem45::b5 =  -0.18;
double   Ttem45::b6 =   0.036363636;
double  Ttem45::b61 =  -0.296296296;
double  Ttem45::b62 =   2.0;
double  Ttem45::b63 =  -1.381676413;
double  Ttem45::b64 =   0.45297271;
double  Ttem45::b65 =  -0.275;


/* **************************************************************
************************************************************** */

Ttem45::Ttem45() : predstr( NUMTEM )
{
  string tem_inpfile = "tem_in.txt";
  
  tol = inittol;
  syint = 1;
  totyr = -99;
  rheqflag = 0;
  
  delta_count = 0;
  
  teminfile.open( tem_inpfile.c_str(), ios::in ); 
  teminfile >> goname;
  teminfile.close();

#ifdef CALIBRATE_TEM
  soilfile = "tsoil45.ecd";
  rootfile = "trotz45.ecd";
  vegfile = "tveg45.ecd";
  mcrvfile = "tmcrv45.ecd";
  agfile = "ag45.ecd";
  gcfile = "tconduct45.ecd";

  rheqflag = 0;
  adapttol = 0;

  sey[0] = GET_VEGC;        //1
  sey[1] = GET_SOILC;       //3
  sey[2] = GET_SOILN;       //4
  sey[3] = GET_LABILEC;     //11
  sey[4] = GET_GPP;         //30
  sey[5] = GET_NPP;         //35
  sey[6] = GET_RH;          //50
  sey[7] = GET_LAI;         //52
  sey[8] = GET_LABILEN;     //17
  sey[9] = GET_VNUP;        //66
  
  swy[0] = GET_RAIN;        //4
  swy[1] = GET_SNWFAL;      //5
  swy[2] = GET_AVLH2O;      //36
  swy[3] = GET_EET;         //13
  swy[4] = GET_WYLD;        //20
  swy[5] = GET_TAIRD;       //10
  swy[6] = GET_VPDD;        //34
  swy[7] = GET_GC;          //14
#endif

// Identify potential output variables from TEM

// Ecosystem carbon pools determined by the integrator**********

  // vegetation carbon pools

  predstr.at( I_LEAFC ) = "LEAFC";

  predstr.at( I_SAPWOODC ) = "SAPWOODC";

  predstr.at( I_HEARTWOODC ) = "HEARTWOODC";

  predstr.at( I_ROOTC ) = "ROOTC";

  predstr.at( I_SEEDC ) = "SEEDC";

  predstr.at( I_LABILEC ) = "LABILEC";

  // reactive soil organic carbon
  predstr.at( I_SOLC ) = "SOILORGC";

// Ecosystem nitrogen pools determined by the integrator********

  // vegetation structural nitrogen pools

  predstr.at( I_LEAFN ) = "LEAFN";

  predstr.at( I_SAPWOODN ) = "SAPWOODN";

  predstr.at( I_HEARTWOODN ) = "HEARTWOODN";

  predstr.at( I_ROOTN ) = "ROOTN";

  predstr.at( I_SEEDN ) = "SEEDN";

  predstr.at( I_LABILEN ) = "LABILEN";


  // reactive soil organic nitrogen
  predstr.at( I_SOLN ) = "SOILORGN";

  // soil available nitrogen
  predstr.at( I_AVLN ) = "AVAILN";
  
  // ozone damage factor
  predstr.at( I_FOZONE ) = "FOZONE";

  // GPP radiation factor
  predstr.at( I_FRDL ) = "FRDL";
  
  //  GPP CO2 factor
  predstr.at( I_FCO2 ) = "FCO2";
 
  // GPP temp factor
  predstr.at( I_TEMP ) = "TEMP";
 
  // GPP water factor
  predstr.at( I_FH2O ) = "FH2O";

  // GPP ozone factor
  predstr.at( I_FO3 ) = "FO3";

// Ecosystem water pools determined by the integrator***********

  // soil moisture
  predstr.at( I_SM ) = "SMOIST";

  // volumetric soil moisture
  predstr.at( I_VSM ) = "VSM";

  // soil moisture expressed as percent total porosity
  predstr.at( I_PCTP ) = "PCTP";

  // groundwater pool resulting from rainfall
  predstr.at( I_RGRW ) = "RGRNDH2O";

  // groundwater pool resulting from snow melt
  predstr.at( I_SGRW ) = "SGRNDH2O";

  // foliar projected cover
  predstr.at( I_FPC ) = "FPC";


// Carbon fluxes for ecosystems ********************************

  // leaf allocation
  predstr.at( I_ALLOCLC ) = "ALLOCLC";

  // sapwood allocation
  predstr.at( I_ALLOCSC ) = "ALLOCSC";

  // heartwood allocation
  predstr.at( I_ALLOCHC ) = "ALLOCHC";

  // root allocation
  predstr.at( I_ALLOCRC ) = "ALLOCRC";

  // seed allocation
  predstr.at( I_ALLOCSEEDC ) = "ALLOCSEEDC";
  
  // leaf investment-allocation
  predstr.at( I_ALLOCILC ) = "ALLOCILC";

  // sapwood investment-allocation
  predstr.at( I_ALLOCISC ) = "ALLOCISC";

  // heartwood investment-allocation
  predstr.at( I_ALLOCIHC ) = "ALLOCIHC";

  // root investment-allocation
  predstr.at( I_ALLOCIRC ) = "ALLOCIRC";

  // seed investment-allocation
  predstr.at( I_ALLOCISEEDC ) = "ALLOCISEEDC";

  // GPP not limited by nutrient availability
  predstr.at( I_INGPP ) = "VEGINGPP";

  // gross primary production
  predstr.at( I_GPP ) = "GPP";

 // NPP not limited by nutrient availability
  predstr.at( I_INNPP ) = "VEGINNPP";

  // net primary production
  predstr.at( I_NPP ) = "NPP";

  // gross plant respiration
  predstr.at( I_GPR ) = "GPR";

  // vegetation maintenance respiration
  predstr.at( I_RVMNT ) = "RVMAINT";

  // leaf respiration
  predstr.at( I_RMLEAF ) = "RMLEAF";

  // sapwood respiration
  predstr.at( I_RMSAPWOOD ) = "RMSAPWOOD";

  // root respiration
  predstr.at( I_RMROOT ) = "RMROOT";

  // seed respiration
  predstr.at( I_RMSEED ) = "RMSEED";

  // labile C respiration
  predstr.at( I_RMLABILE ) = "RMLABILE";


  // vegetation growth respiration
  predstr.at( I_RVGRW ) = "RVGRWTH";

  // leaf litterfall carbon
  predstr.at( I_LTRLC ) = "LTRLC";

  // sapwood litterfall carbon
  predstr.at( I_LTRSC ) = "LTRSC";

  // heartwood litterfall carbon
  predstr.at( I_LTRHC ) = "LTRHC";

  // root litterfall carbon
  predstr.at( I_LTRRC ) = "LTRRC";

  // seed litterfall carbon
  predstr.at( I_LTRSEEDC ) = "LTRSEEDC";

  // heterotrophic respiration
  predstr.at( I_RH ) = "RH";

  // Dissolved Organic Carbon
  predstr.at( I_DOC ) = "DOC";

  // Dissolved Organic Nitrgoen
  predstr.at( I_DON ) = "DON";

// Nitrogen fluxes for ecosystems determined by the integrator

  // leaf nitrogen allocation
  predstr.at( I_ALLOCLN ) = "ALLOCLN";

  // sapwood nitrogen allocation
  predstr.at( I_ALLOCSN ) = "ALLOCSN";

  // heartwood nitrogen allocation
  predstr.at( I_ALLOCHN ) = "ALLOCHN";

  // root nitrogen allocation
  predstr.at( I_ALLOCRN ) = "ALLOCRN";

  // seed nitrogen allocation
  predstr.at( I_ALLOCSEEDN ) = "ALLOCSEEDN";
  
  // leaf nitrogen investment-allocation
  predstr.at( I_ALLOCILN ) = "ALLOCILN";

  // sapwood nitrogen investment-allocation
  predstr.at( I_ALLOCISN ) = "ALLOCISN";

  // heartwood nitrogen investment-allocation
  predstr.at( I_ALLOCIHN ) = "ALLOCIHN";

  // root nitrogen investment-llocation
  predstr.at( I_ALLOCIRN ) = "ALLOCIRN";

  // seed nitrogen investment-allocation
  predstr.at( I_ALLOCISEEDN ) = "ALLOCISEEDN";

  // total nitrogen inputs into ecosystem
  predstr.at( I_NINP ) = "NINPUT";

  // total DOC production into ecosystem
  predstr.at( I_DOCPROD ) = "DOCPROD";

  // total DOC leaching from ecosystem
  predstr.at( I_LCHDOC ) = "LCHDOC";

  // total DON production into ecosystem
  predstr.at( I_DONPROD ) = "DONPROD";

  // total DON leaching from ecosystem
  predstr.at( I_LCHDON ) = "LCHDON";

  // total DON leaching from ecosystem
  predstr.at( I_LCHDIN ) = "LCHDIN";

  // nitrogen fertilization
  predstr.at( I_AGFRTN ) = "AGFERTN";

  // VEGNUP not limited by carbon availability
  predstr.at( I_INNUP ) = "VEGINNUP";

  // nitrogen uptake by vegetation
  predstr.at( I_VNUP ) = "VEGNUP";

  // nitrogen resorption by leaves
  predstr.at( I_NRESORBL ) = "NRESORBL";

  // nitrogen resorption by stems
  predstr.at( I_NRESORBS ) = "NRESORBS";

  // nitrogen resorption by roots
  predstr.at( I_NRESORBR ) = "NRESORBR";

  // nitrogen resorption by seeds
  predstr.at( I_NRESORBSEED ) = "NRESORBSEED";

  // leaf litterfall nitrogen from vegetation
  predstr.at( I_LTRLN ) = "LTRLN";

  // sapwood litterfall nitrogen from vegetation
  predstr.at( I_LTRSN ) = "LTRSN";

  // heartwood litterfall nitrogen from vegetation
  predstr.at( I_LTRHN ) = "LTRHN";

  // root litterfall nitrogen from vegetation
  predstr.at( I_LTRRN ) = "LTRRN";

  // seed litterfall nitrogen from vegetation
  predstr.at( I_LTRSEEDN ) = "LTRSEEDN";

  // total nitrogen immobilization
  predstr.at( I_MNUP ) = "MICRONUP";

  // net nitrogen mineralization
  predstr.at( I_NMIN ) = "NETNMIN";

  // Total nitrogen losses from ecosystems
  predstr.at( I_NLST ) = "NLOST";

  // Symbiotic N Fixation
  predstr.at( I_NFIXS ) = "NFIXS";

  // Nonsymbiotic N Fixation
  predstr.at( I_NFIXN ) = "NFIXN";

// Water fluxes determined by the integrator********************

  // Irrigation
  predstr.at( I_AGIRRIG ) = "IRRIGATE";

  // Initial estimated evapotranspiration
  predstr.at( I_INEET ) = "INEET";

  // estimated evapotranspiration
  predstr.at( I_EET ) = "EET";

  // percolation of rainwater through soil profile
  predstr.at( I_RPERC ) = "RPERC";

  // percolation of snowmelt through soil profile
  predstr.at( I_SPERC ) = "SPERC";

  // runoff of rainwater
  predstr.at( I_RRUN ) = "RRUN";

  // runoff of snowmelt
  predstr.at( I_SRUN ) = "SRUN";

  // canopy conductance
  predstr.at( I_GC ) = "GC";    
  
  // stomatal conductance
  predstr.at( I_GS ) = "GS";
  
  // transpiration
  predstr.at( I_PECAN ) = "PECAN";    

  // soil evaporation
  predstr.at( I_PESOIL ) = "PESOIL";    

// Other ecosystem carbon pools ********************************

  // total carbon pool found in ecosystem excluding products
  predstr.at( I_TOTEC ) = "TOTEC";

  // total carbon (including products if present)
  predstr.at( I_TOTC ) = "VEGC";

// Other ecosystem nitrogen pools ******************************

  // total nitrogen stored in vegetation
  predstr.at( I_VEGN ) = "VEGN";

// Other ecosystem water pools ******************************

  predstr.at( I_SNWPCK ) = "SNOWPACK";  // snowpack
  
  predstr.at( I_AVLW ) = "AVAILH2O";  // plant available soil water

// Other carbon fluxes for ecosystems **************************

  // net ecosystem production
  predstr.at( I_NEP ) = "NEP";

  // net carbon exchange of ecosystem with atmosphere
  predstr.at( I_NCE ) = "NCE";
  
  // leaf area index
  predstr.at( I_LAI ) = "LAI";

// Other water fluxes ******************************************

  // potential evapotranspiration
  predstr.at( I_PET ) = "PET";

  // infiltration into the soil of water from snowmelt
  predstr.at( I_SNWINF ) = "SNOWINF";

  // water yield
  predstr.at( I_WYLD ) = "H2OYIELD";

// Carbon stocks in products ***********************************

  // carbon in agricultural products
  predstr.at( I_AGPRDC ) = "AGPRODC";

  // carbon in lawn clippings
  predstr.at( I_CLIPPINGS ) = "CLIPPINGS";

  // carbon pool of products that decompose in 10 years
  predstr.at( I_PROD10C ) = "PROD10C";

  // carbon pool of products that decompose in 100 years
  predstr.at( I_PROD100C ) = "PROD100C";

  // carbon in all product pools
  predstr.at( I_TOTPRDC ) = "TOTPRODC";


// Carbon stocks in crop residue and stubble********************

  // carbon in crop residue
  predstr.at( I_RESIDC ) = "RESIDC";

  // stubble carbon
  predstr.at( I_AGSTUBC ) = "CRPSTUBC";


// Nitrogen stocks in products *********************************

  // nitrogen in agricultural products
  predstr.at( I_AGPRDN ) = "AGPRODN";

  // nitrogen pool of products that decompose in 10 years
  predstr.at( I_PROD10N ) = "PROD10N";

  // nitrogen pool of products that decompose in 100 years
  predstr.at( I_PROD100N ) = "PROD100N";

  // nitrogen in all product pools
  predstr.at( I_TOTPRDN ) = "TOTPRODN";


// Nitrogen stocks in crop residue and stubble******************

  // nitrogen in crop residue
  predstr.at( I_RESIDN ) = "RESIDN";

  // stubble nitrogen
  predstr.at( I_AGSTUBN ) = "CRPSTUBN";


// Carbon fluxes associated with agricultural conversion *******

  // carbon loss from the ecosystem during conversion
  predstr.at( I_CNVRTC ) = "CONVERTC";

  // carbon loss from vegetation during conversion
  predstr.at( I_VCNVRTC ) = "VCONVRTC";

  // carbon loss from soils during conversion
  predstr.at( I_SCNVRTC ) = "SCONVRTC";

  // carbon associated with slash left after conversion
  predstr.at( I_SLASHC ) = "SLASHC";

  // carbon and nitrogen associated with CWD
  predstr.at( I_STANDDEADC ) = "STANDDEADC";
  predstr.at( I_STANDDEADN ) = "STANDDEADN";
  predstr.at( I_VOLAC ) = "VOLAC";
  predstr.at( I_VOLAN ) = "VOLAN"; 


  // carbon flux from ecosystem (NEP+CONVERTC)
  predstr.at( I_CFLX ) = "CFLUX";


// Nitrogen fluxes associated with agricultural conversion *****

  // nitrogen loss from the ecosystem during conversion
  predstr.at( I_CNVRTN ) = "CONVERTN";

  // nitrogen loss from vegetation during conversion
  predstr.at( I_VCNVRTN ) = "VCONVRTN";

  // nitrogen loss from soils during conversion
  predstr.at( I_SCNVRTN ) = "SCONVRTN";

  // nitrogen associated with slash left after conversion
  predstr.at( I_SLASHN ) = "SLASHN";

  // Total organic N mineralized and retained in ecosystem
  //   after disturbance
  predstr.at( I_NRETNT ) = "NRETENT";

  // Vegetation N mineralized and retained in ecosystem
  //   after disturbance
  predstr.at( I_NVRTNT ) = "NVRETENT";

  // Soil organic N mineralized and retained in ecosystem
  //   after disturbance
  predstr.at( I_NSRTNT ) = "NSRETENT";


// Carbon and nitrogen fluxes to/from products *****************

  // carbon loss to formation of agricultural products
  predstr.at( I_AGFPRDC ) = "AGFPRODC";

  // nitrogen loss to formation of agricultural products
  predstr.at( I_AGPRDN ) = "AGFPRODN";

  // carbon loss to crop residue
  predstr.at( I_FRESIDC ) = "FRESIDC";

  // nitrogen loss to crop residue
  predstr.at( I_FRESIDN ) = "FRESIDN";

  // carbon loss to resulting from decomposition of agricultural
  //   products
  predstr.at( I_AGPRDFC ) = "AGPRODFC";

  // nitrogen loss resulting from decomposition of agricultural
  //   products
  predstr.at( I_AGPRDFN ) = "AGPRODFN";

  // carbon loss from crop residue
  predstr.at( I_RESIDFC ) = "RESIDFC";

  // nitrogen loss from crop residue
  predstr.at( I_RESIDFN ) = "RESIDFN";

  // carbon loss to formation of products that decompose in
  //  10 years
  predstr.at( I_PRDF10C ) = "PRDF10C";

  // nitrogen loss to formation of products that decompose in
  //   10 years
  predstr.at( I_PRDF10N ) = "PRDF10N";

  // carbon loss resulting from decomposition of PROD10C
  predstr.at( I_PRD10FC ) = "PRD10FC";

  // nitrogen loss resulting from decomposition of PROD10N
  predstr.at( I_PRD10FN ) = "PRD10FN";

  // carbon loss to formation of products that decompose in
  //  100 years
  predstr.at( I_PRDF100C ) = "PRDF100C";

  // nitrogen loss to formation of products that decompose in
  //   100 years
  predstr.at( I_PRDF100N ) = "PRDF100N";

  // carbon loss resulting from decomposition of PROD100C
  predstr.at( I_PRD100FC ) = "PRD100FC";

  // nitrogen loss resulting from decomposition of PROD100N
  predstr.at( I_PRD100FN ) = "PRD100FN";

  // carbon loss to the formation of all products
  predstr.at( I_TOTFPRDC ) = "TOTFPRDC";

  // nitrogen loss to the formation of all products
  predstr.at( I_TOTFPRDN ) = "TOTFPRDN";

  // carbon loss resulting from decomposition of all products
  predstr.at( I_TOTPRDFC ) = "TOTPRDFC";

  // nitrogen loss resulting from decomposition of all products
  predstr.at( I_TOTPRDFN ) = "TOTPRDFN";


// Agro-Ecosystem carbon and nitrogen pools *********************

  // crop carbon
  predstr.at( I_CROPC ) = "CROPC";

  // carbon in natural vegetation
  predstr.at( I_NATVEGC ) = "NATVEGC";

  // crop nitrogen
  predstr.at( I_CROPN ) = "CROPN";

  // nitrogen in natural vegetation
  predstr.at( I_NATVEGN ) = "NATVEGN";

  // crop structural N
  predstr.at( I_CSTRN ) = "CROPSTRN";

  // structural N in natural vegetation
  predstr.at( I_NATSTRN ) = "NATSTRN";

  // crop labile N
  predstr.at( I_CSTON ) = "CROPSTON";

  // labile N stored in natural vegetation
  predstr.at( I_NATSTON ) = "NATSTON";


// Crop phenology **********************************************

  // leaf area index (LAI) of crops
  predstr.at( I_CROPLAI ) = "CROPLAI";

  // leaf area index (LAI) of natural vegetation
  predstr.at( I_NATLAI ) = "NATLAI";

  // foliar projected cover (FPC) of crops
  predstr.at( I_CROPFPC ) = "CROPFPC";

  // foliar projected cover (FPC) of natural vegetation
  predstr.at( I_NATFPC ) = "NATFPC";


// Additional carbon fluxes for agro-ecosystems *****************

  // GPP of crops not limited by nutrient availability
  predstr.at( I_AGINGPP ) = "CRPINGPP";

  // GPP of natural vegetation not limited by
  //   nutrient availability
  predstr.at( I_NATINGPP ) = "NATINGPP";

  // gross primary production (GPP) of crops
  predstr.at( I_AGGPP ) = "CROPGPP";

  // gross primary production of natural vegetation
  predstr.at( I_NATGPP ) = "NATGPP";

  // NPP of crops not limited by nutrient availability
  predstr.at( I_AGINNPP ) = "CRPINNPP";

  // NPP of natural vegetation not limited by
  //   nutrient availability
  predstr.at( I_NATINNPP ) = "NATINNPP";

  // net primary production (NPP) of crops
  predstr.at( I_AGNPP ) = "CROPNPP";

  // net primary production (NPP) of natural vegetation
  predstr.at( I_NATNPP ) = "NATNPP";

  // gross plant respiration of crops
  predstr.at( I_AGGPR ) = "CROPGPR";

  // gross plant respiration of natural vegetation
  predstr.at( I_NATGPR ) = "NATGPR";

  // maintenance respiration of crop plants
  predstr.at( I_AGRVMNT ) = "CRPRMNT";

  // maintenance respiration of natural vegetation
  predstr.at( I_NATRVMNT ) = "NATRVMNT";

  // growth respiration of crop plants
  predstr.at( I_AGRVGRW ) = "CRPRGRW";

  // growth respiration of natural vegetation
  predstr.at( I_NATRVGRW ) = "NATRGRW";

  // litterfall carbon from crops
  predstr.at( I_AGLTRC ) = "CROPLTRC";

  // litterfall carbon from natural vegetation
  predstr.at( I_NATLTRC ) = "NATLTRC";

  // Additional nitrogen fluxes for agro-ecosystems ************

  // nitrogen uptake by crops not limited by carbon availability
  predstr.at( I_AGINNUP ) = "CRPINNUP";

  // nitrogen uptake by natural vegetation not limited by carbon
  //   availability
  predstr.at( I_NATINNUP ) = "NATINNUP";

  // nitrogen uptake by crops
  predstr.at( I_AGVNUP ) = "CROPNUP";

  // nitrogen uptake by natural vegetation
  predstr.at( I_NATVNUP ) = "NATVNUP";

  // nitrogen mobilization by crops
  predstr.at( I_AGVNMBL ) = "CRPNMOBL";

  // nitrogen mobilization by natural vegetation
  predstr.at( I_NATVNMBL ) = "NATVNMBL";

  // nitrogen resorption by crops
  predstr.at( I_AGVNRSRB ) = "CRPNRSRB";

  // nitrogen resorption by natural vegetation
  predstr.at( I_NVNRSRB ) = "NVNRSRB";

  // litterfall nitrogen from crops
  predstr.at( I_AGLTRN ) = "CROPLTRN";

  //litterfall nitrogen from natural vegetation
  predstr.at( I_NATLTRN ) = "NATLTRN";


  dbugflg = 0;

};

/* **************************************************************
************************* Functions *****************************
************************************************************** */


/* *************************************************************
************************************************************* */

int Ttem45::adapt( const int& numeq,
                   double pstate[],
                   const double& ptol,
                   const int& pdm,
                   const double& nmax_grow )
{

  int i;
  double ipart;
  double fpart;
  double time = ZERO;
  double dtmax = 1.0;
  double dt;
  int mflag = 0;
  long nintmon = 0;
  double oldstate[NUMEQ];

  #ifdef STEP_DAILY
    dtmax = 0.03333;
  #endif
  dt = dtmax;
  for( i = 0; i < numeq; ++i ) { dz1[i] = ZERO; } // reset dz1 if starting a new month 
  for( i = 0; i < numeq; ++i ) { dz4[i] = 1.0; } // reset dz1 if starting a new month 

  blackhol = 0;
  while( time != 1.0 )
  {
    test = REJECT;
    if( 1 == syint )
    {
      while( test != ACCEPT )
      {
	    #ifdef STEP_DAILY
	      rkbs( numeq, pstate, dt, pdm, nmax_grow );
          test = boundcon( dum2, error, ptol );
        #else 
          rkf( numeq, pstate, dt, pdm, nmax_grow );
          test = boundcon( dum4, error, ptol );
        #endif

	    if( dt <= dtmax*pow( 0.5, maxit ) )
        {
          test = ACCEPT;
	      mflag = 1;

          if( 0 == nintmon )
          {
            for( i = 0; i < numeq; ++i )
            {
              oldstate[i] = pstate[i];
            }
          }

	      ++nintmon;
	    }

        if( ACCEPT == test )
        {
          #ifdef STEP_DAILY
            for( i = 0; i < numeq; ++i ) 
            { 
              pstate[i] = dum2[i]; 
              dz1[i] = dz4[i]; // dz1 for the next step has already been calculated
            }
          #else
            for( i = 0; i < numeq; ++i ) { pstate[i] = dum4[i]; }
          #endif
          time += dt;

          fpart = modf( (0.01 + (time/(2.0*dt))),&ipart );

          if ( fpart < 0.1 && 2.0*dt <= dtmax) { dt *= 2.0; }
          if ( dt > (1.0 - time) ) { dt = 1.0 - time; } // ensure that last step doesn't go over the end time
        }
        else 
        { 
          dt *= 0.500; 
          if ( dt > (1.0 - time) ) { dt = 1.0 - time; } // ensure that last step doesn't go over the end time
        }

        if( nintmon == maxitmon )
        {
          time = 1.0;
          blackhol = 1;

          for( i = 0; i < numeq; ++i ) { pstate[i] = oldstate[i]; }
        }
      }
    }    /* end rkf integrator */
  }      /* end time while */

  return mflag;

};

/* *************************************************************
************************************************************* */


/* **************************************************************
************************************************************** */

int Ttem45::boundcon( double ptstate[],
                      double err[],
                      const double& ptol )
{

  test = ACCEPT;
  int i;

/// Check carbon, nitrogen, and water state variables and fluxes

  for( i = 0; i < NUMEQ; ++i )
  {
    if( err[i] > fabs( ptol * ptstate[i] ) ) { return test = temkey(i )+1; }
  }

  return test;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Ttem45::cropDynamics( const int& pdm, const double& nmax_grow, double pstate[] )
{

  double prob;
  double surf_wetness;
  int nopen;
//  double rsoil;

  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in cropDynamics(), month = %2d ", pdm);
    refresh();
  #endif

//----------------------------------------------//
//  transfer information from pstate to veg biomass variables
  updateVegBiomass( pstate );

//----------------------------------------------//
//  update variables used in natvegdynamics:
//    ag.fertn
//    atms.lwoutd, atms.lwoutn, atms.nirrn
//    soil.avlh2o, soil.gm, soil.kh2o, soil.ninput
//    veg.lai, veg.rmt
//
  ag.fertn = ZERO;
//  if((ag.fert1950flag == 1) && (1 == ag.state) && (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt))&& (ag.getGROWDD() <= ag.getGDDHARVST(ag.cmnt))|| (1 == ag.getISPERENNIAL(ag.cmnt))) {
//   cout << "entering first conditional " << " " << pdm << endl;
//   ag.fertn = 10./5.0;
//   }
  if((ag.fert1950flag == 1) && (ag.state == 1 || ag.state == 3) && pdm == 160) {
//   ag.fertn = 5.0;
//   ag.fertn = 10.0;
   ag.fertn = 15.0;
//   ag.fertn = 19.0;
  }
  else if (ag.fert1950flag == 1 && ag.state == 2 && pdm == 4)
  {
   ag.fertn = 5.0;
  }
  else
  {
   ag.fertn = 0.0;
  }
//  if((ag.fert1950flag == 1) && (ag.state == 1 || ag.state == 3) && pdm == 5) {
//   ag.fertn = 14.0;
//  }

//  if((ag.fert1950flag == 1) && (1 == ag.state)) {
//   ag.fertn = 10.0/12.0;
//  }

// if( (ag.fert1950flag == 1) && (3 == ag.state) && (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt)) && (ag.getGROWDD() <= ag.getGDDHARVST(ag.cmnt))){ 
//    cout << "entering second conditional " << endl;
//    ag.fertn = 10.0/6.0;
//    }


  atms.setLWOUTD( atms.lwrad( atms.getTAIRD(),
                              atms.getVPR(),
                              atms.getNIRR(),
                              atms.getGIRR() ) );
                              
  atms.setLWOUTN( atms.lwrad( atms.getTAIRN(),
                              atms.getVPR(),
                              atms.getNIRR(),
                              atms.getGIRR() ) );
  // nirrn already set

  soil.setAVLH2O( pstate[I_SM] - soil.getWILTPT() );
  if( soil.getAVLH2O() < ZERO )
  {
    soil.setAVLH2O( ZERO );
  }
  soil.setVSM( pstate[I_VSM] );
  soil.setSWP();
  soil.setMOIST( pstate[I_SM] );
  soil.setKH2O( pstate[I_VSM], moistlim );
  soil.setNINPUT( ag.getNRETENT() );
  soil.setNLOST( 0.0);

  if( pstate[I_LEAFC] > ZERO )
  {
    veg.setLAI( (pstate[I_LEAFC]*veg.getSLA(ag.cmnt)) );
  }
  else
  {
    veg.setLAI( ZERO );
  }
  veg.aerodynamics( ag.cmnt,
	                 atms.getWS10() );

  #ifdef TIM_RSOIL   
    prob = 1.0 - exp(-0.005*atms.getPREC() );
    if( prob < ZERO ) { prob = ZERO; }
    
    surf_wetness = soil.getAVLH2O()/soil.getAWCAPMM();
    if( surf_wetness < ZERO ) { surf_wetness = ZERO; }
    
    rsoil = 54.65/(prob*surf_wetness + 0.01);
    
    veg.pen.setR_SS( rsoil );
  #endif

  #ifdef SIB_RSOIL
    //surf_wetness = 100.0*soil.getVSM()/soil.getPCTPOR(); // assume surface wetness is like bulk moisture content
    prob = 1.0 - exp(-0.005*atms.getPREC() );
    if( prob < ZERO ) { prob = ZERO; }
     
    //surf_wetness = soil.getAVLH2O()/soil.getAWCAPMM();
    surf_wetness = 100.0*soil.getVSM()/soil.getPCTPOR();
    if( surf_wetness > 1.0 ) { surf_wetness = 1.0; }
    if( surf_wetness < ZERO ) { surf_wetness = ZERO; }

    //if(soil.getSNOWPACK() > ZERO) { surf_wetness = 1.0; }
  
    //veg.pen.setR_SS( exp(8.206 - 4.205*surf_wetness*sqrt(prob)) ); // using modified formulation from SiB2
    veg.pen.setR_SS( exp(8.206 - 4.205*sqrt(surf_wetness*prob)) ); // using formulation from SiB2
  #endif
	                 
  if( (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt))
      || (1 == ag.getISPERENNIAL(ag.cmnt))) 
  // this conditional appears multiple places below where routines are not needed
  //   if crops have not started to grow, and crops are annual
  {
    veg.pen.hydraulics( ag.cmnt,
                        veg.getPHEN( ag.cmnt ),
                        veg.getLAI(),
                        veg.getSLA( ag.cmnt ),
                        veg.getSAPWOODC(),
                        veg.getROOTC(),
                        soil.getAVLH2O(),
                        soil.getAWCAPMM(),
                        atms.getPREC() );
  }                     
	                 
  veg.rxTLaRS( ag.cmnt,
                  atms.getTAIR(),
                  veg.getRATREF( ag.cmnt ) );
  if( 0 == o3flag ) { veg.setFOZONE( 1.0 ); }
  else { veg.setFOZONE( pstate[I_FOZONE] ); }
  
//----------------------------------------------//
//  determine litterfall and respiration fluxes  
  if( (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt))
      || (1 == ag.getISPERENNIAL(ag.cmnt)))
  {
    veg.litterresp( ag.cmnt,
                    atms.getNDAYS(pdm) );
  }
                  
//----------------------------------------------//
//  determine decomposition and mineralization fluxes  
  // Note: Microbes are assumed to be acting on "old" carbon
  //   (i.e. natural vegetation - veg.cmnt) rather than
  //   "new" carbon associated with crops (i.e. ag.cmnt)

nopen = 0;
#ifdef OPENN
  nopen = 1;
#endif

  microbe.updateDynamics( ag.cmnt,
                          soil.getPCTFLDCAP(),
                          soil.getPCTWILTPT(),
                          soil.getPCTPOR(),
                          pstate[I_SOLC],
                          pstate[I_SOLN],
                          pstate[I_SM],
                          pstate[I_VSM],
                          pstate[I_AVLN],
                          moistlim,
                          ag.tillflag,
                          ag.getTILLFACTOR( ag.cmnt ),
                          soil.getKH2O(),
                          veg.getRLTRC(),
                          nopen,
                          ag.getIMMBADD(),
                          ag.getVOLAC(),
                          ag.getVOLAN());

//----------------------------------------------//
//  determine ingpp and innup; calculations for gpp are used in allocate

  if( (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt))
      || (1 == ag.getISPERENNIAL(ag.cmnt)))
  {
    veg.nupxclm( ag.cmnt,
                 nmax_grow,
                 pstate[I_SM],
                 pstate[I_AVLN],
                 veg.getRMT(),
                 soil.getKH2O(),
                 veg.getFOZONE(),
                 pstate[I_ROOTC] );
               
    veg.gppxclm( ag.cmnt,
                 atms.getNDAYS(pdm),
                 atms.getCO2(),
                 atms.getPAR(),
                 atms.getVPR(),
                 atms.getVPDD(),
                 atms.getDAYL(),
                 veg.pen.getKEXT(ag.cmnt),
                 microbe.getRH(),
                 atms.getPREC());

//----------------------------------------------//
//  determine allocation fluxes
  veg.allocate( ag.cmnt,
                atms.getNDAYS(pdm),
                nfeed,
                pdm,
                ag.getGROWDD(),
                ag.state );

    soil.setNINPUT( soil.getNINPUT() + ag.fertn + atms.getNDEP()/365000. );

    #ifdef OPENN
      soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
      veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
    #endif

//----------------------------------------------//
//  update vegetation dynamics

  veg.updateDynamics( ag.cmnt,
                     soil.getNINPUT(),
                     pstate[I_AVLN],
                     nfeed,
                     ag.state,
                     ag.getISPERENNIAL(ag.cmnt),
                     ag.fert1950flag,
                     microbe.getNETNMIN(),
                     ag.fertn,
                     soil.getSONINP());

  soil.setSONINP(veg.getSONINPUT());

  }
  else
  {
    // No crop plants exist - set all monthly fluxes to zero

    veg.resetMonthlyFluxes();
  }

  veg.setESOILMMMO(0.0);
  veg.petsw( ag.cmnt,
             pdm,
             atms.getNDAYS(pdm),
             atms.getDAYL(),
             atms.getTAIRD(),
             atms.getTAIRN(),
             atms.getCO2(),
             atms.getNIRRN(),
             atms.getLWOUTD(),
             atms.getLWOUTN(),
             atms.getVPR(),
             atms.getVPDD(),
             atms.getVPDN(),
             soil.getSNOWPACK(),
             atms.getPREC(),
             veg.getESOILMMMO(),
             soil.getAVLH2O(),
             elev );
            

   veg.deltafo3( ag.cmnt,
              atms.getAOT40() );

   soil.updateHydrology(elev,
                atms.getTAIR(),
                atms.getPREVTAIR(),
                atms.getPREV2TAIR(),
                atms.getRAIN(),
                veg.getPET(),
                soil.getAVLH2O(),
                pstate[I_RGRW],
                pstate[I_SGRW],
                ag.irrgflag,
                ag.irrigate,
                pdm );

  soil.updateDOCLEACH( pstate[I_DOC],
                 pstate[I_SM] );



/*//  soil.setNINPUT( soil.getNINPUT() + ag.fertn +  (0.102 * (12.0*soil.getEET()/10.0)+ 0.524 )/(10.0*12.0));
  soil.setNINPUT( soil.getNINPUT() + ag.fertn + atms.getNDEP()/365000.  );
#ifdef OPENN
if(ag.state == 1)
{
  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
//  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (12.0*((soil.getEET()/soil.getREET())*130.0)/10.0)+ 0.524 )/(10.0*12.0));
//  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (12.0*((soil.getEET()/soil.getREET())*130.0)/10.0)+ 0.524 )/(10.0*12.0));
}
if (ag.state == 2)
{
  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
//  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (12.0*(soil.getEET()*150/70)/10.0)+ 0.524 ) /(10.0*12.0));
//  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (12.0*(soil.getEET()*150/70)/10.0)+ 0.524) /(10.0*12.0));
// soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (12.0*((soil.getEET()/(1.0+soil.getREET()))*190.0)/10.0)+ 0.524 )/(10.0*12.0));
//   veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (12.0*((soil.getEET()/(1.0+soil.getREET()))*190.0)/10.0)+ 0.524 )/(10.0*12.0));
}
if(ag.state == 3)
{
  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
//  soil.setSONINP((veg.getNNF(ag.cmnt)*0.102 * (12.0*((soil.getEET()/soil.getREET())*136.0)/10.0)+ 0.524 )/(10.0*12.0));
//  veg.setVEGNINP(((1.0-veg.getNNF(ag.cmnt))*0.102 * (12.0*((soil.getEET()/soil.getREET())*136.0)/10.0)+ 0.524 )/(10.0*12.0));
}
//  soil.setSONINP(soil.getSONINP()/2.0);
//  veg.setVEGNINP(veg.getVEGNINP()/2.0);

#endif */
  // Determine nitrogen losses from ecosystem

  if( 1 == avlnflag )
  {

#ifdef OPENN
  if(pstate[I_DOC] > 0.0)
  {
  soil.setLCHDON( soil.getLCHDOC() * pstate[I_DON]/pstate[I_DOC]);
  }
  else
  {
  soil.setLCHDON(ZERO);
  }

    soil.updateNLosses( ag.cmnt,
//                        (atms.getRAIN() + soil.getSNOWINF() - soil.getEET() ),
                        (soil.getRPERC() + soil.getSPERC()),
                        pstate[I_AVLN],
                        pstate[I_SM] );

    soil.setLCHDIN(soil.getNLOST());

/*  if(initFlag == 0)
   {
   if(soil.getNLOST() + soil.getLCHDON() < soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP())
   {
      soil.setNLOST(soil.getNLOST() + (soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP()) - (soil.getNLOST() + soil.getLCHDON()));
   }
   }
   else
   {
      soil.setNLOST( soil.getNLOST() +  soil.getDENITR(veg.cmnt) * (0.01*microbe.getGMIN() + veg.getDENITR()) );
   } */

   soil.setNLOST( soil.getNLOST() +  soil.getDENITR(veg.cmnt) * (0.01*microbe.getGMIN() + veg.getDENITR()) );


#endif
//  soil.setNLOST(soil.getNLOST() + soil.getLCHDON() + ag.getCONVRTFLXN() + ag.getCROPRESIDUEFLXN());

//  }
//  else
//  {
    // Do not allow changes in available nitrogen in soil
    //   (used during calibration process)

    if( soil.getNLOST()  > pstate[I_AVLN] - veg.getNUPTAKE()
         + microbe.getNETNMIN() + soil.getNINPUT() )

    {
      soil.setNLOST( (pstate[I_AVLN]
                     - veg.getNUPTAKE()
                     + microbe.getNETNMIN()
                     + soil.getNINPUT()) );
    }
    if( soil.getNLOST() < ZERO )
    {
      soil.setNLOST( ZERO );

      microbe.setNETNMIN( (soil.getNLOST()
                          + veg.getNUPTAKE()
                          - soil.getNINPUT()
                          - pstate[I_AVLN]) );
    }
  }
  else
  {

// Do not allow changes in available nitrogen in soil
//   (used during calibration process)
    soil.setNLOST( (soil.getNINPUT()
                   + microbe.getNETNMIN()
                       - veg.getNUPTAKE()) );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Ttem45::delta( const int& pdm,
                    const double& nmax_grow,
                    double pstate[],
                    double pdstate[] )
{
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in delta(), month = %2d ", pdm);
    refresh();
  #endif
  
//  ++delta_count;
//  if( delta_count > 1000000 ) { exit(-1); }
  if( 0 == ag.state )
  {
    natvegDynamics( pdm, nmax_grow, pstate );
  }
  else
  {
    cropDynamics( pdm, nmax_grow, pstate );
  }


  // Describe monthly changes to carbon pools and fluxes for ODE
  //   state variables (i.e., pdstate)

  // Carbon pools in ecosystems


  pdstate[I_LEAFC] = veg.getALLOCLC()
                  - veg.getRMLEAF()
                  - veg.getLTRLC();
//  cout << "LEAFC = " << pstate[I_LEAFC] << " " << pdstate[I_LEAFC] << " " << veg.getALLOCLC() << " " << veg.getRMLEAF() << " " << veg.getLTRLC() << " " << pstate[I_LABILEC] <<  endl;

  pdstate[I_SAPWOODC] = veg.getALLOCSC()
                      - veg.getALLOCHC()
                      - veg.getRMSAPWOOD()
                      - veg.getLTRSC();

  pdstate[I_HEARTWOODC] = veg.getALLOCHC()
                        - veg.getLTRHC();

  pdstate[I_ROOTC] = veg.getALLOCRC()
                  - veg.getRMROOT()
                  - veg.getLTRRC();

  pdstate[I_SEEDC] = veg.getALLOCSEEDC()
                    - veg.getRMSEED()
                    - veg.getLTRSEEDC();

  pdstate[I_LABILEC] = veg.getGPP()
                  - veg.getALLOCLC()
                  - veg.getALLOCSC()
                  - veg.getALLOCRC()
                  - veg.getALLOCSEEDC()
                  - veg.getRGRWTH();
// cout << "LabileC = " << pdstate[I_LABILEC] << " " << veg.getGPP() << " " << veg.getALLOCLC() << " " <<  veg.getALLOCSC() << " " << veg.getALLOCRC() << " " << veg.getALLOCSEEDC() << " " << veg.getRGRWTH() << endl;

  pdstate[I_SOLC] = veg.getLTRLC()
                    + veg.getLTRSC()
                    + veg.getLTRHC()
                    + veg.getLTRRC()
                    + veg.getLTRSEEDC()
                    + ag.getSLASHC()
                    - ag.getSCONVRTFLXC()
                    - microbe.getDOCPROD()
                    - microbe.getRH();

//  cout << "soil = " << pstate[I_SOLC] << " " << pdstate[I_SOLC] << " " << veg.getLTRLC() << " " << veg.getLTRSC() << " " << veg.getLTRHC() << " " << veg.getLTRRC() << " " << veg.getLTRSEEDC() << " " << ag.getSLASHC() << " " << ag.getSCONVRTFLXC() << " " << microbe.getRH() << " " << microbe.getDOCPROD() << endl;

  pdstate[I_DOC] = microbe.getDOCPROD() - soil.getLCHDOC();

#ifdef OPENN
  pdstate[I_DON] = microbe.getDONPROD() - soil.getLCHDON();
#endif

  pdstate[I_FOZONE] = veg.getDFO3();
  pdstate[I_FRDL] = veg.getFRDL();
  pdstate[I_FCO2] = veg.getFCO2();
  pdstate[I_TEMP] = veg.getTEMP();
  pdstate[I_FH2O] = veg.getFH2O();
  pdstate[I_FO3] = veg.getFOZONE();

  // Nitrogen pools in ecosystems


  pdstate[I_LEAFN] = veg.getALLOCLN()
                   - veg.getNRESORBL()
                   - veg.getLTRLN();

  pdstate[I_SAPWOODN] = veg.getALLOCSN()
                      - veg.getALLOCHN()
                      - veg.getNRESORBS()
                      - veg.getLTRSN();

  pdstate[I_HEARTWOODN] = veg.getALLOCHN()
                        - veg.getLTRHN();

  pdstate[I_ROOTN] = veg.getALLOCRN()
//                   + veg.getVEGNINP()
                   - veg.getNRESORBR()
                   - veg.getLTRRN();

//  cout << "pdstate = " << pdstate[I_ROOTN] << " " << veg.getALLOCRN() << " " << veg.getNRESORBR() << " " << veg.getLTRRN() << " " << y[I_ROOTN] << endl;

  pdstate[I_SEEDN] = veg.getALLOCSEEDN()
                   - veg.getNRESORBSEED()
                   - veg.getLTRSEEDN();

#ifdef OPENN
  pdstate[I_SOLN] =   veg.getLTRLN()
                    + veg.getLTRSN()
                    + veg.getLTRHN()
                    + veg.getLTRRN()
                    + veg.getLTRSEEDN()
                    + ag.getSLASHN()
                    + soil.getSONINP()
                    - microbe.getDONPROD()
                    - ag.getSCONVRTFLXN()
                    - ag.getNSRETENT()
                    - microbe.getNETNMIN();

   pdstate[I_LABILEN] = veg.getNUPTAKE()
                     + veg.getVEGNINP()
                     - veg.getALLOCLN()
                     - veg.getALLOCSN()
                     - veg.getALLOCRN()
                     - veg.getALLOCSEEDN()
                     + veg.getNRESORBL()
                     + veg.getNRESORBS()
                     + veg.getNRESORBR()
                     + veg.getNRESORBSEED();

#else
  pdstate[I_SOLN] =   veg.getLTRLN()
                    + veg.getLTRSN()
                    + veg.getLTRHN()
                    + veg.getLTRRN()
                    + veg.getLTRSEEDN()
                    + ag.getSLASHN()
                    - ag.getSCONVRTFLXN()
                    - ag.getNSRETENT()
                    - microbe.getNETNMIN();

  pdstate[I_LABILEN] = veg.getNUPTAKE()
                     - veg.getALLOCLN()
                     - veg.getALLOCSN()
                     - veg.getALLOCRN()
                     - veg.getALLOCSEEDN()
                     + veg.getNRESORBL()
                     + veg.getNRESORBS()
                     + veg.getNRESORBR()
                     + veg.getNRESORBSEED();

#endif


  pdstate[I_AVLN] = soil.getNINPUT()
                    + microbe.getNETNMIN()
                    + ag.getVOLAN()
                    - veg.getNUPTAKE()
                    - soil.getNLOST();

//cout << "AVLN = " << pdstate[I_AVLN] << " " << soil.getNINPUT() << " " << microbe.getNETNMIN() << " " << veg.getNUPTAKE() << " " << soil.getNLOST() << endl;

//cout << "diag = " << veg.getVEGNINP() << " " << soil.getSONINP() << " " << soil.getNINPUT() << " " << veg.getNNF(ag.cmnt) << endl;


  // Water pools

  pdstate[I_SM] = soil.getSNOWINF()
                    + atms.getRAIN()
                    + ag.irrigate
                    - soil.getEET()
                    - soil.getRPERC()
                    - soil.getSPERC();
//if (ag.irrigate > 0.0) {cout << "ag.irrigate  = " << ag.irrigate << endl;}
//  cout << "delta sm = " << pdstate[I_SM] << " " << pstate[I_SM] << " " << atms.getRAIN() << "  " << soil.getEET() << " " << soil.getRPERC() <<  " " << soil.getSPERC() << endl;

  pdstate[I_VSM] = pdstate[I_SM]/(soil.getROOTZ() * 1000.0);

  if( pstate[I_VSM]+pdstate[I_VSM] <= ZERO )
  {
    pdstate[I_VSM] = 0.001 - pstate[I_VSM];
  }

  pdstate[I_PCTP] = 100.0 * pdstate[I_SM]/soil.getTOTPOR();

  pdstate[I_RGRW] = soil.getRPERC() - soil.getRRUN();

  pdstate[I_SGRW] = soil.getSPERC() - soil.getSRUN();

  // Phenology

  pdstate[I_FPC] = veg.getFPC();

  // Carbon fluxes in ecosystems

  pdstate[I_ALLOCLC] = veg.getALLOCLC();
  pdstate[I_ALLOCSC] = veg.getALLOCSC();
  pdstate[I_ALLOCHC] = veg.getALLOCHC();
  pdstate[I_ALLOCRC] = veg.getALLOCRC();
  pdstate[I_ALLOCSEEDC] = veg.getALLOCSEEDC();
  
  pdstate[I_ALLOCILC] = veg.getALLOCILC();
  pdstate[I_ALLOCISC] = veg.getALLOCISC();
  pdstate[I_ALLOCIHC] = veg.getALLOCIHC();
  pdstate[I_ALLOCIRC] = veg.getALLOCIRC();
  pdstate[I_ALLOCISEEDC] = veg.getALLOCISEEDC();

  pdstate[I_INGPP] = veg.getINGPP();

  pdstate[I_GPP] = veg.getGPP();

  pdstate[I_INNPP] = veg.getINNPP();

  pdstate[I_NPP] = veg.getNPP();

  pdstate[I_GPR] = veg.getGPR();


  pdstate[I_RVMNT] = veg.getRMLEAF() + veg.getRMSAPWOOD() + veg.getRMROOT()
                   + veg.getRMSEED();

  pdstate[I_RMLEAF] = veg.getRMLEAF();
  pdstate[I_RMSAPWOOD] = veg.getRMSAPWOOD();
  pdstate[I_RMROOT] = veg.getRMROOT();
  pdstate[I_RMSEED] = veg.getRMSEED();
  pdstate[I_RMLABILE] = veg.getRMLABILE();

  pdstate[I_RVGRW] = veg.getRGRWTH();

  pdstate[I_LTRLC] = veg.getLTRLC();
  pdstate[I_LTRSC] = veg.getLTRSC();
  pdstate[I_LTRHC] = veg.getLTRHC();
  pdstate[I_LTRRC] = veg.getLTRRC();
  pdstate[I_LTRSEEDC] = veg.getLTRSEEDC();

  pdstate[I_RH] = microbe.getRH();
  pdstate[I_STANDDEADC] = ag.getSTANDDEADC();
  pdstate[I_STANDDEADN] = ag.getSTANDDEADN();
  pdstate[I_VOLAC] = ag.getVOLAC();
  pdstate[I_VOLAN] = ag.getVOLAN();

  // Nitrogen fluxes in ecosystems

  pdstate[I_ALLOCLN] = veg.getALLOCLN();
  pdstate[I_ALLOCSN] = veg.getALLOCSN();
  pdstate[I_ALLOCHN] = veg.getALLOCHN();
  pdstate[I_ALLOCRN] = veg.getALLOCRN();
  pdstate[I_ALLOCSEEDN] = veg.getALLOCSEEDN();
  
  pdstate[I_ALLOCILN] = veg.getALLOCILN();
  pdstate[I_ALLOCISN] = veg.getALLOCISN();
  pdstate[I_ALLOCIHN] = veg.getALLOCIHN();
  pdstate[I_ALLOCIRN] = veg.getALLOCIRN();
  pdstate[I_ALLOCISEEDN] = veg.getALLOCISEEDN();

  pdstate[I_NINP] = soil.getNINPUT();

  pdstate[I_DOCPROD] = microbe.getDOCPROD();
  pdstate[I_LCHDOC] = soil.getLCHDOC();
//#ifdef OPENN
  pdstate[I_DONPROD] = microbe.getDONPROD();
  pdstate[I_LCHDON] = soil.getLCHDON();
  pdstate[I_LCHDIN] = soil.getLCHDIN();
//#endif

  pdstate[I_AGFRTN] = ag.fertn;

  pdstate[I_INNUP] = veg.getINUPTAKE();

  pdstate[I_VNUP] = veg.getNUPTAKE();

  pdstate[I_NRESORBL] = veg.getNRESORBL();
  pdstate[I_NRESORBS] = veg.getNRESORBS();
  pdstate[I_NRESORBR] = veg.getNRESORBR();
  pdstate[I_NRESORBSEED] = veg.getNRESORBSEED();


  pdstate[I_LTRLN] = veg.getLTRLN();
  pdstate[I_LTRSN] = veg.getLTRSN();
  pdstate[I_LTRHN] = veg.getLTRHN();
  pdstate[I_LTRRN] = veg.getLTRRN();
  pdstate[I_LTRSEEDN] = veg.getLTRSEEDN();

  pdstate[I_MNUP] = microbe.getNUPTAKE();

  pdstate[I_NMIN] = microbe.getNETNMIN();

  pdstate[I_NLST] = soil.getNLOST();

//#ifdef OPENN
  pdstate[I_NFIXN] = soil.getSONINP();
  pdstate[I_NFIXS] = veg.getVEGNINP();
//  pdstate[I_NLST] = soil.getNLOST() + soil.getLCHDON() + ag.getCONVRTFLXN() + ag.getCROPRESIDUEFLXN();
//#endif
  // Water fluxes

  pdstate[I_AGIRRIG] = ag.irrigate;

  pdstate[I_INEET] = soil.getINEET();
  pdstate[I_EET] = soil.getEET();
  pdstate[I_RPERC] = soil.getRPERC();
  pdstate[I_SPERC] = soil.getSPERC();
  pdstate[I_RRUN] = soil.getRRUN();
  pdstate[I_SRUN] = soil.getSRUN();

  // Penmon model

  pdstate[I_GC] = veg.getGC();
  pdstate[I_GS] = veg.getGS();
  
  pdstate[I_PECAN] = veg.getPECANW();
  pdstate[I_PESOIL] = veg.getPESOILW();

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

#ifdef CALIBRATE_TEM

void Ttem45::displayOptionalEflx( const seykey& s )
{
  switch( s )
  {
    case GET_VEGC:    printw(" VEG. C "); break;
    case GET_STRN:    printw(" STR. N "); break;
    case GET_SOILC:   printw(" SOIL C "); break;
    case GET_SOILN:   printw(" SOIL N "); break;
    case GET_AVALN:   printw(" AVL. N "); break;
    
    case GET_LEAFC:    printw(" LEAF C "); break;
    case GET_SAPWOODC:    printw(" SAP. C "); break;
    case GET_HEARTWOODC:    printw(" HRT. C "); break;
    case GET_ROOTC:    printw(" ROOT C "); break;
    case GET_SEEDC:    printw(" SEED C "); break;
    case GET_LABILEC:    printw(" LAB. C "); break;
    case GET_LEAFN:    printw(" LEAF N "); break;
    case GET_SAPWOODN:    printw(" SAP. N "); break;
    case GET_HEARTWOODN:    printw(" HRT. N "); break;
    case GET_ROOTN:    printw(" ROOT N "); break;
    case GET_SEEDN:    printw(" SEED N "); break;
    case GET_LABILEN:    printw(" LAB. N "); break;


    case GET_DOC:     printw("   DOC  "); break;
    case GET_DON:     printw("   DON  "); break;

    case GET_LAI:     printw("   LAI  "); break;
    case GET_FPC:     printw("   FPC  "); break;

    case GET_ALLOCLC:    printw(" A-> LC "); break;
    case GET_ALLOCSC:    printw(" A-> SC "); break;
    case GET_ALLOCHC:    printw(" A-> HC "); break;
    case GET_ALLOCRC:    printw(" A-> RC "); break;
    case GET_ALLOCSEEDC:    printw(" A->SDC "); break;
    case GET_ALLOCILC:    printw(" A->ILC "); break;
    case GET_ALLOCISC:    printw(" A->ISC "); break;
    case GET_ALLOCIHC:    printw(" A->IHC "); break;
    case GET_ALLOCIRC:    printw(" A->IRC "); break;
    case GET_ALLOCISEEDC:    printw(" A->ISDC"); break;
    case GET_INGPP:    printw("  INGPP "); break;
    case GET_GPP:      printw("   GPP  "); break;
    case GET_KPLANT:   printw(" KPLANT "); break;
    case GET_INNPP:    printw("  INNPP "); break;
    case GET_NPP:      printw("   NPP  "); break;
    case GET_GPR:      printw("    RA  "); break;
    case GET_RVMNT:    printw("  RVMNT "); break;
    case GET_RMLEAF:   printw(" RMLEAF "); break;
    case GET_RMSAPWOOD:printw(" RmSAP. "); break;
    case GET_RMROOT:   printw(" RmROOT "); break;
    case GET_RMSEED:   printw(" RmSEED "); break;
    case GET_RMLABILE: printw(" RmLAB. "); break;
    case GET_RVGRW:    printw(" RGROWTH"); break;
    case GET_LTRLC:    printw(" LTRLC  "); break;
    case GET_LTRSC:    printw(" LTRSC  "); break;
    case GET_LTRHC:    printw(" LTRHC  "); break;
    case GET_LTRRC:    printw(" LTRRC  "); break;
    case GET_LTRSEEDC: printw(" LTRSDC "); break;
    case GET_AGSTUBC:  printw(" AgSTUBC"); break;
    case GET_RH:       printw("    RH  "); break;
    case GET_NEP:      printw("   NEP  "); break;

    case GET_D40:      printw(" AOT40  "); break;
    case GET_NDEP:      printw(" NDEP  "); break;
    case GET_FOZONE:   printw(" FOZONE "); break;

    case GET_ALLOCLN:  printw(" A-> LN "); break;
    case GET_ALLOCSN:  printw(" A-> SN "); break;
    case GET_ALLOCHN:  printw(" A-> HN "); break;
    case GET_ALLOCRN:  printw(" A-> RN "); break;
    case GET_ALLOCSEEDN: printw(" A->SDN "); break;
    case GET_ALLOCILN: printw(" A->ILN "); break;
    case GET_ALLOCISN: printw(" A->ISN "); break;
    case GET_ALLOCIHN: printw(" A->IHN "); break;
    case GET_ALLOCIRN: printw(" A->IRN "); break;
    case GET_ALLOCISEEDN: printw(" A->ISDN"); break;
    case GET_NINP:     printw(" NINPUT "); break;
    case GET_AGFRTN:   printw(" AgFERTN"); break;
    case GET_INNUP:    printw("  INNUP "); break;
    case GET_VNUP:     printw(" UPTAKE "); break;
    case GET_NRESORBL: printw(" NRSRBL "); break;
    case GET_NRESORBS: printw(" NRSRBS "); break;
    case GET_NRESORBR:   printw(" NRSRBR "); break;
    case GET_NRESORBSEED:   printw(" NRSRBSD"); break;
    case GET_LTRLN:     printw(" LTRLN  "); break;
    case GET_LTRSN:     printw(" LTRSN  "); break;
    case GET_LTRHN:     printw(" LTRHN  "); break;
    case GET_LTRRN:     printw(" LTRRN  "); break;
    case GET_LTRSEEDN:     printw(" LTRSDN "); break;
    case GET_AGSTUBN:  printw(" AgSTUBN"); break;
    case GET_MNUP:     printw(" NIMMOB "); break;
    case GET_NMIN:     printw(" NETNMIN"); break;
    case GET_NLST:     printw("  NLOST "); break;
    case GET_NFIXN:     printw("  NFIXN "); break;
    case GET_NFIXS:     printw("  NFIXS "); break;

    case GET_CNVRTC:   printw(" CNVRTC "); break;
    case GET_VCNVRTC:  printw(" VCNVRTC"); break;
    case GET_SCNVRTC:  printw(" SCNVRTC"); break;
    case GET_SLASHC:   printw(" SLASHC "); break;
    case GET_STANDDEADC:   printw(" STANDDEADC "); break;
    case GET_STANDDEADN:   printw(" STANDDEADN "); break;
    case GET_VOLAC:   printw(" VOLAC "); break;
    case GET_VOLAN:   printw(" VOLAN "); break;
    case GET_FRDL:   printw("  FRDL "); break;
    case GET_FCO2:   printw("  FCO2 "); break;
    case GET_FH2O:   printw("  FH2O "); break;
    case GET_TEMP:   printw("  TEMP "); break;
    case GET_FO3:   printw("  FO3 "); break;
    case GET_CFLX:     printw("  CFLUX "); break;
    case GET_NCE:      printw("   NCE  "); break;

    case GET_CNVRTN:   printw(" CNVRTN "); break;
    case GET_VCNVRTN:  printw(" VCNVRTN"); break;
    case GET_SCNVRTN:  printw(" SCNVRTN"); break;
    case GET_SLASHN:   printw(" SLASHN "); break;
    case GET_NRETNT:   printw(" NRETNT "); break;
    case GET_NVRTNT:   printw(" NVRTNT "); break;
    case GET_NSRTNT:   printw(" NSRTNT "); break;

    case GET_AGPRDC:   printw(" AGPRODC "); break;
    case GET_CLIPPINGS:   printw(" CLIPPINGS "); break;
    case GET_PROD10C:  printw(" PROD10C "); break;
    case GET_PROD100C: printw(" PROD100C"); break;
    case GET_RESIDC:   printw("  RESIDC "); break;

    case GET_AGPRDN:   printw(" AGPRODN "); break;
    case GET_PROD10N:  printw(" PROD10N "); break;
    case GET_PROD100N: printw(" PROD100N"); break;
    case GET_RESIDN:   printw("  RESIDN "); break;

    case GET_AGFPRDC:  printw(" AGFPRDC "); break;
    case GET_PRDF10C:  printw(" PRDF10C "); break;
    case GET_PRDF100C: printw(" PRDF100C"); break;
    case GET_FRESIDC:  printw(" FRESIDC "); break;
    case GET_AGPRDFC:  printw(" AGPRDFC "); break;
    case GET_PRD10FC:  printw(" PRD10FC "); break;
    case GET_PRD100FC: printw(" PRD100FC"); break;
    case GET_TOTPRDFC: printw(" TOTPRDFC"); break;
    case GET_RESIDFC:  printw(" RESIDFC "); break;

    case GET_AGFPRDN:  printw(" AGFPRDN "); break;
    case GET_PRDF10N:  printw(" PRDF10N "); break;
    case GET_PRDF100N: printw(" PRDF100N"); break;
    case GET_FRESIDN:  printw(" FRESIDN "); break;
    case GET_AGPRDFN:  printw(" AGPRDFN "); break;
    case GET_PRD10FN:  printw(" PRD10FN "); break;
    case GET_PRD100FN: printw(" PRD100FN"); break;
    case GET_TOTPRDFN: printw(" TOTPRDFN"); break;
    case GET_RESIDFN:  printw(" RESIDFN "); break;

    case GET_L2SN:     printw("   LCON  "); break;
    case GET_DOCPROD:  printw(" DOCPROD "); break;
    case GET_LCHDOC:   printw(" LCHDOC  "); break;
    case GET_DONPROD:  printw(" DONPROD "); break;
    case GET_LCHDON:   printw(" LCHDON  "); break;
    case GET_LCHDIN:   printw(" LCHDIN  "); break;
  }

};

#endif

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

#ifdef CALIBRATE_TEM

void Ttem45::displayOptionalWflx( const swykey& s )
{
  switch( s )
  {
    case GET_SH2O:    printw(" SMOIST "); break;
    case GET_PCTP:    printw("  PCTP  "); break;
    case GET_VSM:     printw("  VSM   "); break;

    case GET_RAIN:    printw("  RAIN  "); break;
    case GET_SNWFAL:  printw(" SNWFAL "); break;
    case GET_GDD:     printw("  GDD   ");    break;
    case GET_SNWINF:  printw(" SNWINF "); break;
    case GET_TAIR:    printw(" TAIR   "); break;
    case GET_TAIRD:   printw(" TAIRD  "); break;
    case GET_TAIRN:   printw(" TAIRN  "); break;
    case GET_NIRR:   printw(" NIRR  "); break;
    case GET_PAR:   printw(" PAR  "); break;
    case GET_GIRR:   printw(" GIRR  "); break;
    case GET_CLDS:   printw(" CLDS  "); break;
    
    case GET_AGIRRIG: printw(" IRRIG  "); break;
    case GET_PET:     printw("  PET   "); break;
    case GET_INEET:   printw("  INEET "); break;
    case GET_EET:     printw("  EET   "); break;
    case GET_RPERC:   printw("  RPERC "); break;
    case GET_SPERC:   printw("  SPERC "); break;
    case GET_RRUN:    printw("  RRUN  "); break;
    case GET_SRUN:    printw("  SRUN  "); break;
    case GET_WYLD:    printw("  WYLD  "); break;

    case GET_GC:      printw("   GC   "); break;
    case GET_GS:      printw("   GS   "); break;
    case GET_PESOIL:  printw(" PESOIL "); break;
    case GET_PECAN:   printw("  PECAN "); break;
    case GET_SHFLUX:  printw(" SHFLUX "); break;
    case GET_SWP:     printw("   SWP  "); break;
	case GET_VEGH:    printw("  VEGH  "); break;
    case GET_USTAR:   printw(" USTAR  "); break;
	case GET_ZD:      printw("   ZD   "); break;
	case GET_ZO:      printw("   ZO   "); break;
	
	case GET_R_AA:    printw("  R_AA  "); break;
    case GET_R_AC:    printw("  R_AC  "); break;
	case GET_R_AS:    printw("  R_AS  "); break;
	case GET_R_SS:    printw("  R_SS  "); break;
	
	case GET_VAPR:    printw("  VAPR  "); break;
	case GET_VPDD:    printw("  VPDD  "); break;
	case GET_VPDN:    printw("  VPDN  "); break;
	
    case GET_AVLH2O:  printw(" AVLH2O "); break;
    case GET_RGRNDW:  printw(" RGRNDW "); break;
    case GET_SGRNDW:  printw(" SGRNDW "); break;
    case GET_SNWPCK:  printw(" SNWPCK "); break;

  }

};

#endif

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Ttem45::ecdqc( const int& dcmnt )
{

  int qc = ACCEPT;

  if( leafcb[dcmnt] <= -9999.99 ) { return qc = 102; }

  if( sapwoodcb[dcmnt] <= -9999.99 ) { return qc = 104; }

  if( heartwoodcb[dcmnt] <= -9999.99 ) { return qc = 106; }

  if( rootcb[dcmnt] <= -9999.99 ) { return qc = 108; }

  if( seedcb[dcmnt] <= -9999.99 ) { return qc = 110; }

  if( labilecb[dcmnt] <= -9999.99 ) { return qc = 112; }

  if( leafnb[dcmnt] <= -9999.99 ) { return qc = 114; }

  if( sapwoodnb[dcmnt] <= -9999.99 ) { return qc = 116; }

  if( heartwoodnb[dcmnt] <= -9999.99 ) { return qc = 118; }

  if( rootnb[dcmnt] <= -9999.99 ) { return qc = 120; }

  if( seednb[dcmnt] <= -9999.99 ) { return qc = 122; }

  if( labilenb[dcmnt] <= -9999.99 ) { return qc = 124; }

  if( solcb[dcmnt] <= -9999.99 ) { return qc = 126; }

  if( solnb[dcmnt] <= -9999.99 ) { return qc = 128; }

  if( avlnb[dcmnt] <= -9999.99 ) { return qc = 130; }

  if( veg.getCMAX1B( dcmnt ) <= -9999.99 ) { return qc = 135; }
  if( veg.getTAULEAF( dcmnt ) <= -99.99 ) { return qc = 138; }
  if( veg.getTAUSAPWOOD( dcmnt ) <= -99.99 ) { return qc = 139; }
  if( veg.getTAUHEARTWOOD( dcmnt ) <= -99.99 ) { return qc = 140; }
  if( veg.getTAUROOT( dcmnt ) <= -99.99 ) { return qc = 141; }
  if( veg.getTAUSEED( dcmnt ) <= -99.99 ) { return qc = 142; }

  if( veg.getKRA( dcmnt ) <= -99.99 ) { return qc = 143; }

  if( microbe.getKDB( dcmnt ) <= -99.99 ) { return qc = 146; }

  if( microbe.getLCCLNC( dcmnt ) <= -99.99 ) { return qc = 147; }
  if( microbe.getPROPFTOS( dcmnt ) <= -99.99 ) { return qc = 148; }

  if( veg.getNMAX1B( dcmnt ) <= -9999.99 ) { return qc = 151; }

  if( microbe.getNUPB( dcmnt ) <= -9999.99 ) { return qc = 157; }

  if( soil.getNLOSS( dcmnt ) <= -99.99 ) { return qc = 158; }

  if( soil.getDENITR( dcmnt ) <= -99.99 ) { return qc = 159; }

  if( veg.getCNLTR( dcmnt ) <= -99.99 ) { return qc = 164; }

  if( microbe.getCNSOIL( dcmnt ) <= -9999.99 ) { return qc = 165; }

  if( veg.getO3PARA( dcmnt ) <= -99.99 ) { return qc = 193; }
  if( veg.getO3PARB( dcmnt ) <= -99.99 ) { return qc = 194; }
  if( veg.getO3PARC( dcmnt ) <= -99.99 ) { return qc = 195; }

  return qc;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Ttem45::ECDsetODEstate( const int& pdcmnt,
                             const double& psiplusc )
{
  // Initialize the NUMEQ state variables used in the
  //   ODE integrator from ECD and DAT files

  y[I_LEAFC] =  leafcb[pdcmnt];

  if( y[I_LEAFC] < ZERO ) { y[I_LEAFC] = ZERO; }

  y[I_SAPWOODC] = sapwoodcb[pdcmnt];

  if( y[I_SAPWOODC] < ZERO ) { y[I_SAPWOODC] = ZERO; }

  y[I_HEARTWOODC] =  heartwoodcb[pdcmnt];

  if( y[I_HEARTWOODC] < ZERO ) { y[I_HEARTWOODC] = ZERO; }

  y[I_ROOTC] =  rootcb[pdcmnt];

  if( y[I_ROOTC] < ZERO ) { y[I_ROOTC] = ZERO; }

  y[I_SEEDC] = seedcb[pdcmnt];

  if( y[I_SEEDC] < ZERO ) { y[I_SEEDC] = ZERO; }

  y[I_LABILEC] =  labilecb[pdcmnt];

  if( y[I_LABILEC] < ZERO ) { y[I_LABILEC] = ZERO; }

// BSF COMBO start
  y[I_DOC] =  ZERO;

  y[I_DON] = ZERO;


//BSF COMBO end

  y[I_SOLC] = solcb[pdcmnt];

  if( y[I_SOLC] < ZERO ) { y[I_SOLC] = ZERO; }

  y[I_FOZONE] = 1.0;


  y[I_LEAFN] = leafnb[pdcmnt];

  if( y[I_LEAFN] < ZERO ) { y[I_LEAFN] = ZERO; }

  y[I_SAPWOODN] = sapwoodnb[pdcmnt];

  if( y[I_SAPWOODN] < ZERO ) { y[I_SAPWOODN] = ZERO; }

  y[I_HEARTWOODN] =  heartwoodnb[pdcmnt];

  if( y[I_HEARTWOODN] < ZERO ) { y[I_HEARTWOODN] = ZERO; }

  y[I_ROOTN] = rootnb[pdcmnt];

  if( y[I_ROOTN] < ZERO ) { y[I_ROOTN] = ZERO; }

  y[I_SEEDN] =  seednb[pdcmnt];

  if( y[I_SEEDN] < ZERO ) { y[I_SEEDN] = ZERO; }

  y[I_LABILEN] =  labilenb[pdcmnt];

  if( y[I_LABILEN] < ZERO ) { y[I_LABILEN] = ZERO; }


  y[I_SOLN] = solnb[pdcmnt];

  if( y[I_SOLN] < ZERO ) { y[I_SOLN] = ZERO; }


  y[I_AVLN] =  avlnb[pdcmnt];

  if( y[I_AVLN] < ZERO ) { y[I_AVLN] = ZERO; }


  y[I_SM] = soil.getAWCAPMM() + soil.getWILTPT();

  if( y[I_SM] <= ZERO )
  {
    y[I_SM] = 0.001;
  }


  y[I_VSM] = y[I_SM] / (soil.getROOTZ() * 1000.0);

  if( y[I_VSM] <= ZERO )
  {
    y[I_VSM] = 0.001;
  }


  y[I_PCTP] = 100.0 * y[I_SM] / soil.getTOTPOR();

  y[I_RGRW] = ZERO;

  y[I_SGRW] =  ZERO;


  // Initialize all phenology and flux states to zero

  resetODEflux();

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Ttem45::getenviron( void )
{
  // Determine monthly potential evapotranspiration

//  atms.petjh( atms.getNIRR(), atms.getTAIR(), pdm );



  // Determine contribution of snowmelt to soil moisture

  soil.setSNOWINF( soil.snowmelt( elev,
                                  atms.getTAIR(),
                                  atms.getPREVTAIR(),
                                  soil.getPREVSPACK() ) );

  // Determine new snow pack

  soil.setSNOWPACK( (soil.getPREVSPACK()
                     + atms.getSNOWFALL()
                     - soil.getSNOWINF()) );


  if( soil.getSNOWPACK() < ZERO )
  {
    soil.setSNOWPACK( ZERO );
  }

//  ninput = (soil.getLCHDON()+soil.getNLOST())*atms.getPREC()/atms.yrprec;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

#ifdef CALIBRATE_TEM

double Ttem45::getOptionalEflx( const int& optflx,
                                const int& dm )
{

  double outflux;

  switch( optflx )
  {
    case GET_VEGC:    outflux = outputm[I_TOTC][dm]; break;
    case GET_STRN:    outflux = outputm[I_CSTRN][dm]; break;
    case GET_SOILC:   outflux = outputm[I_SOLC][dm]; break;
    case GET_SOILN:   outflux = outputm[I_SOLN][dm]; break;
    case GET_AVALN:   outflux = outputm[I_AVLN][dm]; break;
    
    case GET_LEAFC:    outflux = outputm[I_LEAFC][dm]; break;
    case GET_SAPWOODC:    outflux = outputm[I_SAPWOODC][dm]; break;
    case GET_HEARTWOODC:    outflux = outputm[I_HEARTWOODC][dm]; break;
    case GET_ROOTC:    outflux = outputm[I_ROOTC][dm]; break;
    case GET_SEEDC:    outflux = outputm[I_SEEDC][dm]; break;
    case GET_LABILEC:    outflux = outputm[I_LABILEC][dm]; break;
    case GET_LEAFN:    outflux = outputm[I_LEAFN][dm]; break;
    case GET_SAPWOODN:    outflux = outputm[I_SAPWOODN][dm]; break;
    case GET_HEARTWOODN:    outflux = outputm[I_HEARTWOODN][dm]; break;
    case GET_ROOTN:    outflux = outputm[I_ROOTN][dm]; break;
    case GET_SEEDN:    outflux = outputm[I_SEEDN][dm]; break;
    case GET_LABILEN:    outflux = outputm[I_LABILEN][dm]; break;

    case GET_LAI:     outflux = outputm[I_LAI][dm]; break;
    case GET_FPC:     outflux = outputm[I_NATFPC][dm]; break;

    case GET_ALLOCLC:    outflux = outputm[I_ALLOCLC][dm]; break;
    case GET_ALLOCSC:    outflux = outputm[I_ALLOCSC][dm]; break;
    case GET_ALLOCHC:    outflux = outputm[I_ALLOCHC][dm]; break;
    case GET_ALLOCRC:    outflux = outputm[I_ALLOCRC][dm]; break;
    case GET_ALLOCSEEDC:    outflux = outputm[I_ALLOCSEEDC][dm]; break;
    case GET_ALLOCILC:    outflux = y[I_ALLOCILC]; break;
    case GET_ALLOCISC:    outflux = y[I_ALLOCISC]; break;
    case GET_ALLOCIHC:    outflux = y[I_ALLOCIHC]; break;
    case GET_ALLOCIRC:    outflux = y[I_ALLOCIRC]; break;
    case GET_ALLOCISEEDC:    outflux = y[I_ALLOCISEEDC]; break;
    case GET_INGPP:    outflux = outputm[I_INGPP][dm]; break;
    case GET_GPP:      outflux = outputm[I_GPP][dm]; break;
    case GET_KPLANT:   outflux = veg.pen.getKPLANT(); break;
    case GET_INNPP:    outflux = outputm[I_INNPP][dm]; break;
    case GET_NPP:      outflux = outputm[I_NPP][dm]; break;
    case GET_GPR:      outflux = outputm[I_GPR][dm]; break;
    case GET_RVMNT:    outflux = outputm[I_RVMNT][dm]; break;
    case GET_RMLEAF:   outflux = outputm[I_RMLEAF][dm]; break;
    case GET_RMSAPWOOD:   outflux = outputm[I_RMSAPWOOD][dm]; break;
    case GET_RMROOT:   outflux = outputm[I_RMROOT][dm]; break;
    case GET_RMSEED:   outflux = outputm[I_RMSEED][dm]; break;
    case GET_RMLABILE:   outflux = outputm[I_RMLABILE][dm]; break;
    case GET_RVGRW:    outflux = outputm[I_RVGRW][dm]; break;
    case GET_LTRLC:     outflux = outputm[I_LTRLC][dm]; break;
    case GET_LTRSC:     outflux = outputm[I_LTRSC][dm]; break;
    case GET_LTRHC:     outflux = outputm[I_LTRHC][dm]; break;
    case GET_LTRRC:     outflux = outputm[I_LTRRC][dm]; break;
    case GET_LTRSEEDC:     outflux = outputm[I_LTRSEEDC][dm]; break;
    case GET_RH:       outflux = outputm[I_RH][dm]; break;
    case GET_DOC:       outflux = outputm[I_DOC][dm]; break;
    case GET_DON:       outflux = outputm[I_DON][dm]; break;
    case GET_NEP:      outflux = outputm[I_NEP][dm]; break;

    case GET_D40:      outflux = atms.getAOT40(); break;
    case GET_NDEP:      outflux = atms.getNDEP(); break;
    case GET_FOZONE:   outflux = outputm[I_FOZONE][dm]; break;

    case GET_ALLOCLN:    outflux = outputm[I_ALLOCLN][dm]; break;
    case GET_ALLOCSN:    outflux = outputm[I_ALLOCSN][dm]; break;
    case GET_ALLOCHN:    outflux = outputm[I_ALLOCHN][dm]; break;
    case GET_ALLOCRN:    outflux = outputm[I_ALLOCRN][dm]; break;
    case GET_ALLOCSEEDN:    outflux = outputm[I_ALLOCSEEDN][dm]; break;
    case GET_ALLOCILN:    outflux = y[I_ALLOCILN]; break;
    case GET_ALLOCISN:    outflux = y[I_ALLOCISN]; break;
    case GET_ALLOCIHN:    outflux = y[I_ALLOCIHN]; break;
    case GET_ALLOCIRN:    outflux = y[I_ALLOCIRN]; break;
    case GET_ALLOCISEEDN:    outflux = y[I_ALLOCISEEDN]; break;
    case GET_NINP:     outflux = outputm[I_NINP][dm]; break;
    case GET_AGFRTN:   outflux = outputm[I_AGFRTN][dm]; break;
    case GET_INNUP:    outflux = outputm[I_INNUP][dm]; break;
    case GET_VNUP:     outflux = outputm[I_VNUP][dm]; break;
    case GET_NRESORBL:   outflux = outputm[I_NRESORBL][dm]; break;
    case GET_NRESORBS:   outflux = outputm[I_NRESORBS][dm]; break;
    case GET_NRESORBR:   outflux = outputm[I_NRESORBR][dm]; break;
    case GET_NRESORBSEED:   outflux = outputm[I_NRESORBSEED][dm]; break;
    case GET_LTRLN:     outflux = outputm[I_LTRLN][dm]; break;
    case GET_LTRSN:     outflux = outputm[I_LTRSN][dm]; break;
    case GET_LTRHN:     outflux = outputm[I_LTRHN][dm]; break;
    case GET_LTRRN:     outflux = outputm[I_LTRRN][dm]; break;
    case GET_LTRSEEDN:     outflux = outputm[I_LTRSEEDN][dm]; break;
    case GET_AGSTUBN:  outflux = outputm[I_AGSTUBN][dm]; break;
    case GET_MNUP:     outflux = outputm[I_MNUP][dm]; break;
    case GET_NMIN:     outflux = outputm[I_NMIN][dm]; break;
    case GET_NLST:     outflux = outputm[I_NLST][dm]; break;
    case GET_NFIXN:     outflux = outputm[I_NFIXN][dm]; break;
    case GET_NFIXS:     outflux = outputm[I_NFIXS][dm]; break;

    case GET_CNVRTC:   outflux = outputm[I_CNVRTC][dm];  break;
    case GET_VCNVRTC:  outflux = outputm[I_VCNVRTC][dm];  break;
    case GET_SCNVRTC:  outflux = outputm[I_SCNVRTC][dm];  break;
    case GET_SLASHC:   outflux = outputm[I_SLASHC][dm];  break;
    case GET_STANDDEADC:   outflux = ag.getSTANDDEADC();  break;
    case GET_STANDDEADN:   outflux = ag.getSTANDDEADN();  break;
    case GET_VOLAC:   outflux = ag.getVOLAC();  break;
    case GET_VOLAN:   outflux = ag.getVOLAN();  break;
    case GET_FRDL:   outflux = outputm[I_FRDL][dm];  break;
    case GET_FCO2:   outflux = outputm[I_FCO2][dm];  break;
    case GET_FH2O:   outflux = outputm[I_FH2O][dm];  break;
    case GET_TEMP:   outflux = outputm[I_TEMP][dm];  break;
    case GET_FO3:   outflux = outputm[I_FO3][dm];  break;
    case GET_CFLX:     outflux = outputm[I_CFLX][dm];  break;
    case GET_NCE:      outflux = outputm[I_NCE][dm];  break;

    case GET_CNVRTN:   outflux = outputm[I_CNVRTN][dm];  break;
    case GET_VCNVRTN:  outflux = outputm[I_VCNVRTN][dm];  break;
    case GET_SCNVRTN:  outflux = outputm[I_SCNVRTN][dm];  break;
    case GET_SLASHN:   outflux = outputm[I_SLASHN][dm];  break;
    case GET_NRETNT:   outflux = outputm[I_NRETNT][dm];  break;
    case GET_NVRTNT:   outflux = outputm[I_NVRTNT][dm];  break;
    case GET_NSRTNT:   outflux = outputm[I_NSRTNT][dm];  break;

    case GET_AGSTUBC:  outflux = outputm[I_AGSTUBC][dm]; break;
    case GET_RESIDC:   outflux = outputm[I_RESIDC][dm];  break;

    case GET_AGPRDC:   outflux = outputm[I_AGPRDC][dm];  break;
    case GET_CLIPPINGS: outflux = outputm[I_CLIPPINGS][dm];  break;
    case GET_PROD10C:  outflux = outputm[I_PROD10C][dm];  break;
    case GET_PROD100C: outflux = outputm[I_PROD100C][dm];  break;

    case GET_AGPRDN:   outflux = outputm[I_AGPRDN][dm];  break;
    case GET_PROD10N:  outflux = outputm[I_PROD10N][dm];  break;
    case GET_PROD100N: outflux = outputm[I_PROD100N][dm];  break;
    case GET_RESIDN:   outflux = outputm[I_RESIDN][dm];  break;

    case GET_FRESIDC:  outflux = outputm[I_FRESIDC][dm];  break;

    case GET_AGFPRDC:  outflux = outputm[I_AGFPRDC][dm];  break;
    case GET_PRDF10C:  outflux = outputm[I_PRDF10C][dm];  break;
    case GET_PRDF100C: outflux = outputm[I_PRDF100C][dm];  break;
    case GET_AGPRDFC:  outflux = outputm[I_AGPRDFC][dm];  break;
    case GET_PRD10FC:  outflux = outputm[I_PRD10FC][dm];  break;
    case GET_PRD100FC: outflux = outputm[I_PRDF100C][dm];  break;
    case GET_TOTPRDFC: outflux = outputm[I_TOTPRDFC][dm];  break;
    case GET_RESIDFC:  outflux = outputm[I_RESIDFC][dm];  break;

    case GET_AGFPRDN:  outflux = outputm[I_AGFPRDN][dm];  break;
    case GET_PRDF10N:  outflux = outputm[I_PRDF10N][dm];  break;
    case GET_PRDF100N: outflux = outputm[I_PRDF100C][dm];  break;
    case GET_FRESIDN:  outflux = outputm[I_FRESIDN][dm];  break;
    case GET_AGPRDFN:  outflux = outputm[I_AGPRDFN][dm];  break;
    case GET_PRD10FN:  outflux = outputm[I_PRD10FN][dm];  break;
    case GET_PRD100FN: outflux = outputm[I_PRDF100N][dm];  break;
    case GET_TOTPRDFN: outflux = outputm[I_TOTPRDFN][dm];  break;
    case GET_RESIDFN:  outflux = outputm[I_RESIDFN][dm];  break;

    case GET_DOCPROD:  outflux = outputm[I_DOCPROD][dm];  break;
    case GET_LCHDOC:  outflux = outputm[I_LCHDOC][dm];  break;
    case GET_DONPROD:  outflux = outputm[I_DONPROD][dm];  break;
    case GET_LCHDON:  outflux = outputm[I_LCHDON][dm];  break;
    case GET_LCHDIN:  outflux = outputm[I_LCHDIN][dm];  break;

    case GET_L2SN:     if ( veg.getSTRN() != ZERO )
                       {
                         outflux = outputm[I_LABILEN][dm]/outputm[I_CSTRN][dm];
                       }
                       else { outflux = MISSING; } break;

    default:           outflux = MISSING;
  }

  return outflux;

};

#endif

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

#ifdef CALIBRATE_TEM

double Ttem45::getOptionalWflx( const int& optflx,
                                const int& dm )
{

  double outflux;

  switch( optflx )
  {
    case GET_SH2O:    outflux = outputm[I_SM][dm]; break;
    case GET_PCTP:    outflux = outputm[I_PCTP][dm]; break;
    case GET_VSM:     outflux = 100.0*outputm[I_VSM][dm]; break;

    case GET_RAIN:    outflux = outputmcl[0][dm]; break;
    case GET_SNWFAL:  outflux = outputmcl[1][dm]; break;
    case GET_GDD:     outflux = outputmcl[2][dm]; break;
    case GET_SNWINF:  outflux = outputm[I_SNWINF][dm]; break;
    case GET_AGIRRIG: outflux = outputm[I_AGIRRIG][dm]; break;
    case GET_TAIR:    outflux = outputmcl[3][dm]; break;
    case GET_TAIRD:    outflux = outputmcl[4][dm]; break;
    case GET_TAIRN:    outflux = outputmcl[5][dm]; break;
    case GET_NIRR:    outflux = outputmcl[20][dm]; break;
    case GET_PAR:    outflux = outputmcl[21][dm]; break;
    case GET_GIRR:    outflux = outputmcl[22][dm]; break;
    case GET_CLDS:    outflux = outputmcl[23][dm]; break;
    
    case GET_PET:     outflux = outputm[I_PET][dm]; break;
    case GET_INEET:   outflux = outputm[I_INEET][dm]; break;
    case GET_EET:     outflux = outputm[I_EET][dm]; break;
    case GET_RPERC:   outflux = outputm[I_RPERC][dm]; break;
    case GET_SPERC:   outflux = outputm[I_SPERC][dm]; break;
    case GET_RRUN:    outflux = outputm[I_RRUN][dm]; break;
    case GET_SRUN:    outflux = outputm[I_SRUN][dm]; break;
    case GET_WYLD:    outflux = outputm[I_WYLD][dm]; break;

    case GET_GC:      outflux = outputm[I_GC][dm]; break;
    case GET_GS:   outflux = outputm[I_GS][dm]; break;
    case GET_PESOIL:   outflux = outputm[I_PESOIL][dm]; break;
    case GET_PECAN:   outflux = outputm[I_PECAN][dm]; break;
    case GET_SHFLUX:   outflux = outputmcl[6][dm]; break;
    case GET_SWP:     outflux = outputmcl[7][dm]; break;
    case GET_VEGH:    outflux = outputmcl[8][dm]; break;
	case GET_USTAR:   outflux = outputmcl[9][dm]; break; 
	case GET_ZD:      outflux = outputmcl[10][dm]; break;
	case GET_ZO:      outflux = outputmcl[11][dm]; break;
	case GET_R_AA:    outflux = outputmcl[12][dm]; break;
    case GET_R_AC:    outflux = outputmcl[13][dm]; break;
	case GET_R_AS:    outflux = outputmcl[14][dm]; break;
	case GET_R_SS:    outflux = outputmcl[15][dm]; break;
	
	case GET_VAPR:    outflux = outputmcl[16][dm]; break;
	case GET_VPDD:    outflux = outputmcl[17][dm]; break;
	case GET_VPDN:    outflux = outputmcl[18][dm]; break;
	
//    case GET_AVLH2O:  outflux = (y[I_SM] - soil.getWILTPT()); break;
    case GET_AVLH2O:  outflux = outputmcl[19][dm]; break;
    case GET_RGRNDW:  outflux = outputm[I_RGRW][dm]; break;
    case GET_SGRNDW:  outflux = outputm[I_SGRW][dm]; break;
    case GET_SNWPCK:  outflux = outputm[I_SNWPCK][dm]; break;
    
    default:          outflux = MISSING;
  }

  return outflux;

};

#endif

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Ttem45::getsitecd( const int& dv, const string&  ecd )
{

  fecd[dv].open( ecd.c_str(), ios::in );

  if( !fecd[dv] )
  {
    cerr << endl;
    cerr << "Cannot open " << ecd << " for site ECD input";
    cerr << endl;

    exit( -1 );
  }

  veg.getXMLsiteRootNode( fecd[dv],
                          "siteECD",
                          version,
                          sitename,
                          sitecol,
                          siterow,
                          developer,
                          updated );

  veg.cmnt = veg.getXMLsiteCommunityNode( fecd[dv],
                                          "siteECD",
                                          description );


  leafcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "leafcb",
                                            veg.cmnt );

  sapwoodcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "sapwoodcb",
                                          veg.cmnt );

  heartwoodcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                        "siteECD",
                                        "heartwoodcb",
                                        veg.cmnt );

  rootcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "rootcb",
                                          veg.cmnt );

  seedcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                        "siteECD",
                                        "seedcb",
                                        veg.cmnt );

  labilecb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "labilecb",
                                          veg.cmnt );

  leafnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "leafnb",
                                            veg.cmnt );

  sapwoodnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "sapwoodnb",
                                          veg.cmnt );

  heartwoodnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                        "siteECD",
                                        "heartwoodnb",
                                        veg.cmnt );

  rootnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "rootnb",
                                          veg.cmnt );

  seednb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                        "siteECD",
                                        "seednb",
                                        veg.cmnt );

  labilenb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                          "siteECD",
                                          "labilenb",
                                          veg.cmnt );

  solcb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                               "siteECD",
                                               "solcb",
                                               veg.cmnt );

  solnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                               "siteECD",
                                               "solnb",
                                               veg.cmnt );

  avlnb[veg.cmnt] = veg.getXMLcmntArrayDouble( fecd[dv],
                                               "siteECD",
                                               "avlnb",
                                               veg.cmnt );

  veg.setCMAX1B( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "vegcmax1b",
                                            veg.cmnt ),
                 veg.cmnt );
//cout << "cmax = " << veg.getCMAX1B(veg.cmnt) << endl;

  veg.setTAULEAF( veg.getXMLcmntArrayDouble( fecd[dv],
                                           "siteECD",
                                           "vegtauleaf",
                                           veg.cmnt ),
                veg.cmnt );

  veg.setTAUSAPWOOD( veg.getXMLcmntArrayDouble( fecd[dv],
                                         "siteECD",
                                         "vegtausapwood",
                                         veg.cmnt ),
              veg.cmnt );

  veg.setTAUHEARTWOOD( veg.getXMLcmntArrayDouble( fecd[dv],
                                       "siteECD",
                                       "vegtauheartwood",
                                       veg.cmnt ),
            veg.cmnt );

  veg.setTAUROOT( veg.getXMLcmntArrayDouble( fecd[dv],
                                         "siteECD",
                                         "vegtauroot",
                                         veg.cmnt ),
              veg.cmnt );

  veg.setTAUSEED( veg.getXMLcmntArrayDouble( fecd[dv],
                                       "siteECD",
                                       "vegtauseed",
                                       veg.cmnt ),
            veg.cmnt );

  veg.setKRA( veg.getXMLcmntArrayDouble( fecd[dv],
                                         "siteECD",
                                         "vegkra",
                                         veg.cmnt ),
              veg.cmnt );

  microbe.setKDB( veg.getXMLcmntArrayDouble( fecd[dv],
                                             "siteECD",
                                             "microbekdb",
                                             veg.cmnt ),
                  veg.cmnt );


  microbe.setLCCLNC( veg.getXMLcmntArrayDouble( fecd[dv],
                                                "siteECD",
                                                "microbelcclnc",
                                                veg.cmnt ),
                     veg.cmnt );

  microbe.setPROPFTOS( veg.getXMLcmntArrayDouble( fecd[dv],
                                                  "siteECD",
                                                  "microbepropftos",
                                                  veg.cmnt ),
                       veg.cmnt );

  veg.setNMAX1B( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "vegnmax1b",
                                            veg.cmnt ),
                 veg.cmnt );

  microbe.setNUPB( veg.getXMLcmntArrayDouble( fecd[dv],
                                              "siteECD",
                                              "microbenupb",
                                              veg.cmnt ),
                   veg.cmnt );

  soil.setNLOSS( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "soilnloss",
                                            veg.cmnt ),
                 veg.cmnt );

  soil.setDENITR( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "soildenitr",
                                            veg.cmnt ),
                 veg.cmnt );

  microbe.setCNSOIL( veg.getXMLcmntArrayDouble( fecd[dv],
                                                "siteECD",
                                                "microbecnsoil",
                                                veg.cmnt ),
                     veg.cmnt );


  veg.setCNLTR( veg.getXMLcmntArrayDouble( fecd[dv],
                                     "siteECD",
                                     "vegcnltr",
                                     veg.cmnt ),
          veg.cmnt );


  veg.setO3PARA( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "o3para",
                                            veg.cmnt ),
                 veg.cmnt );

  veg.setO3PARB( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "o3parb",
                                            veg.cmnt ),
                 veg.cmnt );

  veg.setO3PARC( veg.getXMLcmntArrayDouble( fecd[dv],
                                            "siteECD",
                                            "o3parc",
                                            veg.cmnt ),
                 veg.cmnt );

  veg.endXMLcommunityNode( fecd[dv] );

  fecd[dv].close();

};

/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************** */

void Ttem45::initializeState( const int& CYCLE )
{
  
if(ag.state == 0)
{
  setELMNTecd( veg.cmnt, soil.getPSIPLUSC() );
  ECDsetODEstate( veg.cmnt, soil.getPSIPLUSC() );
}
else
{
  setELMNTecd( ag.cmnt, soil.getPSIPLUSC() );
  ECDsetODEstate( ag.cmnt, soil.getPSIPLUSC() );
}

  
  // agriculture-related variables
  ag.setNATSEEDC( ZERO );
  ag.setNATSEEDSTRN( ZERO );
  ag.setNATSEEDSTON( ZERO );
  ag.setNATTOPT( veg.getTOPT() );
  ag.setCROPTOPT( veg.getTOPT() );
  ag.setPRVCROPNPP( ZERO );
  
//  veg.setADJC2N( (1.0 + ( veg.getDC2N() * ( atms.getCO2() - atms.getINITCO2()))) );
//  veg.setCNEVEN( ( veg.getINITCNEVEN( veg.cmnt ) * veg.getADJC2N()) );

  veg.setRNPP( 50.0 );
  soil.setREET( 0.0);
//  microbe.setRRH( 0.0 );
  veg.setRLTRC( 0.0 );
  veg.setRGPP( 100.0 ); 
  veg.setRINGPP( 100.0 ); 
  veg.setPHICNT( veg.getPHI() );
  veg.setRPLEAF( 1.0 );
  
  veg.setRLABILEC( y[I_LABILEC] );
  veg.setRLABILEN( y[I_LABILEN] );
  
  veg.setRDEMANDC( 100.0 );
  veg.setRDEMANDN( 10.0 );
  
  ag.setPRODUCTYEAR( 0 );
  ag.resetPROD( CYCLE );
  ag.resetMonthlyDisturbFluxes();
  
  resetYrFluxes();
  resetMonthlyELMNTFluxes();
  setPrevState();

};

/* *************************************************************
************************************************************** */
void Ttem45::initializecalibState( const int& CYCLE )
{

  ECDsetODEstate( veg.cmnt, soil.getPSIPLUSC() );

  // agriculture-related variables
  ag.setNATSEEDC( ZERO );
  ag.setNATSEEDSTRN( ZERO );
  ag.setNATSEEDSTON( ZERO );
  ag.setNATTOPT( veg.getTOPT() );
  ag.setCROPTOPT( veg.getTOPT() );
  ag.setPRVCROPNPP( ZERO );

//  veg.setADJC2N( (1.0 + ( veg.getDC2N() * ( atms.getCO2() - atms.getINITCO2()))) );
//  veg.setCNEVEN( ( veg.getINITCNEVEN( veg.cmnt ) * veg.getADJC2N()) );

  veg.setRNPP( 50.0 );
  soil.setREET( 0.0 );
//  microbe.setRRH( 0.0 );
  veg.setRLTRC( 0.0 );
  veg.setRGPP( 100.0 );
  veg.setRINGPP( 100.0 );
  veg.setPHICNT( veg.getPHI() );
  veg.setRPLEAF( 1.0 );

  veg.setRLABILEC( y[I_LABILEC] );
  veg.setRLABILEN( y[I_LABILEN] );

  veg.setRDEMANDC( 100.0 );
  veg.setRDEMANDN( 10.0 );

  ag.setPRODUCTYEAR( 0 );
  ag.resetPROD( CYCLE );
  ag.resetMonthlyDisturbFluxes();

  resetYrFluxes();
  resetMonthlyELMNTFluxes();
  setPrevState();


};


/* *************************************************************
************************************************************** */

void Ttem45::massbal( void )
{


  if( (y[I_SM] - prevy[I_SM]) != (soil.getSNOWINF() + atms.getRAIN()
      + y[I_AGIRRIG] - y[I_RPERC] - y[I_EET] - y[I_SPERC]) )
  {
    y[I_SPERC] = soil.getSNOWINF()
                 + atms.getRAIN()
                 + y[I_AGIRRIG]
                 - y[I_RPERC]
                 - y[I_EET]
                 - y[I_SM]
                 + prevy[I_SM];
  }

if( y[I_SM] <= ZERO ) { y[I_SM] = 0.001; }

  // DWK added following statement on 20020401 to keep SPERC positive
  // when agricultural croplands are abandoned

  if( y[I_SPERC] < ZERO ) { y[I_SPERC] = ZERO; }

  if( y[I_PCTP] != 100.0 * y[I_SM] / soil.getTOTPOR() )
  {
    y[I_PCTP] = 100.0 * y[I_SM] / soil.getTOTPOR();
  }

 if( y[I_PCTP] < ZERO ) { y[I_PCTP] = ZERO; }

  if( y[I_VSM] != y[I_SM] / (soil.getROOTZ() * 1000.0) )
  {
    y[I_VSM] = y[I_SM] / (soil.getROOTZ() * 1000.0);

    if( y[I_VSM] <= ZERO ) { y[I_VSM] = 0.001; }
  }


  if( (y[I_RGRW] - prevy[I_RGRW]) != (y[I_RPERC] - y[I_RRUN]) )
  {
    y[I_RRUN] = y[I_RPERC] - y[I_RGRW] + prevy[I_RGRW];
  }

  if( y[I_RGRW] < ZERO ) { y[I_RGRW] = ZERO; }
  // DWK added following statement on 20020401 to keep RRUN positive
  // when agricultural croplands are abandoned

  if( y[I_RRUN] < ZERO ) { y[I_RRUN] = ZERO; }


  if( (y[I_SGRW] - prevy[I_SGRW]) != (y[I_SPERC] - y[I_SRUN]) )
  {
    y[I_SRUN] = y[I_SPERC] - y[I_SGRW] + prevy[I_SGRW];
  }

 if( y[I_SGRW] < ZERO ) { y[I_SGRW] = ZERO; }

  // DWK added following statement on 20020401 to keep SRUN positive
  // when agricultural croplands are abandoned


  if( y[I_SRUN] < ZERO ) { y[I_SRUN] = ZERO; }



/************************* Carbon Cycle Balances **************************/

  if( y[I_INNPP] < y[I_NPP] ) { y[I_INNPP] = y[I_NPP]; }

  if( y[I_INGPP] < y[I_GPP] ) { y[I_INGPP] = y[I_GPP]; }

  if( y[I_GPR] != y[I_GPP] - y[I_NPP] )
  {
    y[I_GPR] = y[I_GPP] - y[I_NPP];
  }

  if( y[I_GPR] != y[I_RVMNT] + y[I_RVGRW] )
  {
    y[I_RVGRW] = y[I_GPR] - y[I_RVMNT];
  }

  if( y[I_LEAFC] - prevy[I_LEAFC]
      != y[I_ALLOCLC] - y[I_RMLEAF] - y[I_LTRLC] )
  {
    y[I_LTRLC] = y[I_ALLOCLC] - y[I_RMLEAF] - y[I_LEAFC] + prevy[I_LEAFC];
  }

  if( y[I_LEAFC] < ZERO ) { y[I_LEAFC] = ZERO; }

  if( y[I_SAPWOODC] - prevy[I_SAPWOODC]
    != y[I_ALLOCSC] - y[I_ALLOCHC] - y[I_RMSAPWOOD] - y[I_LTRSC] )
  {
    y[I_LTRSC] = y[I_ALLOCSC] - y[I_ALLOCHC] - y[I_RMSAPWOOD] - y[I_SAPWOODC] + prevy[I_SAPWOODC];
  }

  if( y[I_SAPWOODC] < ZERO ) { y[I_SAPWOODC] = ZERO; }

  if( y[I_HEARTWOODC] - prevy[I_HEARTWOODC]
  != y[I_ALLOCHC] - y[I_LTRHC] )
  {
    y[I_LTRHC] = y[I_ALLOCHC] - y[I_HEARTWOODC] + prevy[I_HEARTWOODC];
  }

   if( y[I_HEARTWOODC] < ZERO ) { y[I_HEARTWOODC] = ZERO; }

  if( y[I_ROOTC] - prevy[I_ROOTC]
    != y[I_ALLOCRC] - y[I_RMROOT] - y[I_LTRRC] )
  {
    y[I_LTRRC] = y[I_ALLOCRC] - y[I_RMROOT] - y[I_ROOTC] + prevy[I_ROOTC];
  }

   if( y[I_ROOTC] < ZERO ) { y[I_ROOTC] = ZERO; }

  if( y[I_SEEDC] - prevy[I_SEEDC]
    != y[I_ALLOCSEEDC] - y[I_RMSEED] - y[I_LTRSEEDC] )
  {
    y[I_LTRSEEDC] = y[I_ALLOCSEEDC] - y[I_RMSEED] - y[I_SEEDC] + prevy[I_SEEDC];
  }

  if( y[I_LABILEC] - prevy[I_LABILEC]
    != y[I_GPP] - y[I_ALLOCLC] - y[I_ALLOCSC] - y[I_ALLOCRC] -
   - y[I_ALLOCSEEDC] - y[I_RVGRW] )
  {
  y[I_RVGRW] = y[I_GPP] - y[I_ALLOCLC] - y[I_ALLOCSC] - y[I_ALLOCRC]
       - y[I_ALLOCSEEDC] - y[I_LABILEC] + prevy[I_LABILEC];
  }

  if( y[I_LABILEC] < ZERO ) { y[I_LABILEC] = ZERO; }

  if( y[I_SOLC] - prevy[I_SOLC] != y[I_LTRLC] + y[I_LTRSC] + y[I_LTRHC] + y[I_LTRRC] + y[I_LTRSEEDC]
       + ag.getSLASHC() - ag.getSCONVRTFLXC() - y[I_RH])
  {
    y[I_RH] = y[I_LTRLC]
              + y[I_LTRSC]
              + y[I_LTRHC]
              + y[I_LTRRC]
              + y[I_LTRSEEDC]
              + ag.getSLASHC()
              - ag.getSCONVRTFLXC()
              - y[I_SOLC]
              + prevy[I_SOLC];
  } 

  if( y[I_SOLC] < ZERO ) { y[I_SOLC] = ZERO; }

  if( y[I_DOC] - prevy[I_DOC] != y[I_DOCPROD] - y[I_LCHDOC])
  {
    y[I_LCHDOC] = y[I_DOCPROD] - y[I_DOC] + prevy[I_DOC];
  }


  /*********************Nitrogen Cycle Balances**********************/

  if( y[I_VNUP] < ZERO ) { y[I_VNUP] = ZERO; }

  if( y[I_INNUP] < y[I_VNUP] ) { y[I_INNUP] = y[I_VNUP]; }


  // DWK modified the following conditions for checking the mass
  //   balance on STON on 0020401


  if( y[I_LABILEN] - prevy[I_LABILEN]
       != y[I_VNUP] + y[I_NRESORBL] +  y[I_NRESORBS] + y[I_NRESORBR] + y[I_NRESORBSEED]
       +y[I_NFIXS] - y[I_ALLOCLN] - y[I_ALLOCSN] - y[I_ALLOCRN] - y[I_ALLOCSEEDN] )
  {
    y[I_NRESORBL] = y[I_ALLOCLN]
                  + y[I_ALLOCSN]
                  + y[I_ALLOCRN]
                  + y[I_ALLOCSEEDN]
                  + y[I_NFIXS]
                  - y[I_VNUP]
                  + y[I_LABILEN]
                  - prevy[I_LABILEN]
                  - y[I_NRESORBS]
                  - y[I_NRESORBR]
                  - y[I_NRESORBSEED];
  }

   if( y[I_LABILEN] < ZERO ) { y[I_LABILEN] = ZERO; }

  if(y[I_NRESORBL] < ZERO) { y[I_NRESORBL] = ZERO; }

  if( y[I_LEAFN] - prevy[I_LEAFN]
      != y[I_ALLOCLN] - y[I_NRESORBL] - y[I_LTRLN] )
  {
    y[I_LTRLN] = y[I_ALLOCLN] - y[I_NRESORBL] - y[I_LEAFN] + prevy[I_LEAFN];
  }

 if( y[I_LEAFN] < ZERO ) { y[I_LEAFN] = ZERO; }

  if( y[I_SAPWOODN] - prevy[I_SAPWOODN]
      != y[I_ALLOCSN] - y[I_ALLOCHN] - y[I_NRESORBS] - y[I_LTRSN] )
  {
    y[I_LTRSN] = y[I_ALLOCSN] -y[I_ALLOCHN] - y[I_NRESORBS] - y[I_SAPWOODN] + prevy[I_SAPWOODN];
  }

    if( y[I_SAPWOODN] < ZERO ) { y[I_SAPWOODN] = ZERO; }

  if( y[I_HEARTWOODN] - prevy[I_HEARTWOODN]
      != y[I_ALLOCHN] - y[I_LTRHN] )
  {
    y[I_LTRHN] = y[I_ALLOCHN] - y[I_HEARTWOODN] + prevy[I_HEARTWOODN];
  }

   if( y[I_HEARTWOODN] < ZERO ) { y[I_HEARTWOODN] = ZERO; }

  if( y[I_ROOTN] - prevy[I_ROOTN]
//      != y[I_ALLOCRN] + y[I_NFIXS] - y[I_NRESORBR] - y[I_LTRRN] )
      != y[I_ALLOCRN] - y[I_NRESORBR] - y[I_LTRRN] )
  {
//    y[I_LTRRN] = y[I_ALLOCRN] + y[I_NFIXS] - y[I_NRESORBR] - y[I_ROOTN] + prevy[I_ROOTN];
    y[I_LTRRN] = y[I_ALLOCRN] - y[I_NRESORBR] - y[I_ROOTN] + prevy[I_ROOTN];
  }

 if( y[I_ROOTN] < ZERO ) { y[I_ROOTN] = ZERO; }

  if( y[I_SEEDN] - prevy[I_SEEDN]
    != y[I_ALLOCSEEDN] - y[I_NRESORBSEED] - y[I_LTRSEEDN] )
  {
    y[I_LTRSEEDN] = y[I_ALLOCSEEDN] - y[I_NRESORBSEED] - y[I_SEEDN] + prevy[I_SEEDN];
  }

   if( y[I_SEEDN] < ZERO ) { y[I_SEEDN] = ZERO; }

#ifdef OPENN
  if( y[I_SOLN] - prevy[I_SOLN] != y[I_LTRLN] + y[I_LTRSN] + y[I_LTRHN] + y[I_LTRRN] + y[I_LTRSEEDN]
      + ag.getSLASHN() + y[I_NFIXN]
      - y[I_DONPROD] - ag.getSCONVRTFLXN() - ag.getNSRETENT() - y[I_NMIN] )
  {
    y[I_NMIN] = y[I_LTRLN]
                + y[I_LTRSN]
                + y[I_LTRHN]
                + y[I_LTRRN]
                + y[I_LTRSEEDN]
                + ag.getSLASHN()
                + y[I_NFIXN]
                - y[I_DONPROD]
                - ag.getSCONVRTFLXN()
                - ag.getNSRETENT()
                - y[I_SOLN]
                + prevy[I_SOLN];
  }

  if( y[I_SOLN] < ZERO ) { y[I_SOLN] = ZERO; }

  if( y[I_DON] - prevy[I_DON] != y[I_DONPROD] - y[I_LCHDON] )
  {
   y[I_LCHDON] = y[I_DONPROD] - y[I_DON] + prevy[I_DON];
  }
#else
  if( y[I_SOLN] - prevy[I_SOLN] != y[I_LTRLN] + y[I_LTRSN] + y[I_LTRHN] + y[I_LTRRN] + y[I_LTRSEEDN]
      + ag.getSLASHN()
      - ag.getSCONVRTFLXN() - ag.getNSRETENT() - y[I_NMIN] )
  {
    y[I_NMIN] = y[I_LTRLN]
                + y[I_LTRSN]
                + y[I_LTRHN]
                + y[I_LTRRN]
                + y[I_LTRSEEDN]
                + ag.getSLASHN()
                - ag.getSCONVRTFLXN()
                - ag.getNSRETENT()
                - y[I_SOLN]
                + prevy[I_SOLN];
  }

    if( y[I_SOLN] < ZERO ) { y[I_SOLN] = ZERO; }
#endif

  if( y[I_AGFRTN] < ZERO ) { y[I_AGFRTN] = ZERO; }

  if ( y[I_NINP] != y[I_AGFRTN] + ag.getNRETENT() )
  {
    y[I_NINP] = y[I_AGFRTN] + ag.getNRETENT();
  }

  if ( y[I_NINP] < ZERO ) { y[I_NINP] = ZERO; }

  if ( y[I_NLST] < ZERO ) { y[I_NLST] = ZERO; }


  if( y[I_AVLN] - prevy[I_AVLN]
      > y[I_NINP] + y[I_NMIN] + ag.getVOLAN() - y[I_VNUP] - y[I_NLST] )
  {
//    y[I_NLST] = ZERO;
    y[I_NINP] =  y[I_NLST]
                   - y[I_NMIN]
                   - ag.getVOLAN()
                   + y[I_VNUP]
                   + y[I_AVLN]
                   - prevy[I_AVLN];

  }
  else if( y[I_AVLN] - prevy[I_AVLN]
      < y[I_NINP] + y[I_NMIN] + ag.getVOLAN() - y[I_VNUP] - y[I_NLST] )
  {
//    y[I_NINP] = ZERO;
    y[I_NLST] =  y[I_NINP]
                 + y[I_NMIN]
                 + ag.getVOLAN()
                 - y[I_VNUP]
                 - y[I_AVLN]
                 + prevy[I_AVLN];
  }

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

int Ttem45::monthlyTransient( const int& pdyr,
                              const int& pdm,
                              const double& ptol,
                              const int& CYCLE,
                              const int& ichrt )
{
  endeq = 0;
  initFlag = 1;

  // Reset texture-dependent parameters for element (e.g. grid
  //   cell) each month (Note: required if simulating a vector
  //   of elements month-to-month)

//  Kick's Fix 09272007
//  if ( ag.prvstate == 0 )

  if ( ag.state == 0
        || (ag.state > 0
        && disturbflag > 0
        && pdm < (disturbmonth - 1)) )
  {
    y[I_VSM] = soil.updateRootZ( veg.cmnt,
                                 y[I_SM],
                                 y[I_ROOTC] );

    veg.resetEcds( veg.cmnt, soil.getPSIPLUSC() );
  }
  else
  {
    y[I_VSM] = soil.updateRootZ( ag.cmnt,
                                 y[I_SM],
                                 y[I_ROOTC] );


    veg.resetEcds( ag.cmnt, soil.getPSIPLUSC() );
  }

    microbe.resetEcds( veg.cmnt, soil.getPSIPLUSC() );




  // Update monthly carbon, nitrogen and water dynamics of
  //   terrestrial ecosystem

  stepmonth( pdyr, pdm, intflag, ptol, CYCLE, ichrt );


  if( totyr == startyr ) { wrtyr = 0; }

//  if( (CYCLE-1) == pdm )
//  {
//    if( totyr > startyr ) { ++wrtyr; }
//  }

  return wrtyr;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Ttem45::natvegDynamics( const int& pdm, const double& nmax_grow, double pstate[] )
{
  int agstate = 0;
  int perennial = 1;
  int fertflag = 0;
  int tillflag = 0;
  int irrgflag = 0;
//  int count = 0;
  double prob;
  double surf_wetness;
  int nopen;
//  double rsoil;
//  double pesoilcor;
//  double wevap;
//  double rfrac;
  
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in natvegDynamics(), month = %2d ", pdm);
    refresh();
  #endif

  #ifdef DEBUG_XTEM
    printf(" entering natvegDynamics(), month = %2d \n", pdm);
  #endif

//----------------------------------------------//
//  transfer information from pstate to veg biomass variables
  updateVegBiomass( pstate );

//----------------------------------------------//
//  update variables used in natvegdynamics:
//    ag.fertn
//    atms.lwout, atms.nirrn
//    soil.avlh2o, soil.gm, soil.kh2o, soil.ninput
//    veg.lai, veg.rmt
//
  ag.fertn = ZERO;

  atms.setLWOUTD( atms.lwrad( atms.getTAIRD(),
                              atms.getVPR(),
                              atms.getNIRR(),
                              atms.getGIRR() ) );
                              
  atms.setLWOUTN( atms.lwrad( atms.getTAIRN(),
                              atms.getVPR(),
                              atms.getNIRR(),
                              atms.getGIRR() ) );

  // nirrn already set

  soil.setAVLH2O( pstate[I_SM] - soil.getWILTPT() );
  if( soil.getAVLH2O() < ZERO )
  {
    soil.setAVLH2O( ZERO );
  }
  soil.setVSM( pstate[I_VSM] );
  soil.setSWP();
  soil.setMOIST( pstate[I_SM] );
  soil.setKH2O( pstate[I_VSM], moistlim );
  soil.setNLOST( 0.0);
  soil.setNINPUT( ag.getNRETENT() );
//  cout << "avalh2o = " << soil.getAVLH2O() << " " << soil.getVSM() << " " << soil.getSWP() << " " << soil.getMOIST() << " " << soil.getKH2O() << endl;
//  cout << "avalh2o = " << soil.getAVLH2O() << " " << pstate[I_SM] << " " << soil.getWILTPT() << " " << soil.getAWCAPMM() << endl;
//  cout << "water terms = " << soil.getSNOWINF() << " " << atms.getRAIN() <<  " " << ag.irrigate << " " << soil.getEET() << " " << soil.getRPERC() << " " << soil.getSPERC() << endl;

  if( pstate[I_LEAFC] > ZERO )
  {
    veg.setLAI( (pstate[I_LEAFC]*veg.getSLA(veg.cmnt)) );
  }
  else
  {
    veg.setLAI( ZERO );
  }
//  cout << "lai = " << veg.getLAI() << " " << veg.getSLA(veg.cmnt) << endl;
  
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in natvegDynamics() before aerodynamics(), month = %2d ", pdm);
    refresh();
  #endif
  
  #ifdef DEBUG_XTEM
    printf(" in natvegDynamics(), before aerodynamics() \n");
  #endif
  
  veg.aerodynamics( veg.cmnt,
	                 atms.getWS10() );

  #ifdef TIM_RSOIL   
    prob = 1.0 - exp(-0.005*atms.getPREC() );
    if( prob < ZERO ) { prob = ZERO; }
    
    surf_wetness = soil.getAVLH2O()/soil.getAWCAPMM();
    if( surf_wetness < ZERO ) { surf_wetness = ZERO; }
    
    rsoil = 54.65/(prob*surf_wetness + 0.01);
    
    veg.pen.setR_SS( rsoil );
  #endif
  
  #ifdef SIB_RSOIL
    //surf_wetness = 100.0*soil.getVSM()/soil.getPCTPOR(); // assume surface wetness is like bulk moisture content
    prob = 1.0 - exp(-0.005*atms.getPREC() );
    if( prob < ZERO ) { prob = ZERO; }
     
//    surf_wetness = soil.getAVLH2O()/soil.getAWCAPMM();
    surf_wetness = 100.0*soil.getVSM()/soil.getPCTPOR();
//    if(atms.getPREC() > 10.0) {
//     surf_wetness = soil.getAVLH2O()/soil.getAWCAPMM();
//    }
    if( surf_wetness > 1.0 ) { surf_wetness = 1.0; }
    if( surf_wetness < ZERO ) { surf_wetness = ZERO; }

    //if(soil.getSNOWPACK() > ZERO) { surf_wetness = 1.0; }
  
    //veg.pen.setR_SS( exp(8.206 - 4.205*surf_wetness*sqrt(prob)) ); // using modified formulation from SiB2
//    if(atms.getPREC() > 10.0) {
//    veg.pen.setR_SS(54.65/(prob*surf_wetness + 0.01));
//    }
//    else
//    {
//    BSF  Adapt monthly to daily

    veg.pen.setR_SS(exp(8.206 - 4.205*sqrt(surf_wetness*prob)) ); // using formulation from SiB2
//    }
//    veg.pen.setR_SS((11.489*exp(-11.67*soil.getVSM()))* exp(8.206 - 4.205*sqrt(surf_wetness*prob)) ); // using formulation from SiB2
//    veg.pen.setR_SS((78.935*exp(-20.7*soil.getVSM()))* exp(8.206 - 4.205*sqrt(surf_wetness*prob)) ); // using formulation from SiB2
//    veg.pen.setR_SS((-4.64*log(soil.getVSM())-6.5404)* exp(8.206 - 4.205*sqrt(surf_wetness*prob)) ); // using formulation from SiB2
  #endif
	                 
  veg.pen.hydraulics( veg.cmnt,
                      veg.getPHEN( veg.cmnt ),
                      veg.getLAI(),
                      veg.getSLA( veg.cmnt ),
                      veg.getSAPWOODC(),
                      veg.getROOTC(),
                      soil.getAVLH2O(),
                      soil.getAWCAPMM(),
                      atms.getPREC() );

  veg.rxTLaRS( veg.cmnt,
                  atms.getTAIR(),
                  veg.getRATREF( veg.cmnt ) );
  if( 0 == o3flag ) { veg.setFOZONE( 1.0 ); }
  else { veg.setFOZONE( pstate[I_FOZONE] ); }

//----------------------------------------------//
//  determine litterfall and respiration fluxes  
  veg.litterresp( veg.cmnt,
                  atms.getNDAYS(pdm) );
                  
//----------------------------------------------//
//  determine decomposition and mineralization fluxes

nopen = 0;
#ifdef OPENN
  nopen = 1;
#endif

  microbe.updateDynamics( veg.cmnt,
                          soil.getPCTFLDCAP(),
                          soil.getPCTWILTPT(),
                          soil.getPCTPOR(),
                          pstate[I_SOLC],
                          pstate[I_SOLN],
                          pstate[I_SM],
                          pstate[I_VSM],
                          pstate[I_AVLN],
                          moistlim,
                          tillflag,
                          ag.getTILLFACTOR( veg.cmnt ),
                          soil.getKH2O(),
                          veg.getRLTRC(),
                          nopen,
                          ag.getIMMBADD(),
                          ag.getVOLAC(),
                          ag.getVOLAN());

                 

//----------------------------------------------//
//  determine ingpp and innup; calculations for gpp are used in allocate
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in natvegDynamics() before gppxclm(), month = %2d ", pdm);
    move(DEBUG_ROW+1,1);
    printw(" aerodynamic resistances (raa,rac) = %8.21f  %8.21f, lai = %8.21f ", veg.pen.getR_AA(), veg.pen.getR_AC(), veg.getLAI() );
    refresh();
  #endif
  
  #ifdef DEBUG_XTEM
    printf(" in natvegDynamics(), before gppxclm() \n");
  #endif
  
  veg.nupxclm( veg.cmnt,
               nmax_grow,
               pstate[I_SM],
               pstate[I_AVLN],
               veg.getRMT(),
               soil.getKH2O(),
               veg.getFOZONE(),
               pstate[I_ROOTC] );
               
  veg.gppxclm( veg.cmnt,
               atms.getNDAYS(pdm),
               atms.getCO2(),
               atms.getPAR(),
               atms.getVPR(),
               atms.getVPDD(),
               atms.getDAYL(),
               veg.pen.getKEXT(veg.cmnt),
               microbe.getRH(),
               atms.getPREC());


//  if(atms.getPREC() > 10 && veg.pen.getFWS() < .9) {cout << "fws = " << veg.pen.getFWS() << " " << atms.getPREC() << endl;}

//----------------------------------------------//
//  determine allocation fluxes
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in natvegDynamics() before allocate(), month = %2d ", pdm);
    refresh();
  #endif
  
  #ifdef DEBUG_XTEM
    printf(" in natvegDynamics(), before allocate() \n");
  #endif
  
  veg.allocate( veg.cmnt,
                atms.getNDAYS(pdm),
                nfeed,
                pdm,
                ag.getGROWDD(),
                ag.state );

#ifdef OPENN
  soil.setNINPUT(soil.getNINPUT() + atms.getNDEP()/365000.);
  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
#endif


//----------------------------------------------//
//  update vegetation dynamics

  if ( disturbflag > 1 && pdm == (disturbmonth -1) )
  {
    // set monthly vegetation fluxes to zero
    veg.resetMonthlyFluxes();
  }
  else
  {

    veg.updateDynamics( veg.cmnt,
                        soil.getNINPUT(),
                        pstate[I_AVLN],
                        nfeed,
                        agstate,
                        perennial,
                        fertflag,
                        microbe.getNETNMIN(),
                        ag.fertn,
                        soil.getSONINP() );

   soil.setSONINP(veg.getSONINPUT());
  }
  
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" in natvegdynamics, before petsw(), month = %2d ", pdm);
    refresh();
  #endif
  
  #ifdef DEBUG_XTEM
    printf(" in natvegDynamics(), before petsw() \n");
    for( int i = 0; i < NUMEQ; ++i ) { printf(" pstate[%d] = %4.1lf \n", i, pstate[i]); }
  #endif
  
  veg.setESOILMMMO(0.0);
  veg.petsw( veg.cmnt,
                 pdm,
                 atms.getNDAYS(pdm),
                 atms.getDAYL(),
                 atms.getTAIRD(),
                 atms.getTAIRN(),
                 atms.getCO2(),
                 atms.getNIRRN(),
                 atms.getLWOUTD(),
                 atms.getLWOUTN(),
                 atms.getVPR(),
                 atms.getVPDD(),
                 atms.getVPDN(),
                 soil.getSNOWPACK(),
                 atms.getPREC(),
                 veg.getESOILMMMO(),
                 soil.getAVLH2O(),
                 elev );
                
 if((soil.getAVLH2O() < .5 * soil.getAWCAPMM()) && (veg.getPESOILW() > 0.5 * atms.getPREC()))
{
  veg.setESOILMMMO(0.5 * atms.getPREC());

//if(veg.getPESOILW() >  max(.5*atms.getPREC(),.1*soil.getAVLH2O()))
//{
//  veg.setESOILMMMO( max(.5*atms.getPREC(),.1*soil.getAVLH2O()) );
  veg.petsw( veg.cmnt,
                 pdm,
                 atms.getNDAYS(pdm),
                 atms.getDAYL(),
                 atms.getTAIRD(),
                 atms.getTAIRN(),
                 atms.getCO2(),
                 atms.getNIRRN(),
                 atms.getLWOUTD(),
                 atms.getLWOUTN(),
                 atms.getVPR(),
                 atms.getVPDD(),
                 atms.getVPDN(),
                 soil.getSNOWPACK(),
                 atms.getPREC(),
                 veg.getESOILMMMO(),
                 soil.getAVLH2O(),
                 elev );
}  

//cout << "petsw = " << veg.cmnt << " " << atms.getNDAYS(pdm) << " " << atms.getDAYL() << " " << atms.getTAIRD() << " " << atms.getTAIRN() << " " << atms.getCO2() << " " << atms.getNIRRN() << " " << atms.getLWOUTD() << " " << atms.getLWOUTN() << " " << atms.getVPR() << " " << atms.getVPDD() << " " << atms.getVPDN() << " " << soil.getSNOWPACK() << " " << atms.getPREC() << " " << veg.getESOILMMMO() << " " << elev << endl;
/* while(veg.getPESOILW() >  max(.9*atms.getPREC(),.5*soil.getAVLH2O()))
  {
    count = count+1;
    pesoilcor = 2.0; 
    veg.pen.setR_SS(pesoilcor*veg.pen.getR_SS());
    veg.petsw( ag.cmnt,
             pdm,
             atms.getNDAYS(pdm),
             atms.getDAYL(),
             atms.getTAIRD(),
             atms.getTAIRN(),
             atms.getCO2(),
             atms.getNIRRN(),
             atms.getLWOUTD(),
             atms.getLWOUTN(),
             atms.getVPR(),
             atms.getVPDD(),
             atms.getVPDN(),
             soil.getSNOWPACK(),
             atms.getPREC(),
             soil.getAVLH2O() );
  } */    
//  cout << "pesoil = " << veg.getPESOILW() << endl;

  veg.deltafo3( veg.cmnt,
                atms.getAOT40() );



  soil.updateHydrology(elev,
                     atms.getTAIR(),
                     atms.getPREVTAIR(),
                     atms.getPREV2TAIR(),
                     atms.getRAIN(),
                     veg.getPET(),
                     soil.getAVLH2O(),
                     pstate[I_RGRW],
                     pstate[I_SGRW],
                     irrgflag,
                     ag.irrigate,
                     pdm );

  soil.updateDOCLEACH( pstate[I_DOC],
                 pstate[I_SM] );

/*#ifdef OPENN
//  soil.setNINPUT((0.102 * (12.0*soil.getEET()/10.0)+ 0.524 )/(10.0*12.0));
  soil.setNINPUT(atms.getNDEP()/365000.);
  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (365.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*365.0));
  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (365.0*soil.getEET()/10.0)+ 0.524) /(10.0*365.0));
#endif */
/* if(veg.cmnt == 5)
{
  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (12.0*((soil.getEET()/soil.getREET())*90.0)/10.0)+ 0.524 )/(10.0*12.0));
  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (12.0*((soil.getEET()/soil.getREET())*90.0)/10.0)+ 0.524 )/(10.0*12.0));
}
if(veg.cmnt == 6)
{
  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (12.0*((soil.getEET()/(1.0+soil.yreet))*140.0/12.0)/10.0)+ 0.524 )/(10.0*12.0));
  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (12.0*((soil.getEET()/(1.0+soil.yreet))*140.0/12.0)/10.0)+ 0.524 )/(10.0*12.0));  

//  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (12.0*soil.getEET()/10.0)+ 0.524 ) /(10.0*12.0));
//  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (12.0*soil.getEET()/10.0)+ 0.524) /(10.0*12.0));

//  soil.setSONINP((veg.getNNF(veg.cmnt)*0.102 * (12.0*(soil.getEET()*627/787)/10.0)+ 0.524 ) /(10.0*12.0));
//  veg.setVEGNINP(((1.0-veg.getNNF(veg.cmnt))*0.102 * (12.0*(soil.getEET()*627/787)/10.0)+ 0.524) /(10.0*12.0));
} */

//  soil.setSONINP(soil.getSONINP()/2.0);
//  veg.setVEGNINP(veg.getVEGNINP()/2.0);
//  soil.setNINPUT((0.102 * (12.0*pstate[I_EET]/10.0)+ 0.524 )/(10.0*12.0));
//  cout << "EET = " << soil.getEET() << " " << pstate[I_EET] << endl;
//  if(veg.cmnt == 9) {
 
/*  cout << "evapcorb = " << soil.getRRUN() << " " << soil.getSRUN() << endl;

  wevap = pen.watev( atms.getNIRRN(),
                     atms.getLWOUTD(),    
                     atms.getTAIRD(),
                     atms.getVPDD(),
                     atms.getWS10(),
                     soil.getRRUN(),
                     soil.getSRUN(),
                     pdm );

  if( soil.getRRUN()+soil.getSRUN() > 0.0) 
  {
  rfrac = soil.getRRUN()/(soil.getRRUN()+soil.getSRUN());
 

  soil.setRRUN(soil.getRRUN() - (rfrac*wevap));
  soil.setSRUN(soil.getSRUN() - ((1-rfrac)*wevap));
  if(soil.getRRUN() <= 0.0) {soil.setRRUN( 0.0 );}
  if(soil.getSRUN() <= 0.0) {soil.setSRUN( 0.0 );}
if (rfrac*wevap < 0.0) {cout << "diag is negative " << endl;}
//  cout << "evapcora = " << wevap <<  " " << " " << rfrac << " " << soil.getRRUN() << " " << soil.getSRUN() << endl;
  }
//  }  */ 

  // Determine nitrogen losses from ecosystem

  if( 1 == avlnflag )
  {

#ifdef OPENN
  if(pstate[I_DOC] > 0.0)
  {
  soil.setLCHDON( soil.getLCHDOC() * pstate[I_DON]/pstate[I_DOC]);
  }
  else
  {
  soil.setLCHDON(ZERO);
  }
//cout << "lchdon = " << soil.getLCHDON() << " " << y[I_LCHDON] << " " << pstate[I_LCHDON] << endl;

    soil.updateNLosses( veg.cmnt,
//                        (atms.getRAIN() + soil.getSNOWINF() - soil.getEET() ),
                        (soil.getRPERC() + soil.getSPERC()),
                        pstate[I_AVLN],
                        pstate[I_SM] );

    soil.setLCHDIN(soil.getNLOST());

   if(initFlag == 0)
   {
   if(soil.getNLOST() + soil.getLCHDON() < soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP())
   {
      soil.setNLOST(soil.getNLOST() + (soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP()) - (soil.getNLOST() + soil.getLCHDON()));
   }
   if(soil.getNLOST() + soil.getLCHDON() > soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP())
   {
      soil.setNINPUT(soil.getNINPUT() + ((soil.getNLOST() + soil.getLCHDON()) - ((soil.getNINPUT() + soil.getSONINP() + veg.getVEGNINP()))));
   }

   }
   else
   {
      soil.setNLOST( soil.getNLOST() +  soil.getDENITR(veg.cmnt) * (0.01*microbe.getGMIN() + veg.getDENITR()) );
   }

#endif

//  soil.setNLOST(soil.getNLOST() + soil.getLCHDON() + ag.getCONVRTFLXN() + ag.getCROPRESIDUEFLXN());

  }
  else
  {
    // Do not allow changes in available nitrogen in soil
    //   (used during calibration process)

    soil.setNLOST( (soil.getNINPUT()
                   + microbe.getNETNMIN()
                       - veg.getNUPTAKE()) );
  }

//cout << "nlost = " << soil.getNLOST() << " " << y[I_NLST] << endl;
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" at end of natvegDynamics(), month = %2d ", pdm);
    refresh();
  #endif
  
  #ifdef DEBUG_XTEM
    printf(" at end of natvegDynamics() \n");
  #endif

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

#ifdef CALIBRATE_TEM

void Ttem45::pcdisplayMonth( const int& dyr, const int& pdm )
{
  int iout, iopt;

  double outevar[ESY];
  double outwvar[WSY];
  
  if ( 0 == topwind )
  {
    for( iopt = 0; iopt < ESY; ++iopt )
    {
      move(1,8+8*iopt);
      displayOptionalEflx( sey[iopt] );
    }

    for( iopt = 0; iopt < WSY; ++iopt )
    {
      move(1,8+8*ESY+8*iopt);
      displayOptionalWflx( swy[iopt] );
    }  
    
    topwind = 1;
  }

  // Assign values of optional variables to outflux? for later
  //   screen display

  for( iout = 0; iout < ESY; ++iout ) 
  { 
    outevar[iout] = getOptionalEflx( sey[iout], pdm );
    if(pdm == 0)
    {
      outevarsum[iout] = 0.0;
      outevaravg[iout] = 0.0;
      outevarmax[iout] = -1000000.0;
      outevarmin[iout] = 1000000.0;
    }
    
    outevarsum[iout] += outevar[iout];
    outevaravg[iout] += outevar[iout]/365.0;
    if( outevar[iout] > outevarmax[iout] ) { outevarmax[iout] = outevar[iout]; }
    if( outevar[iout] < outevarmin[iout] ) { outevarmin[iout] = outevar[iout]; }
  }
  
  for( iout = 0; iout < WSY; ++iout ) 
  { 
    outwvar[iout] = getOptionalWflx( swy[iout], pdm );
    if(pdm == 0)
    {
      outwvarsum[iout] = 0.0;
      outwvaravg[iout] = 0.0;
      outwvarmax[iout] = -1000000.0;
      outwvarmin[iout] = 1000000.0;
    }
    
    outwvarsum[iout] += outwvar[iout];
    outwvaravg[iout] += outwvar[iout]/365.0;
    if( outwvar[iout] > outwvarmax[iout] ) { outwvarmax[iout] = outwvar[iout]; }
    if( outwvar[iout] < outwvarmin[iout] ) { outwvarmin[iout] = outwvar[iout]; }
  }
  // Display monthly values for selected C and N pools and fluxes
  
  move(1,1);

  for( iopt = 0; iopt < ESY; ++iopt )
  {
    move(1,8+8*iopt);
    displayOptionalEflx( sey[iopt] );
  }

  for( iopt = 0; iopt < WSY; ++iopt )
  {
    move(1,8+8*ESY+8*iopt);
    displayOptionalWflx( swy[iopt] );
  }
  
  move(VPARAMS_ROW-1, 1);
  printw("CALIBRATION INPUT: ");
  
  move(VPARAMS_ROW, 1);
  printw("SETTINGS:          ");
  
  move(VPARAMS_ROW+1, 1);
  printw("AGFILE:            ");
  
  move(VPARAMS_ROW+2, 1);
  printw("DATFILE:           ");
  
  move(VPARAMS_ROW+3, 1);
  printw("VEGFILE:           ");
  
  move(VPARAMS_ROW+4, 1);
  printw("CONDUCTFILE:       ");
  
  move(VPARAMS_ROW+5, 1);
  printw("OPTIONAL C/N:      ");
  
  move(VPARAMS_ROW+6, 1);
  printw("OPTIONAL H2O:      ");
  
  move(pdm+2,1);
  
  printw( "%3d-%2d %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf %7.3lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf",
           dyr,
               (pdm+1),
                   outevar[0],
                          outevar[1],
                                 outevar[2],
                                        outevar[3],
                                               outevar[4],
                                                      outevar[5],
                                                             outevar[6],
                                                                    outevar[7],
                                                                           outevar[8],
                                                                                  outevar[9],
                                                                                         outwvar[0],
                                                                                                outwvar[1],
                                                                                                       outwvar[2],
                                                                                                              outwvar[3],
                                                                                                                     outwvar[4],
                                                                                                                            outwvar[5],
                                                                                                                                   outwvar[6],
                                                                                                                                          outwvar[7] );
                                                                                                                                          
  if( pdm == 11 )
  {
    move(15,1);
    printw( " SUM   %7.0lf %7.0lf %7.1lf %7.1lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.0lf %7.0lf %7.0lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf",
                   outevarsum[0],
                          outevarsum[1],
                                 outevarsum[2],
                                        outevarsum[3],
                                               outevarsum[4],
                                                      outevarsum[5],
                                                             outevarsum[6],
                                                                    outevarsum[7],
                                                                           outevarsum[8],
                                                                                  outevarsum[9],
                                                                                         outwvarsum[0],
                                                                                                outwvarsum[1],
                                                                                                       outwvarsum[2],
                                                                                                              outwvarsum[3],
                                                                                                                     outwvarsum[4],
                                                                                                                            outwvarsum[5],
                                                                                                                                   outwvarsum[6],
                                                                                                                                          outwvarsum[7] );
    
    move(16,1);
    printw( " MAX   %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf %7.3lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf",
                   outevarmax[0],
                          outevarmax[1],
                                 outevarmax[2],
                                        outevarmax[3],
                                               outevarmax[4],
                                                      outevarmax[5],
                                                             outevarmax[6],
                                                                    outevarmax[7],
                                                                           outevarmax[8],
                                                                                  outevarmax[9],
                                                                                         outwvarmax[0],
                                                                                                outwvarmax[1],
                                                                                                       outwvarmax[2],
                                                                                                              outwvarmax[3],
                                                                                                                     outwvarmax[4],
                                                                                                                            outwvarmax[5],
                                                                                                                                   outwvarmax[6],
                                                                                                                                          outwvarmax[7] );
    
    move(17,1);                                                                                                                                      
    printw( " AVG   %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf %7.3lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf",
                   outevaravg[0],
                          outevaravg[1],
                                 outevaravg[2],
                                        outevaravg[3],
                                               outevaravg[4],
                                                      outevaravg[5],
                                                             outevaravg[6],
                                                                    outevaravg[7],
                                                                           outevaravg[8],
                                                                                  outevaravg[9],
                                                                                         outwvaravg[0],
                                                                                                outwvaravg[1],
                                                                                                       outwvaravg[2],
                                                                                                              outwvaravg[3],
                                                                                                                     outwvaravg[4],
                                                                                                                            outwvaravg[5],
                                                                                                                                   outwvaravg[6],
                                                                                                                                          outwvaravg[7] );
   
    move(18,1);
    printw( " MIN   %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf %7.3lf %7.1lf %7.1lf %7.1lf %7.2lf %7.2lf %7.2lf %7.3lf %7.3lf",
                   outevarmin[0],
                          outevarmin[1],
                                 outevarmin[2],
                                        outevarmin[3],
                                               outevarmin[4],
                                                      outevarmin[5],
                                                             outevarmin[6],
                                                                    outevarmin[7],
                                                                           outevarmin[8],
                                                                                  outevarmin[9],
                                                                                         outwvarmin[0],
                                                                                                outwvarmin[1],
                                                                                                       outwvarmin[2],
                                                                                                              outwvarmin[3],
                                                                                                                     outwvarmin[4],
                                                                                                                            outwvarmin[5],
                                                                                                                                   outwvarmin[6],
                                                                                                                                          outwvarmin[7] );
  }
  refresh();
  
};

#endif

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************** */

void Ttem45::resetMonthlyELMNTFluxes( void )
{

  // Reset all monthly fluxes to zero

  atms.resetMonthlyFluxes();

  veg.resetMonthlyFluxes();

  microbe.resetMonthlyFluxes();

  soil.resetMonthlyFluxes();

  ag.resetMonthlyFluxes();

  // Carbon fluxes

  nep = ZERO;
  nce = ZERO;

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Ttem45::resetODEflux( void )
{
  int i;

  for ( i = MAXSTATE; i < NUMEQ; ++i ) { y[i] = ZERO; }

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

void Ttem45::resetYrFluxes( void )
{

  atms.resetYrFluxes();

  veg.resetYrFluxes();

  microbe.resetYrFluxes();

  soil.resetYrFluxes();

  ag.resetYrFluxes();

  yrtotalc = ZERO;

  // Annual carbon fluxes

  yrnep = ZERO;
  yrnce = ZERO;


};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Ttem45::rkbs( const int& numeq,
                  double pstate[],
                  double& pdt,
                  const int& pdm,
                  const double& nmax_grow )
{

// Runge-Kutta-Bogacki-Shampine method: compares 2nd and 3rd order solutions

  int i;
  double ptdt = ZERO;
  int calc_dz1_flag = 0;

  for( i = 0; i < numeq; ++i )
  {
    dum2[i] = dum3[i] = pstate[i];
    
    if( dz1[i] != dz4[i] ) { ++calc_dz1_flag; }
    // if previous step has been accepted, these arrays will be equal
    zstate[i] = ZERO;
    error[i] = ZERO;
    dz2[i] = dz3[i] = ZERO;
  }
  
  if( calc_dz1_flag != 0 ) { delta( pdm, nmax_grow, pstate, dz1 ); }

  //   since the 4th calculation from the last step should equal the first from the new step, 
  //   there is no need to recalculate
  ptdt = pdt * 0.5;   
  step( numeq, pstate, dz1, zstate, ptdt );
  
  delta( pdm, nmax_grow, zstate, dz2 );

  ptdt = pdt * 0.75;
  step( numeq, pstate, dz2, zstate, ptdt );
  
  delta( pdm, nmax_grow, zstate, dz3 );

  for( i = 0; i < numeq; ++i )
  {
    dp3[i] = 2.0/9.0 * dz1[i] + 1.0/3.0 * dz2[i] + 4.0/9.0 * dz3[i];
  }
  step( numeq, pstate, dp3, dum3, pdt );
  
  delta( pdm, nmax_grow, dum3, dz4 );

  for( i = 0; i < numeq; ++i )
  {
    dp2[i] = 7.0/24.0 * dz1[i] + 1.0/4.0 * dz2[i] + 1.0/3.0 * dz3[i] + 1.0/8.0 * dz4[i];
  }
  step( numeq, pstate, dp2, dum2, pdt );
  
  for ( i = 0; i < numeq; ++i )
  {
    error[i] = fabs( dum2[i] - dum3[i] );
  }

};

/***************************************************************
 ***************************************************************/


/* *************************************************************
************************************************************* */

void Ttem45::rkf( const int& numeq,
                  double pstate[],
                  double& pdt,
                  const int& pdm,
                  const double& nmax_grow )
{
// Runge-Kutta-Fehlberg method: compares 4th and 5th order solutions

  int i;
  double ptdt = ZERO;

  for( i = 0; i < numeq; ++i )
  {
    dum4[i] = dum5[i] = pstate[i];
    yprime[i] = rk45[i] = error[i] = ZERO;
  }

  ptdt = pdt * 0.25;

  delta( pdm,nmax_grow, dum4,f11 );
  step( numeq,yprime,f11,yprime,a1 );
  step( numeq,rk45,f11,rk45,b1 );
  step( numeq,dum4,f11,ydum,ptdt );
  delta( pdm,nmax_grow,ydum,f2 );

  for( i = 0; i < numeq; ++i )
  {
    f13[i] = a31*f11[i] + a32*f2[i];
  }

  step( numeq,dum4,f13,ydum,pdt );
  delta( pdm,nmax_grow,ydum,f3 );
  step( numeq,yprime,f3,yprime,a3 );
  step( numeq,rk45,f3,rk45,b3 );

  for( i = 0; i < numeq; ++i )
  {
    f14[i] = a41*f11[i] + a42*f2[i] + a43*f3[i];
  }

  step( numeq,dum4,f14,ydum,pdt );
  delta( pdm,nmax_grow,ydum,f4 );
  step( numeq,yprime,f4,yprime,a4 );
  step( numeq,rk45,f4,rk45,b4 );

  for( i = 0; i < numeq; ++i )
  {
    f15[i] = a51*f11[i] + a52*f2[i] + a53*f3[i] + a54*f4[i];
  }

  step( numeq,dum4,f15,ydum,pdt );
  delta( pdm,nmax_grow,ydum,f5 );
  step( numeq,yprime,f5,yprime,a5 );
  step( numeq,rk45,f5,rk45,b5 );

  for( i = 0; i < numeq; ++i )
  {
    f16[i] = b61*f11[i] + b62*f2[i] + b63*f3[i] + b64*f4[i] + b65*f5[i];
  }

  step( numeq,dum4,f16,ydum,pdt );
  delta( pdm,nmax_grow,ydum,f6 );
  step( numeq,rk45,f6,rk45,b6 );
  step( numeq,dum4,yprime,dum4,pdt );
  step( numeq,dum5,rk45,dum5,pdt );

  for ( i = 0; i < numeq; ++i )
  {
    error[i] = fabs( dum4[i] - dum5[i] );
  }

};

/***************************************************************
 ***************************************************************/


/* *************************************************************
************************************************************** */

void Ttem45::setELMNTecd( const int& pdcmnt,
                          const double& psiplusc )
{
  // Initialize TEM parameters dependent upon a grid cell's
  //   soil texture

  veg.resetEcds( pdcmnt, psiplusc );

  microbe.resetEcds( pdcmnt, psiplusc );

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Ttem45::setPrevState( void )
{
  for( int i = 0; i < MAXSTATE; ++i ) { prevy[i] = y[i]; }

};

/* *************************************************************
************************************************************* */


/***************************************************************
 ***************************************************************/

void Ttem45::step( const int& numeq,
                   double pstate[],
                   double pdstate[],
                   double ptstate[],
	               double& pdt )
{

  for( int i = 0; i < numeq; ++i )
  {
    ptstate[i] = pstate[i] + (pdt * pdstate[i]);
  }

};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************** */

int Ttem45::stepmonth( const int& pdyr,
                       const int& pdm,
                       int& intflag,
                       const double& ptol,
                       const int& CYCLE,
                       const int& ichrt )
{

//  int mintflag, i;
  int mintflag;
  double avgfac,avgfacsh;
  double mdemandc, mdemandn;
  double cabove, nabove, frcab;
//  double favg;
  double timber, dleaf, dwood;
  double rhmoist,dq10;
  double cwdloss=0.0;

  ag.setSLASHC(0.0);
  ag.setSLASHN(0.0);

//cout << "vegc = " << veg.getVEGC() << " " << pdyr << " " << pdm << endl;
 //cout << "entering stepmonth " << pdyr << " " << pdm << endl;

  avgfac = 1.0/(365.0*(veg.getTAULEAF( veg.cmnt ) + veg.getTAUSAPWOOD( veg.cmnt ) 
           + veg.getTAUROOT( veg.cmnt ))/3.0);
  avgfac = exp(-avgfac);

  avgfacsh = 1.0/(0.5*(veg.getTAULEAF( veg.cmnt ) + veg.getTAUSAPWOOD( veg.cmnt )
           + veg.getTAUROOT( veg.cmnt ))/3.0);
  avgfacsh = exp(-avgfacsh);
  if(pdm == 0) { mxeet = 0.0; }
 
  if((pdyr == 1 || pdyr == 0) && pdm == 0)
  {
  ag.setSTANDDEADC(0.0);
  ag.setSTANDDEADN(0.0);
  ag.setVOLAC(0.0);
  ag.setVOLAN(0.0);
  fireoccur = 0;
  stormoccur = 0;
 }

  // avgfac = exp(-1 / average lifetime of leaves, stem, and roots, in months)
  
 // Reset all monthly fluxes to zero
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" entering resetMonthlyELMNTFluxes() ");
    refresh();
  #endif

  #ifdef DEBUG_XTEM
	    printf(" beginning stepmonth, month = %2d, year = %4d \n", pdm+1, pdyr );
	    printf(" cmnt = %2d, yrnep = %8.21f, tauavg = %4.11f \n", veg.cmnt, yrnep, tauavg );
	    printf(" labilec = %8.21f, yrnpp = %8.21f \n", y[I_LABILEC] , veg.yrnpp );
  #endif

	  resetMonthlyELMNTFluxes();

	  // Reset ODE state variables representing monthly
	  //   fluxes to zero
	  #ifdef DEBUG_CTEM
	    move(DEBUG_ROW,1);
	    printw(" entering resetODEflux() ");
	    refresh();
	  #endif
	  resetODEflux();

	  // If 12 months have passed since disturbance, reset
	  //   all immediate fluxes associated with disturbance
	  //   (i.e., slash, convrtflx, nretent) to zero

	  if ( CYCLE == distmnthcnt )
	  {
	    distmnthcnt = 0;
	    ag.resetMonthlyDisturbFluxes();
	  }

  if (fireoccur == 1 || stormoccur == 1)
  {
    ag.resetMonthlyDisturbFluxes();
  }
    ag.setCONVRTFLXC(0.0);
    fireoccur = 0;
    stormoccur = 0;


	  // Count the number of months since disturbance

	  if( distmnthcnt > 0 ) { ++distmnthcnt; }

	  // If (FRI times 12 months have passed since a fire disturbance,
	  //    reset ag.firendep to zero

	  if( (ag.getFRI() * 12) == firemnthcnt )
	  {
	    firemnthcnt = 0;
	    //ag.setFIRENDEP( ZERO );
	  }

	  if( firemnthcnt > 0 ) { ++firemnthcnt; }


	//  if( 2 == ag.state && 2 == distmnthcnt)
	//  {
	//    prevy[I_AVLN] = y[I_AVLN] = y[I_AVLN]/10.0; 
	//    prevy[I_SOLN] = y[I_SOLN] = y[I_SOLN]/10.0; 
	//  }

	  if( 0 == pdm )
	  {
	    if( 0 == pdyr )
	    {
	      microbe.setKD( microbe.getKDC() );
	      ag.setKD( microbe.getKD() );
	      ag.setNATSOIL( y[I_SOLC] );
	    }
	    else
	    {
	      if( 0 == ag.state && 0 == ag.prvstate )
	      {
		microbe.setKD( microbe.yrkd( nfeed,
					     veg.yrltrc,
					     veg.yrltrn,
					     veg.cmnt ) );

		ag.setKD( microbe.getKD() );
		ag.setNATSOIL( y[I_SOLC] );
	      }
	      else
	      {
		if( y[I_SOLC] < ag.getNATSOIL() )
		{
		  microbe.setKD( (ag.getKD()
				 * y[I_SOLC]
				 / ag.getNATSOIL()) );
		}
		else { microbe.setKD( ag.getKD() ); }
	      }
	    }
	  }

	  // Implement disturbance effects

 if( disturbflag ==  1 && pdm == (disturbmonth-1)) // agriculture
  {
    distmnthcnt = 1;

//  set temperate and boreal forest values from McGuire et al. 01
 if(veg.cmnt == 4 || veg.cmnt == 5 || veg.cmnt == 6 || veg.cmnt == 11 || veg.cmnt == 12)
 {
   ag.setVCONVERT(0.6);
   ag.setPROD10PAR(0.3);
   ag.setPROD100PAR(0.1);
 }
// set tropical forest values
  if(veg.cmnt == 10)
  {
   ag.setVCONVERT(0.6);
   ag.setPROD10PAR(0.4);
   ag.setPROD100PAR(0.0);
  }
// set grassland/tundra values
   if(veg.cmnt == 2 || veg.cmnt == 7 || veg.cmnt == 8)
  {
   ag.setVCONVERT(1.0);
   ag.setPROD10PAR(0.0);
   ag.setPROD100PAR(0.0);
  }
// set shrubland/woodland/savanna values
  if(veg.cmnt == 9 || veg.cmnt == 13)
  {
   ag.setVCONVERT(0.8);
   ag.setPROD10PAR(0.2);
   ag.setPROD100PAR(0.0);
  }

  ag.setSCONVERT(0.0);
  cwdloss = 1.0;

    ag.setNATSEEDC (y[I_SEEDC] );
    ag.setNATSEEDSTON (y[I_SEEDN] );
    ag.conversion( veg.cmnt,
        y[I_ROOTC],
        y[I_ROOTN],
        y[I_LEAFC]/ag.getVCONVERT()+y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_LABILEC],
        y[I_LEAFN]/ag.getVCONVERT()+y[I_SAPWOODN]+y[I_HEARTWOODN],
        y[I_LABILEN],
        y[I_SOLC],
        y[I_SOLN],
        cwdloss,
        CYCLE );

    ag.createWoodProducts( pdyr,
                           (y[I_HEARTWOODC]+y[I_SAPWOODC]+ y[I_LABILEC]),
                           (y[I_HEARTWOODN]+y[I_SAPWOODN]+ y[I_LABILEN]) );
    ag.standingdead( veg.cmnt,
                   (y[I_ROOTC]),
                   (y[I_ROOTN]),
                   0.0,
                   0.0,
                   cwdloss );

    prevy[I_LEAFC] = y[I_LEAFC] = ZERO;
    prevy[I_LEAFN] = y[I_LEAFN] = ZERO;
    prevy[I_ROOTC] = y[I_ROOTC] = ZERO;
    prevy[I_ROOTN] = y[I_ROOTN] = ZERO;
    prevy[I_SEEDC] = y[I_SEEDC] = ZERO;
    prevy[I_SEEDN] = y[I_SEEDN] = ZERO;
    prevy[I_SAPWOODC] = y[I_SAPWOODC] = ZERO;
    prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = ZERO;
    prevy[I_LABILEC] = y[I_LABILEC] = ZERO;
    prevy[I_SAPWOODN] = y[I_SAPWOODN] = ZERO;
    prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = ZERO;
    prevy[I_LABILEN] = y[I_LABILEN] = ZERO;
  }

else if ( disturbflag ==  2 && pdm == (disturbmonth-1)) // timber harvest clearcut
  {
    distmnthcnt = 1;
    timber = 1.0;

//
//  Timber harvest product values from conservatree.org based on Weyerhauser
//
  ag.setVCONVERT(0.21);
  ag.setSCONVERT(0.0);
  ag.setPROD10PAR(0.32);
  ag.setPROD100PAR(0.47);
  cwdloss = 0.0;

    ag.setNATSEEDC (y[I_SEEDC] );
    ag.setNATSEEDSTON (y[I_SEEDN] );
    ag.createWoodProducts( pdyr,
                           timber*(y[I_HEARTWOODC]+y[I_SAPWOODC]+ y[I_LABILEC]),
                           timber*(y[I_HEARTWOODN]+y[I_SAPWOODN]+ y[I_LABILEN]) );

    ag.conversion( veg.cmnt,
        timber*(y[I_LEAFC]+y[I_ROOTC]),
        timber*(y[I_LEAFN]+y[I_ROOTN]),
        timber*(y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_LABILEC]),
        timber*(y[I_SAPWOODN]+y[I_HEARTWOODN]),
        timber*y[I_LABILEN],
        timber*y[I_SOLC],
        timber*y[I_SOLN],
        cwdloss,
        CYCLE );

    prevy[I_LEAFC] = y[I_LEAFC] = (1. - timber)*y[I_LEAFC];
    prevy[I_LEAFN] = y[I_LEAFN] = (1. -  timber)*y[I_LEAFN];
    prevy[I_ROOTC] = y[I_ROOTC] = (1. - timber)*y[I_ROOTC];
    prevy[I_ROOTN] = y[I_ROOTN] = (1. -  timber)*y[I_ROOTN];
    prevy[I_LABILEC] = y[I_LABILEC] = (1. - timber)* y[I_LABILEC];
    prevy[I_LABILEN] = y[I_LABILEN] = (1. -  timber)*y[I_LABILEN];
    prevy[I_SAPWOODC] = y[I_SAPWOODC] = (1. - timber)*y[I_SAPWOODC];
    prevy[I_SAPWOODN] = y[I_SAPWOODN] = (1. - timber)*y[I_SAPWOODN];
    prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = (1. - timber)*y[I_HEARTWOODC];
    prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = (1. - timber)*y[I_HEARTWOODN];
   }

else if ( disturbflag ==  3 && pdm == (disturbmonth-1)) // fire, mid intensity
  {
    ag.setVCONVERT(0.0124);
    ag.setSCONVERT(0.0);
    fireoccur = 1;
    cwdloss = 0.24;
       distmnthcnt = 1;
       firemnthcnt = 1;
       dleaf = 0.5;
       dwood = 0.5;
       ag.setNATSEEDC (y[I_SEEDC] );
       ag.setNATSEEDSTON (y[I_SEEDN] );

       ag.conversion( veg.cmnt,
              dwood*y[I_ROOTC],
              dwood*y[I_ROOTN],
              dwood*(y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_LABILEC]) + y[I_LEAFC]*(dleaf/ag.getVCONVERT()),
              dwood*(y[I_SAPWOODN]+y[I_HEARTWOODN]) + y[I_LEAFN]*(dleaf/ag.getVCONVERT()),
              y[I_LABILEN],
              y[I_SOLC],
              y[I_SOLN],
              cwdloss,
              1 );
//
////  overwrite the slash with same as above but add to standing dead
////
      ag.standingdead( veg.cmnt,
              dwood*y[I_ROOTC],
              dwood*y[I_ROOTN],
              (1.0-ag.getVCONVERT())*(dwood*(y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_LABILEC])),
              (1.0-ag.getVCONVERT())*(dwood*(y[I_SAPWOODN]+y[I_HEARTWOODN]+y[I_LABILEN])),
              cwdloss);

      prevy[I_LABILEC] = y[I_LABILEC] = (1. - dwood)*y[I_LABILEC];
      prevy[I_LABILEN] = y[I_LABILEN] = (1. - dwood) * y[I_LABILEN];
      prevy[I_LEAFC] = y[I_LEAFC] =  (1. - dleaf)*y[I_LEAFC];
      prevy[I_LEAFN] = y[I_LEAFN] =  (1. - dleaf)*y[I_LEAFN];
      prevy[I_SAPWOODC] = y[I_SAPWOODC] = (1. - dwood)*y[I_SAPWOODC];
      prevy[I_SAPWOODN] = y[I_SAPWOODN] = (1. - dwood)*y[I_SAPWOODN];
      prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = (1. - dwood)*y[I_HEARTWOODC];
      prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = (1. - dwood)*y[I_HEARTWOODN];
      prevy[I_ROOTC] = y[I_ROOTC] = (1. - dwood)*y[I_ROOTC];
      prevy[I_ROOTN] = y[I_ROOTN] = (1. - dwood)*y[I_ROOTN ];
    }
else if ( disturbflag ==  4 && pdm == (disturbmonth-1)) //hurricane-strength storm
  {
    ag.setVCONVERT(0.0);
    ag.setSCONVERT(0.0);
    stormoccur = 1;
    cwdloss = 0.0;
       distmnthcnt = 1;
       ag.setNATSEEDC (y[I_SEEDC] );
       ag.setNATSEEDSTON (y[I_SEEDN] );
       ag.standingdead( veg.cmnt,
                       0.2*y[I_LEAFC]+0.11*y[I_ROOTC],
                       0.2*y[I_LEAFN]+0.11*y[I_ROOTN],
                       0.11*(y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_LABILEC]),
                       0.11*(y[I_SAPWOODN]+y[I_HEARTWOODN]+y[I_LABILEN]),
                       cwdloss );


      prevy[I_LABILEC] = y[I_LABILEC] = (1. - 0.11)*y[I_LABILEC];   //destruction values from Gresham et al.
      prevy[I_LABILEN] = y[I_LABILEN] = (1. - 0.11)*y[I_LABILEN];
      prevy[I_LEAFC] = y[I_LEAFC] = (1. - 0.2)*y[I_LEAFC];
      prevy[I_LEAFN] = y[I_LEAFN] = (1. - 0.2)*y[I_LEAFN];
      prevy[I_SAPWOODC] = y[I_SAPWOODC] = (1. - 0.11)*y[I_SAPWOODC];
      prevy[I_SAPWOODN] = y[I_SAPWOODN] = (1. - 0.11)*y[I_SAPWOODN];
      prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = (1. - 0.11)*y[I_HEARTWOODC];
      prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = (1. - 0.11)*y[I_HEARTWOODN];
      prevy[I_ROOTC] = y[I_ROOTC] = (1. - 0.11)*y[I_ROOTC];
      prevy[I_ROOTN] = y[I_ROOTN] = (1. - 0.11)*y[I_ROOTN];
  }

  dq10 = microbe.setDQ10LT( veg.cmnt,
                       atms.getTAIR(),
                       veg.getTOPTMIC() );

  rhmoist = microbe.setRHMOIST( veg.cmnt, soil.getPCTFLDCAP(), soil.getPCTWILTPT(), soil.getPCTPOR(), y[I_VSM], moistlim );
  ag.updatestanddead(pdyr,rhmoist,dq10,microbe.getCNSOIL( veg.cmnt ));

	    if ( 1 == disturbflag && ag.state >= 1 )
	    {
	      // Update rooting depth to be appropriate to crops

	    y[I_VSM] = soil.updateRootZ( ag.cmnt,
					y[I_SM],
					y[I_ROOTC] );



	      // Establish crops

	      prevy[I_LABILEC] = y[I_LABILEC] = ag.getCROPSEEDC( ag.cmnt );
	      prevy[I_LABILEN] = y[I_LABILEN] = ag.getCROPSEEDSTON( ag.cmnt );

	      prevy[I_LEAFC] = y[I_LEAFC] = ZERO;
	      prevy[I_LEAFN] = y[I_LEAFN] = ZERO;
	      prevy[I_SAPWOODC] = y[I_SAPWOODC] = ZERO;
	      prevy[I_SAPWOODN] = y[I_SAPWOODN] = ZERO;
	      prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = ZERO;
	      prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = ZERO;
	      prevy[I_ROOTC] = y[I_ROOTC] = ZERO;
	      prevy[I_ROOTN] = y[I_ROOTN] = ZERO;
	      prevy[I_SEEDC] = y[I_SEEDC] = ZERO;
	      prevy[I_SEEDN] = y[I_SEEDN] = ZERO;

	      // Update soil texture-dependent vegetation parameters for crops

	      veg.resetEcds( ag.cmnt, soil.getPSIPLUSC() );

	      // Update other adaptive parameters

	      atms.yrpet = 1.0;
	      soil.yreet = 1.0;

	      veg.setTOPT( ag.getCROPTOPT() );

	      ag.setPRVCROPNPP( ZERO );
	    }
	//  else { ag.setNoWoodProducts( pdyr ); }

	  // Revert to natural vegetation after cropland abandonment

	  if( 0 == ag.state && 1 == ag.prvstate )
	  {
	    // Update rooting depth to be appropriate to natural vegetation

	   y[I_VSM] = soil.updateRootZ( veg.cmnt,
					y[I_SM],
					y[I_ROOTC] );




	    // Establish natural vegetation

	    prevy[I_LABILEC] = y[I_LABILEC] = ag.getNATSEEDC();
	    prevy[I_LABILEN] = y[I_LABILEN] = ag.getNATSEEDSTON();

	    prevy[I_LEAFC] = y[I_LEAFC] = ZERO;
	    prevy[I_LEAFN] = y[I_LEAFN] = ZERO;
	    prevy[I_SAPWOODC] = y[I_SAPWOODC] = ZERO;
	    prevy[I_SAPWOODN] = y[I_SAPWOODN] = ZERO;
	    prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = ZERO;
	    prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = ZERO;
	    prevy[I_ROOTC] = y[I_ROOTC] = ZERO;
	    prevy[I_ROOTN] = y[I_ROOTN] = ZERO;
		prevy[I_SEEDC] = y[I_SEEDC] = ZERO;
	    prevy[I_SEEDN] = y[I_SEEDN] = ZERO;

	    // Update soil texture-dependent vegetation parameters
	    //   for natural vegetation

	    veg.resetEcds( veg.cmnt, soil.getPSIPLUSC() );


	    // Update other adaptive parameters

	    veg.setTOPT( ag.getNATTOPT() );
	    ag.setPRVCROPNPP( ZERO );
	  }

	  if( 0 == pdyr && 0 == pdm )
	  {
	    //initialize rootz if first year, first month
	    y[I_VSM] = soil.updateRootZ( veg.cmnt,
				       y[I_SM],
				       100.0 );
	    soil.setAVLH2O(y[I_SM] - soil.getWILTPT());
	    if(soil.getAVLH2O() < ZERO) { soil.setAVLH2O( ZERO );}
	  }
	  else
	  {
	    //  update awcapmm every month
	    y[I_VSM] = soil.updateRootZ( veg.cmnt,
				       y[I_SM],
				       y[I_ROOTC] );
	  }


	  // Get environmental conditions for month "dm"
	  #ifdef DEBUG_CTEM
	    move(DEBUG_ROW,1);
	    printw(" entering getenviron() ");
	    refresh();
	  #endif
	  getenviron();


	  // Determine effect of air temperature on GPP (i.e. temp)

	  if( 1 == ag.state )
	  {
	    veg.setTEMP( ag.cmnt, atms.getTAIRD() );
	    veg.phenology(ag.cmnt,
			  pdm,
			  atms.getTAIR(),
			  atms.getTAIRD(),
			  atms.getTAIRN(),
			  soil.getAVLH2O(),
			  soil.getAWCAPMM(),
			  atms.getPREC(),
			  atms.getDAYL());


	//    veg.setRESPQ10( ag.cmnt, atms.getTAIR() );
	  }
	  else
	  {
	    veg.setTEMP( veg.cmnt, atms.getTAIRD() );
	//    cout << "TEMP = " << veg.cmnt << " " << atms.getTAIRD() << "  " << atms.getTAIR() << " " << veg.getTEMP() << endl;
	    veg.phenology(veg.cmnt,
			  pdm,
			  atms.getTAIR(),
			  atms.getTAIRD(),
			  atms.getTAIRN(),
			  soil.getAVLH2O(),
			  soil.getAWCAPMM(),
			  atms.getPREC(),
			  atms.getDAYL());

	//    veg.setRESPQ10( veg.cmnt, atms.getTAIR() );
	  }

	  // Determine effect of temperature on decomposition
	  //   (i.e. dq10)

	//  microbe.setDQ10( veg.cmnt,
	//                   atms.getTAIR(),
	//                   soil.getSNOWPACK() );

	  microbe.setDQ10LT( veg.cmnt,
			       atms.getTAIR(),
                               veg.getTOPTMIC() );

	  // Update growing degree days (GDD)

	  if( (atms.getTAIR() >= ag.getGDDMIN(ag.cmnt)) 
	    && (atms.getTAIRN() > ag.getTKILL(ag.cmnt)) )
	  {
	//    ag.setGROWDD( ag.getGROWDD()
	//                  + ((atms.getTAIR() - ag.getGDDMIN(ag.cmnt))
	//                  * atms.getNDAYS( pdm )) );
	    ag.setGROWDD( ag.getGROWDD()
			  + ((atms.getTAIR() - ag.getGDDMIN(ag.cmnt))));
	  }
	  else
	  {
	    // If "cold snap" (i.e. TAIRN < TKILL) hits after crops
	    //   begin to grow, crops are assumed to die and resulting
	    //   detritus is added to soil organic carbon and nitrogen

	    if( 1 == ag.state
		&& ag.getGROWDD() > ag.getGDDSEED(ag.cmnt)
		&& atms.getTAIRN() <= ag.getTKILL(ag.cmnt) )
	    {
	      #ifdef DEBUG_CTEM
		move(DEBUG_ROW,1);
		printw(" entering frostdamage() ");
		refresh();
	      #endif
	      ag.frostDamage( y[I_LEAFC] + y[I_SAPWOODC] + y[I_HEARTWOODC] + y[I_ROOTC] + y[I_SEEDC] + y[I_LABILEC] - ag.getCROPSEEDC( ag.cmnt ), 
			      y[I_LEAFN] + y[I_SAPWOODN] + y[I_HEARTWOODN] + y[I_ROOTN] + y[I_SEEDN] + y[I_LABILEN] - ag.getCROPSEEDSTON( ag.cmnt ) );
	      
	      y[I_LABILEC] = ag.getCROPSEEDC( ag.cmnt );
	      prevy[I_LABILEC] = ag.getCROPSEEDC( ag.cmnt );
	      y[I_LABILEN] = ag.getCROPSEEDSTON( ag.cmnt );
	      prevy[I_LABILEN] = ag.getCROPSEEDSTON( ag.cmnt );

	      prevy[I_LEAFC] = y[I_LEAFC] = ZERO;
	      prevy[I_LEAFN] = y[I_LEAFN] = ZERO;
	      prevy[I_SAPWOODC] = y[I_SAPWOODC] = ZERO;
	      prevy[I_SAPWOODN] = y[I_SAPWOODN] = ZERO;
	      prevy[I_HEARTWOODC] = y[I_HEARTWOODC] = ZERO;
	      prevy[I_HEARTWOODN] = y[I_HEARTWOODN] = ZERO;
	      prevy[I_ROOTC] = y[I_ROOTC] = ZERO;
	      prevy[I_ROOTN] = y[I_ROOTN] = ZERO;
		  prevy[I_SEEDC] = y[I_SEEDC] = ZERO;
	      prevy[I_SEEDN] = y[I_SEEDN] = ZERO;
	      y[I_SOLC] += ag.getSTUBBLEC();
	      prevy[I_SOLC] = y[I_SOLC];
	      y[I_SOLN] += ag.getSTUBBLEN();
	      prevy[I_SOLN] = y[I_SOLN];
	    }

	    if(0 == ag.getISPERENNIAL( ag.cmnt )
	    && atms.getTAIRN() <= ag.getTKILL(ag.cmnt))
	    {
	      ag.setGROWDD( ZERO );
	    }
	  }

	//   set irrigation

	 if( ag.irrg1950flag == 1 && 3 == ag.state & ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt) && ag.getGROWDD() <=ag.getGDDHARVST(ag.cmnt)) {
	  if(atms.getPREC() < 200)
	   {
	    ag.irrigate = 200.0-atms.getPREC();
	//    cout << "prec,irrigate = " << pdm << " " << pdyr << " " << atms.getPREC() << " " << ag.irrigate << endl;
	   }
	  }

	  // Run TEM for a monthly time step

	  veg.setERRCNT( ZERO );
	  #ifdef DEBUG_CTEM
	    move(DEBUG_ROW,1);
	    printw(" entering adapt( NUMEQ, y, ptol, pdm ) ");
	    refresh();
	  #endif
	  
	  #ifdef DEBUG_XTEM
	    printf(" entering adaptive integrator \n");
	    for( i = 0; i < NUMEQ; ++i ) { printf("y[%2d] = %4.1lf \n", i, y[i]); }
	  #endif
	 
//    if(pdyr >= 97 && veg.cmnt == 6 && initFlag == 1)
    if(pdyr == 0 and pdm == 0) { nmax_grow[ichrt] = veg.getNMAX1B( veg.cmnt );}   
    if(veg.cmnt == 6 && initFlag == 1)
    {
//        nmax_grow[ichrt] += 12.9/365.0;
        nmax_grow[ichrt] = veg.getNMAX1B( veg.cmnt );
    }
    else
    {
    nmax_grow[ichrt] = veg.getNMAX1B( veg.cmnt );
    }

//cout << "nmax_grow = " << veg.cmnt << "  " << nmax_grow[ichrt] << " " << pdyr << " " << pdm << " " << initFlag << endl;

    #ifdef CALIBRATE_TEM
        nmax_grow[ichrt] = veg.getNMAX();
    #endif

	  
	  mintflag = adapt( NUMEQ, y, ptol, pdm, nmax_grow[ichrt] );

	  if( 1 == mintflag ) { intflag = 1; }

	  if ( blackhol != 0 )
	  {
	    if( 0 == initFlag || pdyr < 0 || pdyr >= MAXRTIME ) { qualcon[0] = 10; }
	    else { qualcon[pdyr] += 10; }
	  }

	  // Check mass balance
	  #ifdef DEBUG_CTEM
	    move(DEBUG_ROW,1);
	    printw(" entering massbal( y, prevy ) ");
	    refresh();
	  #endif
	  //massbal( y, prevy );
	  massbal();
//	cout << "year, day = " << pdyr << " " << pdm << endl;
	//cout << "ROOTN = " << y[I_ROOTN] << " " << veg.getALLOCRN() << " " << veg.getNRESORBR() << " " << veg.getLTRRN() << endl;

	//cout << "diag = " << pdyr << " " << pdm << " " << soil.getAVLH2O() << " " <<  soil.getAWCAPMM() << " " << veg.getPESOILW() << " " <<  atms.getPREC() << endl;

	//   if(pdm == 11)
	//   {

	//   vegceq[pdyr] = veg.getVEGC();
//   vegceq[pdyr] = y[I_LEAFC]+y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_ROOTC]+y[I_LABILEC];
//   soilceq[pdyr] = y[I_SOLC];
//   vegneq[pdyr] = veg.getVEGN();
//   vegneq[pdyr] = y[I_LEAFN]+y[I_SAPWOODN]+y[I_HEARTWOODN]+y[I_ROOTN]+y[I_LABILEN];
//   soilneq[pdyr] = y[I_SOLN];
//   cout << "diag = " << pdyr << " " << vegceq[pdyr] << " " << soilceq[pdyr] << " " << vegneq[pdyr] << " " << soilneq[pdyr] << endl;

//   }
//cout << "time = " << pdyr << " " << pdm << endl;
//cout << "water = " << pdyr << " " << pdm << " " << y[I_SM] <<  " " << soil.getAVLH2O() << " " << soil.getSNOWINF() << " " << atms.getRAIN() << " " << y[I_EET] << " " << y[I_RPERC] << " " << y[I_SPERC] << " " << soil.getSNOWINF()+atms.getRAIN()-y[I_EET]-y[I_RPERC]-y[I_SPERC] << " " << y[I_SM] - prevy[I_SM] << " " << y[I_PECAN] << " " << y[I_PESOIL] << " " << y[I_VSM] << endl;

//cout << "leafc = " << y[I_LEAFC] << " " << y[I_ALLOCLC] << " " << y[I_RMLEAF] << " " << y[I_LTRLC] << " " << y[I_LEAFC] - prevy[I_LEAFC] << endl;

//cout <<"vegc = " << y[I_SAPWOODC] << " " << y[I_HEARTWOODC] << " " << y[I_ROOTC] << " " << y[I_SEEDC] << " " << y[I_LABILEC] << " " << y[I_INGPP] << " " << y[I_GPP] << endl;

  wevapd[pdm] = pen.watev( atms.getNIRRN(),
                     atms.getLWOUTD(),
                     atms.getTAIRD(),
                     atms.getVPDD(),
                     atms.getWS10(),
                     soil.getRRUN(),
                     soil.getSRUN(),
                     pdm );

  wevapn[pdm] = pen.watev( 0.0,
                     atms.getLWOUTN(),
                     atms.getTAIRN(),
                     atms.getVPDN(),
                     atms.getWS10(),
                     soil.getRRUN(),
                     soil.getSRUN(),
                     pdm );

  if( soil.getRRUN()+soil.getSRUN() > 0.0)
  {
  rfrac[pdm] = soil.getRRUN()/(soil.getRRUN()+soil.getSRUN());


//  soil.setRRUN(soil.getRRUN() - (rfrac*wevap));
//  soil.setSRUN(soil.getSRUN() - ((1-rfrac)*wevap));
//  if(soil.getRRUN() <= 0.0) {soil.setRRUN( 0.0 );}
//  if(soil.getSRUN() <= 0.0) {soil.setSRUN( 0.0 );}
  } 


  // Determine vegetation total carbon and nitrogen stocks

  veg.setVEGC( y[I_LEAFC] + y[I_SAPWOODC] + y[I_HEARTWOODC] + y[I_ROOTC] + y[I_SEEDC] + y[I_LABILEC] );

//cout << "vegc = " <<  y[I_LEAFC] << " " <<  y[I_SAPWOODC] << " " << y[I_HEARTWOODC] << " " << y[I_ROOTC] << " " <<  y[I_SEEDC] << " " << y[I_LABILEC] << endl;
  veg.setSTRN( y[I_LEAFN] + y[I_SAPWOODN] + y[I_HEARTWOODN] + y[I_ROOTN] + y[I_SEEDN] );

  veg.setVEGN( (veg.getSTRN() + y[I_LABILEN]) );


  // Determine water yield (soil.h2oyld)

  soil.setH2OYLD( (y[I_RRUN] + y[I_SRUN]) );


  // Determine Net Ecosystem Production (nep)

//  nep = y[I_NPP] - y[I_RH];
  nep = y[I_NPP] - y[I_RH] - ag.getVOLAC();

  // Detemine total monthly N inputs to ecosystem

//  soil.setNINPUT( (soil.getNINPUT() + ag.getNRETENT() + y[I_AGFRTN]) + y[I_NFIXN] + y[I_NFIXS] );
/*#ifdef OPENN
  soil.setNINPUT( soil.getNINPUT() + ag.getNRETENT() + y[I_NFIXN] + y[I_NFIXS] );
#else
   soil.setNINPUT( (ag.getNRETENT() + y[I_AGFRTN]) );
#endif */
//  soil.setNINPUT( y[I_NINP] + y[I_NFIXN] + y[I_NFIXS] );
//  soil.setNLOST( (y[I_NLST]
//                  + y[I_LCHDON]
//                  + ag.getCONVRTFLXN()
//                  + ag.getCROPRESIDUEFLXN()) );


  // Determine fluxes from crop residues

//  cout << "diag = " <<  ag.getCROPRESIDUEFLXC() << endl;
  ag.updateCropResidueFluxes( CYCLE );


  // Determine fluxes from decay of products

  ag.decayProducts( CYCLE );


  // Harvest crops after a specific number of growing degree
  //   days; reset growing degree days to zero if crops were
  // harvested this month
//   cout << "harvest = " << ag.getGROWDD() << " " << ag.getGDDHARVST(ag.cmnt) << " " << y[I_NPP] << " " << y[I_SEEDC] << " " << y[I_ALLOCSEEDC] << " " << y[I_RMSEED] << " " << y[I_LTRSEEDC] << endl;
//cout << "gdd " << ag.getGROWDD() << endl;
  if(pdm == 0) {harcnt = 0;}
  if( (1 == ag.state) && ((ag.getGROWDD() >= ag.getGDDHARVST(ag.cmnt)) || (harcnt == 0 && pdm == 9))    && (0 == ag.getFROSTFLAG() ) )
//  if( (1 == ag.state) && (ag.getGROWDD() >= ag.getGDDHARVST(ag.cmnt)) && (0 == ag.getFROSTFLAG() ) )
  {
//    ag.harvest( pdm, y[I_SEEDC], y[I_SEEDN], veg.getVEGC() - ag.getCROPSEEDC( ag.cmnt ), veg.getVEGN() - ag.getCROPSEEDSTON( ag.cmnt ) );
    ag.harvest( pdm, y[I_SEEDC], y[I_SEEDN], veg.getVEGC(), veg.getVEGN());
    if( 0 == ag.getISPERENNIAL( ag.cmnt )) // harvesting an annual kills the crop
    {
      y[I_LABILEC] = ag.getCROPSEEDC( ag.cmnt );
      y[I_LABILEN] = ag.getCROPSEEDSTON( ag.cmnt );

      y[I_LEAFC] = ZERO;
      y[I_LEAFN] = ZERO;
      y[I_SAPWOODC] = ZERO;
      y[I_SAPWOODN] = ZERO;
      y[I_HEARTWOODC] = ZERO;
      y[I_HEARTWOODN] = ZERO;
      y[I_ROOTC] = ZERO;
      y[I_ROOTN] = ZERO;
      y[I_SEEDC] = ZERO;
      y[I_SEEDN] = ZERO;
    }
    else // harvesting a perennial just removes the "seed" pool
    {
      y[I_SEEDC] = ZERO;
      y[I_SEEDN] = ZERO;
    }
    y[I_SOLC] += ag.getSTUBBLEC();
    y[I_SOLN] += ag.getSTUBBLEN();

//    cout << "adding stubble " << ag.getSTUBBLEC() << endl;
    ag.setGROWDD( ZERO );
    harcnt = harcnt + 1; 
//cout << "diag = " << ag.getCROPPRODC() << " " << veg.getVEGC() << endl;
//ag.updateCropResidueFluxes();
  }
  else { ag.setNoCrops( pdm ); }


/*  if( 2 == ag.state & (ag.getGROWDD() >= ag.getGDDHARVST(ag.cmnt))) // pasture
  {
      frcab =(y[I_LEAFC]+y[I_SAPWOODC])/(y[I_LEAFC]+y[I_SAPWOODC]+y[I_ROOTC]);
      cabove = 0.05*(y[I_LABILEC]*frcab + y[I_LEAFC] + y[I_SAPWOODC] + y[I_HEARTWOODC]);
	  nabove = 0.05*(y[I_LABILEN]*frcab + y[I_LEAFN] + y[I_SAPWOODN] + y[I_HEARTWOODN]);
      y[I_LABILEC] = y[I_LABILEC]*frcab*0.95 + (y[I_LABILEC]*(1-frcab));
      y[I_LABILEN] = y[I_LABILEN]*frcab*0.95 + (y[I_LABILEN]*(1-frcab));
      y[I_LEAFC] *= 0.95;
      y[I_LEAFN] *= 0.95;
      y[I_SAPWOODC] *= 0.95;
      y[I_SAPWOODN] *= 0.95;
      y[I_HEARTWOODC] *= 0.95;
      y[I_HEARTWOODN] *= 0.95;
      ag.setPREVPROD1C( ag.getPREVPROD1C()+ 0.83 * cabove );
      y[I_SOLC] += 0.17 * cabove;
      y[I_SOLN] += 0.5 * nabove;
      y[I_AVLN] += 0.5 * nabove;
  } 

 if( 3 == ag.state & (ag.getGROWDD() >= ag.getGDDHARVST(ag.cmnt))) // turflawn
  {
      frcab =(y[I_LEAFC]+y[I_SAPWOODC])/(y[I_LEAFC]+y[I_SAPWOODC]+y[I_ROOTC]);
      cabove = 0.16*(y[I_LABILEC]*frcab + y[I_LEAFC] + y[I_SAPWOODC] + y[I_HEARTWOODC]);
      y[I_LABILEC] = y[I_LABILEC]*frcab*0.84 + (y[I_LABILEC]*(1-frcab));
      y[I_LABILEN] = y[I_LABILEN]*frcab*0.84 + (y[I_LABILEN]*(1-frcab));
      y[I_LEAFC] *= 0.84;
      y[I_LEAFN] *= 0.84;
      y[I_SAPWOODC] *= 0.84;
      y[I_SAPWOODN] *= 0.84;
      y[I_HEARTWOODC] *= 0.84;
      y[I_HEARTWOODN] *= 0.84;
      ag.setPREVPROD1C( ag.getPREVPROD1C()+  cabove );
 } */

if( 2 == ag.state ) // pasture
  {
   ag.grazing(  y[I_LEAFC], y[I_SAPWOODC], y[I_HEARTWOODC], y[I_LABILEC],
                y[I_LEAFN], y[I_SAPWOODN], y[I_HEARTWOODN], y[I_LABILEN],
                y[I_ROOTC] );
    y[I_LEAFC] -= ag.getFORAGECLEAF();
    y[I_SAPWOODC] -= ag.getFORAGECSAP();
    y[I_HEARTWOODC] -= ag.getFORAGECHEART();
    y[I_LABILEC] -= ag.getFORAGECLABILE();
    y[I_LEAFN] -= ag.getFORAGENLEAF();
    y[I_SAPWOODN] -= ag.getFORAGENSAP();
    y[I_HEARTWOODN] -= ag.getFORAGENHEART();
    y[I_LABILEN] -= ag.getFORAGENLABILE();
    y[I_SOLC] += ag.getMANUREC();
    y[I_SOLN] += ag.getMANUREN();
    y[I_AVLN] += ag.getURINE();
  }
  else { ag.setNoGrazing(); } 
//ag.setNoGrazing();

 if( 3 == ag.state && (ag.getGROWDD() >= ag.getGDDSEED(ag.cmnt) && ag.getGROWDD() <= ag.getGDDHARVST(ag.cmnt))) // turflawn
  {
      if((y[I_LEAFC]+y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_ROOTC]) == 0.0)
      {
        frcab = 0.0;
      }
      else
      {
      frcab =(y[I_LEAFC]+y[I_SAPWOODC]+y[I_HEARTWOODC])/(y[I_LEAFC]+y[I_SAPWOODC]+y[I_HEARTWOODC]+y[I_ROOTC]);
      }
      cabove = 0.16*(y[I_LABILEC]*frcab + y[I_LEAFC] + y[I_SAPWOODC] + y[I_HEARTWOODC]);
      nabove = 0.16*(y[I_LABILEN]*frcab + y[I_LEAFN] + y[I_SAPWOODN] + y[I_HEARTWOODN]);
      y[I_LABILEC] = y[I_LABILEC]*frcab*0.84 + (y[I_LABILEC]*(1-frcab));
      y[I_LABILEN] = y[I_LABILEN]*frcab*0.84 + (y[I_LABILEN]*(1-frcab));
      y[I_LEAFC] *= 0.84;
      y[I_LEAFN] *= 0.84;
      y[I_SAPWOODC] *= 0.84;
      y[I_SAPWOODN] *= 0.84;
      y[I_HEARTWOODC] *= 0.84;
      y[I_HEARTWOODN] *= 0.84;
    ag.setCLIPPINGS(cabove);
    y[I_SOLC] += cabove;
    y[I_SOLN] += nabove;
 }  

  // Determine crop residue

  ag.updateCropResidue();


  // Determine standing stock of products

  ag.updateProducts( CYCLE );


  // Determine total amount of products formed
  //   this month

  ag.updateTotalProductFormation();

  // Determine CFLUX from ecosystem from NEP plus
  //   fluxes from burning associated with agricultural
  //   conversion or

  ag.setCFLUX( (nep
                - ag.getCONVRTFLXC()
                - ag.getCROPRESIDUEFLXC()) );

  // Determine Net Carbon Exchange (NCE) with atmosphere
  //   (CFLUX plus decay of agricultural and wood products)

  nce = ag.getCFLUX() - ag.getTOTPRODDECAYC();


  // Determine carbon storage in ecosystem

  ag.setTOTEC( (veg.getVEGC()
             + y[I_SOLC]) );

//  veg.setTOTC( veg.getVEGC() );


  // Determine total carbon in ecosystems plus
  //   agricultural and wood products

  totalc = ag.getTOTEC() + ag.getTOTPRODC();

  // Update total loss of nitrogen from ecosystem for flux
  //   associated with crop residue


//  soil.setNLOST( (soil.getNLOST()
//                  + soil.getLCHDON()
//                  + ag.getCONVRTFLXN()
//                  + ag.getCROPRESIDUEFLXN()) );

  // Detemine total monthly N inputs to ecosystem

//  soil.setNINPUT( (soil.getNINPUT() + ag.getNRETENT() + y[I_AGFRTN]) + y[I_NFIXN] + y[I_NFIXS]);
//  soil.setLCHDIN( (y[I_NLST]
//                  + ag.getCONVRTFLXN()
//                  + ag.getCROPRESIDUEFLXN()) );

//  favg = 1.0 - exp(-1.0 / (365.0*tauavg));
#ifdef OPENN
//  soil.yrlchdin = soil.yrlchdin*(1.0 - favg) + 365.0*y[I_NLST]*favg;
//  soil.yrlchdin = soil.yrlchdin*(1.0 - favg) + 12.0*y[I_NLST]*favg;

//  soil.setNLOST( (y[I_NLST]
  soil.setNLOST( (soil.getNLOST()
                  + soil.getLCHDON()
                  + ag.getCONVRTFLXN()
                  + ag.getCROPRESIDUEFLXN()) );
  soil.setNINPUT( soil.getNINPUT() +  y[I_NFIXN] + y[I_NFIXS] );
#else
  soil.setNLOST( (soil.getNLOST()
                  + ag.getCONVRTFLXN()
                  + ag.getCROPRESIDUEFLXN()) );
  soil.setNINPUT( ( ag.fertn) );
#endif
//  y[I_NLST] = soil.getNLOST();
//  y[I_NLST] += y[I_LCHDON] + ag.getCONVRTFLXN() + ag.getCROPRESIDUEFLXN();

  

//  cout << "din = " << soil.getLCHDON() << " " << soil.getNLOST() << " " << y[I_NLST] << " " << soil.getLCHDOC() << " " << microbe.getDOCDON(veg.cmnt) << endl;
  // Update ANNUAL carbon, nitrogen and water pools and fluxes
  //   from integrator results


  updateYearSummary(pdm );
  if(mxeet < soil.getEET()) { mxeet = soil.getEET(); }

  if(pdyr == (int)tauavg && pdm == CYCLE-1) { yearSummaryExtrapolate(); }

  if(pdm == CYCLE-1)
  {
    if( (veg.yralloclc+veg.yrallocsc+veg.yrallocrc+veg.yrallocseedc
        -veg.yrrmleaf-veg.yrrmsapwood-veg.yrrmroot) != ZERO )
    { 
      veg.yrpleaf = (veg.yralloclc-veg.yrrmleaf)/
        (veg.yralloclc+veg.yrallocsc+veg.yrallocrc+veg.yrallocseedc
        -veg.yrrmleaf-veg.yrrmsapwood-veg.yrrmroot);

      veg.yrpsapwood = (veg.yrallocsc-veg.yrrmsapwood)/
        (veg.yralloclc+veg.yrallocsc+veg.yrallocrc+veg.yrallocseedc
        -veg.yrrmleaf-veg.yrrmsapwood-veg.yrrmroot);

      veg.yrproot = (veg.yrallocrc-veg.yrrmroot)/
        (veg.yralloclc+veg.yrallocsc+veg.yrallocrc+veg.yrallocseedc
        -veg.yrrmleaf-veg.yrrmsapwood-veg.yrrmroot);

      veg.yrpseed = (veg.yrallocseedc)/
        (veg.yralloclc+veg.yrallocsc+veg.yrallocrc+veg.yrallocseedc
        -veg.yrrmleaf-veg.yrrmsapwood-veg.yrrmroot);
    }
    else
    {
      veg.yrpleaf = ZERO;
      veg.yrpsapwood = ZERO;
      veg.yrproot = ZERO;
      veg.yrpseed = ZERO;
    }
    
    if( (veg.yrallocilc+veg.yrallocisc+veg.yrallocirc+veg.yrallociseedc) != ZERO )
    { 
      veg.yrpleafi = veg.yrallocilc/
        (veg.yrallocilc+veg.yrallocisc+veg.yrallocirc+veg.yrallociseedc);

      veg.yrpsapwoodi = veg.yrallocisc/
        (veg.yrallocilc+veg.yrallocisc+veg.yrallocirc+veg.yrallociseedc);

      veg.yrprooti = veg.yrallocirc/
        (veg.yrallocilc+veg.yrallocisc+veg.yrallocirc+veg.yrallociseedc);

      veg.yrpseedi = veg.yrallociseedc/
        (veg.yrallocilc+veg.yrallocisc+veg.yrallocirc+veg.yrallociseedc);
    }
    else
    {
      veg.yrpleafi = ZERO;
      veg.yrpsapwoodi = ZERO;
      veg.yrprooti = ZERO;
      veg.yrpseedi = ZERO;
    }
    
    veg.setRPLEAF( veg.getRPLEAF()*pow(avgfac,365) + veg.yrpleafi*(1.0 - pow(avgfac,365)));
  }


  #ifdef CALIBRATE_TEM
    // Display monthly results to DOS screen
//    pcdisplayMonth( pdyr, pdm );
  #endif


  if( 1 == ag.state && ag.getPRVCROPNPP() < y[I_NPP] )
  {
     ag.setPRVCROPNPP( y[I_NPP] );
  }
  else { ag.setPRVCROPNPP( ZERO ); }

// Reset growing degree days to zero if crops were
// harvested this month

  if( 1 == ag.state && ag.getGROWDD() >= ag.getGDDHARVST(ag.cmnt) )
  {
    ag.setGROWDD( ZERO );
  }

  //if( atms.getTAIR() < ag.getGDDMIN(ag.cmnt) ) { ag.setGROWDD( ZERO ); }
  if( atms.getTAIRN() < ag.getTKILL(ag.cmnt) ) { ag.setGROWDD( ZERO ); }
  ag.setFROSTFLAG( 0 );

  veg.setRPREC( veg.getRPREC()*avgfac+atms.getPREC()*(1.0-avgfac));
  veg.setRNPP( veg.getRNPP()*avgfac+y[I_NPP]*(1.0-avgfac));
  if(pdm == CYCLE - 1)
  {
//  cout << "mxeet = " << mxeet << endl;
  soil.setREET( mxeet*avgfac+mxeet*(1.0-avgfac));
//  soil.setREET( soil.getREET()*avgfac+y[I_EET]*(1.0-avgfac));
  }
  microbe.setRRH( microbe.getRRH()*avgfac+y[I_RH]*(1.0-avgfac));
  veg.setRLTRC( veg.getRLTRC()*avgfac+(y[I_LTRLC]+ y[I_LTRSC] + y[I_LTRHC] + y[I_LTRRC] + y[I_LTRSEEDC])*(1.0-avgfac));
    
  veg.setRGPP( veg.getRGPP()*avgfac+y[I_GPP]*(1.0-avgfac));
  veg.setRINGPP( veg.getRINGPP()*avgfac+y[I_INGPP]*(1.0-avgfac));
  veg.setRTAIR( veg.getRTAIR()*avgfac+atms.getTAIRD()*(1.0-avgfac));
  veg.setRTAIRPHI( veg.getRTAIRPHI()*avgfac+atms.getTAIRD()* veg.getPHI() *(1.0-avgfac));
  veg.setRPHI( veg.getRPHI()*avgfac+veg.getPHI()*(1.0-avgfac));
  veg.setRPHISH( veg.getRPHISH()*avgfacsh+veg.getPHI()*(1.0-avgfacsh));
    
  veg.setRLABILEC( veg.getRLABILEC()*avgfac+y[I_LABILEC]*(1.0-avgfac));
  veg.setRLABILEN( veg.getRLABILEN()*avgfac+y[I_LABILEN]*(1.0-avgfac));
    
  mdemandc = y[I_ALLOCLC] + y[I_ALLOCSC] + y[I_ALLOCRC] + y[I_ALLOCSEEDC] + y[I_RVGRW];
  mdemandn = y[I_ALLOCLN] + y[I_ALLOCSN] + y[I_ALLOCRN] + y[I_ALLOCSEEDN]
              -y[I_NRESORBL] -y[I_NRESORBS] -y[I_NRESORBR] -y[I_NRESORBSEED];
    
  veg.setRDEMANDC( veg.getRDEMANDC()*avgfac+mdemandc*(1.0-avgfac));
  veg.setRDEMANDN( veg.getRDEMANDN()*avgfac+mdemandn*(1.0-avgfac));

  // Update atms.prevco2 for next month

  atms.setPREVCO2( atms.getCO2() );

  // Update atms.prevtair and atms.prev2tair for next month

  atms.setPREV2TAIR( atms.getPREVTAIR() );
  atms.setPREVTAIR( atms.getTAIR() );

  // Update previous snowpack for next month

  soil.setPREVSPACK( soil.getSNOWPACK() );

  // Update ag.prevPROD1, ag.prevPROD10 and ag.prevPROD100
  // for next month

  ag.setPREVPROD1C( ag.getPROD1C() );
  ag.setPREVPROD1N( ag.getPROD1N() );

  ag.setPREVPROD10C( ag.getPROD10C() );
  ag.setPREVPROD10N( ag.getPROD10N() );

  ag.setPREVPROD100C( ag.getPROD100C() );
  ag.setPREVPROD100N( ag.getPROD100N() );

  // Update ag.prevCropResidue for next month

  ag.setPREVCROPRESIDUEC( ag.getCROPRESIDUEC() );
  ag.setPREVCROPRESIDUEN( ag.getCROPRESIDUEN() );

  //  Update maximum EET, maximum PET, GPP optimum temperature
  //    (veg.topt), and maximum leaf cover (veg.prvleafmx) for
  //    the current year

  if( 0 == pdm )
  {
    veg.setNEWTOPT( atms.getTAIRD() );
  }
  else
  {
    if( 0 == ag.state )
    {
     if((pdm%30) == 0.0 ) { veg.setNEWTOPT(atms.getTAIRD());}
      veg.resetNEWTOPT( veg.cmnt,
                        atms.getTAIRD(),
//                        veg.getRTAIR(),
                        veg.getRTAIRPHI(),
                        veg.getRPHI());
    }
    else
    {
      veg.resetNEWTOPT( ag.cmnt,
                        atms.getTAIRD(),
//                        veg.getRTAIR(),
                        veg.getRTAIRPHI(),
                        veg.getRPHI());
    }
  }

  // Save state of all the ODE state variables
  //   representing pools to allow checking
  //   of mass balance

  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" entering setPrevState() ");
    refresh();
  #endif
  setPrevState();


  // Update annual parameters for next year

  if( (CYCLE-1) == pdm )
  {
//   cout << "TOPT = " << veg.getTOPT() << " " << veg.getNEWTOPT() << " " << endl;
    veg.setTOPT( veg.getTOPT()*pow(avgfac,365)+veg.getNEWTOPT()*(1.0 - pow(avgfac,365)));
    veg.setTOPTMIC( veg.getTOPTMIC()*pow(avgfac,12)+veg.getNEWTOPT()*(1.0 - pow(avgfac,12)));
//   cout << "newTOPT = "  << veg.getTOPT() << " " << pow(avgfac,365) << " " << veg.getNEWTOPT() << endl;
    // annual number, so use avgfac^12 instead of just avgfac

    // Update optimum temperature parameters for GPP

    if( 0 == ag.state )
    {

      veg.boundTOPT( veg.cmnt );

//      cout << "topt after bound = " << veg.getTOPT() << endl;
    // Update adaptive parameters

      ag.setNATTOPT( veg.getTOPT() );


    }
    else
    {
      veg.boundTOPT( ag.cmnt );

      ag.setCROPTOPT( veg.getTOPT() );

    }

    // Update next year ag.prvstate with current year ag.state

    ag.prvstate = ag.state;

    if( veg.yrstructn != ZERO )
    {
      veg.yrc2n  = veg.yrcarbon / veg.yrstructn;
    }

    if( soil.yrorgn != ZERO )
    {
      soil.yrc2n = soil.yrorgc / soil.yrorgn;
    }
    if( 1 == baseline )
    {
      soil.yrnin = ZERO;
      soil.yrnlost = ZERO;

      if( (soil.yrorgc/microbe.getCNSOIL( veg.cmnt ) > soil.yrorgn)
          &&  (y[I_SOLC]/microbe.getCNSOIL( veg.cmnt ) > y[I_SOLN]) )
      {
        soil.yrnin = (soil.yrorgc / microbe.getCNSOIL( veg.cmnt )) - soil.yrorgn;
        soil.yrnin += (y[I_SOLC]/microbe.getCNSOIL( veg.cmnt )) - y[I_SOLN];
        soil.yrnin /= 2.0;
      }
      else if( (soil.yrorgc/microbe.getCNSOIL( veg.cmnt ) < soil.yrorgn)
               &&  (y[I_SOLC]/microbe.getCNSOIL( veg.cmnt ) < y[I_SOLN]) ) 
      {
        soil.yrnlost = soil.yrorgn - (soil.yrorgc / microbe.getCNSOIL( veg.cmnt ));
        soil.yrnlost += y[I_SOLN] - (y[I_SOLC]/microbe.getCNSOIL( veg.cmnt ));
        soil.yrnlost /= 2.0;
      }

      y[I_SOLN] = y[I_SOLN] + soil.yrnin - soil.yrnlost;
    }

    if( endeq > 0 )
    {
      ++endeq;
    }
  }
  
  #ifdef DEBUG_CTEM
    move(DEBUG_ROW,1);
    printw(" at end of stepmonth, month %2d ", pdm);
    refresh();
  #endif
  
  return endeq;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Ttem45::testEquilibrium( const int& pdyr,
                             const int& nyears,
                             const double& vegceq, 
                             const double& soilceq,
                             const double& vegneq,
                             const double& soilneq )
{
//  cout << "diag = " << pdyr << " " << vegceq[pdyr] << " " << soilceq[pdyr] << " " << vegneq[pdyr] << " " << soilneq[pdyr] << " " << vegceq[pdyr]+soilceq[pdyr] << " " << (vegceq[pdyr-39]+soilceq[pdyr-39]) << " " << vegceq[pdyr-39] << " " <<soilceq[pdyr-39] << endl;

 if (pdyr >= nyears+strteq)
 {
  if( 0 == nfeed && 0 == rheqflag
      && wtol >= fabs( atms.yrrain + atms.yrsnowfall
      - soil.yreet - soil.yrrrun - soil.yrsrun )
      && (ctol >= fabs( veg.yrnpp - veg.yrltrc )) )
  {
    return 1;
  }

  if( 0 == nfeed && 1 == rheqflag
      && wtol >= fabs( atms.yrrain
                       + atms.yrsnowfall
                       - soil.yreet
                       - soil.yrrrun
                       - soil.yrsrun )

//      && (ctol >= fabs( yrnep ))
//      && (ctol >= fabs( veg.yrnpp - veg.yrltrc ))
//      && (ctol >= fabs( veg.yrltrc - microbe.yrrh )) )
        && (ctol >= (veg.getVEGC()+y[I_SOLC])-
                    (vegceq+soilceq))
        && (ctol >= (veg.getVEGC()-vegceq))
        && (ctol >= (y[I_SOLC]-soilceq)) )
  {
    return 1;
  }

//cout << "equilibrium = " << wtol << " " << atms.yrrain << " " << atms.yrsnowfall << " " << soil.yreet << " " << soil.yrrrun << " " << soil.yrsrun << endl;
cout << "equil = " << veg.getVEGC() << " " << vegceq << " " << y[I_SOLC] << " " << soilceq << " " << soil.yrnin << " " << soil.yrnlost << endl;
if(ag.state == 0)
{

  if( 1 == nfeed && 1 == rheqflag
      && wtol >= fabs( atms.yrrain
                       + atms.yrsnowfall
                       - soil.yreet
                       - soil.yrrrun
                       - soil.yrsrun )

//      && (ntol >= fabs( soil.yrnin - soil.yrnlost ))
//      && (ntol >= fabs( veg.yrnup - veg.yrltrn ))
//      && (ntol >= fabs( veg.yrnup - microbe.yrnmin ))
//      && (ntol >= fabs( veg.yrltrn - microbe.yrnmin ))

//      && (ctol >= fabs( yrnep ))
//      && (ctol >= fabs( veg.yrnpp - veg.yrltrc ))
//      && (ctol >= fabs( veg.yrltrc - microbe.yrrh )) )

        && (ntol >= fabs( soil.yrnin - soil.yrnlost ))
        && (ntol >= fabs((veg.getVEGN()+y[I_SOLN]))-
                    (vegneq+soilneq))
        && (ntol >= fabs((veg.getVEGN()-vegneq)))
        && (ntol >= fabs((y[I_SOLN]-soilneq)))

        && (ctol >= fabs((veg.getVEGC()+y[I_SOLC]))-
                    (vegceq+soilceq))
        && (ctol >= fabs((veg.getVEGC()-vegceq)))
        && (ctol >= fabs((y[I_SOLC]-soilceq))) )
  {
      return 1;
  }
  }
else
{
  if( 1 == nfeed && 1 == rheqflag
      && wtol >= fabs( atms.yrrain
                       + atms.yrsnowfall
                       - soil.yreet
                       - soil.yrrrun
                       - soil.yrsrun )
        && (ntol >= fabs(veg.getVEGN()+y[I_SOLN])-
                    (vegneq+soilneq))
        && (ntol >= fabs((veg.getVEGN()-vegneq)))
        && (ntol >= fabs((y[I_SOLN]-soilneq)))

        && (ctol >= fabs((veg.getVEGC()+y[I_SOLC]))-
                    (vegceq+soilceq))
        && (ctol >= fabs((veg.getVEGC()-vegceq)))
        && (ctol >= fabs((y[I_SOLC]-soilceq))) )
  {
      return 1;
  }
}
}


  return 0;

};


/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************** */
void Ttem45::updateVegBiomass( double pstate[] )
{
  // Update veg object's biomass variables, which are used in veg.updateDynamics
  veg.setLEAFC( pstate[I_LEAFC] );
  veg.setLEAFN( pstate[I_LEAFN] );

  veg.setSAPWOODC( pstate[I_SAPWOODC] );
  veg.setSAPWOODN( pstate[I_SAPWOODN] );

  veg.setHEARTWOODC( pstate[I_HEARTWOODC] );
  veg.setHEARTWOODN( pstate[I_HEARTWOODN] );

  veg.setROOTC( pstate[I_ROOTC] );
  veg.setROOTN( pstate[I_ROOTN] );

  veg.setSEEDC( pstate[I_SEEDC] );
  veg.setSEEDN( pstate[I_SEEDN] );

  veg.setLABILEC( pstate[I_LABILEC] );
  veg.setLABILEN( pstate[I_LABILEN] );

  veg.setSTRC( pstate[I_LEAFC] + pstate[I_SAPWOODC] + pstate[I_HEARTWOODC] + pstate[I_ROOTC] + pstate[I_SEEDC] );
  veg.setSTRN( pstate[I_LEAFN] + pstate[I_SAPWOODN] + pstate[I_HEARTWOODN] + pstate[I_ROOTN] + pstate[I_SEEDN] );

  veg.setVEGC( veg.getSTRC() + veg.getLABILEC() );
  veg.setVEGN( veg.getSTRN() + veg.getLABILEN() );



};
/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************** */

void Ttem45::updateYearSummary(const int& pdm )
{

//  double favg = 1.0 - exp(-1.0 / (12.0*tauavg));
  double favg = 1.0 - exp(-1.0 / (365.0*tauavg));

  // Update sum of annual carbon storage in ecosystems

  veg.yrcarbon  = veg.yrcarbon*(1.0 - favg) + veg.getVEGC()*favg;
  soil.yrorgc = soil.yrorgc*(1.0 - favg) + y[I_SOLC]*favg;
  yrtotalc = yrtotalc*(1.0 - favg) + totalc*favg;

  // Update sum of annual nitrogen storage in ecosystems

  veg.yrstructn = veg.yrstructn*(1.0 - favg) + veg.getSTRN()*favg;
  veg.yrstoren = veg.yrstoren*(1.0 - favg) + y[I_LABILEN]*favg;
  soil.yrorgn = soil.yrorgn*(1.0 - favg) + y[I_SOLN]*favg;
  soil.yravln = soil.yravln*(1.0 - favg) + y[I_AVLN]*favg;

  veg.yrnitrogen = veg.yrnitrogen*(1.0 - favg) + veg.getVEGN()*favg;

  // Update sum of annual water storage in ecosystems

  soil.yravlh2o = soil.yravlh2o*(1.0 - favg) + (y[I_SM] - soil.getWILTPT())*favg;
  soil.yrsmoist = soil.yrsmoist*(1.0 - favg) + y[I_SM]*favg;
  soil.yrvsm = soil.yrvsm*(1.0 - favg) + y[I_VSM]*favg;
  soil.yrpctp = soil.yrpctp*(1.0 - favg) + y[I_PCTP]*favg;
  soil.yrsnowpack = soil.yrsnowpack*(1.0 - favg) + soil.getSNOWPACK()*favg;
  soil.yrrgrndh2o = soil.yrrgrndh2o*(1.0 - favg) + y[I_RGRW]*favg;
  soil.yrsgrndh2o = soil.yrsgrndh2o*(1.0 - favg) + y[I_SGRW]*favg;

  // Update sum of annual phenology in natural ecosystems

  veg.yrfpc = veg.yrfpc*(1.0 - favg) + y[I_FPC]*favg;

//  Penman variables

  veg.yrgc = veg.yrgc*(1.0 - favg) + y[I_GC]*favg;
  veg.yrgs = veg.yrgs*(1.0 - favg) + y[I_GS]*favg;

  // Update sum of annual carbon fluxes in ecosystems

  veg.yringpp = veg.yringpp*(1.0 - favg) + 365.0*y[I_INGPP]*favg;
  veg.yrgpp   = veg.yrgpp*(1.0 - favg) + 365.0*y[I_GPP]*favg;
  veg.yrinnpp = veg.yrinnpp*(1.0 - favg) + 365.0*y[I_INNPP]*favg;
  veg.yrnpp   = veg.yrnpp*(1.0 - favg) + 365.0*y[I_NPP]*favg;
  veg.yrgpr   = veg.yrgpr*(1.0 - favg) + 365.0*y[I_GPR]*favg;
  veg.yrrmaint = veg.yrrmaint*(1.0 - favg) + 365.0*(y[I_RMLEAF] + y[I_RMSAPWOOD] + y[I_RMROOT] + y[I_RMSEED])*favg;
  veg.yrrmleaf = veg.yrrmleaf*(1.0 - favg) + 365.0*y[I_RMLEAF]*favg;
  veg.yrrmsapwood = veg.yrrmsapwood*(1.0 - favg) + 365.0*y[I_RMSAPWOOD]*favg;
  veg.yrrmroot = veg.yrrmroot*(1.0 - favg) + 365.0*y[I_RMROOT]*favg;
  veg.yrrgrowth = veg.yrrgrowth*(1.0 - favg) + 365.0*y[I_RVGRW]*favg;


  veg.yrltrc  = veg.yrltrc*(1.0 - favg) + 365.0*(y[I_LTRLC] + y[I_LTRSC] + y[I_LTRHC] + y[I_LTRRC] + y[I_LTRSEEDC])*favg;
  microbe.yrrh = microbe.yrrh*(1.0 - favg) + 365.0*y[I_RH]*favg;

  veg.yralloclc = veg.yralloclc*(1.0 - favg) + 365.0*y[I_ALLOCLC]*favg;
  veg.yrallocsc = veg.yrallocsc*(1.0 - favg) + 365.0*y[I_ALLOCSC]*favg;
  veg.yrallocrc = veg.yrallocrc*(1.0 - favg) + 365.0*y[I_ALLOCRC]*favg;
  
  veg.yrallocilc = veg.yrallocilc*(1.0 - favg) + 365.0*y[I_ALLOCILC]*favg;
  veg.yrallocisc = veg.yrallocisc*(1.0 - favg) + 365.0*y[I_ALLOCISC]*favg;
  veg.yrallocirc = veg.yrallocirc*(1.0 - favg) + 365.0*y[I_ALLOCIRC]*favg;
  veg.yrallociseedc = veg.yrallociseedc*(1.0 - favg) + 365.0*y[I_ALLOCISEEDC]*favg;
  
  veg.yrallocseedc = veg.yrallocseedc*(1.0 - favg) + 365.0*y[I_ALLOCSEEDC]*favg;
  veg.yrallocseedn = veg.yrallocseedn*(1.0 - favg) + 365.0*y[I_ALLOCSEEDN]*favg;

  yrnep = yrnep*(1.0 - favg) + 365.0*nep*favg;
  yrnce = yrnce*(1.0 - favg) + 365.0*nce*favg;


 // Update sum of annual nitrogen fluxes in ecosystems

//  soil.yrnin = soil.yrnin*(1.0 - favg) + 12.0*(y[I_NINP] + y[I_NFIXN] + y[I_NFIXS])*favg;
  soil.yrnin = soil.yrnin*(1.0 - favg) + 365.0*soil.getNINPUT()*favg;

  ag.yrfertn = ag.yrfertn*(1.0 - favg) + 365.0*y[I_AGFRTN]*favg;

  veg.yrinnup = veg.yrinnup*(1.0 - favg) + 365.0*y[I_INNUP]*favg;
  veg.yrnup   = veg.yrnup*(1.0 - favg) + 365.0*y[I_VNUP]*favg;
  veg.yrnrsorb = veg.yrnrsorb*(1.0 - favg) + 365.0*(y[I_NRESORBL] + y[I_NRESORBS] + y[I_NRESORBR] + y[I_NRESORBSEED])*favg;

  veg.yrltrn = veg.yrltrn*(1.0 - favg) + 365.0*(y[I_LTRLN] + y[I_LTRSN] + y[I_LTRHN] + y[I_LTRRN] + y[I_LTRSEEDN])*favg;

  microbe.yrnuptake = microbe.yrnuptake*(1.0 - favg) + 365.0*y[I_MNUP]*favg;
  microbe.yrnmin = microbe.yrnmin*(1.0 - favg)  + 365.0*y[I_NMIN]*favg;

//  soil.yrnlost = soil.yrnlost*(1.0 - favg) + 12.0*(y[I_NLST])*favg;
  soil.yrnlost = soil.yrnlost*(1.0 - favg) + 365.0*(soil.getNLOST())*favg;


   // Update sum of annual water fluxes in ecosystems

  ag.yrirrig = ag.yrirrig*(1.0 - favg) + 365.0*y[I_AGIRRIG]*favg;
  soil.yrineet = soil.yrineet*(1.0 - favg) + 365.0*y[I_INEET]*favg;
  soil.yreet = soil.yreet*(1.0 - favg) + 365.0*y[I_EET]*favg;
  soil.yrrperc = soil.yrrperc*(1.0 - favg) + 365.0*y[I_RPERC]*favg;
  soil.yrsperc = soil.yrsperc*(1.0 - favg) + 365.0*y[I_SPERC]*favg;
  soil.yrrrun = soil.yrrrun*(1.0 - favg) + 365.0*y[I_RRUN]*favg;
  soil.yrsrun = soil.yrsrun*(1.0 - favg) + 365.0*y[I_SRUN]*favg;

  atms.yrrain = atms.yrrain*(1.0 - favg) + 365.0*atms.getRAIN()*favg;
  atms.yrsnowfall = atms.yrsnowfall*(1.0 - favg) + 365.0*atms.getSNOWFALL()*favg;
//  atms.yrpet += atms.getPET();
  atms.yrpet = atms.yrpet*(1.0 - favg) + 365.0*veg.getPET()*favg;
  soil.yrsnowinf = soil.yrsnowinf*(1.0 - favg) + 365.0*soil.getSNOWINF()*favg;
  soil.yrh2oyld = soil.yrh2oyld*(1.0 - favg) + 365.0*soil.getH2OYLD()*favg;

  if( (atms.getTAIR() >= ag.getGDDMIN(ag.cmnt))&&(atms.getTAIRN() > ag.getTKILL(ag.cmnt)) )
  {
//    ag.yrgrowdd = ag.yrgrowdd*(1.0 - favg) + 12.0*(atms.getTAIR() - ag.getGDDMIN(ag.cmnt))*atms.getNDAYS( pdm )*favg;
    ag.yrgrowdd = ag.yrgrowdd*(1.0 - favg) + 365.0*(atms.getTAIR() - ag.getGDDMIN(ag.cmnt))*favg;
  }
  else
  {
    ag.yrgrowdd = ag.yrgrowdd*(1.0 - favg);
  }
  ag.yrfrost = ag.yrfrost*(1.0 - favg) + 365.0*ag.getFROSTFLAG()*favg;

  ag.yrstubC = ag.yrstubC*(1.0 - favg) + 365.0*ag.getSTUBBLEC()*favg;
  ag.yrstubN = ag.yrstubN*(1.0 - favg) + 365.0*ag.getSTUBBLEN()*favg;

 // Update sum of annual carbon and nitrogen fluxes from
 //   agricultural conversion

  ag.yrconvrtC = ag.yrconvrtC*(1.0 - favg) + 365.0*ag.getCONVRTFLXC()*favg;
  ag.yrvconvrtC = ag.yrvconvrtC*(1.0 - favg) + 365.0*ag.getVCONVRTFLXC()*favg;
  ag.yrsconvrtC = ag.yrsconvrtC*(1.0 - favg) + 365.0*ag.getSCONVRTFLXC()*favg;
  ag.yrslashC = ag.yrslashC*(1.0 - favg) + 365.0*ag.getSLASHC()*favg;
  ag.yrcflux = ag.yrcflux*(1.0 - favg) + 365.0*ag.getCFLUX()*favg;

  ag.yrconvrtN = ag.yrconvrtN*(1.0 - favg) + 365.0*ag.getCONVRTFLXN()*favg;
  ag.yrvconvrtN = ag.yrvconvrtN*(1.0 - favg) + 365.0*ag.getVCONVRTFLXN()*favg;
  ag.yrsconvrtN = ag.yrsconvrtN*(1.0 - favg) + 365.0*ag.getSCONVRTFLXN()*favg;
  ag.yrslashN = ag.yrslashN*(1.0 - favg) + 365.0*ag.getSLASHN()*favg;

  ag.yrnrent = ag.yrnrent*(1.0 - favg) + 365.0*ag.getNRETENT()*favg;
  ag.yrnvrent = ag.yrnvrent*(1.0 - favg) + 365.0*ag.getNVRETENT()*favg;
  ag.yrnsrent = ag.yrnsrent*(1.0 - favg) + 365.0*ag.getNSRETENT()*favg;

  ag.yrformResidueC = ag.yrformResidueC*(1.0 - favg) + 365.0*ag.getFORMCROPRESIDUEC()*favg;
  ag.yrformResidueN = ag.yrformResidueN*(1.0 - favg) + 365.0*ag.getFORMCROPRESIDUEN()*favg;

  ag.yrfluxResidueC = ag.yrfluxResidueC*(1.0 - favg) + 365.0*ag.getCROPRESIDUEFLXC()*favg;
  ag.yrfluxResidueN = ag.yrfluxResidueN*(1.0 - favg) + 365.0*ag.getCROPRESIDUEFLXN()*favg;


 // Update sum of annual carbon and nitrogen fluxes from
 //   products

  ag.yrformPROD1C = ag.yrformPROD1C*(1.0 - favg) + 365.0*ag.getCROPPRODC()*favg;
  ag.yrformPROD1N = ag.yrformPROD1N*(1.0 - favg) + 365.0*ag.getCROPPRODN()*favg;

  ag.yrdecayPROD1C = ag.yrdecayPROD1C*(1.0 - favg) + 365.0*ag.getPROD1DECAYC()*favg;
  ag.yrdecayPROD1N = ag.yrdecayPROD1N*(1.0 - favg) + 365.0*ag.getPROD1DECAYN()*favg;

  ag.yrformPROD10C = ag.yrformPROD10C*(1.0 - favg) + 365.0*ag.getFORMPROD10C()*favg;
  ag.yrformPROD10N = ag.yrformPROD10N*(1.0 - favg) + 365.0*ag.getFORMPROD10N()*favg;

  ag.yrdecayPROD10C = ag.yrdecayPROD10C*(1.0 - favg) + 365.0*ag.getPROD10DECAYC()*favg;
  ag.yrdecayPROD10N = ag.yrdecayPROD10N*(1.0 - favg) + 365.0*ag.getPROD10DECAYN()*favg;

  ag.yrformPROD100C = ag.yrformPROD100C*(1.0 - favg) + 365.0*ag.getFORMPROD100C()*favg;
  ag.yrformPROD100N = ag.yrformPROD100N*(1.0 - favg) + 365.0*ag.getFORMPROD100N()*favg;

  ag.yrdecayPROD100C = ag.yrdecayPROD100C*(1.0 - favg) + 365.0*ag.getPROD100DECAYC()*favg;
  ag.yrdecayPROD100N = ag.yrdecayPROD100N*(1.0 - favg) + 365.0*ag.getPROD100DECAYN()*favg;

  ag.yrformTOTPRODC = ag.yrformTOTPRODC*(1.0 - favg) + 365.0*ag.getFORMTOTPRODC()*favg;
  ag.yrformTOTPRODN = ag.yrformTOTPRODN*(1.0 - favg) + 365.0*ag.getFORMTOTPRODN()*favg;

  ag.yrdecayTOTPRODC = ag.yrdecayTOTPRODC*(1.0 - favg) + 365.0*ag.getTOTPRODDECAYC()*favg;
  ag.yrdecayTOTPRODN = ag.yrdecayTOTPRODN*(1.0 - favg) + 365.0*ag.getTOTPRODDECAYN()*favg;

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************** */

void Ttem45::yearSummaryExtrapolate( void )
{
  double fxtra = 1.0/(1.0 - exp(-1.0));
  
  // Extrapolate values at year = tauavg mark based on 
  //   expectation of exponential relaxation from initial value of 0 to equilibrium value,
  //   with characteristic time tauavg 
  
  // Update sum of annual carbon storage in ecosystems

  veg.yrcarbon  *= fxtra;
  soil.yrorgc *= fxtra;
  yrtotalc *= fxtra;

  // Update sum of annual nitrogen storage in ecosystems

  veg.yrstructn *= fxtra;
  veg.yrstoren *= fxtra;
  soil.yrorgn *= fxtra;
  soil.yravln *= fxtra;

  veg.yrnitrogen *= fxtra;

  // Update sum of annual water storage in ecosystems

  soil.yravlh2o *= fxtra;
  soil.yrsmoist *= fxtra;
  soil.yrvsm *= fxtra;
  soil.yrpctp *= fxtra;
  soil.yrsnowpack *= fxtra;
  soil.yrrgrndh2o *= fxtra;
  soil.yrsgrndh2o *= fxtra;

//  Penman variables

  veg.yrgc *= fxtra;
  veg.yrgs *= fxtra;

  // Update sum of annual carbon fluxes in ecosystems

  veg.yringpp *= fxtra;
  veg.yrgpp   *= fxtra;
  veg.yrinnpp *= fxtra;
  veg.yrnpp   *= fxtra;
  veg.yrgpr   *= fxtra;
  veg.yrrmaint *= fxtra;
  veg.yrrmleaf *= fxtra;
  veg.yrrmsapwood *= fxtra;
  veg.yrrmroot *= fxtra;
  veg.yrrgrowth *= fxtra;


  veg.yrltrc  *= fxtra;
  microbe.yrrh *= fxtra;

  veg.yralloclc *= fxtra;
  veg.yrallocsc *= fxtra;
  veg.yrallocrc *= fxtra;
  
  veg.yrallocilc *= fxtra;
  veg.yrallocisc *= fxtra;
  veg.yrallocirc *= fxtra;
  
  veg.yrallocseedc *= fxtra;
  veg.yrallocseedn *= fxtra;

 // Update sum of annual nitrogen fluxes in ecosystems

  veg.yrinnup *= fxtra;
  veg.yrnup   *= fxtra;
  veg.yrnrsorb *= fxtra;

  veg.yrltrn *= fxtra;

  microbe.yrnuptake *= fxtra;
  microbe.yrnmin *= fxtra;

   // Update sum of annual water fluxes in ecosystems

  ag.yrirrig *= fxtra;
  soil.yrineet *= fxtra;
  soil.yreet *= fxtra;
  soil.yrrperc *= fxtra;
  soil.yrsperc *= fxtra;
  soil.yrrrun *= fxtra;
  soil.yrsrun *= fxtra;

  atms.yrrain *= fxtra;
  atms.yrsnowfall *= fxtra;
  atms.yrpet *= fxtra;
  soil.yrsnowinf *= fxtra;
  soil.yrh2oyld *= fxtra;
  
  ag.yrgrowdd *= fxtra;
  ag.yrfrost *= fxtra;

  ag.yrstubC *= fxtra;
  ag.yrstubN *= fxtra;
  
  ag.yrformPROD1C *= fxtra;
  ag.yrformPROD1N *= fxtra;

};

/* *************************************************************
************************************************************* */
/* *************************************************************
************************************************************* */
#ifdef CALIBRATE_TEM
void Ttem45::coutputTEMmonth( const int& pdm,
                     const int& CYCLE )
{

  int ndays[12],i,dm,mondays;

  if(CYCLE == 365)
  {
  ndays[0] = 0;
  ndays[1] = 31;
  ndays[2] = 59;
  ndays[3] = 90;
  ndays[4] = 120;
  ndays[5] = 151;
  ndays[6] = 181;
  ndays[7] = 212;
  ndays[8] = 243;
  ndays[9] = 273;
  ndays[10] = 304;
  ndays[11] = 334;
  }
  else
  {
  ndays[0] = 0;
  ndays[1] = 31;
  ndays[2] = 60;
  ndays[3] = 91;
  ndays[4] = 121;
  ndays[5] = 152;
  ndays[6] = 182;
  ndays[7] = 213;
  ndays[8] = 244;
  ndays[9] = 274;
  ndays[10] = 305;
  ndays[11] = 335;
  }

    for(i = 0; i < NUMTEM; ++i )
    {
     for( dm = 0; dm < 12; ++dm )
      {
        if(pdm == ndays[dm])
        {
        outputm[i][dm] = ZERO;
        }
      }

    }

    for(i = 0; i < 24; ++i )
    {
     for( dm = 0; dm < 12; ++dm )
      {
        if(pdm == ndays[dm])
        {
        outputmcl[i][dm] = ZERO;
        }
      }

    }

        if(pdm < ndays[1])
        {
         dm = 0;
        }
        if(pdm >= ndays[1] && pdm < ndays[2])
        {
         dm = 1;
        }
        if(pdm >= ndays[2] && pdm < ndays[3])
        {
         dm = 2;
        }
        if(pdm >= ndays[3] && pdm < ndays[4])
        {
         dm = 3;
        }
        if(pdm >= ndays[4] && pdm < ndays[5])
        {
         dm = 4;
        }
        if(pdm >= ndays[5] && pdm < ndays[6])
        {
         dm = 5;
        }
        if(pdm >= ndays[6] && pdm < ndays[7])
        {
         dm = 6;
        }
        if(pdm >= ndays[7] && pdm < ndays[8])
        {
         dm = 7;
        }
        if(pdm >= ndays[8] && pdm < ndays[9])
        {
         dm = 8;
        }
        if(pdm >= ndays[9] && pdm < ndays[10])
        {
         dm = 9;
        }
        if(pdm >= ndays[10] && pdm < ndays[11])
        {
         dm = 10;
        }
        if(pdm >= ndays[11])
        {
         dm = 11;
        }

   if(dm < 11)
   {
    mondays = ndays[dm+1]-ndays[dm];
   }
   else
   {
    mondays = CYCLE-ndays[dm]+1;
   }



  // Ecosystem carbon pools determined in integrator


  outputm[I_LEAFC][dm] = y[I_LEAFC];

  outputm[I_SAPWOODC][dm] = y[I_SAPWOODC];

  outputm[I_HEARTWOODC][dm] = y[I_HEARTWOODC];

  outputm[I_ROOTC][dm] = y[I_ROOTC];

  outputm[I_SEEDC][dm] = y[I_SEEDC];

  outputm[I_LABILEC][dm] = y[I_LABILEC];

  outputm[I_SOLC][dm] = y[I_SOLC]; 

  // Ecosystem nitrogen pools determined in integrator

  outputm[I_LEAFN][dm] = y[I_LEAFN];

  outputm[I_SAPWOODN][dm] = y[I_SAPWOODN];

  outputm[I_HEARTWOODN][dm] = y[I_HEARTWOODN];

  outputm[I_ROOTN][dm] = y[I_ROOTN]; 

  outputm[I_SEEDN][dm] = y[I_SEEDN];

  outputm[I_LABILEN][dm] = y[I_LABILEN];

  outputm[I_SOLN][dm] = y[I_SOLN];

  outputm[I_AVLN][dm] = y[I_AVLN];


  // Ecosystem water pools determined in integrator

  outputm[I_AVLW][dm] = y[I_SM] - soil.getWILTPT();

//  cout << "output availw = " << outputm[I_AVLW][dm] << endl;
  if( outputm[I_AVLW][dm] < ZERO ) {
    outputm[I_AVLW][dm] = 0.;
  }

  outputm[I_SM][dm] = y[I_SM];

  outputm[I_VSM][dm] = y[I_VSM];

  outputm[I_PCTP][dm] = y[I_PCTP];

  outputm[I_RGRW][dm] = y[I_RGRW];

  outputm[I_SGRW][dm] = y[I_SGRW];

  // Monthly phenology determined in integrator

  outputm[I_FPC][dm] = y[I_FPC];

  // Monthly carbon fluxes in ecosystems determined in integrator

  outputm[I_INGPP][dm] += y[I_INGPP];

  outputm[I_GPP][dm] += y[I_GPP];

//  cout << "diag = " << dm << " " << y[I_GPP] << " " << outputm[I_GPP][dm] << endl;

  outputm[I_FOZONE][dm] += y[I_FOZONE];

  outputm[I_INNPP][dm] += y[I_INNPP];

  outputm[I_NPP][dm] += y[I_NPP];

  outputm[I_GPR][dm] += y[I_GPR];

  outputm[I_ALLOCLC][dm] += y[I_ALLOCLC];

  outputm[I_ALLOCSC][dm] += y[I_ALLOCSC];

  outputm[I_ALLOCHC][dm] += y[I_ALLOCHC];

  outputm[I_ALLOCRC][dm] += y[I_ALLOCRC];

  outputm[I_ALLOCSEEDC][dm] += y[I_ALLOCSEEDC];

  outputm[I_RVMNT][dm] += y[I_RVMNT];

  outputm[I_RMLEAF][dm] += y[I_RMLEAF];

  outputm[I_RMSAPWOOD][dm] += y[I_RMSAPWOOD];

  outputm[I_RMROOT][dm] += y[I_RMROOT];

  outputm[I_RMSEED][dm] += y[I_RMSEED];

  outputm[I_RMLABILE][dm] += y[I_RMLABILE];

  outputm[I_RVGRW][dm] += y[I_RVGRW];

  outputm[I_LTRLC][dm] += y[I_LTRLC];

  outputm[I_LTRSC][dm] += y[I_LTRSC];

  outputm[I_LTRHC][dm] += y[I_LTRHC];

  outputm[I_LTRRC][dm] += y[I_LTRRC];

  outputm[I_LTRSEEDC][dm] += y[I_LTRSEEDC];

  outputm[I_RH][dm] += y[I_RH];

  outputm[I_DOC][dm] += y[I_DOC];

  outputm[I_DON][dm] += y[I_DON];


  // Monthly nitrogen fluxes in ecosystems determined in
  //   integrator

  outputm[I_NINP][dm] += y[I_NINP];

  outputm[I_AGFRTN][dm] += y[I_AGFRTN];

  outputm[I_INNUP][dm] += y[I_INNUP];

  outputm[I_VNUP][dm] += y[I_VNUP];

  outputm[I_ALLOCLN][dm] += y[I_ALLOCLN];

  outputm[I_ALLOCSN][dm] += y[I_ALLOCSN];

  outputm[I_ALLOCHN][dm] += y[I_ALLOCHN];

  outputm[I_ALLOCRN][dm] += y[I_ALLOCRN];

  outputm[I_ALLOCSEEDN][dm] += y[I_ALLOCSEEDN];

  outputm[I_LTRLN][dm] += y[I_LTRLN];

  outputm[I_LTRSN][dm] += y[I_LTRSN];

  outputm[I_LTRHN][dm] += y[I_LTRHN];

  outputm[I_LTRRN][dm] += y[I_LTRRN];

  outputm[I_LTRSEEDN][dm] += y[I_LTRSEEDN];

  outputm[I_MNUP][dm] += y[I_MNUP];

  outputm[I_NMIN][dm] += y[I_NMIN];

  outputm[I_NLST][dm] += y[I_NLST];

  outputm[I_LCHDON][dm] += y[I_LCHDON];

  outputm[I_LCHDOC][dm] += y[I_LCHDOC];

  outputm[I_DONPROD][dm] += y[I_DONPROD];

  outputm[I_DOCPROD][dm] += y[I_DOCPROD];

  outputm[I_NFIXN][dm] += y[I_NFIXN];

  outputm[I_NFIXS][dm] += y[I_NFIXS];

  outputm[I_NRESORBL][dm] += y[I_NRESORBL];

  outputm[I_NRESORBS][dm] += y[I_NRESORBS];

  outputm[I_NRESORBR][dm] += y[I_NRESORBR];

  outputm[I_NRESORBSEED][dm] += y[I_NRESORBSEED];

  // Monthly water fluxes in ecosystems

  outputm[I_AGIRRIG][dm] += y[I_AGIRRIG];

  outputm[I_INEET][dm] += y[I_INEET];

  outputm[I_EET][dm] += y[I_EET];

  outputm[I_RPERC][dm] += y[I_RPERC];

  outputm[I_SPERC][dm] += y[I_SPERC];

  outputm[I_RRUN][dm] += y[I_RRUN];

  outputm[I_SRUN][dm] += y[I_SRUN];

  outputm[I_GC][dm] += y[I_GC]/mondays; 

  outputm[I_GS][dm] += y[I_GS]/mondays;

  outputm[I_PECAN][dm] += y[I_PECAN];

  outputm[I_PESOIL][dm] += y[I_PESOIL];

  //  Diagnostic GPP terms
  
  outputm[I_FRDL][dm] += y[I_FRDL]/mondays;

  outputm[I_FCO2][dm] += y[I_FCO2]/mondays;

  outputm[I_FH2O][dm] += y[I_FH2O]/mondays;

  outputm[I_TEMP][dm] += y[I_TEMP]/mondays;

  outputm[I_FO3][dm] += y[I_FO3]/mondays;

  // Other ecosystem carbon pools

  outputm[I_LAI][dm] = veg.getLAI();

  outputm[I_TOTEC][dm] = ag.getTOTEC();

  outputm[I_TOTC][dm] = veg.getVEGC();

  // Other ecosystem nitrogen pools

  outputm[I_VEGN][dm] = veg.getVEGN();


  // Other ecosystem water pools

  outputm[I_SNWPCK][dm] = soil.getSNOWPACK();

  // Other monthly carbon fluxes in ecosystems

  outputm[I_NEP][dm] += getNEP();

  outputm[I_NCE][dm] += getNCE();


  // Other monthly water fluxes in ecosystems

  outputm[I_PET][dm] += veg.getPET();

  outputm[I_SNWINF][dm] += soil.getSNOWINF();

  outputm[I_WYLD][dm] += soil.getH2OYLD();



  // Carbon in Human product pools

  outputm[I_AGPRDC][dm] = ag.getPROD1C();

  outputm[I_CLIPPINGS][dm] = ag.getCLIPPINGS();

  outputm[I_PROD10C][dm] = ag.getPROD10C();

  outputm[I_PROD100C][dm] = ag.getPROD100C();

  outputm[I_TOTPRDC][dm] = ag.getTOTPRODC();

  // Carbon in crop residue pool

  outputm[I_RESIDC][dm] = ag.getCROPRESIDUEC();

  outputm[I_AGSTUBC][dm] = ag.getSTUBBLEC();

  // Nitrogen in Human product pools

  outputm[I_AGPRDN][dm] = ag.getPROD1N();

  outputm[I_PROD10N][dm] = ag.getPROD10N();

  outputm[I_PROD100N][dm] = ag.getPROD100N();

  outputm[I_TOTPRDN][dm] = ag.getTOTPRODN();

  // Nitrogen in crop residue pool

  outputm[I_RESIDN][dm] = ag.getCROPRESIDUEN();

  outputm[I_AGSTUBN][dm] = ag.getSTUBBLEN();

  // Monthly carbon fluxes associated with
  //  agricultural conversion

  outputm[I_CNVRTC][dm] += ag.getCONVRTFLXC();

  outputm[I_VCNVRTC][dm] += ag.getVCONVRTFLXC();

  outputm[I_SCNVRTC][dm] += ag.getSCONVRTFLXC();

  outputm[I_SLASHC][dm] += ag.getSLASHC();

  outputm[I_STANDDEADC][dm] += ag.getSTANDDEADC();

  outputm[I_VOLAC][dm] += ag.getVOLAC();

  outputm[I_CFLX][dm] += ag.getCFLUX();

  // Monthly nitrogen fluxes associated with
  //  agricultural conversion

  outputm[I_CNVRTN][dm] += ag.getCONVRTFLXN();

  outputm[I_VCNVRTN][dm] += ag.getVCONVRTFLXN();

  outputm[I_SCNVRTN][dm] += ag.getSCONVRTFLXN();

  outputm[I_SLASHN][dm] += ag.getSLASHN();

  outputm[I_STANDDEADN][dm] += ag.getSTANDDEADN();

  outputm[I_VOLAN][dm] += ag.getVOLAN();

  outputm[I_NRETNT][dm] += ag.getNRETENT();

  outputm[I_NVRTNT][dm] += ag.getNVRETENT();

  outputm[I_NSRTNT][dm] += ag.getNSRETENT();

  // Monthly carbon and nitrogen fluxes from agricultural
  //   ecosystems

  outputm[I_AGFPRDC][dm] += ag.getCROPPRODC();
  outputm[I_AGFPRDN][dm] += ag.getCROPPRODN();

  outputm[I_FRESIDC][dm] += ag.getFORMCROPRESIDUEC();
  outputm[I_FRESIDN][dm] += ag.getFORMCROPRESIDUEN();

  outputm[I_AGPRDFC][dm] += ag.getPROD1DECAYC();
  outputm[I_AGPRDFN][dm] += ag.getPROD1DECAYN();

  outputm[I_RESIDFC][dm] += ag.getCROPRESIDUEFLXC();
  outputm[I_RESIDFN][dm] += ag.getCROPRESIDUEFLXN();


  // Monthly carbon and nitrogen fluxes from products

  outputm[I_PRDF10C][dm] += ag.getFORMPROD10C();
  outputm[I_PRDF10N][dm] += ag.getFORMPROD10N();

  outputm[I_PRD10FC][dm] += ag.getPROD10DECAYC();
  outputm[I_PRD10FN][dm] += ag.getPROD10DECAYN();

  outputm[I_PRDF100C][dm] += ag.getFORMPROD100C();
  outputm[I_PRDF100N][dm] += ag.getFORMPROD100N();

  outputm[I_PRD100FC][dm] += ag.getPROD100DECAYC();
  outputm[I_PRD100FN][dm] += ag.getPROD100DECAYN();

  outputm[I_TOTFPRDC][dm] += ag.getFORMTOTPRODC();
  outputm[I_TOTFPRDN][dm] += ag.getFORMTOTPRODN();

  outputm[I_TOTPRDFC][dm] += ag.getTOTPRODDECAYC();
  outputm[I_TOTPRDFN][dm] += ag.getTOTPRODDECAYN();

//  Climate Variables

    outputmcl[0][dm] += atms.getRAIN();
    outputmcl[1][dm] += atms.getSNOWFALL();
    outputmcl[2][dm] += ag.getGROWDD()/mondays;
    outputmcl[3][dm] += atms.getTAIR()/mondays;
    outputmcl[4][dm] += atms.getTAIRD()/mondays;
    outputmcl[5][dm] += atms.getTAIRN()/mondays;
    outputmcl[6][dm] += veg.getSHFLUXW()/mondays;
    outputmcl[7][dm] += soil.getSWP()/mondays;
    outputmcl[8][dm] += veg.pen.getVEGH()/mondays;
    outputmcl[9][dm] += veg.pen.getUSTAR()/mondays;
    outputmcl[10][dm] += veg.pen.getZD()/mondays;
    outputmcl[11][dm] += veg.pen.getZO()/mondays;
    outputmcl[12][dm] += veg.pen.getR_AA()/mondays;
    outputmcl[13][dm] += veg.pen.getR_AC()/mondays;
    outputmcl[14][dm] += veg.pen.getR_AS()/mondays;
    outputmcl[15][dm] += veg.pen.getR_SS()/mondays;
    outputmcl[16][dm] += atms.getVPR()/mondays;
    outputmcl[17][dm] += atms.getVPDD()/mondays;
    outputmcl[18][dm] += atms.getVPDN()/mondays;
    outputmcl[19][dm] += soil.getAVLH2O()/mondays;
    outputmcl[20][dm] += atms.getNIRR()/mondays;
    outputmcl[21][dm] += atms.getPAR()/mondays;
    outputmcl[22][dm] += atms.getGIRR()/mondays;
    outputmcl[23][dm] += atms.getCLDS()/mondays;

  //  Output agricultural area-specific vs natural area-specific
  //    results

  if( 0 == ag.state )
  {
    outputm[I_CROPC][dm] = ZERO;
    outputm[I_NATVEGC][dm] = y[I_LEAFC] + y[I_SAPWOODC]+ y[I_HEARTWOODC] + y[I_ROOTC] + y[I_LABILEC];

    outputm[I_CROPN][dm] = ZERO;
    outputm[I_NATVEGN][dm] = y[I_LEAFN] + y[I_SAPWOODN]+ y[I_HEARTWOODN] + y[I_ROOTN] + y[I_LABILEN];

    outputm[I_CSTRN][dm] = ZERO;
    outputm[I_NATSTRN][dm] = ZERO;

    outputm[I_CSTON][dm] = ZERO;
    outputm[I_NATSTON][dm] = y[I_LABILEN];

    outputm[I_CROPLAI][dm] = ZERO;
    outputm[I_NATLAI][dm] = veg.getLAI();

    outputm[I_CROPFPC][dm] = ZERO;
    outputm[I_NATFPC][dm] = y[I_FPC];

    outputm[I_AGINGPP][dm] = ZERO;
    outputm[I_NATINGPP][dm] += y[I_INGPP];

    outputm[I_AGGPP][dm] = ZERO;
    outputm[I_NATGPP][dm] += y[I_GPP];

    outputm[I_AGINNPP][dm] = ZERO;
    outputm[I_NATINNPP][dm] += y[I_INNPP];

    outputm[I_AGNPP][dm] = ZERO;
    outputm[I_NATNPP][dm] += y[I_NPP];

    outputm[I_AGGPR][dm] = ZERO;
    outputm[I_NATGPR][dm] += y[I_GPR];

    outputm[I_AGRVMNT][dm] = ZERO;
    outputm[I_NATRVMNT][dm] += y[I_RVMNT];

    outputm[I_AGRVGRW][dm] = ZERO;
    outputm[I_NATRVGRW][dm] += y[I_RVGRW];

    outputm[I_AGLTRC][dm] = ZERO;
    outputm[I_NATLTRC][dm] += y[I_RVGRW];

    outputm[I_AGINNUP][dm] = ZERO;
    outputm[I_NATINNUP][dm] += y[I_INNUP];

    outputm[I_AGVNUP][dm] = ZERO;
    outputm[I_NATVNUP][dm] += y[I_VNUP];

    outputm[I_AGLTRN][dm] = ZERO;
    outputm[I_NATLTRN][dm] += y[I_LTRLN] + y[I_LTRSN]
                + y[I_LTRHN] + y[I_LTRRN];
  }
  else
  {
    outputm[I_CROPC][dm] = y[I_LEAFC] + y[I_SAPWOODC]+ y[I_HEARTWOODC] + y[I_ROOTC] + y[I_LABILEC];
    outputm[I_NATVEGC][dm] = ZERO;

    outputm[I_CROPN][dm] = y[I_LEAFN] + y[I_SAPWOODN]+ y[I_HEARTWOODN] + y[I_ROOTN] + y[I_LABILEN];
    outputm[I_NATVEGN][dm] = ZERO;

    outputm[I_CSTRN][dm] = y[I_LEAFN] + y[I_SAPWOODN] + y[I_HEARTWOODN] + y[I_ROOTN];
    outputm[I_NATSTRN][dm] = ZERO;

    outputm[I_CSTON][dm] = y[I_LABILEN];
    outputm[I_NATSTON][dm] = ZERO;

    outputm[I_CROPLAI][dm] = veg.getLAI();
    outputm[I_NATLAI][dm] = ZERO;

    outputm[I_CROPFPC][dm] = y[I_FPC];
    outputm[I_NATFPC][dm] = ZERO;

    outputm[I_AGINGPP][dm] += y[I_INGPP];
    outputm[I_NATINGPP][dm] = ZERO;

    outputm[I_AGGPP][dm] += y[I_GPP];
    outputm[I_NATGPP][dm] = ZERO;

    outputm[I_AGINNPP][dm] += y[I_INNPP];
    outputm[I_NATINNPP][dm] = ZERO;

    outputm[I_AGNPP][dm] += y[I_NPP];
    outputm[I_NATNPP][dm] = ZERO;

    outputm[I_AGGPR][dm] += y[I_GPR];
    outputm[I_NATGPR][dm] = ZERO;

    outputm[I_AGRVMNT][dm] += y[I_RVMNT];
    outputm[I_NATRVMNT][dm] = ZERO;

    outputm[I_AGRVGRW][dm] += y[I_RVGRW];
    outputm[I_NATRVGRW][dm] = ZERO;

    outputm[I_AGLTRC][dm] += y[I_LTRLC] + y[I_LTRSC]
                + y[I_LTRHC] + y[I_LTRRC];
    outputm[I_NATLTRC][dm] = ZERO;

    outputm[I_AGINNUP][dm] += y[I_INNUP];
    outputm[I_NATINNUP][dm] = ZERO;

    outputm[I_AGVNUP][dm] += y[I_VNUP];
    outputm[I_NATVNUP][dm] = ZERO;


    outputm[I_AGLTRN][dm] += y[I_LTRLN] + y[I_LTRSN]
                + y[I_LTRHN] + y[I_LTRRN];
    outputm[I_NATLTRN][dm] = ZERO;


  }
};
#endif
