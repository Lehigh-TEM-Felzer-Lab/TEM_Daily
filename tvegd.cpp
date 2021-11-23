/** *************************************************************
****************************************************************
TVEG45.CPP - object describing characteristics of vegetation
                used in the Terrestrial Ecosystem Model (TEM)
                
  (Comments below)
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
  using std::log;
  using std::pow;
  using std::fabs;
  using std::sqrt;
  using std::min;
  using std::max;

#include<string>

  using std::string;

#include "tvegd.h"

/**
    Static phenology variables, used in veg.phenology, are initialized
    the variable "phen" for each PFT is defined in the tveg.ecd file
*/
  enum{ I_EVERGREEN = 0, I_COLD_DEC = 1, I_STRESS_DEC = 2 };

//const int I_EVERGREEN = 0;
//const int I_COLD_DEC = 1;
//const int I_STRESS_DEC = 2;

Tveg45::Tveg45() : ProcessXML45()
{
  int dcmnt;

/**
    Tveg constructor: initialize values of most variables to "MISSING",
    a value defined in the temconsts file
*/
  
  plant.carbon = MISSING;
  plant.nitrogen = MISSING;
  plant.allocc = MISSING;
  plant.allocn = MISSING;
  plant.allocic = MISSING;
  plant.allocin = MISSING;
  plant.ltrc = MISSING;
  plant.ltrn = MISSING;
  plant.nresorb = MISSING;
  plant.rm = MISSING;

  strctrl.carbon = MISSING;
  strctrl.nitrogen = MISSING;
  strctrl.allocc = MISSING;
  strctrl.allocn = MISSING;
  strctrl.allocic = MISSING;
  strctrl.allocin = MISSING;
  strctrl.ltrc = MISSING;
  strctrl.ltrn = MISSING;
  strctrl.nresorb = MISSING;
  strctrl.rm = MISSING;

  labile.carbon = MISSING;
  labile.nitrogen = MISSING;
  labile.allocc = MISSING;
  labile.allocn = MISSING;
  labile.allocic = MISSING;
  labile.allocin = MISSING;
  labile.ltrc = MISSING;
  labile.ltrn = MISSING;
  labile.nresorb = MISSING;
  labile.rm = MISSING;

  leaf.carbon = MISSING;
  leaf.nitrogen = MISSING;
  leaf.allocc = MISSING;
  leaf.allocn = MISSING;
  leaf.allocic = MISSING;
  leaf.allocin = MISSING;
  leaf.ltrc = MISSING;
  leaf.ltrn = MISSING;
  leaf.nresorb = MISSING;
  leaf.rm = MISSING;

  sapwood.carbon = MISSING;
  sapwood.nitrogen = MISSING;
  sapwood.allocc = MISSING;
  sapwood.allocn = MISSING;
  sapwood.allocic = MISSING;
  sapwood.allocin = MISSING;
  sapwood.ltrc = MISSING;
  sapwood.ltrn = MISSING;
  sapwood.nresorb = MISSING;
  sapwood.rm = MISSING;

  heartwood.carbon = MISSING;
  heartwood.nitrogen = MISSING;
  heartwood.allocc = MISSING;
  heartwood.allocn = MISSING;
  heartwood.allocic = MISSING;
  heartwood.allocin = MISSING;
  heartwood.ltrc = MISSING;
  heartwood.ltrn = MISSING;

  root.carbon = MISSING;
  root.nitrogen = MISSING;
  root.allocc = MISSING;
  root.allocn = MISSING;
  root.allocic = MISSING;
  root.allocin = MISSING;
  root.ltrc = MISSING;
  root.ltrn = MISSING;
  root.nresorb = MISSING;
  root.rm = MISSING;

  seed.carbon = MISSING;
  seed.nitrogen = MISSING;
  seed.allocc = MISSING;
  seed.allocn = MISSING;
  seed.allocic = MISSING;
  seed.allocin = MISSING;
  seed.ltrc = MISSING;
  seed.ltrn = MISSING;
  seed.nresorb = MISSING;
  seed.rm = MISSING;


  inuptake = MISSING;
  nuptake = MISSING;

  ingpp = MISSING;
  gpp = MISSING;

  innpp = MISSING;
  npp = MISSING;

  rg = MISSING;
  gpr = MISSING;

  lai = MISSING;
  fpc = MISSING;


  inprodcn = MISSING;

  yralloclc = MISSING;
  yrallocsc = MISSING;
  yrallocrc = MISSING;
  
  yrallocilc = MISSING;
  yrallocisc = MISSING;
  yrallocirc = MISSING;
  
  yrallocseedc = MISSING;
  yrallocseedn = MISSING;

  yrcarbon = MISSING;
  yrnitrogen = MISSING;

  yrstructn = MISSING;
  yrc2n = MISSING;

  yrstoren = MISSING;

  yrltrc = MISSING;
  yrltrn = MISSING;

  yringpp = MISSING;
  yrgpp = MISSING;

  yrinnpp = MISSING;
  yrnpp = MISSING;

  yrgpr = MISSING;
  yrrmaint = MISSING;
  yrrmleaf = MISSING;
  yrrmsapwood = MISSING;
  yrrmroot = MISSING;

  yrrgrowth = MISSING;

  yrinnup = MISSING;

  yrnup = MISSING;

  inprodcn = MISSING;

  yrnrsorb = MISSING;

  yrfpc = MISSING;
  
  yrpleaf = MISSING;
  yrpsapwood = MISSING;
  yrproot = MISSING;
  yrpseed = MISSING;

  for( dcmnt = 0; dcmnt < MAXCMNT; ++dcmnt )
  {
    phen[dcmnt] = 0;
    
    cnltr[dcmnt] = MISSING;

    sla[dcmnt] = MISSING;

    cmax1b[dcmnt] = MISSING;

    kc[dcmnt] = MISSING;
    ki[dcmnt] = MISSING;

    tmax[dcmnt] = MISSING;
    tmin[dcmnt] = MISSING;
    toptmin[dcmnt] = MISSING;
    toptmax[dcmnt] = MISSING;

    kra[dcmnt] = MISSING;

    raalpha[dcmnt] = MISSING;
    rabeta[dcmnt] = MISSING;
    ragamma[dcmnt] = MISSING;
    raqref[dcmnt] = MISSING;
    ratref[dcmnt] = MISSING;

    nmax1b[dcmnt] = MISSING;

    kn1[dcmnt] = MISSING;

    tauleaf[dcmnt] = MISSING;
    tausapwood[dcmnt] = MISSING;
    tauheartwood[dcmnt] = MISSING;
    tauroot[dcmnt] = MISSING;
    tauseed[dcmnt] = MISSING;

  }

  dc2n = MISSING;
  adjc2n = MISSING;

  cmax = MISSING;

  topt = MISSING;
  newtopt = MISSING;
  toptmic = MISSING;

  kr = MISSING;

  nmax = MISSING;

  string temdiagfile = "tem_diag.txt";


};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Tveg45::aerodynamics( const int& pdcmnt,
						   const double& windspeed)
{
/**
    Calculate aerodynamic resistances within the soil-plant-atmosphere system.
    These aerodynamic resistances are then used to calculate energy and 
    water vapor exchanges (in petsw -> penmon.shuttlewallace).  
    Values are based on vegetation height, lai, and windspeed climatology.  
*/  
  
  double ustar, zh, zd, zo; //(u*, height, displacement height, roughness length)
  double xcan, cabove, dum1, dum2, za, ua;

  double n_eddy = 2.5;
  const double k_karman = 0.41;
  const double zb = 296.97; // see Federer, 2006
  const double zow = 0.005; // see Federer, 2006
  const double zw = 10.0;

  cabove = (leaf.carbon + heartwood.carbon + sapwood.carbon)/1000.0; //kgC m^-2 aboveground biomass
  
  if( cabove > 100.0) { cabove = 100.0; } // correction to avoid fatal errors below -- no vegC greater than 100 kg m^-2 allowed
  
  if(cabove > 0.00001) { zh = href[pdcmnt] * pow(cabove, kallom[pdcmnt]); }
  else { zh = href[pdcmnt] * pow(0.00001, kallom[pdcmnt]); }
  za = zh + 2.0; // reference height za set to 2 m above canopy (Federer, 1996)

  pen.setVEGH(zh);

  xcan = pen.getCDLEAF(pdcmnt) * lai;
  if (lai < 0.01) { xcan = pen.getCDLEAF(pdcmnt) * 0.01; }

  zd = min(1.1 * zh * log(1.0 + pow( xcan, 0.25 )) , 0.9 * zh); 
  zo = 0.3 * zh * min(0.01 + pow( xcan, 0.5 ), 1.0 - zd/zh);

  pen.setZD(zd);
  pen.setZO(zo);

  ua = windspeed * (log(zb/zow)/log(zb/zo))*(log((za-zd)/zo)/log(zw/zow));
  // conversion of weather-station windspeed at 10 m to windspeed at za 
  if( ua < 0.0 ) { cout << " fatal error! ua < 0 " << ua << " " << za << " " << zh 
             << " " << zd << " " << zo << " " << cabove << " " << labile.carbon << endl; }
  
  ustar = k_karman * ua / log((za-zd)/zo);
  pen.setUSTAR(ustar);

  if( ustar < 0.0 ) { cout << " fatal error! u* < 0 " << ustar << endl; }

  if( zh > 1.0) { n_eddy = 2.306 + 0.194 * zh; }
  if( zh > 10.0) { n_eddy = 4.25; } // see Zhou, 2006

//  cout << "diag = " << n_eddy << " " << zh << " " << zd << " " << zo << endl;
  dum1 = 100.0 * n_eddy * pow(pen.getWLEAF(pdcmnt) * k_karman, 0.5);
  dum2 = 2.0 * lai * pow(ustar * log((zh - zd)/zo), 0.5) * (1.0 - exp(-0.5*n_eddy));
  if (lai < 0.01) 
  { 
    dum2 = 2.0 * 0.01 * pow(ustar * log((zh - zd)/zo), 0.5) * (1.0 - exp(-0.5*n_eddy));
  }	  

  if (ua == 0.0)
  {
   pen.setR_AC(0.0);
   pen.setR_AS(0.0);
  }
  else
  {
  pen.setR_AC(dum1 / dum2);

  dum1 = zh * exp(n_eddy) * (1.0 - exp(-n_eddy*(zd+zo)/zh));
  dum2 = k_karman * ustar * n_eddy * (zh - zd);
  pen.setR_AS(dum1 / dum2);
 

  dum1 = zh * (exp(n_eddy*(1.0 - (zd+zo)/zh)) - 1.0)/(n_eddy*(zh-zd));
  dum2 = log((za - zd) / (zh - zd));
  pen.setR_AA((dum1 + dum2) / (ustar * k_karman));
  }

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */
void Tveg45::allocate( const int& pdcmnt,
                       const int& pndays,
                       const int& nfeed,
                       const int& pdm,
                       const double& gdd,
                       const int& agstate,
                       const int& agharvest  )
{
/** 
    determine allocation of labile carbon and nitrogen to structural pools.
    Approach is based on dividing allocation into "investment" and "windfall" 
    categories.
    
    "Investment" -- based on benefit:cost analysis of allocation for each of the
    3 structural compartments.  Leaves may benefit photosynthsis by adding
    to light-acquisition, but may harm it by increasing evaporative demand
    and thus drought stress.  Roots and stems both increase hydraulic
    conductivity, and thus alleviate drought stress, and roots also increase
    nitrogen uptake.  The costs of all compartments include construction and 
    maintenance. *.allocc, *.allocn   
    
    "Windfall" -- based on required allocation of labile carbon if the labile 
    pool grows larger than its allowed storage space. Only mechanism of 
    allocating to the seed pool. *.allocic, *.allocin
    
    Both investment- and  windfall- allocation rates may be constrained by the 
    amount of nitrogen in the labile pool -- investment allocation to any given 
    pool is not allowed to empty either labile nitrogen or labile carbon with a 
    characteristic time less than MC/(MB-MC) months.  Windfall allocation is not 
    allowed to empty the excess carbon with a time of less than 1 month. 
    
    Also determined here: growth respiration, which is deducted from the flux of 
    allocated carbon.   
*/  

  int lclimflag;
  
  double pseedc, pseedn;
  double tauseff; /// effective sapwood lifetime
  double norm;
  
  // Labile-carbon and -nitrogen emptying rates for investment-allocation
  double fleafc, fsapwoodc, frootc, ftotc;  
  double fleafn, fsapwoodn, frootn; 

  double mb_l, mcm_l, mcc_l; // marginal benefit and cost of incremental lai
  double mb_s, mcm_s, mcc_s; // marginal benefit and cost of incremental stem tissue
  double mb_r, mcm_r, mcc_r; // marginal benefit and cost of incremental roots
  
  // Labile-carbon and -nitrogen emptying rates for windfall-allocation
  double prootlc, prootln, psapwoodlc, psapwoodln, pleaflc, pleafln;
  
  double extra;

  double krgcmo;
  
  prootlc = ZERO;
  psapwoodlc = ZERO;
  pleaflc = ZERO; 
  
// convert respiration rate from micromol co2 m^-2 s^-1 to gC m^-2 month^-1
//  krgcmo = kra[pdcmnt]*(12.0*86400.0*pndays)/1000000.0;
  krgcmo = kra[pdcmnt]*(12.0*86400.0)/1000000.0;
  
// initialize investment-allocation fluxes
  leaf.allocic = ZERO;
  sapwood.allocic = ZERO;
  root.allocic = ZERO;
  heartwood.allocic = ZERO;
  seed.allocic = ZERO;
  
  leaf.allocin = ZERO;
  sapwood.allocin = ZERO;
  root.allocin = ZERO;
  heartwood.allocin = ZERO;
  seed.allocin = ZERO;

// assess benefit and cost of marginal lai, stem, roots
  mb_l = dgppdleafc;
  mb_s = dgppdstemc;
  mb_r = dgppdrootc;

//  cout << mb_l << " " << mb_s << " " << mb_r << endl;

  mcm_l = krgcmo*rmt/cnleaf[pdcmnt];
  mcm_s = krgcmo*fsaplive[pdcmnt]*rmt/cnsapwood[pdcmnt];
  mcm_r = krgcmo*rmt/cnroot[pdcmnt];
  
  switch( phen[pdcmnt] )
  {
    case I_EVERGREEN:
      //mcc_l = (1.25/(((rpleaf+pleafce[pdcmnt])/2.0)*12.0*tauleaf[pdcmnt]*sla[pdcmnt]))
      //       *(12.0*pndays/365.0);
      //mcc_l = (1.25/(pleafce[pdcmnt]*12.0*tauleaf[pdcmnt]))*(12.0*pndays/365.0);
//      mcc_l = 1.25/(12.0*tauleaf[pdcmnt])*12.0*pndays/365.0;
//      mcc_l = 1.25/(12.0*tauleaf[pdcmnt])*12.0*pndays/365.0;
      mcc_l = 1.25/(365.0*tauleaf[pdcmnt]);
      break;
    case I_COLD_DEC:
//      cout << "rphi = " << rphi << " " << phicnt << " " << 365.0*rphi-phicnt << " " << 365.0*rphi << " " << rphish << endl;
//      cout << "daylslope = " << pdm << " " << daylslope[pdm] << endl;
//      if((365.0*rphish-phicnt) > ZERO)
//      if((365.0*phi-phicnt) > 0.01)
      if((365.0*rphi-phicnt) > ZERO)
      {
        //mcc_l = (1.25/(rpleaf*(12.0*rphi-phicnt)))*(12.0*pndays/365.0);
//        mcc_l = 1.25/(12.0*rphi-phicnt)*12.0/365.0;
//        mcc_l = 1.25/(365.0*phi-phicnt);
        mcc_l = 1.25/(365.0*rphi-phicnt);
//        mcc_l = ((2.4/365.0)*exp(phicnt/152)-(2.4/365.0))*phi;
//        mcc_l = 1.25/(365.0*phi-phicnt);
//        mcc_l = 1.54/(365.0*rphi-phicnt);
//        mcc_l = 3.0/(365.0*rphi-phicnt);
//      cout << "mb_l, mcm_l,mcc_l = " << mb_l << " " << mcm_l << " " << mcc_l << " " << ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l)) << " " << phi <<   endl;
      }
//      else if( (365.0*phi > 350.0)||(phicnt > 335.0))
      else if( (365.0*rphi > 350.0)||(phicnt > 335.0))
      {
        // let vegetation obey evergreen rules
//        mcc_l = (1.25/(rpleaf*12.0*tauleaf[pdcmnt]*sla[pdcmnt]))*(12.0*pndays/365.0);
        //mcc_l = (1.25/((max(rpleaf , pleafce[pdcmnt]))*12.0*tauleaf[pdcmnt]))
        //     *(12.0*pndays/365.0);
//        mcc_l = 1.25/(12.0*tauleaf[pdcmnt])*12.0/365.0;
        mcc_l = 1.25/(365.0*tauleaf[pdcmnt]);
      }
      else
      {
        mcc_l = mb_l; // ensures that mb-mcc-mcm is negative, so no allocation occurs
      }
//     cout << "second mb_l, mcm_l,mcc_l = " << mb_l << " " << mcm_l << " " << mcc_l << " " << ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l)) << " " << phi << endl;
      break;
    case I_STRESS_DEC:
      //if((12.0*rphi-phicnt) > ZERO)
      if( phi == 1)
      {
        //mcc_l = 1.25/(12.0*rphi-phicnt)*12.0*pndays/365.0;
        //mcc_l = 1.25/12.0*pndays/365.0;
//        mcc_l = 1.25/2.0*12.0*pndays/365.0;
//        mcc_l = 1.25/2.0;
        mcc_l = 1.25/(365.0*rphi);    //BSF COMBO
      }
      //else if( (12.0*rphi > 11.5)||(phicnt > 11.0))
      //{
        // // let vegetation obey evergreen rules
        //mcc_l = 1.25/(12.0*tauleaf[pdcmnt])*12.0*pndays/365.0;
      //}
      else
      {
        mcc_l = mb_l; // ensures that mb-mcc-mcm is negative, so no allocation occurs
      }
      break;
  }
  
  tauseff = 1.0 / ( 1.0 / tausapwood[pdcmnt] + 1.0 / tauheartwood[pdcmnt] );
  
//  mcc_s = 1.25/(12.0*tauseff)*12.0*pndays/365.0;
//  mcc_s = 1.25/(12.0*tauseff)*12.0/365.0;
 mcc_s = 1.25/(365.0*tauseff);
//  mcc_r = 1.25/(12.0*tauroot[pdcmnt])*12.0*pndays/365.0;
//  mcc_r = 1.25/(12.0*tauroot[pdcmnt])*12.0/365.0;
  mcc_r = 1.25/(365.0*tauroot[pdcmnt]);
//
//   BSF COMBO START
//
  switch( phen[pdcmnt] )
  {
   case I_EVERGREEN:
//leaf net benefit:cost ratio
//  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > ZERO )
  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > 0.001 )
  {
//    fleafc = ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l))*(12.0*pndays/365.0);
    fleafc = ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l))*(12.0/365.0);
//    fleafc = ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l));
  }
  else
  {
    fleafc = ZERO;
  }

