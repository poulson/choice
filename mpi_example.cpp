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
        int nb, height, nbLocal;
        double tol, ratio;

        MpiArgs args( argc, argv, comm );
        nb = args.Required<int>("--nb", "performance tuning parameter");
        height = args.Required<int>("--gridHeight", "process grid height");
        tol = args.Optional("--tol", 1.e-6, "rel. tolerance for GMRES");
        nbLocal = args.Optional("--nbLocal", 32, "local blocksize");
        ratio = args.Optional("--ratio", 0.5, "height/width ratio for QR");
        // This should always be called and ensures that all of the required
        // command-line arguments were specified 
        // (otherwise the report is printed and an exception is thrown).
        // It also prints the report and throws an exception if "--help" is 
        // specified as a command-line option.
        args.Process();
    }
    catch( ArgException& e )
    {
        // There is no reason to do anything
    }

    MPI_Finalize();
    return 0;
}
