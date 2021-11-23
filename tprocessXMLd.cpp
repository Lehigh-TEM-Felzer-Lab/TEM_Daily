/* *************************************************************
TPROCESSMXLD.CPP - Functions to process XML files for TEM

Programmer: David Kicklighter
Creation Date: 17 June 2003

Modifications:

20060126 - DWK created by modifying tprocessXML51.cpp
20060126 - DWK changed include from tprocessXML51.h to
           tprocessXML437.h
20060126 - DWK changed ProcessXML50:: to ProcessXML43::
20070105 - TWC changed name to tprocessxml45

************************************************************* */

#include<iostream>

  using std::cerr;
  using std::endl;

#include<fstream>

  using std::ifstream;

#include<cstdlib>

  using std::exit;
  using std::atof;
  using std::atoi;
  using std::atol;

#include<string>

  using std::string;

#include "tprocessXMLd.h"


ProcessXML45::ProcessXML45()
{

};

/* *************************************************************
************************************************************* */

void ProcessXML45::endXMLcommunityNode( ifstream& infile )

{
  string line;

  while ( line.find( "</community>" ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

void ProcessXML45::endXMLtvegNode( ifstream& infile )

{
  string line;

  while ( line.find( "</temveg>" ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLcommunityNode( ifstream& infile,
                                       const string& rootnode )

{
  string line;
  string value;

  string tempstr;

  unsigned int startString;

  while ( line.find( ">" ) == string::npos
          && !infile.eof() )
  {
    getline( infile, tempstr );
    if ( tempstr.size() > 0 )
    {
      line += tempstr;
      tempstr.erase();
    }
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find community type in " << rootnode;
    cerr << " !!!" << endl;
    exit( -1 );
  }

  startString = line.find( "<community type = " );
  tempstr = line.substr( startString, 30 );
  startString = tempstr.find( '"' );
  value = tempstr.substr( (startString+1), 5 );

  return atoi( value.c_str() );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double ProcessXML45::getXMLcmntArrayDouble( ifstream& infile,
                                            const string& rootnode,
                                            const string& varnode,
                                            const int& index )

{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode << " for community type ";
    cerr << index << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return MISSING; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atof( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLcmntArrayInt( ifstream& infile,
                                      const string& rootnode,
                                      const string& varnode,
                                      const int& index )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode << " for community type ";
    cerr << index << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return -99; }
  else
  {
    value = line.substr( (startString+1), 1 );
    return atoi( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

long ProcessXML45::getXMLcmntArrayLong( ifstream& infile,
                                        const string& rootnode,
                                        const string& varnode,
                                        const int& index )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode << " for community type ";
    cerr << index << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return -99; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atol( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double ProcessXML45::getXMLdouble( ifstream& infile,
                                   const string& rootnode,
                                   const string& varnode )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode;
    cerr << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return MISSING; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atof( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLint( ifstream& infile,
                             const string& rootnode,
                             const string& varnode )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode;
    cerr << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return -99; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atoi( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

long ProcessXML45::getXMLlong( ifstream& infile,
                               const string& rootnode,
                               const string& varnode )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode;
    cerr << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return -99; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atol( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLrootNode( ifstream& infile,
                                  const string& rootnode )

{
  string line;

  while ( line.find( rootnode ) == string::npos && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << rootnode << " !!!" << endl;
    exit( -1 );
  }

  return 0;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLsiteCommunityNode( ifstream& infile,
                                           const string& rootnode,
                                           string& description )

{
  string line;
  string value;

  string tempstr;
  string tempstr2;

  unsigned int startString;
  unsigned int endString;

  while ( line.find( ">" ) == string::npos
          && !infile.eof() )
  {
    getline( infile, tempstr );
    if ( tempstr.size() > 0 )
    {
      line += tempstr;
      tempstr.erase();
    }
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find community type in " << rootnode;
    cerr << " !!!" << endl;
    exit( -1 );
  }

  startString = line.find( "<community type = " );
  tempstr = line.substr( startString, 30 );
  startString = tempstr.find( '"' );
  value = tempstr.substr( (startString+1), 5 );

  tempstr.erase();
  startString = line.find( "description" );
  tempstr = line.substr( startString, 50 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 50 );
  endString = tempstr2.find( '"' );
  description = tempstr2.substr( 0, endString );

  return atoi( value.c_str() );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLsiteRootNode( ifstream& infile,
                                      const string& rootnode,
                                      string& version,
                                      string& sitename,
                                      string& sitecol,
                                      string& siterow,
                                      string& developer,
                                      string& updated )
{

  string line;
  string tempstr;
  string tempstr2;

  unsigned int startString;
  unsigned int endString;

  while ( line.find( rootnode ) == string::npos && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << rootnode << " !!!" << endl;
    exit( -1 );
  }

  while ( line.find( ">" ) == string::npos && !infile.eof() )
  {
    getline( infile, tempstr );
    if ( tempstr.size() > 0 )
    {
      line += tempstr;
      tempstr.erase();
    }
  }


  startString = line.find( "version" );
  tempstr = line.substr( startString, 50 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 50 );
  endString = tempstr2.find( '"' );
  version = tempstr2.substr( 0, endString );

  tempstr.erase();
  startString = line.find( "site" );
  tempstr = line.substr( startString, 80 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 80 );
  endString = tempstr2.find( '"' );
  sitename = tempstr2.substr( 0, endString );

  tempstr.erase();
  startString = line.find( "longitude" );
  tempstr = line.substr( startString, 50 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 50 );
  endString = tempstr2.find( '"' );
  sitecol = tempstr2.substr( 0, endString );

  tempstr.erase();
  startString = line.find( "latitude" );
  tempstr = line.substr( startString, 50 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 50 );
  endString = tempstr2.find( '"' );
  siterow = tempstr2.substr( 0, endString );

  tempstr.erase();
  startString = line.find( "developedBy" );
  tempstr = line.substr( startString, 80 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 80 );
  endString = tempstr2.find( '"' );
  developer = tempstr2.substr( 0, endString );

  tempstr.erase();
  startString = line.find( "updated" );
  tempstr = line.substr( startString, 50 );
  startString = tempstr.find( '"' );
  tempstr2 = tempstr.substr( (startString+1), 50 );
  endString = tempstr2.find( '"' );
  updated = tempstr2.substr( 0, endString );

  return 0;

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

string ProcessXML45::getXMLstring( ifstream& infile,
                               const string& rootnode,
                               const string& varnode )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;
  unsigned int endString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode;
    cerr << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  endString = line.find( "</" );
  if ( startString == string::npos ) { return varnode + " is missing"; }
  else
  {
    value = line.substr( (startString+1), (endString - startString - 1) );
    return value;
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLtemvegNode( ifstream& infile,
                                    const string& rootnode )
{
  string line;
  string value;

  string tempstr;
  unsigned int startString;


  while ( line.find( ">" ) == string::npos
          && !infile.eof() )
  {
    getline( infile, tempstr );
    if ( tempstr.size() > 0 )
    {
      line += tempstr;
      tempstr.erase();
    }
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find TEMVEG type in " << rootnode;
    cerr << " !!!" << endl;
    exit( -1 );
  }

  startString = line.find( "<temveg type = " );
  tempstr = line.substr( startString, 30 );
  startString = tempstr.find( '"' );
  value = tempstr.substr( (startString+1), 5 );

  return atoi( value.c_str() );

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

double ProcessXML45::getXMLtvegArrayDouble( ifstream& infile,
                                            const string& rootnode,
                                            const string& varnode,
                                            const int& index )

{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode << " for TEMVEG = ";
    cerr << index << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return MISSING; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atof( value.c_str() );
  }

};

/* *************************************************************
************************************************************* */


/* *************************************************************
************************************************************* */

int ProcessXML45::getXMLtvegArrayInt( ifstream& infile,
                                      const string& rootnode,
                                      const string& varnode,
                                      const int& index )
{
  string line;
  string value;

  string endVarnode = "</" + varnode + ">";
  unsigned int startString;

  while ( line.find( endVarnode ) == string::npos
          && !infile.eof() )
  {
    getline( infile, line );
  }

  if ( 0 == line.size() )
  {
    cerr << endl << "Cannot find " << varnode << " for TEMVEG = ";
    cerr << index << " in " << rootnode << endl;
    exit( -1 );
  }

  startString = line.find( ">" );
  if ( startString == string::npos ) { return (int) MISSING; }
  else
  {
    value = line.substr( (startString+1), 20 );
    return atoi( value.c_str() );
  }

};


