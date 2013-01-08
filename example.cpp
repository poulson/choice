/*
   Copyright (c) 2012-2013, Jack Poulson
   All rights reserved.

   This file is part of Choice and is under the BSD 2-Clause License, 
   which can be found in the LICENSE file in the root directory, or at 
   http://opensource.org/licenses/BSD-2-Clause
*/
#include "choice.hpp"
using namespace choice;

int
main( int argc, char** argv )
{
    try
    {
        Args args( argc, argv );

        // Required arguments
        const int nb = args.Input<int>("--nb","performance tuning parameter");
        const int height = args.Input<int>("--height","matrix height");

        // Optional arguments (which have default values)
        const double tol = args.Input("--tol","relative tolerance",1.e-6);
        const bool print = args.Input("--print","print results?",false);

        // This should always be called and ensures that all of the required
        // command-line arguments were specified 
        // (otherwise the report is printed and an exception is thrown)
        args.Process();

        if( print )
            std::cout << "Would have printed data." << std::endl;
    }
    catch( ArgException& e ) 
    { 
        // There is no reason to do anything 
    }

    return 0;
}