//stem net benefit:cost ratio  
//  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > ZERO )
  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > 0.001 )
  {
//    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s))*(12.0*pndays/365.0);
    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s))*(12.0/365.0);
//    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s));
  }
  else
  {
    fsapwoodc = ZERO;
  }

//cout << "fsapwoodc = " << fsapwoodc <<  " " << mb_s << " " << mcm_s << " " << mcc_s << endl;
//root net benefit:cost ratio   
//  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > ZERO )
  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > 0.001 )
  {
//    frootc = ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r))*(12.0*pndays/365.0);
    frootc = ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r))*(12.0/365.0);
//    frootc = ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r));
  }
  else
  {
    frootc = ZERO;
  }
  
  break;

    case I_COLD_DEC:

//leaf net benefit:cost ratio
//  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > ZERO )
  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > 0.001 )
  {
    fleafc =  ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l))*12.0/365.0;
//    fleafc =  ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l));
  }
  else
  {
    fleafc = ZERO;
  }

//stem net benefit:cost ratio
//  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > ZERO )
  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > 0.001 )
  {
    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s))*12.0/365.0;
//    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s));
  }
  else
  {
    fsapwoodc = ZERO;
  }

//root net benefit:cost ratio
//  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > ZERO )
  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > 0.001 )
  {
    frootc =((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r))*12.0/365.0;
//    frootc =((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r));
  }
  else
  {
    frootc = ZERO;
  }
  break;

    case I_STRESS_DEC:

//leaf net benefit:cost ratio
//  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > ZERO )
  if((mb_l-(mcm_l+mcc_l)) > ZERO && (mcm_l+mcc_l) > 0.001 )
  {
    fleafc =  0.75* ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l))*12.0/365.0;
