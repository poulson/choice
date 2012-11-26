/*
   Copyright (c) 2012, Jack Poulson
   All rights reserved.

   This file is part of Choice, a simple command-line option library.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    - Neither the name of the owner nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MPI_CHOICE_H
#define MPI_CHOICE_H 1

#include "mpi.h"
#include "choice.hpp"

namespace choice {

class MpiArgs
{
private:
    Args args_;
    MPI_Comm comm_;
public:
    MpiArgs
    ( int argc, char** argv, 
      MPI_Comm comm=MPI_COMM_WORLD, 
      std::ostream& error=std::cerr ) 
    : args_( argc, argv, error ), comm_(comm) { };

    template<typename T>
    T Required( std::string name, std::string desc="N/A" )
    { return args_.Required<T>( name, desc ); }

    template<typename T>
    T Optional( std::string name, T defaultVal, std::string desc="N/A" )
    { return args_.Optional( name, defaultVal, desc ); }

    void EnsureValid( std::ostream& output=std::cout ) const
    { 
        int commRank;
        MPI_Comm_rank( comm_, &commRank );

        int flag;
        if( commRank == 0 )
        {
            flag = 1;
            try { args_.EnsureValid( output ); }
            catch( std::logic_error& e ) { flag = 0; }
            MPI_Bcast( &flag, 1, MPI_INT, 0, comm_ );
        }
        else
        {
            MPI_Bcast( &flag, 1, MPI_INT, 0, comm_ ); 
        }

        if( flag == 0 )
            throw std::logic_error("Missing command-line arguments");
    }

    void PrintReport( std::ostream& output=std::cout ) const
    {
        int commRank;
        MPI_Comm_rank( comm_, &commRank );

        if( commRank == 0 )
            args_.PrintReport( output );
    }
};

} // namespace choice

#endif // ifndef MPI_CHOICE_H
