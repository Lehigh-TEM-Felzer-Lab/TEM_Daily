#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

int gisend,i,yr,icht,leap;
double v[366],tarea;
char file[100];
string infile, chrtfname, outfile;
int startyr, lastyr, firstchrtyr, tlulcflag;

#include "ttemdailyread.h"
#include "tmaxcohortdat45.h"

        Temdailyread temdat;
        MaxCohortdat45 mxdat;
        FILE* ifile;
        FILE* itfile;

int main ()
{


   cout << endl << "Enter the name of the input file: ";

   cin >> infile;

   cout << endl << "Enter the name of the mxcohort file: ";

   cin >> chrtfname;

   cout << endl << "total area of grid cell: ";

   cin >> tarea;

    cout << "Enter the first year in the input file: ";

    cin >> startyr;

    cout << "Enter the last year in the input file: ";

    cin >> lastyr;

    cout << "Does land cover change over time in the output: ";
    cout << endl;
    cout << "Enter 0 for no changes in land cover: " << endl;
    cout << "Enter 1 for changing land cover: " << endl;

    cin >> tlulcflag;

    if( 1 == tlulcflag )
    {
      cout << "Enter the first year of the maximum number of ";
      cout << "cohorts data: " << endl;

      cin >> firstchrtyr;
    }

   cout << endl << "Enter the name of the output file: ";

   cin >> outfile;

//   tarea = 142.0;

//    ifile = fopen("/share/partition7/bfelzer/tem/msb_lulc/crumxcohrts1895_2011.hvd", "r");
    ifile = fopen(chrtfname.c_str(), "r");

        if (!ifile){
                cerr << "lulc file could not be opened\n";
                exit(1);
        }

//    itfile = fopen("/share/partition7/tem/temhydro_combo/temhydro_cwd/NEP_MONTR.HVD", "r");
    itfile = fopen( infile.c_str(), "r");

        if (!itfile){
                cerr << "tem file could not be opened\n";
                exit(1);
        }

        ofstream fout;
//        fout.open("/share/partition7/tem/temhydro_combo/temhydro_cwd/sum_mon_NEP_MONTR.HVD", ios::out);
        fout.open(outfile.c_str(), ios::out);

       if(tlulcflag == 1)
       {
        if(firstchrtyr < startyr)
        {
        gisend = mxdat.getdel(ifile);
        }
       }
       if(tlulcflag == 0)
       {
        gisend = mxdat.getdel(ifile);
       }

//        for(yr=1896; yr<2007; yr++) {
        for(yr=startyr; yr<lastyr + 1; yr++) {

        cout << "year = " << yr << endl;

      if(yr == 1992 || yr == 1996 || yr == 2000 || yr == 2004 || yr == 2008)
      {
        leap = 1;
      }
      else
      {
        leap = 0;
      }

          if(tlulcflag == 1)
          {
	   gisend = mxdat.getdel(ifile);
          }
         for(i=0; i < 366; i++)
         {
         v[i] = 0.0;
         }
         cout << "cohorts = " << mxdat.total << endl;
         for(icht = 0; icht < mxdat.total; icht++) {
         if(leap == 1) 
         {
         gisend = temdat.getdel_leap(itfile);
//         cout << "reading in leap = " << temdat.year << " " << temdat.subarea << " " << temdat.region << endl;
         for(i=0; i < 366; i++)
         {
            v[i] = v[i] + temdat.mon[i] * temdat.subarea/tarea;
//            cout << "leap diag = " << i << " " << temdat.mon[i] << " " << temdat.subarea << " " << tarea << endl;
         }
         }
         else
         {
         gisend = temdat.getdel(itfile);
//         cout << "reading in nonleap = " << temdat.year << " " << temdat.subarea << " " << temdat.region << endl;
         for(i=0; i < 365; i++)
         {
            v[i] = v[i] + temdat.mon[i] * temdat.subarea/tarea;
//         cout << "nonleap diag = " << i << " " << temdat.mon[i] << " " << temdat.subarea << " " << tarea << endl;
         }

         }

            }  // cohort loop

        if(leap == 1)
        {
         for(i=0; i < 366; i++)
         {
          fout << yr << " " << i+1 << " " << v[i] << endl;
         }
        }
        else
        {
         for(i=0; i < 365; i++)
         {
          fout << yr << " " << i+1 << " " << v[i] << endl;
         }

        }

	  }  // year loop 

	fclose( ifile );
	fclose( itfile );
        fout.close();

};
