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
