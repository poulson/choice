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
#ifndef CHOICE_H
#define CHOICE_H 1

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

namespace choice {

class Args
{
public:
    Args( int argc, char** argv );

    template<typename T>
    T Required( std::string name, std::string desc="N/A" );

    template<typename T>
    T Optional( std::string name, T defaultVal, std::string desc="N/A" );

    void EnsureValid( std::ostream& os=std::cout ) const;
    void PrintReport( std::ostream& os=std::cout ) const;

private:
    int argc_;
    char** argv_;
    std::vector<bool> usedArgs_;

    struct RequiredArg
    { 
        std::string name, desc, typeInfo, usedVal; 
        bool found;

        RequiredArg
        ( std::string n, std::string d, std::string t, std::string uv, bool f ) 
        : name(n), desc(d), typeInfo(t), usedVal(uv), found(f) { };
    };

    struct OptionalArg
    { 
        std::string name, desc, typeInfo, defaultVal, usedVal; 
        bool found;

        OptionalArg
        ( std::string n, std::string d, std::string t, 
          std::string dv, std::string uv, bool f )
        : name(n), desc(d), typeInfo(t), 
          defaultVal(dv), usedVal(uv), found(f) { } 
    };

    template<typename TOut,typename TIn>
    TOut Cast( const TIn& input )
    {
        std::stringstream stream;
        TOut output;

        stream << input;
        stream >> output;

        return output;
    }

    std::vector<RequiredArg> requiredArgs_;
    std::vector<OptionalArg> optionalArgs_;
};

inline
Args::Args( int argc, char** argv )
: argc_(argc), argv_(argv), usedArgs_(argc,false) 
{ }

template<typename T>
inline T
Args::Required( std::string name, std::string desc )
{
    char** arg = std::find( argv_, argv_+argc_, name );
    const bool found = ( arg != argv_+argc_ );
    const bool invalidFound = ( arg == argv_+argc_-1 );
    if( invalidFound )
        throw std::logic_error("Missing value for last command-line argument");

    std::string typeInfo( typeid(T).name() );
    std::string usedVal = ( found ? arg[1] : "N/A" );
    requiredArgs_.push_back( RequiredArg(name,desc,typeInfo,usedVal,found) );

    // Before returning, store the used indices and check for duplication
    if( found )
    {
        const int offset = arg - argv_;
        if( usedArgs_[offset] || usedArgs_[offset+1] )
        {
            std::cerr << "WARNING: conflict with " << name << " detected at ";
            if( usedArgs_[offset] && usedArgs_[offset+1] )
                std::cerr << "arguments " << offset << " and " << offset+1
                          << std::endl;
            else if( usedArgs_[offset] )
                std::cerr << "argument " << offset << std::endl;
            else
                std::cerr << "argument " << offset+1 << std::endl;
            std::cerr << "Please ensure that you did request argument " 
                      << name << " multiple times" << std::endl;
        }
        usedArgs_[offset+0] = true;
        usedArgs_[offset+1] = true;

        arg = std::find( arg+1, argv_+argc_, name );
        if( arg != argv_+argc_ )
            std::cerr << "WARNING: " << name << " was specified twice and only "
                      << "the first instance is used" << std::endl;
    }

    return Cast<T>( usedVal );
}

template<typename T>
inline T
Args::Optional( std::string name, T defaultVal, std::string desc )
{
    char** arg = std::find( argv_, argv_+argc_, name );
    const bool found = ( arg != argv_+argc_ );
    const bool invalidFound = ( arg == argv_+argc_-1 );
    if( invalidFound )
        throw std::logic_error("Missing value for last command-line argument");

    std::string typeInfo( typeid(T).name() );

    std::string defValString = Cast<std::string>( defaultVal );
    std::string usedVal = ( found ? arg[1] : defValString );

    optionalArgs_.push_back
    ( OptionalArg(name,desc,typeInfo,defValString,usedVal,found) );

    // Before returning, store the used indices and check for duplication
    if( found )
    {
        const int offset = arg - argv_;
        if( usedArgs_[offset] || usedArgs_[offset+1] )
        {
            std::cerr << "WARNING: conflict with " << name << " detected at ";
            if( usedArgs_[offset] && usedArgs_[offset+1] )
                std::cerr << "arguments " << offset << " and " << offset+1
                          << std::endl;
            else if( usedArgs_[offset] )
                std::cerr << "argument " << offset << std::endl;
            else
                std::cerr << "argument " << offset+1 << std::endl;
            std::cerr << "Please ensure that you did request argument " 
                      << name << " multiple times" << std::endl;
        }
        usedArgs_[offset+0] = true;
        usedArgs_[offset+1] = true;

        arg = std::find( arg+1, argv_+argc_, name );
        if( arg != argv_+argc_ )
            std::cerr << "WARNING: " << name << " was specified twice and only "
                      << "the first instance is used" << std::endl;
    }

    if( found )
        return Cast<T>( usedVal );
    else
        return defaultVal; // avoid the double-cast
}

inline void 
Args::EnsureValid( std::ostream& os ) const
{
    int numFailed = 0;
    const int numRequired = requiredArgs_.size();
    for( int i=0; i<numRequired; ++i )
        if( !requiredArgs_[i].found )
            ++numFailed;
    if( numFailed > 0 )
    {
        PrintReport( os );
        throw std::logic_error("Missing command-line arguments");
    }
}

inline void 
Args::PrintReport( std::ostream& os ) const
{
    const int numRequired = requiredArgs_.size();
    const int numOptional = optionalArgs_.size();

    os << "Required arguments:\n";
    int numReqFailed = 0;
    for( int i=0; i<numRequired; ++i )
    {
        const RequiredArg& reqArg = requiredArgs_[i];
        if( !reqArg.found )
            ++numReqFailed;
        os << "  [" << i << "]:\n"
           << "    label:       " << reqArg.name << "\n"
           << "    description: " << reqArg.desc << "\n"
           << "    type string: " << reqArg.typeInfo << "\n"
           << "    used value:  " << reqArg.usedVal << "\n"
           << "    found:       " << reqArg.found << "\n";
    }
    std::cout << "\n";

    os << "Optional arguments:\n";
    int numOptFailed = 0;
    for( int i=0; i<numOptional; ++i )
    {
        const OptionalArg& optArg = optionalArgs_[i];
        if( !optArg.found )
            ++numOptFailed;
        os << "  [" << i << "]:\n"
           << "    label:         " << optArg.name << "\n"
           << "    description:   " << optArg.desc << "\n"
           << "    type string:   " << optArg.typeInfo << "\n"
           << "    default value: " << optArg.defaultVal << "\n"
           << "    used value:    " << optArg.usedVal << "\n"
           << "    found:         " << optArg.found << "\n";
    }
    std::cout << "\n";

    os << "Out of " << numRequired << " required arguments, " 
       << numReqFailed << " were not specified." << std::endl;

    os << "Out of " << numOptional << " optional arguments, "
       << numOptFailed << " were not specified.\n" << std::endl;
}

} // namespace choice

#endif // ifndef CHOICE_H