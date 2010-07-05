/*
 * (c) 2010 Maximilian Marx <mmarx@wh2.tu-dresden.de>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef FM_TR1_MEMORY_HPP
#define FM_TR1_MEMORY_HPP

/** try to portably include a tr1-conforming <memory> header,
 * falling back on boost if the compiler doesn't ship one.
 */

// __GLIBCXX__, _HAS_TR1
#include <cstddef>

// GNU C++ or Intel C++ using libstd++.
#if defined (__GNUC__) && __GNUC__ >= 4 && defined (__GLIBCXX__)
#include <tr1/memory>
// VC++ or Intel C++ using VC++ standard library.
#elif defined (_MSC_VER) && (_MSC_VER == 1500 && \
  defined (_HAS_TR1) || _MSC_VER > 1500)
#include <memory>
// Boost fall-back.
#else
#include <boost/tr1/memory.hpp>
#endif

using std::tr1::shared_ptr;

/** Array deleter for use with shared_ptr
 */

template<typename T>
struct array_deleter
{
  void operator()(T* d)
  {
    delete[] d;
  }
};

/** shared_array
 */

template<typename T>
shared_ptr<T> shared_array(T* p)
{
  return shared_ptr<T>(p, array_deleter<T>());
}

#endif
