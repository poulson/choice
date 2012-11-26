#include "mpi_choice.hpp"
using namespace choice;

int
main( int argc, char** argv )
{
    MPI_Init( &argc, &argv );
    MPI_Comm comm = MPI_COMM_WORLD;

    int commRank;
    MPI_Comm_rank( comm, &commRank );

    try
    {
        int nb, height, width, nbLocal;
        double tol, ratio;

        MpiArgs args( argc, argv, comm );
        nb = args.Required<int>("--nb", "performance tuning parameter");
        height = args.Required<int>("--gridHeight", "process grid height");
        width = args.Required<int>("--gridWidth", "process grid width");
        tol = args.Required<double>("--tol", "rel. tolerance for GMRES");
        nbLocal = args.Optional("--nbLocal", 32, "local blocksize");
        ratio = args.Optional("--ratio", 0.5, "height/width ratio for QR");

        // This should always be called and ensures that all of the required
        // command-line arguments were specified 
        // (otherwise the report is printed and an exception is thrown)
        args.EnsureValid();

        // This prints the detailed report for the command-line options
        args.PrintReport();
        
        // A sanity check on the actual values
        if( commRank == 0 )
        {
            std::cout 
                << "nb      = " << nb << "\n"
                << "height  = " << height << "\n"
                << "width   = " << width << "\n"
                << "nbLocal = " << nbLocal << "\n"
                << "ratio   = " << ratio << "\n"
                << "tol     = " << tol << "\n"
                << std::endl;
        }
    }
    catch( std::logic_error& e )
    {
        std::ostringstream os;
        os << "Process " << commRank << " caught exception: " << e.what() 
           << std::endl;
        std::cerr << os.str();
    }

    MPI_Finalize();
    return 0;
}