//    fleafc =  0.75* ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l));
//    fleafc =  ((mb_l-(mcm_l+mcc_l))/(mcm_l+mcc_l))*(12.0*pndays/365.0);
  }
  else
  {
    fleafc = ZERO;
  }

//stem net benefit:cost ratio
//  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > ZERO )
  if((mb_s-(mcm_s+mcc_s)) > ZERO && (mcm_s+mcc_s) > 0.001 )
  {
    fsapwoodc = 0.75* ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s))*12.0/365.0;
//    fsapwoodc = 0.75* ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s));
//    fsapwoodc = ((mb_s-(mcm_s+mcc_s))/(mcm_s+mcc_s))*(12.0*pndays/365.0);
  }
  else
  {
    fsapwoodc = ZERO;
  }

//root net benefit:cost ratio
//  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > ZERO )
  if((mb_r-(mcm_r+mcc_r)) > ZERO && (mcm_r+mcc_r) > 0.001 )
  {
    frootc = 0.75* ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r))*12.0/365.0;
//    frootc = 0.75* ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r));
//    frootc = ((mb_r-(mcm_r+mcc_r))/(mcm_r+mcc_r))*(12.0*pndays/365.0);
  }
  else
  {
    frootc = ZERO;
  }
//  cout << "fleaf = " << fleafc << " " << fsapwoodc << " " << frootc << endl;
  break;
}
//  BSF COMBO END

  fleafc *= phi; // no allocation to leaves when phi is zero
//  if(rphish < 0.98) {fleafc = 0.0;}
//  fsapwoodc *= phi;
//  frootc *= phi;

//  cout << "alloc diag = " << mb_l << " " << mb_s << " " << mb_r << " " << mcm_l << " " << mcm_s << " " << mcm_r << " " << mcc_l << " " << mcc_s << " " << mcc_r << " " << fleafc << " " << fsapwoodc << " " << frootc << endl;
  
  ftotc = fleafc + fsapwoodc + frootc;

  if(ftotc < ZERO) { ftotc = ZERO; }
    
  if(ftotc > ZERO)
  { 
    // Nitrogen allocation fractions     
    norm = ftotc/(fleafc/cnleaf[pdcmnt]+fsapwoodc/cnsapwood[pdcmnt]+frootc/cnroot[pdcmnt]);
    fleafn = (fleafc/cnleaf[pdcmnt])*norm;
    fsapwoodn = (fsapwoodc/cnsapwood[pdcmnt])*norm;
    frootn = (frootc/cnroot[pdcmnt])*norm;

    // Initial C allocation
    leaf.allocic = labile.carbon*fleafc;
    sapwood.allocic = labile.carbon*fsapwoodc;
//    cout << "sapwood.allocic = " << sapwood.allocic <<  " " << labile.carbon << endl;
    root.allocic = labile.carbon*frootc;
//    seed.allocic = labile.carbon;

    // Initial N allocation
    leaf.allocin = labile.nitrogen*fleafn;
    sapwood.allocin = labile.nitrogen*fsapwoodn;
    root.allocin = labile.nitrogen*frootn;
//    seed.allocin = seed.allocic/cnseed[pdcmnt];
  }

// initialize total-allocation fluxes to investment-allocation
  
//  if(agstate == 1 && gdd > 1500)
  if(agstate == 1 && gdd > agharvest)
  {
  leaf.allocc = ZERO;
  sapwood.allocc = ZERO;
  root.allocc = ZERO;
  heartwood.allocc = ZERO;
// 3.0 = empty 95% of pool; 2.0 = empty 86% of pool; 1.0 = empty 63% of pool
  seed.allocc = 3.0*labile.carbon;
  }
  else
  {
  leaf.allocc = leaf.allocic;
  sapwood.allocc = sapwood.allocic;
  root.allocc = root.allocic;
//  heartwood.allocc = sapwood.carbon*pndays/(365.0*tausapwood[pdcmnt]);
  heartwood.allocc = sapwood.carbon/(365.0*tausapwood[pdcmnt]);
  seed.allocc = seed.allocic;
  }
  
//  cout << "diag = " << fleafc << " " << fsapwoodc << " " << frootc << " " << labile.carbon << endl;
//  if(agstate == 1 && gdd > 1500)
  if(agstate == 1 && gdd > agharvest)
  {
  leaf.allocn = ZERO;
  sapwood.allocn = ZERO;
  root.allocn = ZERO;
  heartwood.allocn = ZERO;
  seed.allocn = seed.allocc/cnseed[pdcmnt];
  }
  else
  {
  leaf.allocn = leaf.allocin;
  sapwood.allocn = sapwood.allocin;
  root.allocn = root.allocin;
//  heartwood.allocn = sapwood.nitrogen*pndays/(365.0*tausapwood[pdcmnt]);
  heartwood.allocn = sapwood.nitrogen/(365.0*tausapwood[pdcmnt]);
  seed.allocn = seed.allocin;
  }

//  if(agstate == 1 && pdm == 9 && labile.carbon > 0) {
  if(agstate == 1 && pdm == 245 && labile.carbon > 0) {
//     cout << "labile carbon  = " << labile.carbon << endl;
     seed.allocc += 3.0 * labile.carbon;
     seed.allocn = seed.allocc/cnseed[pdcmnt];
   }
  // Check to see if carbohydrate-limited
  if( (leaf.rm + sapwood.rm + root.rm + labile.rm) <= labile.carbon )
  {
    lclimflag = 0;

    if( labile.carbon > 0.67*(fsaplive[pdcmnt]*sapwood.carbon + root.carbon + leaf.carbon) )
    {
      // Windfall allocation
      extra = labile.carbon
              - 0.67*(fsaplive[pdcmnt]*sapwood.carbon + root.carbon + leaf.carbon);

      // Windfall-C allocation fractions
      //pleaflc = pleafce[pdcmnt];
	  pseedc = pseedlc[pdcmnt];
	  
	  switch( phen[pdcmnt] ) 
      // windfall allocation differs for different phenologies 
      // don't allow windfall allocation to leaves for deciduous types
	  {
        case I_EVERGREEN:
          if( fleafc > fsapwoodc && fleafc > frootc ) { pleaflc = 1.0 - pseedc; }
          else if( fsapwoodc > fleafc && fsapwoodc > frootc ) { psapwoodlc = 1.0 - pseedc; }
          else if( frootc > fleafc && frootc > fsapwoodc ) { prootlc = 1.0 - pseedc; }
          else
          {
            pleaflc = (1.0 - pseedc)/3.0;
            psapwoodlc = (1.0 - pseedc)/3.0;
            prootlc = (1.0 - pseedc)/3.0;
          }
          break; 
        case I_COLD_DEC:
          pleaflc = ZERO;
          if( fsapwoodc > frootc ) { psapwoodlc = 1.0-pseedc; } 
          else if( fsapwoodc < frootc ) { prootlc = 1.0-pseedc; }
          else
          {
            psapwoodlc = (1.0 - pseedc)/2.0;
            prootlc = (1.0 - pseedc)/2.0;
          }
          break;     
        case I_STRESS_DEC:
          pleaflc = ZERO;
          if( fsapwoodc > frootc ) { psapwoodlc = 1.0-pseedc; } 
          else if( fsapwoodc < frootc ) { prootlc = 1.0-pseedc; }
          else
          {
            psapwoodlc = (1.0 - pseedc)/2.0;
            prootlc = (1.0 - pseedc)/2.0;
          }
          break;             
      } 

      // Windfall-N allocation fractions
      norm = 1.0/(pleaflc/cnleaf[pdcmnt]+pseedc/cnseed[pdcmnt]+psapwoodlc/cnsapwood[pdcmnt]+prootlc/cnroot[pdcmnt]);
      pleafln = (pleaflc/cnleaf[pdcmnt])*norm;
	  pseedn = (pseedc/cnseed[pdcmnt])*norm;
      psapwoodln = (psapwoodlc/cnsapwood[pdcmnt])*norm;
      prootln = (prootlc/cnroot[pdcmnt])*norm;

      // Windfall-C allocation rates
//      leaf.allocc += pleaflc*extra*(12.0*pndays/365.0);
//	  seed.allocc += pseedc*extra*(12.0*pndays/365.0);
//      sapwood.allocc += psapwoodlc*extra*(12.0*pndays/365.0);
//      root.allocc += prootlc*extra*(12.0*pndays/365.0);

      leaf.allocc += pleaflc*extra*12.0/365.0;
      seed.allocc += pseedc*extra*12.0/365.0;
      sapwood.allocc += psapwoodlc*extra*12.0/365.0;
      root.allocc += prootlc*extra*12.0/365.0;
//      leaf.allocc += pleaflc*extra;
//      seed.allocc += pseedc*extra;
//      sapwood.allocc += psapwoodlc*extra;
//      cout << "windfall = " << sapwood.allocc << " " << psapwoodlc << " " << extra << endl;
//      root.allocc += prootlc*extra*12.0/365.0;
//      root.allocc += prootlc*extra;
      // Windfall-N allocation rates 
//      leaf.allocn += pleafln*((extra/labile.carbon)*labile.nitrogen)*
//                     (12.0*pndays/365.0);
//	  seed.allocn += pseedn*((extra/labile.carbon)*labile.nitrogen)*
//		            (12.0*pndays/365.0);
//      sapwood.allocn += psapwoodln*((extra/labile.carbon)*labile.nitrogen)*
//                     (12.0*pndays/365.0);
//      root.allocn += prootln*((extra/labile.carbon)*labile.nitrogen)*
//                  (12.0*pndays/365.0);

      leaf.allocn += pleafln*((extra/labile.carbon)*labile.nitrogen)*12.0/365.0;
      seed.allocn += pseedn*((extra/labile.carbon)*labile.nitrogen)*12.0/365.0;
      sapwood.allocn += psapwoodln*((extra/labile.carbon)*labile.nitrogen)*12.0/365.0;
      root.allocn += prootln*((extra/labile.carbon)*labile.nitrogen)*12.0/365.0;

//      leaf.allocn += pleafln*((extra/labile.carbon)*labile.nitrogen);
//      seed.allocn += pseedn*((extra/labile.carbon)*labile.nitrogen);
//      sapwood.allocn += psapwoodln*((extra/labile.carbon)*labile.nitrogen);
//      root.allocn += prootln*((extra/labile.carbon)*labile.nitrogen);
      /* (extra/labile.carbon)*labile.nitrogen is the sub-pool
       of labile nitrogen that has the same fractional size
       as extra has to labile.carbon

       Basing N-allocation on it ensures that N-allocation will not
       exceed the size of the labile N pool, as was before possible
      */
    }    
  }
  else
  {
    lclimflag = 1;
    if(labile.carbon > 0.67*(fsaplive[pdcmnt]*sapwood.carbon + root.carbon + leaf.carbon))
    {
//        cout << " error: labile carbon limiting to allocation, " ;
//        cout << " but also exceeds maximum storage size in sapwood pool " << endl;
        errcnt += 1.0; // This should happen rarely in the newer version
  //      exit(-1);
    }
  }

