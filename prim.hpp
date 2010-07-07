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

#ifndef FM_PRIM_HPP
#define FM_PRIM_HPP

#include <vector>
#include <algorithm>

#include "graph.hpp"

using std::find;
using std::vector;

/**
 * Prim's algorithm
 * @param graph undirected graph
 * @return undirected graph that is a minimal spanning tree
 */
template<typename W>
shared_ptr<Undirected_Graph<W> > prim(shared_ptr<Undirected_Graph<W> > graph)
{
  int n = graph->vertex_count();

  vector<int> V = vector<int>(1, graph->source());
  shared_ptr<Undirected_Graph<W> > p(new Undirected_Graph<W>(n,
							     graph->source()));

  for(int i = 1; i < n; ++i)
    {
      W w = numeric_limits<W>::infinity();
      int l = -1;
      int m = -1;

      for(vector<int>::iterator u = V.begin(); u != V.end(); ++u)
	{
	  for(typename Undirected_Graph<W>::iterator e = graph->leaving((*u));
	      e != graph->end(); ++e)
	    {
	      if(find(V.begin(), V.end(), e.sink()) != V.end())
		{
		  // already got that sink, ignore it.
		  continue;
		}

	      if(e.weight() < w)
		{
		  w = e.weight();
		  l = e.source();
		  m = e.sink();
		}
	    }
	}

      p->add_edge(l, m, w);
      V.push_back(m);
    }

  return p;
}

#endif
