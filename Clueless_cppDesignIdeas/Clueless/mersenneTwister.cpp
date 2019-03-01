////////////////////////////////////////////////////////////////////////////////
//
// Johns Hopkins University/Applied Physics Laboratory copyright does not apply.
//
////////////////////////////////////////////////////////////////////////////////
//
// 	AAWSim
//
////////////////////////////////////////////////////////////////////////////////
//
// File:	MersenneTwister.cpp
// Library:	<Library Name>
// Purpose:	Implements functions of the Mersenne Twister Pseudo-Random Number
//          Generator (PRNG) class.
//
//          The Mersenne Twister was developed and refined by Makoto Matsumoto
//          and his associates.  The streamlined initialization version includes
//          modifications by Matthe Bellew, Shawn Cokus, and Isaku Wada.
//
// Programmer Notes:
//          - The MersenneTwister class separates the elements of the original
//            C code into header and source files to follow the C++ paradigm
//            for use in an APL developed simulation.  Additionally, comments
//            and variable name changes have been put in place in keeping with
//            current group-wide coding standards (v1.0, 2005).
//          - The copyright statement on M. Maksumoto's website permits free
//            usage of the code and algorithm even by a commerial venture.
//
//              "This version is free, in the sense that we adoped BSD license,
//               which permits modifications and use in a commercial product."
//
//              (http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html)
//
//          - Original file header information retained as specified by the
//            author(s).
//
// Programmer:	 M. Mello
// Date Created: 06 September 2005
//
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

#include "mersenneTwister.h"


//------------------------------------------------------------------------------
/* Period parameters */
//------------------------------------------------------------------------------
const unsigned short NNN = 624;
const unsigned short MMM = 397;
const unsigned long  MATRIX_A = 0x9908b0dfUL;  /* constant vector a */
const unsigned long  UMASK = 0x80000000UL; /* most significant w-r bits */
const unsigned long  LMASK = 0x7fffffffUL; /* least significant r bits */

//------------------------------------------------------------------------------
// populate static member variables
//------------------------------------------------------------------------------
//NOTE: since decrement at begin of rn request, essentially means no draws avail
int  MersenneTwister::unused_draws_left = 1;
bool MersenneTwister::is_initialized    = false;

//------------------------------------------------------------------------------
// define additional static variables
//------------------------------------------------------------------------------
static unsigned long state[NNN]; /* the array for the state vector  */
static unsigned long *next = 0;  //ptr to draw from state array which will be
                                 // used for next request


//------------------------------------------------------------------------------
//-- Name:   initialize                                  
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Initializes state[N] with a seed.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  None
//--    - See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
//--    - In the previous versions, MSBs of the seed affect only MSBs of the
//--      array state[].
//--      2002/01/09 modified by Makoto Matsumoto
//--    - old name: init_genrand
//------------------------------------------------------------------------------
void
MersenneTwister::initialize( unsigned long seed ) //i
{
	//populate first element of state array with seed
    state[0] = seed & 0xffffffffUL;

	//for each element of state array
    for (int jjj = 1; jjj < NNN; ++jjj)
	{
		//populate with initial value based on previous element
        state[jjj] =
			(1812433253UL * (state[jjj-1] ^ (state[jjj-1] >> 30)) + jjj);

        state[jjj] &= 0xffffffffUL;  /* for >32 bit machines */
    }

    unused_draws_left = 1;
	is_initialized    = 1;

} //end routine MersenneTwister::initialize(seed)


//------------------------------------------------------------------------------
//-- Name:   initialize                                  
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Initialize by an array with array-length
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:
//--    - slight change for C++, 2004/2/26, Makoto Matsumoto
//--    - old name: init_by_array
//------------------------------------------------------------------------------
void
MersenneTwister::initialize(
	unsigned long init_key[], //i - array for initializing keys
	int key_length )          //i - init keys array length
{
	//initial pass at populating of state array (N elements) 
    initialize( 19650218UL );

    int iii = 1; //state array index
	int jjj = 0; //initialization key index
    int kkk = ( NNN > key_length ? NNN : key_length ); //primary loop index

	//for all elements of state and init key arrays
    for (; kkk > 0; kkk--)
	{
		//populate state array by incorporating values from all elements of
		// state and initialization key arrays
        state[iii] = (state[iii] ^ ((state[iii-1] ^ (state[iii-1] >> 30)) * 1664525UL))
          + init_key[jjj] + jjj; /* non linear */
        state[iii] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */

		//increment array indices
        ++iii;
		++jjj;

		//make sure do not access beyond end of state array
        if (iii >= NNN)
		{
			state[0] = state[NNN-1];
			iii = 1; //reset
		}

		//make sure do not access beyond end of init_key array
        if ( jjj >= key_length )
		{
			jjj=0; //reset
		}

    } //end for (all array elements)

	//for each element of state array
    for (kkk = NNN-1; kkk > 0; kkk--)
	{
        state[iii] =
			(state[iii] ^ ((state[iii-1] ^ (state[iii-1] >> 30)) * 1566083941UL))
            - iii; /* non linear */

        state[iii] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */

        ++iii; //increment array index

		//make sure do not access beyond end of state array
        if (iii >= NNN)
		{
			state[0] = state[NNN-1];
			iii = 1;
		}

    } //end for (each element of state array)

    state[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
    unused_draws_left = 1;
	is_initialized    = 1;

} //end MersenneTwister::initialize(init_key_array, key_array_length)


//------------------------------------------------------------------------------
//-- Name:  drawInt32
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,0xffffffff]-interval.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - old name: genrand_int32
//------------------------------------------------------------------------------
unsigned long
MersenneTwister::drawInt32(void)
{
    unsigned long yyy;

	//decrement state array elements left and compare
    if (--unused_draws_left == 0)
	{
		//repopulate state array with next sequence of draws
		next_state();
	}

	//assign current value of next to yyy and increment next
    yyy = *next++;

    /* Tempering */
    yyy ^= (yyy >> 11);
    yyy ^= (yyy << 7) & 0x9d2c5680UL;
    yyy ^= (yyy << 15) & 0xefc60000UL;
    yyy ^= (yyy >> 18);

    return yyy;

} //end routine MersenneTwister::drawInt32