// check to see if overall allocation is C-limited or N-limited
  if(nfeed == 1)
  {
    // check leaf allocation
//    cout << "alloclc = " << leaf.allocc << endl;
    if( leaf.allocn*cnleaf[pdcmnt] < leaf.allocc ) // n-limited
    {
      leaf.allocic = leaf.allocin*cnleaf[pdcmnt];
      leaf.allocc = leaf.allocn*cnleaf[pdcmnt];
//      cout << "N-limited = " << leaf.allocc << endl;
    }
    else // c-limited
    {
      leaf.allocin = leaf.allocic/cnleaf[pdcmnt];
      leaf.allocn = leaf.allocc/cnleaf[pdcmnt];
    }

    // check sapwood allocation
    if( sapwood.allocn*cnsapwood[pdcmnt] < sapwood.allocc ) // n-limited
    {
//    cout << "N-limited sapwood correction " << sapwood.allocc << endl;
      sapwood.allocic = sapwood.allocin*cnsapwood[pdcmnt];
      sapwood.allocc = sapwood.allocn*cnsapwood[pdcmnt];
//    cout << "N-limiated sapwood allocation " << sapwood.allocc << endl;
    }
    else // c-limited
    {
      sapwood.allocin = sapwood.allocic/cnsapwood[pdcmnt];   
      sapwood.allocn = sapwood.allocc/cnsapwood[pdcmnt];
    }

    // check root allocation
//     cout << "not N-limited = " << root.allocc << endl;
    if( root.allocn*cnroot[pdcmnt] < root.allocc ) // n-limited
    {
//      cout << "N-limited root allocation " << root.allocc << endl;
      root.allocic = root.allocin*cnroot[pdcmnt];  
      root.allocc = root.allocn*cnroot[pdcmnt];
//      cout << "N-limited root correction " << root.allocc << endl;
    }
    else // c-limited
    {
      root.allocin = root.allocic/cnroot[pdcmnt];   
      root.allocn = root.allocc/cnroot[pdcmnt];
    }

    // check seed allocation
    if( seed.allocn*cnseed[pdcmnt] < seed.allocc ) // n-limited
    {
      seed.allocic = seed.allocin*cnseed[pdcmnt];  
      seed.allocc = seed.allocn*cnseed[pdcmnt];
    }
    else // c-limited
    {
      seed.allocin = seed.allocic/cnseed[pdcmnt];  
      seed.allocn = seed.allocc/cnseed[pdcmnt];
    } 
  }

// determine growth respiration: 25% of allocation
  leaf.rg = 0.25*leaf.allocc;
  sapwood.rg = 0.25*sapwood.allocc;
  root.rg = 0.25*root.allocc;
  seed.rg = 0.25*seed.allocc;

// if plant is not carbohydrate-limited (lclimflag = 0),
//   increment allocation to account for rm
  if( lclimflag == 0 )
  {
    leaf.allocc += leaf.rm;
//    cout << "carblim = " << leaf.allocc << endl;
    sapwood.allocc += sapwood.rm;
    root.allocc += root.rm;
//cout << "notcarblim = " << sapwood.allocc << " " << sapwood.rm << endl;
  }
  else
  {
    // incremented to litterfall-related leaf n resorption
    leaf.nresorb += leaf.rm/cnleaf[pdcmnt]; 
    sapwood.nresorb = sapwood.rm/cnsapwood[pdcmnt];
    root.nresorb = root.rm/cnroot[pdcmnt];
  }

// determine cnprod -- the c:n demanded by the structural comparments
//   from the labile compartment, in the current timestep
  cnprod = leaf.allocc + sapwood.allocc + root.allocc + seed.allocc
       + leaf.rg + sapwood.rg + root.rg + seed.rg + labile.rm;
  if( leaf.allocn + sapwood.allocn + root.allocn + seed.allocn > ZERO )
  {
    cnprod /= (leaf.allocn + sapwood.allocn + root.allocn + seed.allocn);
  }
  else { cnprod = 1000.0; }

  if( cnprod < ZERO ) { cnprod = ZERO; }

//  fpc = cnprod;

//  cout << "diag = " << root.rm << " " << root.nresorb << " " << root.allocc << " "
//  << root.allocn << endl;

  plant.rm = leaf.rm + sapwood.rm + root.rm + seed.rm;
  plant.rg = leaf.rg + sapwood.rg + root.rg + seed.rg;
  plant.ltrc = leaf.ltrc + sapwood.ltrc + heartwood.ltrc + root.ltrc + seed.ltrc;
  plant.ltrn = leaf.ltrn + sapwood.ltrn + heartwood.ltrn + root.ltrn + seed.ltrn;
  gpr = plant.rm + plant.rg;

  
  checkBiomassFluxes( leaf );
  checkBiomassFluxes( sapwood );
  checkBiomassFluxes( heartwood );
  checkBiomassFluxes( root );
  checkBiomassFluxes( seed );
  checkBiomassFluxes( labile );
  checkBiomassFluxes( plant );
  checkBiomassFluxes( strctrl );
  

};
/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Tveg45::boundTOPT( const int& pcmnt )
{
/**
  constrain topt based on toptmin and toptmax
*/  
  
  if( topt > toptmax[pcmnt] ) {	topt = toptmax[pcmnt]; }
  if( toptmic > toptmax[pcmnt] ) {toptmic = toptmax[pcmnt]; }

  if( topt < toptmin[pcmnt] ) { topt = toptmin[pcmnt]; }
  if( toptmic < toptmin[pcmnt] ) { toptmic = toptmin[pcmnt]; }

};

