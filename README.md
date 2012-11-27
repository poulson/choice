Choice
======

Choice is an extremely simple set of two header-file command-line parsers, 
[choice::Args](http://github.com/poulson/choice/blob/master/choice.hpp) and 
[choice::MpiArgs](http://github.com/poulson/choice/blob/master/mpi_choice.hpp). 
Their usage is demonstrated through the following
[serial](http://github.com/poulson/choice/blob/master/example.cpp) and 
[parallel](http://github.com/poulson/choice/blob/master/mpi_example.cpp) 
example drivers, and the only remaining detail is that the argument `--help`
causes the routines `Args::Process` and `MpiArgs::Process` to print the 
lists of required and optional arguments (as well as their default values 
and status within the current invocation).

The purpose of the library was to fulfill the needs of the many example 
driver of the projects 
[Elemental](http://code.google.com/p/elemental), [Clique](http://github.com/poulson/Clique), and [PSP](http://github.com/poulson/PSP).
[Elemental's example drivers](http://github.com/poulson/Elemental/blob/master/examples) have already been converted to make use of 
[choice::MpiArgs](http://github.com/poulson/choice/blob/master/mpi_choice.hpp) 
and the remaining drivers are soon to follow.

Usage
-----

The 
[choice::Args example](http://github.com/poulson/choice/blob/master/example.cpp)
can typically be compiled with the command:

    g++ -I. example.cpp -o example

Running it without any command-line options, e.g., with

    ./example

would produce the output

    Required arguments:
      --nb
        description: performance tuning parameter
        type string: i
        used value:  N/A
        NOT found
    
      --gridHeight
        description: process grid height
        type string: i
        used value:  N/A
        NOT found
    
    Optional arguments:
      --tol
        description:   rel. tolerance for GMRES
        type string:   d
        default value: 1e-06
        used value:    1e-06
        NOT found
    
      --nbLocal
        description:   local blocksize
        type string:   i
        default value: 32
        used value:    32
        NOT found
    
      --ratio
        description:   height/width ratio for QR
        type string:   d
        default value: 0.5
        used value:    0.5
        NOT found
    
    Out of 2 required arguments, 2 were not specified.
    Out of 3 optional arguments, 3 were not specified.
 
whereas the command

    ./example --nb 64 --gridHeight 3

should successfully run. The 
[choice::MpiArgs example](http://github.com/poulson/choice/blob/master/mpi_example.cpp)
can similarly be compiled with the command

    mpicxx -I. mpi_example.cpp -o mpi_example    

and the commands

    mpirun -np 8 ./mpi_example

and 

    mpirun -np 8 ./mpi_example --nb 64 --gridHeight 3

would use eight MPI processes and have output equivalent to in the sequential
case.
