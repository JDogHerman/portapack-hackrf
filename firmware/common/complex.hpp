/*
 * Copyright (C) 2014 Jared Boone, ShareBrained Technology, Inc.
 *
 * This file is part of PortaPack.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef __COMPLEX_H__
#define __COMPLEX_H__

#include <cstdint>
#include <complex>
#include <cmath>

#include <hal.h>

constexpr float pi { 3.141592653589793238462643383279502884f };

namespace std {

template<> struct complex<int8_t> {
public:
	typedef int8_t value_type;
	typedef uint16_t rep_type;

	// constexpr complex(
	// 	rep_type r
	// ) : _rep { r }
	// {
	// }

	constexpr complex(
		int8_t re = 0,
		int8_t im = 0
	) : _v { re, im }
	{
	}

	// constexpr complex(
	// 	const complex& o
	// ) : _rep { o._rep }
	// {
	// }

	constexpr int8_t real() const { return _v[0]; }
	constexpr int8_t imag() const { return _v[1]; }

	void real(int8_t v) { _v[0] = v; }
	void imag(int8_t v) { _v[1] = v; }

	constexpr uint16_t __rep() const {
		return _rep;
	}

private:
	union {
		int8_t _v[2];
		uint16_t _rep;
	};
};

template<> struct complex<int16_t> {
public:
	typedef int16_t value_type;
	typedef uint32_t rep_type;

	// constexpr complex(
	// 	rep_type r
	// ) : _rep { r }
	// {
	// }

	constexpr complex(
		int16_t re = 0,
		int16_t im = 0
	) : _v { re, im }
	{
	}

	// constexpr complex(
	// 	const complex& o
	// ) : _rep { o._rep }
	// {
	// }

	constexpr int16_t real() const { return _v[0]; }
	constexpr int16_t imag() const { return _v[1]; }

	void real(int16_t v) { _v[0] = v; }
	void imag(int16_t v) { _v[1] = v; }

	constexpr uint32_t __rep() const {
		return _rep;
	}

private:
	union {
		int16_t _v[2];
		uint32_t _rep;
	};
};

} /* namespace std */

using complex8_t  = std::complex<int8_t>;
using complex16_t = std::complex<int16_t>;
using complex32_t = std::complex<int32_t>;

static_assert(sizeof(complex8_t) == 2, "complex8_t size wrong");
static_assert(sizeof(complex16_t) == 4, "complex16_t size wrong");
static_assert(sizeof(complex32_t) == 8, "complex32_t size wrong");

#if defined(LPC43XX_M4)
static inline complex32_t multiply_conjugate_s16_s32(const complex16_t::rep_type a, const complex16_t::rep_type b) {
	// conjugate: conj(a + bj) = a - bj
	// multiply: (a + bj) * (c + dj) = (ac - bd) + (bc + ad)j
	// conjugate-multiply: (ac + bd) + (bc - ad)j
	//return { a.real() * b.real() + a.imag() * b.imag(), a.imag() * b.real() - a.real() * b.imag() };
	const int32_t rr = __SMULBB(a, b);
	const int32_t ii = __SMULTT(a, b);
	const int32_t r = __QADD(rr, ii);
	const int32_t ir = __SMULTB(a, b);
	const int32_t ri = __SMULBT(a, b);
	const int32_t i = __QSUB(ir, ri);
	return { r, i };
}
#endif

#endif/*__COMPLEX_H__*/