//------------------------------------------------------------------------------
//-- Name:  drawInt31
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,0x7fffffff]-interval.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - old name: genrand_int31
//------------------------------------------------------------------------------
long
MersenneTwister::drawInt31(void)
{
    unsigned long yyy;

	//decrement state array elements left and compare
    if (--unused_draws_left == 0)
	{
		//repopulate state array with next sequence of draws
		next_state();
	}

	//assign current value of next to yyy and increment next
    yyy = *next++;

    /* Tempering */
    yyy ^= (yyy >> 11);
    yyy ^= (yyy << 7) & 0x9d2c5680UL;
    yyy ^= (yyy << 15) & 0xefc60000UL;
    yyy ^= (yyy >> 18);

    return (long)(yyy >> 1);

} //end routine MersenneTwister::drawInt31


//------------------------------------------------------------------------------
//-- Name:   drawReal1
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,1]-real-interval.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - These real versions are due to Isaku Wada, 2002/01/09 added
//--    - old name: genrand_real1
//------------------------------------------------------------------------------
double
MersenneTwister::drawReal1(void)
{
    unsigned long yyy;

	//decrement state array elements left and compare
    if (--unused_draws_left == 0)
	{
		//repopulate state array with next sequence of draws
		next_state();
	}

	//assign current value of next to yyy and increment next
    yyy = *next++;

    /* Tempering */
    yyy ^= (yyy >> 11);
    yyy ^= (yyy << 7) & 0x9d2c5680UL;
    yyy ^= (yyy << 15) & 0xefc60000UL;
    yyy ^= (yyy >> 18);

    return (double)yyy * (1.0 / 4294967295.0); 
    /* divided by 2^32-1 */ 

} //end routine MersenneTwister::drawReal1


//------------------------------------------------------------------------------
//-- Name:  drawReal2
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,1)-real-interval.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - These real versions are due to Isaku Wada, 2002/01/09 added
//--    - old name: genrand_real2
//------------------------------------------------------------------------------
double
MersenneTwister::drawReal2(void)
{
    unsigned long yyy;

	//decrement state array elements left and compare
    if (--unused_draws_left == 0)
	{
		//repopulate state array with next sequence of draws
		next_state();
	}

	//assign current value of next to yyy and increment next
    yyy = *next++;

    /* Tempering */
    yyy ^= (yyy >> 11);
    yyy ^= (yyy << 7) & 0x9d2c5680UL;
    yyy ^= (yyy << 15) & 0xefc60000UL;
    yyy ^= (yyy >> 18);

    return (double)yyy * (1.0 / 4294967296.0); 
    /* divided by 2^32 */

} //end routine MersenneTwister::drawReal2


//------------------------------------------------------------------------------
//-- Name:  drawReal3
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on (0,1)-real-interval.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - These real versions are due to Isaku Wada, 2002/01/09 added
//--    - old name: genrand_real3
//------------------------------------------------------------------------------
double
MersenneTwister::drawReal3(void)
{
    unsigned long yyy;

	//decrement state array elements left and compare
    if (--unused_draws_left == 0)
	{
		//repopulate state array with next sequence of draws
		next_state();
	}

	//assign current value of next to yyy and increment next
    yyy = *next++;

    /* Tempering */
    yyy ^= (yyy >> 11);
    yyy ^= (yyy << 7) & 0x9d2c5680UL;
    yyy ^= (yyy << 15) & 0xefc60000UL;
    yyy ^= (yyy >> 18);

    return ((double)yyy + 0.5) * (1.0 / 4294967296.0); 
    /* divided by 2^32 */

} //end routine MersenneTwister::drawReal3


