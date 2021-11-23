/* *************************************************************
ELMNT45.CPP - Contains functions to manage elements used in GIS

Modifications:

20060113 - DWK created by modifying elmnt50b5.cpp
20060113 - DWK changed include from elmnt50b5.h to elmnt437.h
20060113 - DWK changed Elmnt50:: to Elmnt43
20070105 - TWC changed name to Elmnt45

************************************************************** */

//#define BORLAND_CPP

#include<iostream>

  using std::cout;
  using std::ios;
  using std::endl;

#include<iomanip>

  using std::setprecision;

#include<fstream>

  using std::ofstream;

#include<string>

  using std::string;

#ifdef BORLAND_CPP

  #include<time>

  using std::time_t;

#else
  #include<ctime>

  using std::time_t;

#endif

#include "elmnt45.h"


Elmnt45::Elmnt45( void )
{

  count = 0;
  numskip  = 0;
  numgrids = 64000;
  grdcnt  = 64000;
  totyr    = -99;
  endflag  = 1;
  stopflag = 0;

  col = -999.9;
  row = -999.9;
  carea = -99;
  elev  = -999.9;

  return;
};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Elmnt45::show( ofstream& rflog1,
                    const float& col,
                    const float& row )
{

  time_t timer;

  timer = time( NULL );

   cout.setf( ios::fixed,ios::floatfield );
   cout.setf( ios::showpoint );
   rflog1.setf( ios::fixed,ios::floatfield );
   rflog1.setf( ios::showpoint );


  if( 0 == count || (grdcnt != -99 && count <= grdcnt) )
  {
    cout << "Finished cell " << (count+numskip) << " (";
    cout << setprecision( 1 ) << col << " , " << row << ") ";
    cout << ctime( &timer );
  }

    if( count == grdcnt ||
        (count < grdcnt && 0 == endflag && count == numgrids) )
    {
      cout << "Finished printing to the screen.  GOOD LUCK!!!!!" << endl;
    }

    rflog1 << "Finished cell ";
    rflog1 << setprecision( 0 ) << (count+numskip) << " (";
    rflog1 << setprecision(1) << col << " , " << row << ") ";
    rflog1 << ctime(&timer);

    if( 0 == endflag && count == numgrids ) { stopflag = 1; }

    ++count;


};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void Elmnt45::show( ofstream& rflog1,
                    const float& col,
                    const float& row,
                    const long& totyr,
                    const double& tol,
                    const double& errcnt )
{

  time_t timer;

  timer = time( NULL );

  cout.setf( ios::fixed,ios::floatfield );
  cout.setf( ios::showpoint );
  rflog1.setf( ios::fixed,ios::floatfield );
  rflog1.setf( ios::showpoint );

  if( 0 == count || (grdcnt != -99 && count <= grdcnt) )
  {
    cout << "Finished cell " << (count+numskip) << " (";
    cout << setprecision( 1 ) << col << " , ";
    cout << row << ")  TOTYR = ";
    cout << setprecision( 0 ) << totyr << " TOL = ";
    cout << setprecision( 6 ) << tol << " " << ctime( &timer );
  }

  if( count == grdcnt ||
      (count < grdcnt && 0 == endflag && count == numgrids) )
  {
    cout << "Finished printing to the screen.  GOOD LUCK!!!!!" << endl;
  }

  rflog1 << "Finished cell " << setprecision( 0 ) << (count+numskip) << " (";
  rflog1 << setprecision( 1 ) << col << " , " << row << ")  TOTYR = ";
  rflog1 << setprecision( 0 ) << totyr << " TOL = ";
  rflog1 << setprecision( 6 ) << tol << " " << ctime( &timer );
  if(errcnt > 0.0) {rflog1 << "labile carbon error occurs" << endl;}

  if( 0 == endflag && count == numgrids ) { stopflag = 1; }

  ++count;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int Elmnt45::coregerr( ofstream& rflog1,
                       const string& varname1,
                       const float& col1,
                       const float& row1,
                       const string& varname2,
                       const float& col2,
                       const float& row2 )
{

  int fatalerr = 0;

  if( col1 != col2 || row1 != row2 )
  {
    fatalerr = 1;

    cout << "ERROR:  " << varname1 << " data and ";
    cout << varname2 << "data are not coregistered." << endl;
    cout << "COL = " << col1 << " and ROW = " << row1;
    cout << " in " << varname1 << " data" << endl;
    cout << "COL = " << col2 << " and ROW = " << row2;
    cout << " in " << varname2 << " data" << endl;


    rflog1 << "ERROR:  " << varname1 << " data and ";
    rflog1 << varname2 << "data are not coregistered." << endl;
    rflog1 << "COL = " << col1 << " and ROW = " << row1;
    rflog1 << " in " << varname1 << " data" << endl;
    rflog1 << "COL = " << col2 << " and ROW = " << row2;
    rflog1 << " in " << varname2 << " data" << endl;
  }

  return fatalerr;

};
