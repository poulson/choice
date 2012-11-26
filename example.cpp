#include "choice.hpp"
using namespace choice;

int
main( int argc, char** argv )
{
    try
    {
        int nb, height, width, nbLocal;
        double tol, ratio;

        Args args( argc, argv );
        nb = args.Required<int>("--nb", "performance tuning parameter");
        height = args.Required<int>("--gridHeight", "process grid height");
        width = args.Required<int>("--gridWidth", "process grid width");
        tol = args.Required<double>("--tol", "rel. tolerance for GMRES");
        nbLocal = args.Optional("--nbLocal", 32, "local blocksize");
        ratio = args.Optional("--ratio", 0.5, "height/width ratio for QR");

        // This should always be called and ensures that all of the required
        // command-line arguments were specified 
        // (otherwise the report is printed and an exception is thrown)
        args.Process();

        // This prints the detailed report for the command-line options
        args.PrintReport();
        
        // A sanity check on the actual values
        std::cout 
            << "nb      = " << nb << "\n"
            << "height  = " << height << "\n"
            << "width   = " << width << "\n"
            << "nbLocal = " << nbLocal << "\n"
            << "ratio   = " << ratio << "\n"
            << "tol     = " << tol << "\n"
            << std::endl;
    }
    catch( ArgException& e ) 
    { 
        // There is no reason to do anything 
    }

    return 0;
}
