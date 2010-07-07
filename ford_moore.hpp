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

#ifndef FM_FORD_MOORE_HPP
#define FM_FORD_MOORE_HPP

#include <list>
#include <vector>

#include "graph.hpp"

using std::list;
using std::vector;

template<typename W>
struct policy
{
  virtual bool better(W l, W r) = 0;
};

template<typename W>
struct shortest_path_policy
{
  bool better(W l, W r)
  {
    return l < r;
  }
};

template<typename W>
struct longest_path_policy
{
  bool better(W l, W r)
  {
    // treat infinity with special care: if r is infinite,
    // l can't be worse, so just assume it's better
    return (r != numeric_limits<W>::infinity())
      ? (l > r) : true;
  }
};

template<typename W>
struct path
{
  W cost;
  list<int> vertices;

  path(int end) : cost(0), vertices(list<int>(1, end)) {}
};

template<typename W>
ostream& operator<<(ostream& out, path<W> const& p)
{
  out << "Path from " << (*p.vertices.begin())
      << " to " << (*p.vertices.rbegin())
      << " (cost " << p.cost << "): ";

  bool first = true;

  for(list<int>::const_iterator i = p.vertices.begin();
      i != p.vertices.end(); ++i)
    {
      if(!first)
	{
	  out << " -> ";
	}
      else
	{
	  first = false;
	}

      out << (*i);
    }

  out << std::endl;

  return out;
}

template<typename W, typename P>
list<path<W> > ford_moore(shared_ptr<Directed_Graph<W> > graph)
{
  list<path<W> > paths;
  int s = graph->source();
  int n = graph->vertex_count();

  vector<W> d = vector<W>(n, numeric_limits<W>::infinity());
  vector<int> p = vector<int>(n, -1);

  d[s - 1] = 0;

  int k = 0;
  list<int> A = list<int>(1, graph->source());
  list<int> B;

  do
    {
      ++k;
      B.clear();

      for(list<int>::iterator v = A.begin(); v != A.end(); ++v)
	{
	  for(typename Directed_Graph<W>::iterator x = graph->leaving(*v);
	      x != graph->end(); ++x)
	    {
	      int z = x.sink() - 1;
	      W w = d[(*v) - 1] + x.weight();
	      if(P().better(w, d[z]))
		{
		  d[z] = w;
		  p[z] = (*v);

		  B.push_back(x.sink());
		}
	    }
	}
      A = B;
    }
  while((k < n) && B.size());

  for(int i = 0; i < n; ++i)
    {
      if(s == (i + 1))
	{
	  continue;
	}

      path<W> cp(i + 1);
      int j = p[i];

      if(j < 0)
	{
	  continue;
	}

      cp.cost = d[i];

      while(true)
	{
	  cp.vertices.push_front(j);

	  if(j == s)
	    {
	      break;
	    }

	  j = p[j - 1];
	}

      paths.push_back(cp);
    }

  return paths;
}

#endif
