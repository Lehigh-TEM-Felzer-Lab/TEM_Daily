/* *************************************************************
ELMNT45.H - Contains functions to manage elements used in GIS
             (Note: Elements are usually grid cells)

Modifications:

20060113 - DWK created by modifying elmnt50b5.h
20060113 - DWK change class Elmnt50 to class Elmnt43
20070105 - TWC changed name to elmnt45

************************************************************** */

#ifndef ELMNT45_H
#define ELMNT45_H

class Elmnt45
{

   public:

     Elmnt45( void );

/* **************************************************************
                 Public Functions
************************************************************** */

     int coregerr( ofstream& rflog1,
                   const string& varname1,
                   const float& col1,
                   const float& row1,
                   const string& varname2,
                   const float& col2,
                   const float& row2 );

     void show( ofstream& rflog1,
                const float& col,
                const float& row );

     void show( ofstream& rflog1,
                const float& col,
                const float& row,
                const long& totyr,
                const double& tol,
                const double& errcnt );

/* **************************************************************
                 Public Variables
************************************************************** */

     // Column or longitude of element
     float col;

     // Area of element
     long carea;

     // Continent location of element
     string contnent;

     // Count of elements in a region
     long count;

     // Mean Elevation of element
     double elev;

     int end;

     long grdcnt;

     long numskip;

     // Row or latitude of element
     float row;

     int stopflag;

     int strtflag;

     int  totyr;
     
     int endflag;
     
     long numgrids;


   private:

/* **************************************************************
                 Private Variables
************************************************************** */

};

#endif

