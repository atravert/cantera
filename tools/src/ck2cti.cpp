/**
 * @file ck2cti.cpp
 * $Id$
 *
 * Program to convert Chemkin-II-format reaction mechanism files to
 * Cantera input format. The resulting Cantera input file contains a
 * definition of one ideal_gas entry that represents an ideal gas
 * mixture corresponding to the Chemkin-II reaction mechanism.  The
 * file also contains Cantera-format definitions for each species and
 * each reaction in the input reaction mechanism file.
 *
 * Usage: ck2cti -i input -t thermo -tr transport -id idtag
 *
 * The Cantera-format text is written to the standard output.
 *
 * @param input Chemkin-II reaction mechanism file to be converted. Required.
 *
 * @param thermo Thermodynamic property database. If the THERMO section of the 
 * input file is missing or does not have entries for one or more species,
 * this file will be searched for the required thermo data. This file may 
 * be another reaction mechanism file containing a THERMO section, or
 * a Chemkin-II-compatible thermodynamic database file.
 *
 * @param transport Transport property database. If this file name is supplied,
 * transport property parameters will be taken from this file and 
 * included in the output Cantera-format file. If this parameter is omitted, 
 * no transport property parameters will be included in the output.
 *
 * @param id idtag. The ideal_gas entry in the Cantera-format output 
 * has name \i idtag. If this parameter is omitted, it will be set to the
 * input file name without the extension. Since only one phase definition
 * is present in the ck2cti output, this parameter is not required.  
 */
#ifdef WIN32
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include <iostream>
#include <string>
using namespace std;

#include "ct_defs.h"
#include "global.h"
#include "converters/ck2ct.h"

using namespace Cantera;

int showHelp() {
    cout << "\nck2cti: convert a CK-format reaction mechanism file to Cantera input format.\n"
         << "\n   D. G. Goodwin, Caltech \n"
         << "   Version 1.0, August 2003.\n\n"
         << endl;
    cout << "options:" << endl;
    cout << "    -i  <input file> \n"
         << "    -t  <thermo database> \n"
         << "    -tr <transport database> \n"
         << "    -id  <identifier> \n"
         << "    -d  print debugging output \n\n"
         << "The results are written to the standard output.\n";
    return 0;
}

int main(int argc, char** argv) {
    string infile="chem.inp", dbfile="", trfile="", logfile;
    string idtag = "gas";
    bool debug = false;
    int i=1;
    if (argc == 1) return showHelp();
 
    while (i < argc) {
        string arg = string(argv[i]);
        if (i < argc-1) {
            if (arg == "-i") {
                infile = argv[i+1];
                ++i;
            }
            else if (arg == "-t") {
                dbfile = argv[i+1];
                ++i;
            }
            else if (arg == "-tr") {
                trfile = argv[i+1];
                ++i;
            }
            else if (arg == "-id") {
                idtag = argv[i+1];
            }
        }
        else if (arg == "-d") {
            debug = true;
            cout << "### DEBUG MODE ###" << endl;
        }
        else if (arg == "-h" || argc < 3) {
            return showHelp(); 
        }
        ++i;
    }

    int ierr = pip::convert_ck(infile.c_str(), dbfile.c_str(), trfile.c_str(), 
        idtag.c_str(), debug);

    if (ierr < 0) {
        showErrors(cerr);
    }
    return ierr;
}
