/*
 * (c) 2010 Maximilian Marx <mmarx@wh2.tu-dresden.de>
n *
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

#ifndef FM_GRAPH_HPP
#define FM_GRAPH_HPP

#include <limits>
#include <string>
#include <fstream>

#include "tr1_memory.hpp"
#include "noncopyable.hpp"

using std::string;
using std::ostream;
using std::numeric_limits;

template<typename W>
class edge_iterator : public noncopyable
{
protected:
  edge_iterator();
  virtual ~edge_iterator() = 0;

public:
  W const weight() const = 0;
};

template<typename W>
class Directed_Graph;

template<typename W>
class directed_edge_iterator : public edge_iterator<W>
{
private:
  directed_edge_iterator() {}
  directed_edge_iterator(int start, int end, int pos,
			 shared_ptr<Directed_Graph<W> > graph)
    : start_(start), end_(end), vertex_(start), edge_(pos), graph_(graph) {}
  ~directed_edge_iterator() {}

  int start_;
  int end_;
  int vertex_;
  int edge_;
  shared_ptr<Directed_Graph<W> > graph_;

  friend class Directed_Graph<W>;
public:
  W const weight() const
  {
    return graph_->weight(edge_);
  }

  directed_edge_iterator& operator++()
  {
  }
};

template<typename W>
struct Graph
{
  typedef W weight_type;
  virtual ~Graph() {}

  virtual int vertex_count() const = 0;
  virtual int edge_count() const = 0;
  virtual ostream& format_adjacency(ostream& out) const = 0;
  // virtual weight_type weight(int edge) = 0;
  // virtual edge_iterator<W> edges() = 0;
  // virtual edge_iterator<W> entering(int vertex) = 0;
  // virtual edge_iterator<W> leaving(int vertex) = 0;
};

template<typename W>
class Directed_Graph : public Graph<W>
{
  typedef directed_edge_iterator<W> iterator_type;

public:
  Directed_Graph(int vertices) : edges_(0), vertices_(vertices),
				 adjacency_(shared_array<
					      shared_ptr<W> >
					    (new shared_ptr<W>[vertices]))
  {
    for(int i = 0; i < vertices; ++i)
      {
	adjacency_.get()[i] = shared_array(new W[vertices]);

	for(int j = 0; j < vertices; ++j)
	  {
	    adjacency_.get()[i].get()[j] = numeric_limits<W>::infinity();
	  }
      }
  }

  int vertex_count() const
  {
    return vertices_;
  }

  int edge_count() const
  {
    return edges_;
  }

  void add_edge(int source, int sink, W weight)
  {
    adjacency_.get()[source - 1].get()[sink - 1] = weight;
  }

  ostream& format_adjacency(ostream& out) const
  {
    using std::endl;

    for(int i = 0; i < vertices_; ++i)
      {
	for(int j = 0; j < vertices_; ++j)
	  {
	    out << adjacency_.get()[i].get()[j] << "\t";
	  }
	out << endl;
      }

    return out;
  }

private:
  int edges_;
  int vertices_;
  shared_ptr<shared_ptr<W> > adjacency_;
};

template<typename W>
class Undirected_Graph : public Graph<W>
{
};

template<typename W>
ostream& operator<<(ostream& out, Graph<W> const& g)
{
  using std::endl;
  out << "Graph with " << g.vertex_count()
      << " vertices and " << g.edge_count()
      << " edges." << endl;
    //     << "Source is " << g.source()

  out << "Adjacency matrix:" << endl;
  g.format_adjacency(out);

  return out;
}

template<typename W>
shared_ptr<Directed_Graph<W> >  graph_from_file(string const& filename)
{
  using std::cerr;
  using std::endl;
  using std::ifstream;

  unsigned int vertices, edges;
  unsigned int source, sink;
  W weight;

  ifstream file (filename.c_str ());
  file >> vertices;
  file >> edges;
  file >> source;

  shared_ptr<Directed_Graph<W> > graph(new Directed_Graph<W>(vertices));
  for(unsigned int i = 1; i <= edges; ++i)
    {
      file >> source >> sink >> weight;

      if (weight >= 0)
	{
	  graph->add_edge(source, sink, weight);
	}
      else
	{
	  cerr << "-!- negative edge weight, ignoring." << endl;
	}
    }

  return graph;
}

#endif