//------------------------------------------------------------------------------
//-- Name:  drawRes53
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,1) with 53-bit resolution.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  
//--    - These real versions are due to Isaku Wada, 2002/01/09 added
//--    - old name: genrand_res53
//------------------------------------------------------------------------------
double
MersenneTwister::drawRes53(void) 
{ 
    unsigned long aaa = drawInt32() >> 5;
	unsigned long bbb = drawInt32() >> 6;

    return(aaa * 67108864.0 + bbb) * (1.0 / 9007199254740992.0);

} //end routine MersenneTwister::drawRes53


//chng  24 Mar 2006  mem

//------------------------------------------------------------------------------
//-- Name:  drawReal*(lower_bound, upper_bound)
//--
//-- Point of Contact:  M. Mello
//--
//-- Description: Generates a random number on specified real interval.
//--              Translates draw from corresponding 0,1 interval.
//--
//--                 method     orig     user specified interval
//--                ---------   -----   --------------------------
//--                drawReal1   [0,1]   [lower_bound, upper_bound]
//--                drawReal2   [0,1)   [lower_bound, upper_bound)
//--                drawReal3   (0,1)   (lower_bound, upper_bound)
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  None
//------------------------------------------------------------------------------
double
MersenneTwister::drawReal1(
	double low_bound,   //i - user specified lower interval boundary
	double up_bound)    //i - user specified upper interval boundary
{
	//return draw scaled to specified interval
	return ( (up_bound - low_bound) * drawReal1() + low_bound );

} //end routine MersenneTwister::drawReal1(lower, upper)


double
MersenneTwister::drawReal2(
	double low_bound,   //i - user specified lower interval boundary
	double up_bound)    //i - user specified upper interval boundary
{
	//return draw scaled to specified interval
	return ( (up_bound - low_bound) * drawReal2() + low_bound );

} //end routine MersenneTwister::drawReal2(lower, upper)


double
MersenneTwister::drawReal3(
	double low_bound,   //i - user specified lower interval boundary
	double up_bound)    //i - user specified upper interval boundary
{
	//return draw scaled to specified interval
	return ( (up_bound - low_bound) * drawReal3() + low_bound );

} //end routine MersenneTwister::drawReal3(lower, upper)

//endchng  mem  24 Mar 2006


//------------------------------------------------------------------------------
//-- Name:   next_state                                  
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Populates state array with next sequence of draws.  Resets
//--              indicators for number of draws requested from current state
//--              array.  Resets pointer for next draw to be used to head of
//--              state array.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:
//--    - If state array initialization (via init_genrand()) has not been
//--      executed, default seed will be used for initialization.
//------------------------------------------------------------------------------
void
MersenneTwister::next_state(void)
{
    unsigned long *p_state = state;
    int jjj; //local loop counter

    //if initialization not yet performed
    if ( !MersenneTwister::is_initialized )
	{
		//initialize with default seed
		initialize( 5489UL );
	}

	//reset variables indicating state array use
    unused_draws_left = NNN; //no draws yet requested from new state array
    next = state;            //next draw used from head of state array

	//for first N-M elements of state array
    for (jjj = NNN-MMM+1; --jjj; p_state++)
	{
        *p_state = p_state[MMM] ^ twist(p_state[0], p_state[1]);
	}

	//for next M elements of state array
    for (jjj = MMM; --jjj; p_state++)
	{
        *p_state = p_state[MMM-NNN] ^ twist(p_state[0], p_state[1]);
	}

	//populate last element of state array
    *p_state = p_state[MMM-NNN] ^ twist(p_state[0], state[0]);

} //end routine MersenneTwister::next_state


//------------------------------------------------------------------------------
//-- Name:  mixBits
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,1) with 53-bit resolution.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  None
//--    - Based on original defn:
//--        #define MIXBITS(u,v) ( ((u) & UMASK) | ((v) & LMASK) )
//------------------------------------------------------------------------------
unsigned long
MersenneTwister::mixBits(
	unsigned long uuu,
	unsigned long vvv )
{
	return ( (uuu & UMASK) | (vvv & LMASK) );

} //end routine MersenneTwister::mixBits


//------------------------------------------------------------------------------
//-- Name:  twist
//--
//-- Point of Contact:  M. Matsumoto et al.
//--
//-- Description: Generates a random number on [0,1) with 53-bit resolution.
//--
//-- Exceptions:  None
//--
//-- Programmer Notes:  None
//--    - Based on original defn:
//--        #define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v)&1UL ? MATRIX_A : 0UL))
//------------------------------------------------------------------------------
unsigned long
MersenneTwister::twist(
	unsigned long uuu,
	unsigned long vvv )
{
	return ( (mixBits(uuu, vvv) >> 1) ^ (vvv & 1UL ? MATRIX_A : 0UL) );

} //end routine MersenneTwister::twist

