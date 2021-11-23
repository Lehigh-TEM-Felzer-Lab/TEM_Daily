/* *************************************************************
ATMDEPDAT.H - object to read and write the structure of land
                  use / land cover cohort data from/to files

20060422 - DWK created by modifying lulcdat437.h
20060422 - DWK changed class Lulcdata43 to class Lulcdata44
20060422 - DWK deleted public double RAP
20060422 - DWK added public int disturbflag and public int
           disturbmonth
20070105 - TWC changed name to lulcdat45

************************************************************* */

#ifndef ATMDEPDAT_H
#define ATMDEPDAT_H

class Atmdepdat
{

  public:

     Atmdepdat( void );

/* *************************************************************
                      Public Functions
************************************************************* */

// read data structure.
     int getdel( FILE* infile );


/* *************************************************************
                     Public Variables
************************************************************* */

     // column or longitude of grid cell (degrees)
     float col;

     // row or latitude of grid cell (degrees)
     float row;

     // year
     int year;

     // ndep value
     float adepval;


  private:

/* *************************************************************
                      Private Variables
************************************************************* */

     int atmdepend;
     long curpos;
     long lagpos;

};

#endif

