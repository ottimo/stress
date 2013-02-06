/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
stress is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * stress is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TS_GENERATOR_H_
#define _TS_GENERATOR_H_

#include <stdint.h>

# if __WORDSIZE == 64
#  define __INT64_C(c)  c ## L
#  define __UINT64_C(c) c ## UL
# else
#  define __INT64_C(c)  c ## LL
#  define __UINT64_C(c) c ## ULL
# endif

//# define UINT64_MAX             (__UINT64_C(18446744073709551615))
#define UINT64_MAX             (__UINT64_C(9223372036854775808))
//#define UINT64_MAX 0x1000000000000000000000000000000000000000000000000000000000000000

class TSGenerator{
	public:
		virtual bool enable(int id, int injector_id) = 0;
		virtual uint64_t getNextInjectionId(uint64_t& id) = 0;
};

class ExpTSGenerator : public TSGenerator{
	public:
		ExpTSGenerator();
		virtual bool enable(int id, int injector_id);
		virtual uint64_t getNextInjectionId(uint64_t& id);
};

class SeqTSGenerator : public TSGenerator{
	public:
		SeqTSGenerator();
		virtual bool enable(int id, int injector_id);
		virtual uint64_t getNextInjectionId(uint64_t& id);	
};

#endif // _TS_GENERATOR_H_
