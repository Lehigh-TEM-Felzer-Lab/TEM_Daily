/* **************************************************************
TCO2DAT45.H - object to read and write the structure of
                  transient CO2 data from/to files used by the
	          Terrestrial Ecosystem Model (TEM)

Modifications:

20060118 - DWK created by modifying tco2dat425.h
20060118 - DWK changed class CO2data to class CO2data43
20060118 - DWK deleted include tco2dat425.cpp from bottom of file
20070105 - TWC changed to tco2dat45

************************************************************** */

#ifndef TCO2DAT45_H
#define TCO2DAT45_H

class CO2dat45
{

  public:

     CO2dat45( void );

/* **************************************************************
		    Public Functions
************************************************************** */

     int get( ifstream& ifile );
     int getdel( FILE* infile,
                 const int& CYCLE );
     void out( ofstream& ofile, float& year, double& bco2 );
     void outdel( ofstream& ofile, float& year, double& bco2 );

/* **************************************************************
		     Public Variables
************************************************************** */


     // atmospheric CO2 concentration in July (ppmv)
     double mco2;

     // column or longitude of grid cell (degrees)
     float col;

     // row or latitude of grid cell (degrees)
     float row;

     // area covered by grid cell (sq. km)
     int carea;

      // date (year) of data
     //long year;
     int year;
     int month;
     int day;

     int CYCLE;
     // monthly values for the grid cell
     double mon[366];

      // name of continent containing grid cell
     string contnent;
     // climate variable name
     string varname;

  private:

/* **************************************************************
		      Private Variables
************************************************************** */

     int co2end;
     long curpos;
     long lagpos;


};

#endif