/* *************************************************************
************************************************************* */
void Tveg45::checkBiomassFluxes( Biomass& pveg )
{
/**
  ensures that no biomass fluxes are negative.  Directions of fluxes have been
  defined so that negativity is unphysical and should be corrected.  Called at
  end of allocate.  
*/  
  
  if( pveg.rm < ZERO)
  {
    pveg.rm = ZERO;
  }
  if( pveg.rg < ZERO)
  {
    pveg.rg = ZERO;
  }
  if( pveg.ltrc < ZERO)
  {
    pveg.ltrc = ZERO;
  }
  if( pveg.ltrn < ZERO)
  {
    pveg.ltrn = ZERO;
  }
  if( pveg.allocc < ZERO)
  {
    pveg.allocc = ZERO;
  }
  if( pveg.allocn < ZERO)
  {
    pveg.allocn = ZERO;
  }
  if( pveg.allocic < ZERO)
  {
    pveg.allocic = ZERO;
  }
  if( pveg.allocin < ZERO)
  {
    pveg.allocin = ZERO;
  }
  if( pveg.nresorb < ZERO)
  {
    pveg.nresorb = ZERO;
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::deltafo3( const int& pdcmnt,
                       const double& d40 )

{
/**
    determine change in ozone damage with time, based on sum of recovery and 
    new damages.  Recovery occurs due to the addition of new leaves 
    (or turnover of leaves if lai is constant); new damages occur due to uptake
    of ozone by stomata.  Approach is quasi- flux-based, using stomatal
    conductance, the AOT40 ozone index, and a pft-specific damage coefficient  
*/

  double newdamage = ZERO;
  double recovery = ZERO;
  double deltaleafc = ZERO;

  deltaleafc = leaf.allocc - leaf.rm - leaf.ltrc;

  if(leaf.carbon > ZERO && deltaleafc > ZERO)
  {
    recovery = (1.0 - fozone) * (1.0/(365.0*tauleaf[pdcmnt]) + deltaleafc/leaf.carbon );
  }
  else
  {
    recovery = (1.0 - fozone) * (1.0/(365.0*tauleaf[pdcmnt]));
  }

  //  current month's ozone effect
  newdamage = (o3para[pdcmnt] * pow( 10.0, -6 ) * gs * d40);

  dfo3 = recovery - newdamage;

  if(dfo3 < -fozone) { dfo3 = -fozone;}
  if(dfo3 > 3.0 * (1.0-fozone)) { dfo3 = 3.0 * (1.0-fozone); }


};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::getecd( const string& ecd )
{
/**
    reads in ecological community data from the "veg.ecd" file, using methods
    inherited from tprocessxml.  
*/  
  
  int comtype;
  int dcmnt;
  ifstream infile;


  infile.open( ecd.c_str(), ios::in );

  if( !infile )
  {
    cerr << endl << "Cannot open " << ecd;
    cerr << " for vegetation ECD input" << endl;

    exit( -1 );
  }

  getXMLrootNode( infile, "vegECD" );

  for( dcmnt = 1; dcmnt < MAXCMNT; ++dcmnt )
  {

    comtype = getXMLcommunityNode( infile, "vegECD" );


    if( comtype >= MAXCMNT )
    {
      cerr << endl << "comtype is >= MAXCMNT" << endl;
      cerr << "comtype cannot be greater than ";
      cerr << (MAXCMNT-1) << endl;
      cerr << " in vegECD" << endl;

      exit( -1 );
    }

    kc[comtype]= getXMLcmntArrayDouble( infile,
                                        "vegECD",
                                        "kc",
                                        comtype );

    ki[comtype]= getXMLcmntArrayDouble( infile,
                                        "vegECD",
                                        "ki",
                                        comtype );

    tmin[comtype]= getXMLcmntArrayDouble( infile,
                                          "vegECD",
                                          "tmin",
                                          comtype );

    toptmin[comtype]= getXMLcmntArrayDouble( infile,
                                             "vegECD",
                                             "toptmin",
                                             comtype );

    toptmax[comtype]= getXMLcmntArrayDouble( infile,
                                             "vegECD",
                                             "toptmax",
                                             comtype );

    tmax[comtype]= getXMLcmntArrayDouble( infile,
                                          "vegECD",
                                          "tmax",
                                          comtype );

    raalpha[comtype]= getXMLcmntArrayDouble( infile,
                                             "vegECD",
                                             "raalpha",
                                             comtype );

    rabeta[comtype]= getXMLcmntArrayDouble( infile,
                                            "vegECD",
                                            "rabeta",
                                            comtype );

    ragamma[comtype]= getXMLcmntArrayDouble( infile,
                                             "vegECD",
                                             "ragamma",
                                             comtype );

    raqref[comtype]= getXMLcmntArrayDouble( infile,
                                            "vegECD",
                                            "raqref",
                                            comtype );

    ratref[comtype]= getXMLcmntArrayDouble( infile,
                                            "vegECD",
                                            "ratref",
                                            comtype );

    kn1[comtype]= getXMLcmntArrayDouble( infile,
                                         "vegECD",
                                         "kn1",
                                         comtype );

    sla[comtype]= getXMLcmntArrayDouble( infile,
                                         "vegECD",
                                         "sla",
                                         comtype );

    href[comtype]= getXMLcmntArrayDouble( infile,
                                         "vegECD",
                                         "href",
                                         comtype );

    krnup[comtype]= getXMLcmntArrayDouble( infile,
                                         "vegECD",
                                         "krnup",
                                         comtype );

    kallom[comtype]= getXMLcmntArrayDouble( infile,
                                           "vegECD",
                                           "kallom",
                                           comtype );

    cnleaf[comtype]= getXMLcmntArrayDouble( infile,
                                       "vegECD",
                                       "cnleaf",
                                       comtype );

    cnsapwood[comtype]= getXMLcmntArrayDouble( infile,
                                   "vegECD",
                                   "cnsapwood",
                                   comtype );

    cnheartwood[comtype]= getXMLcmntArrayDouble( infile,
                               "vegECD",
                               "cnheartwood",
                               comtype );

    cnroot[comtype]= getXMLcmntArrayDouble( infile,
                                       "vegECD",
                                       "cnroot",
                                       comtype );

    cnseed[comtype]= getXMLcmntArrayDouble( infile,
                                   "vegECD",
                                   "cnseed",
                                   comtype );

    pseedlc[comtype]= getXMLcmntArrayDouble( infile,
                           "vegECD",
                           "pseedlc",
                           comtype );

    fsaplive[comtype]= getXMLcmntArrayDouble( infile,
                           "vegECD",
                           "fsaplive",
                           comtype );
                           
    phen[comtype]= getXMLcmntArrayInt( infile,
                                       "vegECD",
                                       "phen",
                                       comtype );

    nnf[comtype]= getXMLcmntArrayDouble( infile,
                                       "vegECD",
                                       "nnf",
                                       comtype );

    endXMLcommunityNode( infile );
  }

  if( dcmnt < MAXCMNT )
  {
    cerr << endl << " Parameters found for only " << dcmnt;
    cerr << " community types out of a maximum of ";
    cerr << (MAXCMNT-1) << " types in vegECD" << endl;

    exit( -1 );
  }

  infile.close();

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::gppxclm( const int& pdcmnt,
                        const int& pndays,
                        const double& co2,
                        const double& par,
                        const double& vpr,
                        const double& vpd,
                        const double& pdayl,
                        const double& kext,
                        const double& rhmcrb,
                        const double& prec,
                        ofstream& temdiagfile)
{
/**
    Calculation of photosynthesis, based on integrated leaf-level light formula,
    drought stress function (from penmon.hydraulics), temperature modifier
    (from settemp), ozone damage (derivative in deltafo3), and internal co2
    (setci called within this routine; fairly complex routine).  
    
    This routine also calculates partial derivatives of gpp with respect to 
    leaf, stem, and root carbon, based on combined effects on light acquisition,
    drought stress, and nutrient uptake.  These derivatives are then used to 
    drive allocation.
*/

  double veggpp;
//  double parw, frdl, frad, kiw;
  double parw, frad, kiw;
//  double fd, fp, ci, fco2, fh2o;
  double fd, fp, ci;
  double cmaxgcmo;
  double cnavg;

// convert par to W m^-2 from cal cm^-2 day^-1, considering daylength (daytime par)
  parw = par*41840.0/(3600.0*pdayl);
//  parw = par;
// convert ki to W m^-2 from cal cm^-2 day^-1 (not considering daylength)
  kiw = ki[pdcmnt]*(41840.0/86400.0);
// convert cmax from micromol co2 m^-2 s^-1 to gC m^-2 month^-1 (considering daylength)
//  cmaxgcmo = cmax * (12.0*3600.0*pdayl*pndays)/1000000.0;
  cmaxgcmo = cmax * (12.0*3600.0*pdayl)/1000000.0;

  if(cmax == 0.0)
  {
   cout << "cmax = 0, missing dat file" << endl;
   exit(-1);
  }

//cout << "cmax = " << cmax << " " << pdayl << " " << cmaxgcmo << endl;
// frdl: integrated canopy light response
// frad: bottom-of-canopy leaf-level light response
  if(lai > 0.01)
  {
    frdl = (1.0/kext)*log((kiw+kext*parw)/(kiw+kext*parw*exp(-kext*lai)));
    frad = kext*parw*exp(-kext*lai)/(kiw+kext*parw*exp(-kext*lai));
  }
  else
  {
    frdl = (1.0/kext)*log((kiw+kext*parw)/(kiw+kext*parw*exp(-kext*0.01)));
    frad = kext*parw*exp(-kext*0.01)/(kiw+kext*parw*exp(-kext*0.01));
  }

//   cout << "frdl = " << frdl << " " << kext << " " << kiw << " " << lai << " " << parw << endl;
// fh2o: water stress function
  fh2o = pen.getFWS();
//  if(prec > 10.0) {fh2o = 1.0;}

  fd = vpr/(vpr+vpd); // using relative humidity as function for reducing gc
  fp = frdl * temp * fozone * fh2o; // all factors but fco2
  
  if( lai > 0.01 && fp > ZERO )
  {
    ci = setCI( pdcmnt,
                pndays, 
                fp,
                fd,
                co2,
                rhmcrb,
                pdayl );
  }
  else { ci = co2; } // and ci is basically irrelevant 
  
//  ci = (1.0-1.6/(gsa*fd))*co2;
//  if( ci < ZERO ) { ci = ZERO; }

//  fpc = ci;
  fco2 = ci/(kc[pdcmnt] + ci);
//  cout << "fco2 in gppxclm = " << fco2 << " " << ci << " " << kc[pdcmnt] << endl;
  
  veggpp = cmaxgcmo * frdl * fco2 * temp * fozone * fh2o;
//  cout << "veggpp " << veggpp << " " << cmax << " " << cmaxgcmo << " " << frdl << " " << fco2 << " " << temp << " " << fozone << " " << fh2o << " " << lai << " " << parw << endl;
//  temdiagfile << "veggpp " << veggpp << " " << cmax << " " << cmaxgcmo << " " << frdl << " " << fco2 << " " << temp << " " << fozone << " " << fh2o << " " << lai << " " << parw << endl;
//  cout << "veggpp " << veggpp << " " << frdl << " " << frad << " " << lai << " " << parw << " " << pdayl << endl;
  

  dgppdleafc = cmaxgcmo * frad * fco2 * temp * fozone * fh2o * sla[pdcmnt];
  dgppdleafc += cmaxgcmo * frdl * fco2 * temp * fozone * pen.getDFWS_DLEAFC();
  dgppdleafc *= rgpp/ringpp;
  
//  cout << "dgppdleafc = " << dgppdleafc << " " << cmaxgcmo << " " << frad << " " << frdl << " " << fco2 << " " << temp << " " << fozone << " " << fh2o << " " << pen.getDFWS_DLEAFC() << endl;

  dgppdstemc = cmaxgcmo * frdl * fco2 * temp * fozone * pen.getDFWS_DSTEMC();
  dgppdstemc *= rgpp/ringpp;
  
//  cout << "mb_s = " << dgppdstemc << " " << frdl << " " << fco2 << " " << temp << " " << fozone << " " << pen.getDFWS_DSTEMC() << endl;
  dgppdrootc = cmaxgcmo * frdl * fco2 * temp * fozone * pen.getDFWS_DROOTC();
  dgppdrootc *= rgpp/ringpp;

  
  if (yrpleaf + yrpsapwood + yrproot +yrpseed > ZERO)
  {
    cnavg = 1.0/(yrpleaf/cnleaf[pdcmnt] + yrpsapwood/cnsapwood[pdcmnt] + yrproot/cnroot[pdcmnt] + yrpseed/cnseed[pdcmnt]);
  }
  if( rgpp < ringpp && rgpp > ZERO) 
  { 
    dgppdrootc += dvnupdrootc * cnavg * (1.0 - rgpp/ringpp); 
  }

  if( veggpp < ZERO ) { veggpp = ZERO; }
  if( dgppdleafc < ZERO ) { dgppdleafc = ZERO; }
  if( dgppdstemc < ZERO ) { dgppdstemc = ZERO; }
  if( dgppdrootc < ZERO ) { dgppdrootc = ZERO; }

  ingpp = veggpp;
  gpp = ingpp;

};

/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Tveg45::litterresp( const int& pdcmnt,
                         const int& pndays )
{
/** 
    Plant litterfall and respiration.  Litterfall calculated based on
    lifetime-forumation; rates normally depend on plant compartment lifetime, 
    found in datfile, but are accelerated for the leaves of deciduous vegetation
    types.  Respiration depends on tissue nitrogen, as well as a temperature 
    function, rmt, which is set in rxtlars.  
*/

  double krgcmo;

// convert respiration rate from micromol co2 m^-2 s^-1 to gC m^-2 month^-1
//  krgcmo = kra[pdcmnt]*(12.0*86400.0*pndays)/1000000.0;
  krgcmo = kra[pdcmnt]*(12.0*86400.0)/1000000.0;

  // Determine Maintenance Respiration of vegetation
  // (leaf.rm, sapwood.rm, root.rm, labile.rm)
  leaf.rm = krgcmo*rmt*leaf.carbon/cnleaf[pdcmnt];
  sapwood.rm = krgcmo*rmt*fsaplive[pdcmnt]*sapwood.carbon/cnsapwood[pdcmnt];
  root.rm = krgcmo*rmt*root.carbon/cnroot[pdcmnt];
  labile.rm = ZERO;
  seed.rm = ZERO;

  // Determine litterfall rates for C and N, and leaf N resorption
//  leaf.ltrc = leaf.carbon*pndays / (365.0 * tauleaf[pdcmnt]);
// leaf.ltrc = leaf.carbon / (365.0 * 30.5 * tauleaf[pdcmnt]);
 leaf.ltrc = leaf.carbon / (365.0  * tauleaf[pdcmnt]);
/*  if( phi == ZERO )
  {
//    leaf.ltrc = 3.0*leaf.carbon*(12.0*pndays/365.0);
    leaf.ltrc = 3.0*leaf.carbon;
    // value of 3.0 corresponds to exponential leaf drop
    //   of ~95% per month
  } */
  if( phi == ZERO )
  {
  switch( phen[pdcmnt] ) //BSF COMBO
  {
//    case I_EVERGREEN:
//    leaf.ltrc = 3.0*leaf.carbon*(12.0*pndays/365.0);
    // value of 3.0 corresponds to exponential leaf drop
    //   of ~95% per month
//  break;
    case I_COLD_DEC:
//    leaf.ltrc = (3.0/30.5)*leaf.carbon;
    leaf.ltrc = (3.0*12.0/365.0) *leaf.carbon;
    // value of 3.0 corresponds to exponential leaf drop
    //   of ~95% per month
  break;
    case I_STRESS_DEC:
//    leaf.ltrc = (3.0/30.5)*leaf.carbon / (365.0 * tauleaf[pdcmnt]);
    leaf.ltrc = 3.0*leaf.carbon / (365.0 * tauleaf[pdcmnt]);
//    cout << "diag = " << leaf.ltrc << endl;
    // value of 3.0 corresponds to exponential leaf drop
    //   of ~95% per month
  break;
 }

    // leaf.ltrc = 3.0*leaf.carbon*(12.0*pndays/365.0);
    // value of 3.0 corresponds to exponential leaf drop
    //   of ~95% per month
  }

  leaf.ltrn = leaf.ltrc/cnltr[pdcmnt];
  leaf.nresorb = (leaf.ltrc/cnleaf[pdcmnt])-(leaf.ltrc/cnltr[pdcmnt]);

//  sapwood.ltrc = sapwood.carbon*pndays / (365.0 * tauheartwood[pdcmnt]);
//  sapwood.ltrc = sapwood.carbon / ((365.0 * 30.5) * tauheartwood[pdcmnt]);
  sapwood.ltrc = sapwood.carbon / (365.0 * tauheartwood[pdcmnt]);
  sapwood.ltrn = sapwood.ltrc/cnsapwood[pdcmnt];
  sapwood.nresorb = ZERO;

//  heartwood.ltrc = heartwood.carbon*pndays / (365.0 * tauheartwood[pdcmnt]);
//  heartwood.ltrc = heartwood.carbon / ((365.0 * 30.5)  * tauheartwood[pdcmnt]);
  heartwood.ltrc = heartwood.carbon / (365.0  * tauheartwood[pdcmnt]);
  heartwood.ltrn = heartwood.ltrc/cnsapwood[pdcmnt];

//  root.ltrc = root.carbon*pndays / (365.0 * tauroot[pdcmnt]);
//  root.ltrc = root.carbon / ((365.0 * 30.5) * tauroot[pdcmnt]);
  root.ltrc = root.carbon / (365.0 * tauroot[pdcmnt]);
  root.ltrn = root.ltrc/cnroot[pdcmnt];
  root.nresorb = ZERO;

//  seed.ltrc = seed.carbon*pndays / (365.0 * tauseed[pdcmnt]);
//  seed.ltrc = seed.carbon / ((365.0 * 30.5) * tauseed[pdcmnt]);
  seed.ltrc = seed.carbon / (365.0  * tauseed[pdcmnt]);
  seed.ltrn = seed.ltrc/cnseed[pdcmnt];
  seed.nresorb = ZERO;  

};


/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */

void Tveg45::nupxclm( const int& pdcmnt,
                        const double& nmax_grow,
                        const double& soilh2o,
                        const double& availn,
                        const double& rofT,
                        const double& ksoil,
                        const double& fozone,
                        const double& rootmass )
{
/** 
    Determine nitrogen uptake by vegetation, in units of grams N per square 
    meter per month.  Nitrogen uptake depends hyperbolically on root biomass,
    hyperbolically on the quantity of 
    [total available N * vsm^3 / soil moisture], and is also modified by ozone 
    damage, and temperature.  This function also calculates the derivative of
    vegetation nitrogen uptake with respect to root biomass, and this is used
    to partially drive allocation.  
*/

  double vegnup;

  // convert nmax from per month to per day
  //


  vegnup  = (availn * ksoil) / soilh2o;
  vegnup *= (nmax_grow/30.5) / (kn1[pdcmnt] + vegnup);
  vegnup *= rofT;
  vegnup *= fozone;
  vegnup *= rootmass / (krnup[pdcmnt] + rootmass);

  if( vegnup < ZERO ) { vegnup = ZERO; }
  
  if (rootmass > 0.01)
  {
    dvnupdrootc = vegnup * krnup[pdcmnt]/(rootmass * (rootmass + krnup[pdcmnt]));
  }
  else { dvnupdrootc = vegnup * krnup[pdcmnt]/(0.01 * (0.01 + krnup[pdcmnt])); }

  inuptake = vegnup;
  nuptake = inuptake;


};


/* *************************************************************
************************************************************* */

/* *************************************************************
************************************************************* */
void Tveg45::petsw( const int& pdcmnt,
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
                        const double& soilmmmo,
                        const double& avlh2o,
                        const double& elev )

{
/**
    Calculate stomatal conductance (penmon.gcpet) 
    using Ball-Berry method: gc = gmin*lai + ga*rh*(gpp-rleaf)/ca, and 
    evapotranspiration according to Shuttleworth-Wallace formula (
    penmon.petsw). Daytime and nightttime rates are calculated separately, 
    then weighted by daylength to give monthly average evapotranspiration.   
*/

  double npet, dpet;
  double dnirr, ngc;
  double rcanmm; // canopy respiration, in mmol m^2 s^-1
  
//  rcanmm = (leaf.rm + leaf.rg) / (12.0e-3 * 24.0 * 3600.0 * pndays);
  rcanmm = (leaf.rm + leaf.rg) / (12.0 * 24.0 * 3600.0 / 1000.0);

  gc = pen.gcpet( pdcmnt,
                   pdm,
                   lai,
                   gpp,
                   rcanmm,
                   vpr,
                   vpdd,
                   co2,
                   pdayl );

  ngc = pen.gcpet( pdcmnt,
                 pdm,
                 lai,
                 0.0,
                 0.0,
                 vpr,
                 vpdn,
                 co2,
                 24.0-pdayl );

//if (((avlh2o + precip) < 50.0) && phen[pdcmnt] == 2) //BSF COMBO
if (((avlh2o + precip) < 5.0) && phen[pdcmnt] == 2) //BSF COMBO
  {
    gc = 0.0;
    ngc = 0.0;
//  gc = gc * (avlh2o + precip)/(avlh2o + precip + 50.0);
//  ngc = ngc * (avlh2o + precip)/(avlh2o + precip + 50.0);
//  gpp = gpp * (avlh2o + precip)/(avlh2o + precip + 50.0);
    gpp = 0.0;
  }

  dnirr = nirrn * 24./pdayl;

  dpet = pen.shuttlewallace( pdcmnt,
                dnirr,
                lwoutd,
                lai,
                taird,
                vpdd,
                pdm,
                gc,
                snowcover,
                pdayl*3600.0,
                precip,
                esoilmmmo,
                elev,
                phen[pdcmnt] );

//cout << "petdiag = " <<  "" << dpet << " " << pdcmnt << " " << dnirr << " " << lwoutd << " " << lai << " " << taird << " " << vpdd << " " << pdm << " " << gc << " " << snowcover << " " << pdayl << " " << precip << " " << esoilmmmo << " " << elev << endl;

  pesoilw = pen.getPESOIL();
  pecanw = pen.getPECAN();
  shfluxw = pen.getSHFLUX()*pdayl/24.0;

  npet = pen.shuttlewallace( pdcmnt,
              0.0,
              lwoutn,
              lai,
              tairn,
              vpdn,
              pdm,
              ngc,
              snowcover,
              (24.0-pdayl)*3600.0,
              precip,
              esoilmmmo,
              elev,
              phen[pdcmnt] );

  pesoilw += pen.getPESOIL();
  pecanw += pen.getPECAN();
  shfluxw += pen.getSHFLUX()*(24.0-pdayl)/24.0;

  setPET( dpet + npet );

  dpet = pen.shuttlewallacell( pdcmnt,
                dnirr,
                lwoutd,
                lai,
                taird,
                vpdd,
                pdm,
                gc,
                pdayl*3600.0,
                esoilmmmo,
                elev,
                phen[pdcmnt] );

  npet = pen.shuttlewallacell( pdcmnt,
              0.0,
              lwoutn,
              lai,
              tairn,
              vpdn,
              pdm,
              ngc,
              (24.0-pdayl)*3600.0,
              esoilmmmo,
              elev,
              phen[pdcmnt] );
  setPETLL( dpet + npet );

  gc = gc*1000.0;
  if(lai <= 0.01)
  {
    gs = ZERO;
  }
  else
  {
    gs = gc/lai;
  }

//cout << "diag2 = " << getPET() << " " << pesoilw << " " << pecanw << endl;

};

/* *************************************************************
************************************************************** */

/* *************************************************************
************************************************************* */
void Tveg45::phenology(const int& pdcmnt,
                       const int& pdm,
                       const double& tair,
                       const double& ptaird,
                       const double& ptairn,
                       const double& avlh2o,
                       const double& awcapmm,
                       const double& prec,
                       const double& pdayl)
{
/**
    Determine whether or not new leaf growth is allowed in a given month.
    Calculation is based on vegetation phenology type (evergreen, cold-
    deciduous, or stress-deciduous), and temperature and/or 
    moisture conditions.  For cold-deciduous types, daytime temperature
    in a month must exceed the average of a critical temperature (taken to be
    13 C), and a running mean of daytime air temperature.  This allows 
    vegetation from colder locations to leaf out at a lower temperature than 
    vegetation from warm locations.  Nighttime temperature for the month must 
    also be greater than tmin.  Stress-deciduous phenology types require that
    cold-deciduous conditions be met, and that soil moisture exceed a critical 
    value as well. 
*/

//  double tcrit = 5.0;
  double tcrit = 13.0;
  double mcrit = 0.05;  //BSF COMBO
//  double mfrac = (prec+avlh2o)/awcapmm;
  double mfrac = avlh2o/awcapmm;

//  const int I_EVERGREEN = 0;
//  const int I_COLD_DEC = 1;
//  const int I_STRESS_DEC = 2;

  switch( phen[pdcmnt] )
  {
    case I_EVERGREEN: 
     phi = 1.0; 
//     cout << "evergreen phi = " << phi << endl;
     break;

/*    case I_COLD_DEC:
       if( (ptaird < (rtair + tcrit)/2.0 )||(ptairn < tmin[pdcmnt]) )
      {
        phi = 0.0;
        phicnt = 0.0;
      }
      else
      {
        phi = 1.0;
        phicnt += 1.0;
      }
      break; */





    case I_COLD_DEC:
      //if(tair < tcrit)
      if( (ptaird < (rtair + tcrit)/2.0 )||(ptairn < tmin[pdcmnt]) || (pdayl < 11.5 && daylslope[pdm] < 0.0 ) || (pdayl < 13.9 && daylslope[pdm] > 0.0 ))  
//      if( (ptaird < (rtair + tcrit)/2.0 )||(ptairn < tmin[pdcmnt]) )  
//      if( (ptaird < (rtair + tcrit)/2.0 )||(ptairn < tmin[pdcmnt]) || (daylslope[pdm] < 0.0) )  
      {
        phi = 0.0;
        phicnt = 0.0;
      }
      else
      {
//        if(pdm >= 0 && pdm <= 30){ 
//        cout << "diag = " << pdm << " " << ptaird <<  " " << (rtair + tcrit)/2.0 << " " << ptairn << " " << tmin[pdcmnt] << " " << pdayl << " " << daylslope[pdm] << endl;
//        }
        phi = 1.0;
        phicnt += 1.0;
      }
//      if((prvphi == 0 && phi == 1) || (prvphi > 0 && prvphi < 1))
//      {
//       phi = prvphi+phi/7.0;
//       if(phi > 1.0) {phi = 1.0;}
//      }
//      prvphi = phi; 
//      cout << "deciduous phi = " << phi << endl;
      break; 

    case I_STRESS_DEC:
      if( (ptaird < (rtair + tcrit)/2.0 )||(ptairn < tmin[pdcmnt])||(mfrac < mcrit) )  
      {
        phi = 0.0;
        phicnt = 0.0;
//       cout << "phi = " << phi << " " << ptaird << " " << (rtair + tcrit)/2.0
//       << " " << ptairn << " " << tmin[pdcmnt] << " " << avlh2o/awcapmm << 
//       " " << mcrit << endl;
      }
      else
      {
        phi = 1.0;
        phicnt += 1.0;
//       cout << "phenology = " << phi << " " << ptaird << " " << (rtair + tcrit)/2.0
//       << " " << ptairn << " " << tmin[pdcmnt] << " " << avlh2o/awcapmm <<
//       " " << mcrit << endl;
      }
      break;
  }
//       cout << "phenology = " << phi << " " << ptaird << " " << (rtair + tcrit)/2.0
//       << " " << ptairn << " " << tmin[pdcmnt] << " " << pdayl << endl;
//      fpc = phi;       
};

/* *************************************************************
************************************************************** */

void Tveg45::resetEcds( const int& pcmnt,
                        const double& psiplusc )
{
/**
    resets cmax and nmax; while these nominally depend on soil texture,
    most of the "a" coefficients should be zero, with the "b" coefficients
    carrying the real value
*/ 

    cmax =  cmax1b[pcmnt];

//   cout << "cmax in reset = " << cmax << " " << pcmnt << endl;
  if( cmax < ZERO ) { cmax = ZERO; }

    nmax =  nmax1b[pcmnt];

  if( nmax < ZERO ) { nmax = ZERO; }


};

/* *************************************************************
************************************************************** */


/* *************************************************************
************************************************************* */

void Tveg45::resetMonthlyFluxes( void )
{
/**
    Set veg. variables that correspond to monthly fluxes back to zero
*/  
  
  lai = ZERO;
  fpc = ZERO;

  // Reset monthly fluxes to zero

  ingpp = ZERO;
  gpp = ZERO;

  innpp = ZERO;
  npp = ZERO;

  gpr = ZERO;
  rg = ZERO;

  inuptake = ZERO;
  nuptake = ZERO;
  
  pet = ZERO;
  
  dfo3 = ZERO;

  gc = ZERO;
  gs =  ZERO;

// Reset fluxes of all biomass variables
  labile.allocc = ZERO;
  labile.allocn = ZERO;
  labile.allocic = ZERO;
  labile.allocin = ZERO;
  labile.ltrc = ZERO;
  labile.ltrn = ZERO;
  labile.nresorb = ZERO;
  labile.rm = ZERO;
  labile.rg = ZERO;

  leaf.allocc = ZERO;
  leaf.allocn = ZERO;
  leaf.allocic = ZERO;
  leaf.allocin = ZERO;
  leaf.ltrc = ZERO;
  leaf.ltrn = ZERO;
  leaf.nresorb = ZERO;
  leaf.rm = ZERO;
  leaf.rg = ZERO;

  sapwood.allocc = ZERO;
  sapwood.allocn = ZERO;
  sapwood.allocic = ZERO;
  sapwood.allocin = ZERO;
  sapwood.ltrc = ZERO;
  sapwood.ltrn = ZERO;
  sapwood.nresorb = ZERO;
  sapwood.rm = ZERO;
  sapwood.rg = ZERO;

  heartwood.allocc = ZERO;
  heartwood.allocn = ZERO;
  heartwood.allocic = ZERO;
  heartwood.allocin = ZERO;
  heartwood.ltrc = ZERO;
  heartwood.ltrn = ZERO;
  heartwood.nresorb = ZERO;
  heartwood.rm = ZERO;
  heartwood.rg = ZERO;

  root.allocc = ZERO;
  root.allocn = ZERO;
  root.allocic = ZERO;
  root.allocin = ZERO;
  root.ltrc = ZERO;
  root.ltrn = ZERO;
  root.nresorb = ZERO;
  root.rm = ZERO;
  root.rg = ZERO;
  
  seed.allocc = ZERO;
  seed.allocn = ZERO;
  seed.allocic = ZERO;
  seed.allocin = ZERO;
  seed.ltrc = ZERO;
  seed.ltrn = ZERO;
  seed.nresorb = ZERO;
  seed.rm = ZERO;
  seed.rg = ZERO;

// (strctrl = leaf + sapwood + heartwood + root)
  strctrl.allocc = ZERO;
  strctrl.allocn = ZERO;
  strctrl.allocic = ZERO;
  strctrl.allocin = ZERO;
  strctrl.ltrc = ZERO;
  strctrl.ltrn = ZERO;
  strctrl.nresorb = ZERO;
  strctrl.rm = ZERO;
  strctrl.rg = ZERO;

// (plant = labile + strctrl)
  plant.allocc = ZERO;
  plant.allocn = ZERO;
  plant.allocic = ZERO;
  plant.allocin = ZERO;
  plant.ltrc = ZERO;
  plant.ltrn = ZERO;
  plant.nresorb = ZERO;
  plant.rm = ZERO;
  plant.rg = ZERO;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::resetNEWTOPT( const int& pcmnt,
                           const double& tair,
                           const double& prtairphi,
                           const double& prphi)
{
/**
    Change value of "newtopt", which is used to advance the value of topt
    based on a multi-year running mean, in tem.stepmonth
*/   
  
//  cout << "newtopt at start = " << newtopt << " " << tair << endl;
  if(phen[pcmnt] == I_STRESS_DEC) { newtopt = prtairphi/prphi; }
//  else{ if( tair > newtopt ) { newtopt = tair; } }
   else{ newtopt = newtopt + tair/30.0;} 

  if( newtopt > toptmax[pcmnt] ) { newtopt = toptmax[pcmnt]; }

  if( newtopt < toptmin[pcmnt] ) { newtopt = toptmin[pcmnt]; }

//  cout << "newtopt = " << newtopt << endl;
    
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::resetYrFluxes( void )
{
/**
  Reset annual fluxes and summary variables to zero.  Note that most of these
  'yr' values are actually running means over a longer time period (tem.tauavg)
*/

  yrcarbon = ZERO;

  yrstructn = ZERO;
  yrstoren = ZERO;

  yrnitrogen = ZERO;

  // Phenology

  yrlai = ZERO;
  yrfpc = ZERO;

  // Carbon fluxes

  yralloclc = ZERO;
  yrallocsc = ZERO;
  yrallocrc = ZERO;
  
  yrallocilc = ZERO;
  yrallocisc = ZERO;
  yrallocirc = ZERO;
  
  yrallocseedc = ZERO;
  yrallocseedn = ZERO;
  
  yrpleaf = ZERO;
  yrpsapwood = ZERO;
  yrproot = ZERO;
  yrpseed = ZERO;
  
  yringpp = ZERO;
  yrgpp = ZERO;
  yrinnpp = ZERO;
  yrnpp = ZERO;
  yrgpr = ZERO;
  yrrmaint = ZERO;
  yrrmleaf = ZERO;
  yrrmsapwood = ZERO;
  yrrmroot = ZERO;
  yrrgrowth = ZERO;

  yrltrc = ZERO;

  // Nitrogen fluxes

  yrinnup = ZERO;
  yrnup = ZERO;
  yrnrsorb = ZERO;
  yrltrn = ZERO;

  // Penman fluxes

  yrgc = ZERO;
  yrgs = ZERO;

};
/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Tveg45::rxTLaRS( const int& pdcmnt,
                        const double& ptair,
                        const double& ptref )
{
/**   
    relative plant respiration rate as a function of temperature, based on
    the lars model, and personal communication with Amthor, 2007
*/

  double rabase;
  double raexp;
  double radenm;
  double rxT;
  double ratopt;
 

  rabase = raqref[pdcmnt]*exp(-raalpha[pdcmnt]*(ptair - ptref));
  raexp = (ptair - ptref)/10.0;
  radenm = 1.0 + exp(rabeta[pdcmnt] - ptair) + exp(ptair - ragamma[pdcmnt]);

  rxT = pow(rabase, raexp)/radenm;

  rabase = raqref[pdcmnt]*exp(-raalpha[pdcmnt]*(topt - ptref));
  raexp = (topt - ptref)/10.0;
  radenm = 1.0 + exp(rabeta[pdcmnt] - topt) + exp(topt - ragamma[pdcmnt]);

  ratopt = pow(rabase, raexp)/radenm;

  rxT /= ratopt;

  rmt = rxT;

  fpc = topt;
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double Tveg45::setCI( const int& pdcmnt,
                      const int& pndays,
                      const double& fp,
                      const double& fd,
                      const double& ca,
                      const double& rhmicrobe,
                      const double& pdayl )
{
/**    
    solve quadratic equation for internal co2 based on gpp equation,  
    stomatal conductance equation, and assumption that Ca=Cl.  
*/
  
  double rcan, cam, kcm, acan, g0, g1;
  double aq, bq, cq;
  double dwc;
  
  double tstci;
  

  dwc = 1.563; // molecular diffusivity ratio of water to co2
  cam = ca / 1000000.0; // mixing ratio instead of ppm
  kcm = kc[pdcmnt] / 1000000.0;
  acan = cmax * fp / 1000.0; // change units to mmol/m^2-s instead of umol/m^2-s
  g1 = pen.getGSA(pdcmnt)*fd; // canopy stomatal conductance slope adjusted for humidity
  g0 = pen.getGSMIN(pdcmnt)*lai; // canopy stomatal conductance intercept
  
  // convert from gC m^-2 mo^-1 to mmol m^-2 s^-1
//  rcan = (leaf.rm + leaf.rg) / (12.0e-3 * 24.0 * 3600.0 * pndays);
  rcan = (leaf.rm + leaf.rg) / (12.0 * 24.0 * 3600.0 / 1000.0);

  if( acan > rcan ) // stomatal conductance formula is gc = g0 + g1 (gpp-rcan)/ca
  {
    aq = -g0*cam - g1*(acan - rcan);
    bq = cam*(g1 - dwc)*(acan - rcan) + g1*rcan*kcm + g0*cam*(cam - kcm);
    cq = cam*kcm*(g0*cam - rcan*(g1 - dwc));
    
    if( (bq*bq - 4.0*aq*cq) > ZERO )
    {
      tstci = (-bq - sqrt(bq*bq - 4.0*aq*cq))/(2.0*aq);
    }
    else { tstci = cam; }
    
    if( acan*tstci/(kcm+tstci) < rcan ) // gpp will be less than rcan; recalculate using gc = g0
    {
      //aq = -g0;
      //bq = g0*(cam - kcm) + dwc*(rcan - acan);
      //cq = kcm*(g0*cam + dwc*rcan);
      
      //tstci = (-bq - sqrt(bq*bq - 4.0*aq*cq))/(2.0*aq);
      
      // or just assume ci = ca
      tstci = cam;
    } 
  }
  else // gpp will be less than rcan; recalculate using gc = g0
  {
    //aq = -g0;
    //bq = g0*(cam - kcm) + dwc*(rcan - acan);
    //cq = kcm*(g0*cam + dwc*rcan);
      
    //tstci = (-bq - sqrt(bq*bq - 4.0*aq*cq))/(2.0*aq);
    
    // or just assume ci = ca
    tstci = cam;
  } 
  
// cout << "fco2 in setCI= " << ca << " " << 1000000.0*tstci << endl;
  return 1000000.0*tstci;



};

/* *************************************************************
************************************************************* */



/* *************************************************************
************************************************************* */

void Tveg45::setTEMP( const int& pdcmnt, const double& tair )

{
/** 
    determine the effects of temperature on primary productivity -- function is
    equal to zero above tmax and below tmin, and bell-shaped in between, with a
    peak at topt.  As topt moves closer to tmax or tmin, the peak narrows, 
    reflecting ability of vegetation to adapt, but increasing specialization
    at the extremes of temperature range for a pft   
*/

  double pow1, pow2;

  pow1 = 1.0;
  pow2 = pow1*(topt-tmin[pdcmnt])/(tmax[pdcmnt]-topt);

  if( tair <= tmin[pdcmnt] || tair >= tmax[pdcmnt] )
  {
    temp = ZERO;
  }
  else
  {
    temp = pow((tmax[pdcmnt]-tair),pow1)*pow((tair - tmin[pdcmnt]),pow2);
    temp /= pow((tmax[pdcmnt]-topt),pow1)*pow((topt - tmin[pdcmnt]),pow2);
//    cout << "temp = " << temp << " " << tmax[pdcmnt] << " " << tair << " " << topt << " " << tmin[pdcmnt] << endl;
  }

};

/* *************************************************************
************************************************************* */



/* *************************************************************
************************************************************* */
void Tveg45::updateDynamics( const int& pdcmnt,
                             const double& ninput,
                             const double& availn,
                             const int& nfeed,
                             const int& agstate,
                             const int& agperennial,
                             const int& fertflag,
                             const double& netnmin,
                             double& agfertn,
                             const double& soninp )
{
/**
    Determine downregulation of gpp, npp, and nitrogen uptake based on 
    nitrogen- or carbon- limitation, labile c and n stocks, 
    ingpp, innup, and fertilization
    
    (probably should be renamed -- much has been moved out of this routine) 
*/

  double fnupdownreg, fgppdownreg;
//  double cnavg;
    
  innpp = ingpp - plant.rm - plant.rg;
  npp = innpp;
  // Assume CROP NPP is always positive or zero for non-perennial crops
//  if( 1 == agstate && 0 == agperennial )
//  {
//    if( npp < ZERO ) { npp = ZERO; }
//  }


// Nitrogen feedback of GPP ( 1 == nfeed)
  if( 1 == nfeed )
  {
    fgppdownreg = 1.0;
    fnupdownreg = 1.0;
    
    if ( rdemandn > ZERO && rdemandc > ZERO &&
         rlabilen > ZERO && rlabilec > ZERO &&
         labile.nitrogen > ZERO && labile.carbon > ZERO &&
         cnprod > ZERO )
    {
      if( (rlabilec/rlabilen > rdemandc/rdemandn) 
          && (labile.carbon/labile.nitrogen > cnprod) )
      {
//        fgppdownreg = ((rdemandc/rdemandn) + cnprod)/
//                      ((rlabilec/rlabilen) + (labile.carbon/labile.nitrogen));
        fgppdownreg = (rdemandc/rdemandn)/(rlabilec/rlabilen);
        soninput = soninp;
        denitr = 0.0;
      }
      else if( (rlabilen/rlabilec > rdemandn/rdemandc) 
          && (labile.nitrogen/labile.carbon > 1.0/cnprod) )
      {
//        fnupdownreg = ((rdemandn/rdemandc) + 1.0/cnprod)/
//                      ((rlabilen/rlabilec) + (labile.nitrogen/labile.carbon));
        fnupdownreg = (rdemandn/rdemandc)/(rlabilen/rlabilec);
        soninput = 0.0;
        vegninp = 0.0;
        denitr = 0.5*(rlabilen/rlabilec - rdemandn/rdemandc);
      }
    }
    else
    {
      soninput = 0.0;
    }

    gpp = ingpp*fgppdownreg;
    nuptake = inuptake*fnupdownreg;
           
    if( gpp < ZERO ) { gpp = ZERO; }
    npp = gpp - plant.rm - plant.rg;
//    cout << "diag = " << gpp << " " << ingpp << " " << fgppdownreg << " " << rdemandc << " " << rdemandn << " " << rlabilec << " " << rlabilen <<  endl;
//    exit(-1);
  }
  
  // Determine if monthly primary production is carbon-limited
  //   or nitrogen-limited
    
    /*
    if( ZERO == (nuptake + labile.nitrogen) )
    {
      inprodcn = (ingpp + labile.carbon) / 0.000001;
    }
    else
    {
      inprodcn = (ingpp + labile.carbon) / (nuptake + labile.nitrogen);
    }
    */
    
    

  // If primary production is nitrogen-limited,
  //   (i.e., veg.inprodcn > cnprod) re-evaluate NPP, RG and
  //   GPP based on nitrogen availability

/*


//    if( inprodcn > cneven )
    if( inprodcn > cnprod )
    {
//      if( 1 == fertflag && 1 == agstate )
      if( 1 == fertflag )
      {
      // Assume nitrogen is never limiting if fertilized
      // Also assume that fertilized crops are based solely
      //   on a nitrate economy

//        nuptake = ((gpp + labile.carbon) / cneven) -  labile.nitrogen;
        nuptake = ((gpp + labile.carbon) / cnprod) -  labile.nitrogen;

        if( nuptake > inuptake )
	    {
          agfertn = nuptake - inuptake;
	    }
      }
      else
      {
//       gpp = cneven * (nuptake +  labile.nitrogen) - labile.carbon;
//        gpp = cnprod * (nuptake +  labile.nitrogen) - labile.carbon;
//        gpp = 0.2*(cnprod*(nuptake + labile.nitrogen) - labile.carbon)
//              +( 1.0 - 0.2)*ingpp;

//        gpp = cnprod*(nuptake + labile.nitrogen)*
//            (2.0 - cnprod*(nuptake + labile.nitrogen)/(ingpp + labile.carbon))
//            - labile.carbon;

        gpp = ingpp*( rgpp/ringpp );

//          gpp = cneven*(nuptake + labile.nitrogen)*
//            (2.0 - cneven*(nuptake + labile.nitrogen)/(ingpp + labile.carbon))
//            - labile.carbon;

        if( gpp < ZERO ) { gpp = ZERO; }
        npp = gpp - plant.rm - plant.rg;
      }
    }

    // If primary production is carbon-limited,
    //   (i.e., veg.inprodcn < veg.cneven) revaluate nitrogen
    //   uptake by vegetation based on carbon availability

//    if( inprodcn <= cneven )
    if( inprodcn <= cnprod )
    {
        
*/

/*      nuptake = nuptake
                * (inprodcn - cnmin[pdcmnt])
                * (inprodcn - 2 * cneven + cnmin[pdcmnt]);

      nuptake /= ((inprodcn - cnmin[pdcmnt])
                 * (inprodcn - 2 * cneven + cnmin[pdcmnt]))
                 - pow( inprodcn - cneven, 2.0 );
//
      nuptake = nuptake
                * (inprodcn - cnmin[pdcmnt])
                * (inprodcn - 2 * cnprod + cnmin[pdcmnt]);

      nuptake /= ((inprodcn - cnmin[pdcmnt])
                 * (inprodcn - 2 * cnprod + cnmin[pdcmnt]))
                 - pow( inprodcn - cnprod, 2.0 );
*/

//      nuptake = 0.2*((ingpp + labile.carbon)/cnprod - labile.nitrogen)
//               +(1.0 - 0.2)*nuptake;

//      nuptake = ((ingpp + labile.carbon)/cnprod)*
//              (2.0 - (ingpp + labile.carbon)/(cnprod*(nuptake + labile.nitrogen)))
//              - labile.nitrogen;

//      nuptake = ((ingpp + labile.carbon)/cneven)*
//             (2.0 - (ingpp + labile.carbon)/(cneven*(nuptake + labile.nitrogen)))
//             - labile.nitrogen;

/*
      if( nuptake > availn + netnmin + ninput )
      {
        nuptake = availn + netnmin + ninput;
      }
*/

/*
      if( nuptake < ZERO )
      {
        nuptake = ZERO;
      }
    }

  }
*/

};
