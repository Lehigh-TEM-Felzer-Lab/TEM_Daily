/* *************************************************************
****************************************************************
HUMNACTD_CWD.CPP - describes human disturbances to natural
                     ecosystems

Modifications:

20060126 - DWK created by modifying humnact50b5.cpp
20060126 - DWK changed include from humnact50b5.h to
           humnact437.h
20060126 - DWK changed Humanact50:: to Humanact::
20060126 - DWK changed inheritance of ProcessXML50 to
           ProcessXML43 in Humanactflux()
20070105 - TWC changed Humanactflux:: to Humnact45::
2007 - TWC/BSF summary
     getecd: add GDDMIN, GDDSEED, GDDHARVST, tkill,
	         isPerennial
	 harvest: account for no N loss if not fertilized
	          and perennial crops
	 setAgricFlags: irrgflag and fertflag are new
	 updateCropResidueFluxes: special provisions if no harvest

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

#include<string>

  using std::string;

#include "humnactd_cwd.h"


/* *************************************************************
************************************************************* */

Humnact45::Humnact45() : ProcessXML45()
{

  int i;
  int dm;
  c2n = 54.29;
//  cfall = 0.20;
//  nfall = 0.20;

  state = 0;
  prvstate = 0;

  massbalflg = 1;
  fertflag = 0;
  irrgflag = 0;
  frostflag = 0;

  mez = CROPVEG - 1;

  productYear = 0;

  prevPROD1.carbon = ZERO;
  prevPROD1.nitrogen = ZERO;

  prevPROD10.carbon = ZERO;
  prevPROD10.nitrogen = ZERO;

  prevPROD100.carbon = ZERO;
  prevPROD100.nitrogen = ZERO;

  prevCropResidue.carbon = ZERO;
  prevCropResidue.nitrogen = ZERO;

  cropResidue.carbon = ZERO;
  cropResidue.nitrogen = ZERO;

  for( dm = 0; dm < 366; ++dm )
  {
    initPROD1[dm].carbon = ZERO;
    initPROD1[dm].nitrogen = ZERO;
  }

  for( i = 0; i < 10; ++i )
  {
    initPROD10[i].carbon = ZERO;
    initPROD10[i].nitrogen = ZERO;
  }

  for( i = 0; i < 100; ++i )
  {
    initPROD100[i].carbon = ZERO;
    initPROD100[i].nitrogen = ZERO;
  }

  forage.carbon = ZERO;
  forage.nitrogen = ZERO;

  manure.carbon = ZERO;
  manure.nitrogen = ZERO;

  animalresp = ZERO;

  urine = ZERO;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::conversion( const int& pdcmnt,
                             const double& slashc,
                             const double& slashn,
                             const double& vegc,
                             const double& vstrn,
                             const double& vston,
                             const double& soilc,
                             const double& soiln,
                             const double& cwdloss,
                             const int& time )
{
  // Assume annual conversion fluxes are partitioned equally
  //   across each of 12 months - determine monthly fluxes

//  slash.carbon = slashpar * vegc / (double) CYCLE;
  slash.carbon = slashc ;
//  slash.nitrogen = slashpar  * (vstrn + vston) / (double) CYCLE;
  slash.nitrogen = slashn;

  vconvrtflx.carbon = (vconvert * vegc + cwdloss * standdead.carbon) / (double) time;
  sconvrtflx.carbon =  (sconvert * soilc)/ (double) time;


  convrtflx.carbon = vconvrtflx.carbon + sconvrtflx.carbon;

  vconvrtflx.nitrogen = ((1.0 - nvretconv[pdcmnt])
                        * vconvert
                        * (vstrn + vston + cwdloss * standdead.nitrogen))
                        / (double) time;


  sconvrtflx.nitrogen = ((1.0 - nsretconv[pdcmnt])
                        * sconvert * soiln)
                        / (double) time;

  convrtflx.nitrogen = vconvrtflx.nitrogen + sconvrtflx.nitrogen;

  nvretent = (nvretconv[pdcmnt] * vconvert * ((vstrn + vston) + cwdloss * standdead.nitrogen))
              / (double) time;

  nsretent = (nsretconv[pdcmnt] * sconvert * soiln)
              / (double) time;

  nretent = nvretent + nsretent;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::createWoodProducts( const int& pdyr,
                                     const double& stemc,
                                     const double& stemn )
{
  int i;

  formPROD10.carbon  = prod10par * stemc;
  formPROD10.nitrogen  = prod10par * stemn;
  formPROD100.carbon = prod100par * stemc;
  formPROD100.nitrogen = prod100par * stemn;

  if( pdyr < productYear )
  {
    productYear += pdyr;
  }
  else { productYear = pdyr; }

  i = productYear%10;
  initPROD10[i].carbon = formPROD10.carbon;
  initPROD10[i].nitrogen = formPROD10.nitrogen;

  i = productYear%100;
  initPROD100[i].carbon = formPROD100.carbon;
  initPROD100[i].nitrogen = formPROD100.nitrogen;

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Humnact45::decayProducts(  const int& CYCLE )
{

  int i;
  int dm;
  double yrtrash;


  PROD1decay.carbon  = ZERO;
  PROD1decay.nitrogen  = ZERO;

  for( dm = 0; dm < CYCLE; ++dm )
  {
    PROD1decay.carbon += initPROD1[dm].carbon / (double) CYCLE;

    PROD1decay.nitrogen += initPROD1[dm].nitrogen
                          / (double) CYCLE;
  }

  PROD10decay.carbon  = ZERO;
  PROD10decay.nitrogen  = ZERO;

  for( i = 0; i < 10; ++i )
  {
    yrtrash = initPROD10[i].carbon * 0.10 / (double) CYCLE;
    PROD10decay.carbon += yrtrash;
    yrtrash = initPROD10[i].nitrogen * 0.10 / (double) CYCLE;
    PROD10decay.nitrogen += yrtrash;
  }


  PROD100decay.carbon = ZERO;
  PROD100decay.nitrogen = ZERO;

  for( i = 0; i < 100; ++i )
  {
    yrtrash = initPROD100[i].carbon * 0.01 / (double) CYCLE;
    PROD100decay.carbon += yrtrash;
    yrtrash = initPROD100[i].nitrogen * 0.01 / (double) CYCLE;
    PROD100decay.nitrogen += yrtrash;
  }

  TOTPRODdecay.carbon = PROD1decay.carbon
                        + PROD10decay.carbon
                        + PROD100decay.carbon
                        + animalresp;

  TOTPRODdecay.nitrogen = PROD1decay.nitrogen
                          + PROD10decay.nitrogen
                          + PROD100decay.nitrogen;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::frostDamage( const double& vegc,
                              const double& vegn )
{
  frostflag = 1;

  // Assume all crop biomass becomes stubble

  stubble.carbon = vegc;
  stubble.nitrogen = vegn;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::getecd( const string& ecd )
{
  ifstream infile;
  int dcmnt;
  int comtype;

  infile.open( ecd.c_str(), ios::in );

  if( !infile )
  {
    cerr << "\nCannot open " << ecd;
    cerr << " for agriculture ECD input" << endl;

    exit( -1 );
  }

  getXMLrootNode( infile, "agECD" );

  for( dcmnt = 1; dcmnt < MAXCMNT; ++dcmnt )
  {
    comtype = getXMLcommunityNode( infile, "agECD" );

    if( comtype >= MAXCMNT )
    {
      cerr << endl << "comtype is >= MAXCMNT" << endl;
      cerr << "comtype cannot be greater than " << (MAXCMNT-1) << endl;
      cerr << " in agECD" << endl;

      exit( -1 );
    }


    nvretconv[comtype] = getXMLcmntArrayDouble( infile,
                                                "agECD",
                                                "nvretconv",
                                                comtype );

    nsretconv[comtype] = getXMLcmntArrayDouble( infile,
                                                "agECD",
                                                "nsretconv",
                                                comtype );

    tillfactor[comtype] = getXMLcmntArrayDouble( infile,
                                                 "agECD",
                                                 "tillfactor",
                                                 comtype );

    harvstC[comtype] = getXMLcmntArrayDouble( infile,
                                              "agECD",
                                              "harvstC",
                                              comtype );

    harvstN[comtype] = getXMLcmntArrayDouble( infile,
                                              "agECD",
                                              "harvstN",
                                              comtype );

    residueC[comtype] = getXMLcmntArrayDouble( infile,
                                               "agECD",
                                               "residueC",
                                               comtype );

    residueN[comtype] = getXMLcmntArrayDouble( infile,
                                               "agECD",
                                               "residueN",
                                               comtype );

    cropseedC[comtype] = getXMLcmntArrayDouble( infile,
                                                "agECD",
                                                "cropseedC",
                                                comtype );


    cropseedSTON[comtype] = getXMLcmntArrayDouble( infile,
                                               "agECD",
                                               "cropseedSTON",
                                               comtype );

    GDDMIN[comtype] = getXMLcmntArrayDouble( infile,
                                           "agECD",
                                           "gddmin",
                                           comtype );

    GDDSEED[comtype] = getXMLcmntArrayDouble( infile,
                                            "agECD",
                                            "gddseed",
                                            comtype );

    GDDHARVST[comtype] = getXMLcmntArrayDouble( infile,
                                             "agECD",
                                             "gddharvst",
                                             comtype );

    tkill[comtype] = getXMLcmntArrayDouble( infile,
                                          "agECD",
                                          "tkill",
                                          comtype );

    isPerennial[comtype] = getXMLcmntArrayInt( infile,
                                             "agECD",
                                             "isPerennial",
                                             comtype );


    endXMLcommunityNode( infile );
  }

  if( dcmnt < MAXCMNT )
  {
    cerr << endl << " Parameters found for only " << dcmnt;
    cerr << " community types out of a maximum of ";
    cerr << (MAXCMNT-1) << " types in agECD" << endl;

    exit( -1 );
  }

  infile.close();

};

/* *************************************************************
************************************************************* */
void Humnact45::grazing( const double& leafc,
                          const double& sapwoodc,
                          const double& heartwoodc,
                          const double& labilec,
                          const double& leafn,
                          const double& sapwoodn,
                          const double& heartwoodn,
                          const double& labilen,
                          const double& rootc )
{
  // Assume 7.5 percent of vegetation biomass is consumed each
  //   month

double frcab, grazint;

//  grazint = 0.075;
  grazint = 0.05/30.5;

  if(leafc+sapwoodc+heartwoodc+rootc == 0.0)
  {
    frcab = 0.0;
  }
  else
  {
  frcab = (leafc+sapwoodc+heartwoodc)/(leafc+sapwoodc+heartwoodc+rootc);
  }


  forage.cleaf = leafc * grazint;
  forage.csap = sapwoodc * grazint;
  forage.cheart = heartwoodc * grazint;
  forage.clabile = labilec*frcab*grazint;

  forage.nleaf = leafn * grazint;
  forage.nsap = sapwoodn * grazint;
  forage.nheart = heartwoodn * grazint;
  forage.nlabile = labilen*frcab*grazint;

  // Assume 83 percent of forage carbon is respired and 50
  //   percent of nitrogen is mineralized in livestock
  //   metabolism

  animalresp = (forage.cleaf+forage.csap+forage.cheart+forage.clabile) * 0.83;

  urine = (forage.nleaf+forage.nsap+forage.nheart+forage.nlabile) * 0.50;

  // Assume remaineder of forage is returned to soil as manure

  manure.carbon = (forage.cleaf+forage.csap+forage.cheart+forage.clabile) * 0.17;

  manure.nitrogen = (forage.nleaf+forage.nsap+forage.nheart+forage.nlabile) * 0.50;

};


/* *************************************************************
************************************************************* */

void Humnact45::harvest( const int& pdm,
                          const double& seedc,
                          const double& seedn,
                          const double& vegc,
                          const double& vegn )
{

  double initresidueC;
  double initresidueN;

// Determine crop production

  cropprod.carbon = seedc;
//  cropprod.carbon = vegc * 0.4;

  if ( cropprod.carbon < ZERO )
  {
    cropprod.carbon = ZERO;
  }
//
//   BSF/TWC update to account for no N loss ag scenario if no fert
//
//  fert1950flag = 0;
  if(fert1950flag == 1)
  {
    cropprod.nitrogen = seedn;
  }
  else
  {
    cropprod.nitrogen = ZERO;
  }

//cropprod.nitrogen = seedn;
  if ( cropprod.nitrogen < ZERO )
  {
    cropprod.nitrogen = ZERO;
  }

  initPROD1[pdm].carbon  = cropprod.carbon;
  initPROD1[pdm].nitrogen  = cropprod.nitrogen;

  // Determine amount of carbon and nitrogen left
  // in crop residue

  if ( 0 == isPerennial[cmnt] )
  {
    initresidueC = vegc - cropprod.carbon;
  }
  else { initresidueC = ZERO; }

  if ( initresidueC < ZERO )
  {
    initresidueC = ZERO;
  }
//
//  TWC/BSF  Correction for no N loss case with perennial crops
//
  if ( 0 == isPerennial[cmnt] )
  {
    initresidueN = vegn - cropprod.nitrogen;
  }
  else
  {
    if ( fert1950flag == 1 )
    {
      initresidueN = cropprod.nitrogen;
    }
	else
	{
	  initresidueN = ZERO;
	}
//    initresidueN = cropprod.nitrogen;
  }

  if ( initresidueN < ZERO )
  {
    initresidueN = ZERO;
  }

  // Determine amount of carbon and nitrogen in
  // crop residue that will be lost from ecosystem

  formCropResidue.carbon = initresidueC
                           * residueC[cmnt];
  formCropResidue.nitrogen = initresidueN
                             * residueN[cmnt];


  // Determine amount of stubble carbon and nitrogen
  // added to soils

  stubble.carbon = initresidueC
                   * ( 1.0 - residueC[cmnt]);
  stubble.nitrogen = initresidueN
                     * ( 1.0 - residueN[cmnt]);
//cout << "stubble terms = " << stubble.carbon << " " << initresidueC << " " << vegc << " " << seedc << " " << cropprod.carbon << endl;
//  fert1950flag = 1;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::resetMonthlyDisturbFluxes( void )
{
  // Initialize disturbance carbon fluxes to zero

  convrtflx.carbon = ZERO;
  vconvrtflx.carbon = ZERO;
  sconvrtflx.carbon = ZERO;
  slash.carbon = ZERO;

    // Initialize nitrogen fluxes during conversion to zero

  convrtflx.nitrogen = ZERO;
  vconvrtflx.nitrogen = ZERO;
  sconvrtflx.nitrogen = ZERO;
  slash.nitrogen = ZERO;

  nretent = ZERO;
  nvretent = ZERO;
  nsretent = ZERO;

  cropResidueFlux.carbon = ZERO;
  cropResidueFlux.nitrogen = ZERO;

};

/* *************************************************************
************************************************************* */



/* *************************************************************
************************************************************* */

void Humnact45::resetMonthlyFluxes( void )
{
  // Reset monthly fluxes to zero

  fertn = ZERO;

  irrigate = ZERO;

  // Initialize stubble to zero

  stubble.carbon = ZERO;
  stubble.nitrogen = ZERO;

  cflux = ZERO;

  // Initialize carbon fluxes related to formation and
  //   decomposition of products and crop residue to zero

  cropprod.carbon = ZERO;
  cropprod.nitrogen = ZERO;

  formCropResidue.carbon = ZERO;
  formCropResidue.nitrogen = ZERO;

  PROD1decay.carbon = ZERO;
  PROD1decay.nitrogen = ZERO;

  formPROD10.carbon = ZERO;
  formPROD10.nitrogen = ZERO;

  PROD10decay.carbon = ZERO;
  PROD10decay.nitrogen = ZERO;

  formPROD100.carbon = ZERO;
  formPROD100.nitrogen = ZERO;

  PROD100decay.carbon = ZERO;
  PROD100decay.nitrogen = ZERO;

  formTOTPROD.carbon = ZERO;
  formTOTPROD.nitrogen = ZERO;

  TOTPRODdecay.carbon = ZERO;
  TOTPRODdecay.nitrogen = ZERO;


};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::resetPROD(  const int& CYCLE )
{
  int i;
  int dm;

  prevPROD1.carbon = ZERO;
  prevPROD1.nitrogen = ZERO;

  prevPROD10.carbon = ZERO;
  prevPROD10.nitrogen = ZERO;

  prevPROD100.carbon = ZERO;
  prevPROD100.nitrogen = ZERO;

  prevCropResidue.carbon = ZERO;
  prevCropResidue.nitrogen = ZERO;

  cropResidue.carbon = ZERO;
  cropResidue.nitrogen = ZERO;


  for( dm = 0; dm < CYCLE; ++dm )
  {
    initPROD1[dm].carbon = ZERO;
    initPROD1[dm].nitrogen = ZERO;
  }

  for( i = 0; i < 10; ++i )
  {
    initPROD10[i].carbon = ZERO;
    initPROD10[i].nitrogen = ZERO;
  }
  for( i = 0; i < 100; ++i )
  {
    initPROD100[i].carbon = ZERO;
    initPROD100[i].nitrogen = ZERO;
  }

  PROD1.carbon = ZERO;
  PROD1.nitrogen = ZERO;

  PROD10.carbon = ZERO;
  PROD10.nitrogen = ZERO;

  PROD100.carbon = ZERO;
  PROD100.nitrogen = ZERO;

  TOTPROD.carbon = ZERO;
  TOTPROD.nitrogen = ZERO;

  PROD1decay.carbon = ZERO;
  PROD1decay.nitrogen = ZERO;

  formPROD10.carbon  = ZERO;
  formPROD10.nitrogen  = ZERO;

  PROD10decay.carbon  = ZERO;
  PROD10decay.nitrogen  = ZERO;

  formPROD100.carbon = ZERO;
  formPROD100.nitrogen = ZERO;

  PROD100decay.carbon = ZERO;
  PROD100decay.nitrogen = ZERO;

  formTOTPROD.carbon = ZERO;
  formTOTPROD.nitrogen = ZERO;

  TOTPRODdecay.carbon = ZERO;
  TOTPRODdecay.nitrogen = ZERO;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::resetYrFluxes( void )
{
  // Reset annual fluxes and summary variables to zero

  yrfertn = ZERO;

  yrirrig = ZERO;
  
  yrgrowdd = ZERO;
  yrfrost = ZERO;

  yrstubC = ZERO;
  yrstubN = ZERO;

  // Annual carbon fluxes from agricultural conversion

  yrconvrtC = ZERO;
  yrvconvrtC = ZERO;
  yrsconvrtC = ZERO;
  yrslashC = ZERO;
  yrcflux = ZERO;

 // Annual nitrogen fluxes from agricultural conversion

  yrconvrtN = ZERO;
  yrvconvrtN = ZERO;
  yrsconvrtN = ZERO;
  yrslashN = ZERO;
  yrnrent = ZERO;
  yrnvrent = ZERO;
  yrnsrent = ZERO;

  // Annual carbon and nitrogen fluxes in the formation of
  //   agricultural products

  yrformPROD1C   = ZERO;
  yrformPROD1N   = ZERO;


 // Annual carbon and nitrogen flux from crop residue formation

  yrformResidueC = ZERO;
  yrformResidueN = ZERO;

  // Annual carbon and nitrogen fluxes in the decomposition of
  //   agricultural products

  yrdecayPROD1C   = ZERO;
  yrdecayPROD1N   = ZERO;

 // Annual carbon and nitrogen fluxes from burning crop residue

  yrfluxResidueC = ZERO;
  yrfluxResidueN = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   formation of 10-year wood products

  yrformPROD10C  = ZERO;
  yrformPROD10N  = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   decomposition of 10-year wood products

  yrdecayPROD10C  = ZERO;
  yrdecayPROD10N  = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   formation of 100-year wood products

  yrformPROD100C = ZERO;
  yrformPROD100N = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   decomposition of 100-year wood products

  yrdecayPROD100C = ZERO;
  yrdecayPROD100N = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   formation of all agricultural and wood products

  yrformTOTPRODC = ZERO;
  yrformTOTPRODN = ZERO;

  // Annual carbon and nitrogen fluxes resulting from
  //   decomposition of all agricultural and wood products

  yrdecayTOTPRODC = ZERO;
  yrdecayTOTPRODN = ZERO;

};

/* *************************************************************
************************************************************* */

void Humnact45::setAgricFlags( ofstream& rflog1 )
{

  cout << "Are agricultural soils tilled?" << endl;
  cout << "Enter 0 for no:" << endl;
  cout << "Enter 1 for yes:" << endl;

  cin >> tillflag;

  rflog1 << "Are agricultural soils tilled?" << endl;
  rflog1 << "Enter 0 for no:" << endl;
  rflog1 << "Enter 1 for yes:" << endl;
  rflog1 << tillflag << endl << endl;

};

/* *************************************************************
************************************************************* */

void Humnact45::setFireNDEP( void )
{
  double newndep;
  double frftemp;

  // Determine firendep from total nitrogen volatilized during
  //   fire for reintroduction to the soil (Note:
  //   vconvrtflx.nitrogen and sconvrtflx.nitrogen are monthly
  //   fluxes that occur over a time period of a year

  newndep = vconvrtflx.nitrogen + sconvrtflx.nitrogen;

  // Determine the potential number of years until next fire event
  //   (i.e. Fire Return Interval or FRI)

  frftemp = FRI;

  if ( frftemp > MAXFRI ) { frftemp = MAXFRI; }

  // Assume that (1/FRI) of newndep is deposited each month

  firendep = newndep / ( frftemp -1 );

};

/* *************************************************************
************************************************************* */

void Humnact45::setNoCrops( const int& pdm )
{
  cropprod.carbon = ZERO;
  cropprod.nitrogen = ZERO;

  formCropResidue.carbon = ZERO;
  formCropResidue.nitrogen = ZERO;

  if( frostflag != 1 )
  {
    stubble.carbon = ZERO;
    stubble.nitrogen = ZERO;
  }

  frostflag = 0;

  initPROD1[pdm].carbon = ZERO;
  initPROD1[pdm].nitrogen = ZERO;


};

/* *************************************************************
************************************************************* */

void Humnact45::setNoGrazing( void )
{
  forage.cleaf = ZERO;
  forage.csap = ZERO;
  forage.cheart = ZERO;
  forage.clabile = ZERO;
  forage.nleaf = ZERO;
  forage.nsap = ZERO;
  forage.nheart = ZERO;
  forage.nlabile = ZERO;

  manure.carbon = ZERO;
  manure.nitrogen = ZERO;

  animalresp = ZERO;

  urine = ZERO;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::setNoWoodProducts( const int& pdyr )
{
  int i;

  formPROD10.carbon = ZERO;
  formPROD10.nitrogen = ZERO;

  formPROD100.carbon = ZERO;
  formPROD100.nitrogen = ZERO;

  if( pdyr < productYear )
  {
    productYear += pdyr;
  }
  else { productYear = pdyr; }

  i = productYear%10;
  initPROD10[i].carbon = ZERO;
  initPROD10[i].nitrogen = ZERO;

  i = productYear%100;
  initPROD100[i].carbon = ZERO;
  initPROD100[i].nitrogen = ZERO;

};


/* *************************************************************
************************************************************* */
/* *************************************************************
 * ************************************************************* */
void Humnact45::standingdead( const int& pdcmnt,
                             const double& slashc,
                             const double& slashn,
                             const double& vegc,
                             const double& vstrn,
                             const double& cwdloss )
{
// Assume annual conversion fluxes are partitioned equally
//   across each of 12 months - determine monthly fluxes
  
    slash.carbon = slashc ;
    slash.nitrogen = slashn;
  
 
//cout << "slashc = " << vegc << " " << standdead.carbon << endl;

    standdead.carbon -= cwdloss * standdead.carbon;
    standdead.nitrogen -= cwdloss * standdead.nitrogen;
    standdead.carbon +=  vegc;
    standdead.nitrogen += vstrn;

//cout << "slash_after = " << vegc << " " << standdead.carbon << endl;

//    vola.carbon += (2./3.) * vegc;
//    vola.nitrogen = 0.0;
  if(standdead.carbon < 0.0) {standdead.carbon = 0.0;}
  if(standdead.nitrogen < 0.0) {standdead.nitrogen = 0.0;}

};

/* *************************************************************
************************************************************* */

void Humnact45::updatestanddead(const int& pdyr,
                                const double& rhmoist,
                                const double& dq10,
                                const double& cnltr)
{

  double c_add, n_add;
  if(standdead.carbon > 0){
  c_add = standdead.carbon * (11./36500.) * rhmoist * dq10;
  n_add = standdead.nitrogen * (11./36500.) * rhmoist * dq10;

  slash.carbon += (1./3.) * c_add;       //since this happens each month, decay only 0.1 * 1/12
  slash.nitrogen += (1./3.) * n_add;
  vola.carbon = (2./3.) * c_add;
  vola.nitrogen = (2./3.) * n_add;
  standdead.carbon -= c_add;
  standdead.nitrogen -= n_add;
  if(standdead.carbon < 0.0) {standdead.carbon = 0.0;}
  if(standdead.nitrogen < 0.0) {standdead.nitrogen = 0.0;}
//  cout << "standdead = " << slash.carbon << " " << standdead.carbon << " " << slash.nitrogen << " " << standdead.nitrogen << endl;

  immbadd = (0.33 * c_add/cnltr) - 0.33 * n_add;
       }
  else
  {
  immbadd = 0.0;
  }
   }

/* *************************************************************
************************************************************* */

void Humnact45::updateCropResidue( void )
{

  cropResidue.carbon = prevCropResidue.carbon
                       + formCropResidue.carbon
                       - cropResidueFlux.carbon;

  cropResidue.nitrogen = prevCropResidue.nitrogen
                         + formCropResidue.nitrogen
                         - cropResidueFlux.nitrogen;

};
/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::updateCropResidueFluxes(  const int& CYCLE )
{
  int kdm;

  cropResidueFlux.carbon = ZERO;
  cropResidueFlux.nitrogen = ZERO;

  if ( 1 == state )
  {
    for( kdm = 0; kdm < CYCLE; ++kdm )
    {
      cropResidueFlux.carbon += initPROD1[kdm].carbon
                                * (1.000000 - harvstC[cmnt])
                                / harvstC[cmnt]
                                * residueC[cmnt]
                                / (double) CYCLE;

      cropResidueFlux.nitrogen += initPROD1[kdm].nitrogen
                                  * (1.000000 - harvstN[cmnt])
                                  / harvstN[cmnt]
                                  * residueN[cmnt]
                                  / (double) CYCLE;
    }
  }

};
/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::updateProducts(  const int& CYCLE )
{
  int i;

  // Carbon in Products

  PROD1.carbon = prevPROD1.carbon
                 + cropprod.carbon
                 - PROD1decay.carbon;

  if( PROD1.carbon < 0.1 )
  {
    PROD1.carbon = ZERO;
    for(i = 0; i < CYCLE; ++i )
    {
      initPROD1[i].carbon = ZERO;
      initPROD1[i].nitrogen = ZERO;
    }
  }

  PROD10.carbon = prevPROD10.carbon
                  + formPROD10.carbon
                  - PROD10decay.carbon;

  if( PROD10.carbon < 0.1 )
  {
    PROD10.carbon = ZERO;

    for(i = 0; i < 10; ++i )
    {
      initPROD10[i].carbon = ZERO;
      initPROD10[i].nitrogen = ZERO;
    }
  }

  PROD100.carbon = prevPROD100.carbon
                   + formPROD100.carbon
                   - PROD100decay.carbon;

  if( PROD100.carbon < 0.1 )
  {
    PROD100.carbon = ZERO;
    for(i = 0; i < 100; ++i )
    {
      initPROD100[i].carbon = ZERO;
      initPROD100[i].nitrogen = ZERO;
    }
  }

  TOTPROD.carbon = PROD1.carbon
                   + PROD10.carbon
                   + PROD100.carbon;

  // Nitrogen in Products

  PROD1.nitrogen = prevPROD1.nitrogen
                   + cropprod.nitrogen
                   - PROD1decay.nitrogen;

  if( PROD1.nitrogen < 0.000001 )
  {
    PROD1.nitrogen = ZERO;
  }

  PROD10.nitrogen = prevPROD10.nitrogen
                    + formPROD10.nitrogen
                    - PROD10decay.nitrogen;

  if( PROD10.nitrogen < 0.000001 )
  {
    PROD10.nitrogen = ZERO;
  }

  PROD100.nitrogen = prevPROD100.nitrogen
                     + formPROD100.nitrogen
                     - PROD100decay.nitrogen;

  if( PROD100.nitrogen < 0.000001 )
  {
    PROD100.nitrogen = ZERO;
  }

  TOTPROD.nitrogen = PROD1.nitrogen
                     + PROD10.nitrogen
                     + PROD100.nitrogen;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Humnact45::updateTotalProductFormation( void )
{
  // Carbon in Total Product Formation

  formTOTPROD.carbon = cropprod.carbon
                       + formPROD10.carbon
                       + formPROD100.carbon;

  // Nitrogen in Total Product Formation

  formTOTPROD.nitrogen = cropprod.nitrogen
                         + formPROD10.nitrogen
                         + formPROD100.nitrogen;


};

