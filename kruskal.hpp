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

#ifndef FM_KRUSKAL_HPP
#define FM_KRUSKAL_HPP

#include <list>
#include <vector>
#include <algorithm>

#include "graph.hpp"

using std::list;
using std::vector;
using std::min_element;

template<typename W>
struct edge
{
  int source;
  int sink;
  W weight;

  edge(int so, int si, W we) : source(so), sink(si), weight(we) {}

  bool operator<(edge const& other)
  {
    return weight < other.weight;
  }
};

template<typename W>
shared_ptr<Undirected_Graph<W> > kruskal(shared_ptr<Undirected_Graph<W> >
					 graph)
{
  int n = graph->vertex_count();

  shared_ptr<Undirected_Graph<W> > k(new Undirected_Graph<W>(n,
							     graph->source()));

  vector<int> forrest;

  for(int i = 0; i < n; ++i)
    {
      forrest.push_back(i);
    }

  list<edge<W> > S;

  for(typename Undirected_Graph<W>::iterator i = graph->edges();
      i != graph->end(); ++i)
    {
      S.push_back(edge<W>(i.source(), i.sink(), i.weight()));
    }

  for(int i = 0; (i < n) && S.size();)
    {
      typename list<edge<W> >::iterator e = min_element(S.begin(), S.end());

      if(forrest[e->source - 1] != forrest[e->sink - 1])
	{
	  ++i;
	  k->add_edge(e->source, e->sink, e->weight);

	  int v = forrest[e->sink - 1];

	  for(int j = 0; j < n; ++j)
	    {
	      if(forrest[j] == v)
		{
		  forrest[j] = forrest[e->source - 1];
		}
	    }
	}

      S.erase(e);
    }

  return k;
}

#endif
