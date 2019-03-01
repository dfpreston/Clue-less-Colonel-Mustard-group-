////////////////////////////////////////////////////////////////////////////////
///
/// 	Clueless
///
////////////////////////////////////////////////////////////////////////////////
///
/// \file MersenneTwister.h
/// \brief Defines functions of the Mersenne Twister Pseudo-Random Number
///        Generator (PRNG) class.
///
///        The Mersenne Twister was developed and refined by Makoto Matsumoto
///        and his associates.  The streamlined initialization version includes
///        modifications by Matthe Bellew, Shawn Cokus, and Isaku Wada.
///
/// \note
/// - The MersenneTwister class separates the elements of the original C code
///   into header and source files to follow the C++ paradigm for use in an APL
///   developed simulation.  Additionally, comments and variable name changes
///   have been put in place in keeping with current group-wide coding standards
///   (v1.0, 2005).
/// - The copyright statement on M. Maksumoto's website permits free usage of
///   the code and algorithm even by a commerial venture.
///
///   "This version is free, in the sense that we adoped BSD license, which
///   permits modifications and use in a commercial product."
///
///       (http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html)
///
/// - Original file header information retained as specified by the author(s).
///
/// \date    06 September 2005
///
/// Modifications:
///  24 Mar 2006  mem  add draw methods to extend drawReal1, 2, & 3 for user
///                    specified interval
///
////////////////////////////////////////////////////////////////////////////////

/* 
   A C-program for MT19937, with initialization improved 2002/2/10.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   This is a faster version by taking Shawn Cokus's optimization,
   Matthe Bellew's simplification, Isaku Wada's real version.

   Before using, initialize the state by using init_genrand(seed) 
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

#ifndef _MersenneTwister_H_
#define _MersenneTwister_H_


class MersenneTwister
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Construction / Destruction
	////////////////////////////////////////////////////////////////////////////
	//``````````````````````````````````````````````````````````````````````````
	// Default Constructor
	//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	MersenneTwister()
	{
	}

	//``````````````````````````````````````````````````````````````````````````
	// Description: Extended Constructor 1.
	//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	MersenneTwister( unsigned long seed )
	{
		//initialize PRNG with specified seed
		initialize( seed );

	} //end routine MersenneTwister::MersenneTwister


	//``````````````````````````````````````````````````````````````````````````
	// Description: Extended Constructor 2.
	//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	MersenneTwister(
		unsigned long initialization_key_array[],
		int key_array_length )
	{
		//initialize PRNG with specified key array and key length
		initialize( initialization_key_array, key_array_length );

	} //end routine MersenneTwister::MersenneTwister


	//``````````````````````````````````````````````````````````````````````````
	// Destructor
	//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	~MersenneTwister()
	{
	} //end routine MersenneTwister::~MersenneTwister


	//--------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------
	// initializes state[N] with a seed (old name: init_genrand)
	static void initialize( unsigned long seed );

	// initialize by an array with array-length (old name: init_by_array)
	static void initialize(
		unsigned long initialization_key_array[], // array for initializing keys
		int key_array_length );                   // array's length

	////////////////////////////////////////////////////////////////////////////
	// Additional Member Functions
	////////////////////////////////////////////////////////////////////////////
	// generates a random number on [0,0xffffffff]-interval
	// old name: genrand_int32
	static unsigned long drawInt32(void);

	// generates a random number on [0,0x7fffffff]-interval
	// old name: genrand_int31
	static long drawInt31(void);

	// generates a random number on [0,1]-real-interval
	// old name: genrand_real1
	static double drawReal1(void);

	// generates a random number on [0,1)-real-interval
	// old name: genrand_real2
	static double drawReal2(void);

	// generates a random number on (0,1)-real-interval
	// old name: genrand_real2
	static double drawReal3(void);

	// generates a random number on [0,1) with 53-bit resolution
	// old name: genrand_res53
	static double drawRes53(void);

//chng  24 Mar 2006  mem
	// generates random number on [a,b]-real-interval
	static double drawReal1(double lower_bound, double upper_bound); // [a,b]

	// generates random number on [a,b)-real-interval
	static double drawReal2(double lower_bound, double upper_bound); // [a,b)

	// generates random number on (a,b)-real-interval
	static double drawReal3(double lower_bound, double upper_bound); // (a,b)

//endchng  mem  24 Mar 2006

private:
	static void next_state(void);

	static unsigned long mixBits( unsigned long first, unsigned long second );
	static unsigned long twist( unsigned long first, unsigned long second );

	////////////////////////////////////////////////////////////////////////////
	// Data Members
	////////////////////////////////////////////////////////////////////////////
private:
	static int  unused_draws_left;
	static bool is_initialized;

}; //end class MersenneTwister defn


#endif //_MersenneTwister_H_ defn
