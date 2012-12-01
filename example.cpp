#include "choice.hpp"
using namespace choice;

int
main( int argc, char** argv )
{
    try
    {
        int nb, height, nbLocal;
        double tol, ratio;

        Args args( argc, argv );

        // Required arguments
        nb = args.Input<int>("--nb", "performance tuning parameter");
        height = args.Input<int>("--gridHeight", "process grid height");

        // Optional arguments (which have default values)
        tol = args.Input("--tol", "rel. tolerance for GMRES", 1.e-6);
        nbLocal = args.Input("--nbLocal", "local blocksize", 32);
        ratio = args.Input("--ratio", "height/width ratio for QR", 0.5);

        // This should always be called and ensures that all of the required
        // command-line arguments were specified 
        // (otherwise the report is printed and an exception is thrown)
        args.Process();
    }
    catch( ArgException& e ) 
    { 
        // There is no reason to do anything 
    }

    return 0;
}
