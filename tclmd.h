/* *************************************************************
****************************************************************
TCLMD.H - describes climate module used by TEM

20060126 - DWK created by modifying tclm50b5.h
20060126 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20060126 - DWK changed include from atms50b5.h to atms437.h
20060126 - DWK changed class TEMclm50 to class TEMclm43
20060126 - DWK changed inheritance from Atmosphere50 to
           Atmosphere43
20060126 - DWK added I_AOT40 to TEMclm43()
20060126 - DWK added public functions void initO3(),
           double mkd40() and void setO3Flags()
20060126 - DWK added public string io3end, string io3fname and
           int to3flag
2007 - TWC/BSF summary
    clmkey: add VPD, TRANGE, DAYL, AOT40B, 
	Public Functions: initVPD, initTRANGE, setTrangeFlags,
	   setVPDFlags
	Public Variables: io3bname, ivprend, ivprfname,
	   itrangeend, itrangefname, ttrangeflag, tvprflag

2008 - TWC added WS10, TAIRD, TAIRN, VPDD, VPDN -- 
    windspeed at 10 m, day/night temperature and vpd
****************************************************************
************************************************************* */

#ifndef TCLMD_H
#define TCLMD_H

// Tclm45 uses the global constants CYCLE, MISSING, NUMATMS
//   and NUMATMS
#include "temconstsd.hpp"

// Tclm45 inherits class Atms45
#include "atmsd.h"


class Tclm45 : public Atms45
{

  public:

     Tclm45();

     enum clmkey { I_GIRR,   I_NIRR,   I_PAR,    I_CLDS,  I_TAIR,
                   I_PREC,   I_RAIN,   I_SNWFAL, I_CO2,   I_VPR,
                   I_TRANGE, I_TAIRD,  I_TAIRN,  I_VPDD,  I_VPDN,   
                   I_WS10,   I_AOT40,  I_DAYL };

/* *************************************************************
		 Public Functions
************************************************************* */

     /* Determine cloudiness based on solar radiation at the top
        of the atmosphere and solar radiation at the top of the
        vegetation canopy */

     double mkclds( const double& girr, const double& nirr );

     double mkd40( const double& lon,
                   const double& lat,
	  	   const string& contnent,
                   const double& o3,
	  	   const int& pdm );

     /* Determine solar radiation at the top of the atmosphere (i.e.,
        "gross irradiance" or GIRR) based on the solar constant, time
        of year and latitude as described by S. M. Turton (1986)
        Solar radiation under cloudless skies.  Weatherwise 39:
        223-224.  */

     double xgirr( const float& plat,
                   const int& pdm,
                   double& psumday,
                   const int ndays[] );

     /* Determine solar radiation at the top of the vegetation canopy
        (i.e., "net irradiance" or NIRR) based on GIRR and cloudiness */

     double xnirr( const double& clds, const double& girr );

     /* Determine phototsynthetically active radiation (PAR) based on
        NIRR and cloudiness.  Algorithm determined by Jim Raich from
        a variety of studies [e.g., McCree (1966) Agricul. Meteorol.
        3: 353-366; Stigter and Musabilha (1982) Journal of Applied
        Ecology 19: 853-858] that indicate that cloud cover increases
        the proportion of PAR. */

     double xpar( const double& clds, const double& nirr );


/* *************************************************************
		 Public Variables
************************************************************* */


     // "Do data sets have cloudiness data or NIRR data?" flag
     int cldflag;

//     int co2year[MAXRTIME];       // year of CO2 data

     ifstream fco2;

     // Name of file extension and beginning of file name
     //  for cloudiness data

     string icldsend;
     string icldsfname;

     // Name of file extension and beginning of file name
     //  for atmospheric CO2 concentration data

     string ico2end;
     string ico2fname;

     // Name of file extension and beginning of file name
     //  for gross irradiance (i.e., solar radiation at the
     //  top of the atmosphere) data

     string igirrend;
     string igirrfname;

     // Name of file extension and beginning of file name
     //  for net irradiance (i.e. solar radiation at the
     //  top of the vegetation canopy) data

     string inirrend;
     string inirrfname;

     // Name of file extension and beginning of file name
     //  for ozone (i.e. AOT40) data

     string io3end;
     string io3fname;


     // Name of file extension and beginning of file name
     //  for photosynthetically active radiation data

     string iparend;
     string iparfname;

     // Name of file extension and beginning of file name
     //  for precipitation data

     string iprecend;
     string iprecfname;


     // Name of file extension and beginning of file name
     //  for air temperature data

     string itairend;
     string itairfname;

     // Name of file extension and beginning of file name
     //  for daily temperature range data

     string itrangeend;
     string itrangefname;

     // Name of file extension and beginning of file name
     //  for vapor pressure data

     string ivprend;
     string ivprfname;

     // Name of file extension and beginning of file name
     // for windspeed data
     
     string iws10end;
     string iws10fname;

     int parflag;     // Read in PAR from spatially explicit data sets?

     int predflag;    // Write climate module output to files?

     // Names of climate output variables

     vector<string> predstr;

     int sradflag;    // Run solar radiation module?

     // Initial year of transient portion of simulation

     int startyr;

     // Flag for transient cloudiness data
     //   ( =0 for long-term mean data
     //      1 for transient data)

     int tcldsflag;

     // transient CO2 concentration (ppmv)

//     double tco2[MAXRTIME][CYCLE];

     // Flag for transient atmospheric CO2 data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int tco2flag;

     // Flag for transient ozone (AOT40) data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int to3flag;

     // Flag for transient ozone (AOT40) data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int tndepflag;
     
     // Flag for transient precipitation data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int tprecflag;


     // Flag for transient air temperature data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int ttairflag;

     // Flag for transient temperature range
     //   ( =0 for long-term mean data
     //      1 for transient data)	
     int ttrangeflag;

     // Flag for transient vapor pressure
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int tvprflag;

     // Flag for transient windspeed data
     //   ( =0 for long-term mean data
     //      1 for transient data)
     int tws10flag;

     // Year represented by the climate data

     int year;

     // Number of days since beginning of year

     double yrsumday;


};

#endif
