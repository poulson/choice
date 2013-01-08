Choice
======

Choice is an extremely simple set of two header-file command-line parsers, 
[choice::Args](http://github.com/poulson/choice/blob/master/choice.hpp) and 
[choice::MpiArgs](http://github.com/poulson/choice/blob/master/mpi_choice.hpp),
which are both inspired by the command-line processing in 
[Madagascar](http://ahay.org). 
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
      --nb [i,N/A,NOT found]
        performance tuning parameter
    
      --height [i,N/A,NOT found]
        matrix height
    
    Optional arguments:
      --tol [d,1e-06,1e-06,NOT found]
        relative tolerance
    
      --print [b,0,0,NOT found]
        print results?
    
    Out of 2 required arguments, 2 were not specified.
    Out of 2 optional arguments, 2 were not specified.
 
whereas the command

    ./example --nb 64 --height 100

should successfully run. The 
[choice::MpiArgs example](http://github.com/poulson/choice/blob/master/mpi_example.cpp)
can similarly be compiled with the command

    mpicxx -I. mpi_example.cpp -o mpi_example    

and the commands

    mpirun -np 8 ./mpi_example

and 

    mpirun -np 8 ./mpi_example --nb 64 --height 100

would use eight MPI processes and have output equivalent to in the sequential
case.
