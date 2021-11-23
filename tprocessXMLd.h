/* *************************************************************
TPROCESSMXLD.H - Functions to process XML files for TEM

Programmer: Shaomin Hu and David Kicklighter
Creation Date: 17 June 2003

Modifications:

20060126 - DWK created by modifying ttprocessXML51.h
20060126 - DWK changed class ProcessXML50 to class ProcessXML43
20060126 - DWK changed include from temconsts51.hpp to
           temconsts43.hpp
20070105 - TWC changed name to tprocessxml45

************************************************************** */

#ifndef PROCESSXMLD_H
#define PROCESSXMLD_H

#include "temconstsd.hpp"

class ProcessXML45
{
  public:

  ProcessXML45();

  void endXMLcommunityNode( ifstream& infile );
  void endXMLtvegNode( ifstream& infile );

  double getXMLcmntArrayDouble( ifstream& infile,
                                const string& rootnode,
                                const string& varnode,
                                const int& index );

  int getXMLcmntArrayInt( ifstream& infile,
                          const string& rootnode,
                          const string& varnode,
                          const int& index );

  long getXMLcmntArrayLong( ifstream& infile,
                            const string& rootnode,
                            const string& varnode,
                            const int& index );

  int getXMLcommunityNode( ifstream& infile,
                           const string& rootnode );

  double getXMLdouble( ifstream& infile,
                       const string& rootnode,
                       const string& varnode );

  int getXMLint( ifstream& infile,
                 const string& rootnode,
                 const string& varnode );

  long getXMLlong( ifstream& infile,
                   const string& rootnode,
                   const string& varnode );

  int getXMLrootNode( ifstream& infile, const string& rootnode );

  int getXMLsiteCommunityNode( ifstream& infile,
                               const string& rootnode,
                               string& description );

  int getXMLsiteRootNode( ifstream& infile,
                          const string& rootnode,
                          string& version,
                          string& sitename,
                          string& sitecol,
                          string& siterow,
                          string& developer,
                          string& updated );
                          
  string getXMLstring( ifstream& infile,
                       const string& rootnode,
                       const string& varnode );

  int getXMLtemvegNode( ifstream& infile, const string& rootnode );

  double getXMLtvegArrayDouble( ifstream& infile,
                                const string& rootnode,
                                const string& varnode,
                                const int& index );

  int getXMLtvegArrayInt( ifstream& infile,
                          const string& rootnode,
                          const string& varnode,
                          const int& index );

};

#endif
