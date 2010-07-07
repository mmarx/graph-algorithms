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

#ifndef FM_GRAPH_HPP
#define FM_GRAPH_HPP

#include <limits>
#include <string>
#include <fstream>

#include "tr1_memory.hpp"

using std::string;
using std::ostream;
using std::numeric_limits;

template<typename W>
class Directed_Graph;

/**
 * edge iterator, allows to enumerate a set of edges
 */
template<typename W>
class directed_edge_iterator
{
private:
  // construct an `end'-iterator
  directed_edge_iterator() : valid_(false) {}

  // construct a valid iterator
  directed_edge_iterator(shared_ptr<Directed_Graph<W> > graph)
    : graph_(graph), valid_(true), restrict_(false), incoming_(false),
      sink_(1), source_(1)
  {
    // if we didn't start on an edge, find the first
    if(!have_edge())
      {
	find_next_edge();
      }

    // graph contains no edges
    if(!have_edge())
      {
	valid_ = false;
	graph_ = graph_->end().graph_;
      }
  }

  // construct a restricted iterator
  directed_edge_iterator(shared_ptr<Directed_Graph<W> > graph, int vertex,
			 bool incoming = true)
    : graph_(graph), valid_(true), restrict_(true),
      incoming_(incoming)
  {
    // we want incoming edges
    if(incoming)
      {
	sink_ = vertex;
	source_ = 1;
      }
    // outgoing edges
    else
      {
	sink_ = 1;
	source_ = vertex;
      }

    // find a valid edge
    if(!have_edge())
      {
	find_next_edge();
      }

    // if we still don't have a valid edge, there are none
    if(!have_edge())
      {
	valid_ = false;
	graph_ = graph_->end().graph_;
      }
  }

  shared_ptr<Directed_Graph<W> > graph_;
  bool valid_;
  bool restrict_;
  bool incoming_;

  int sink_;
  int source_;

  friend class Directed_Graph<W>;

  /**
   * is the current edge valid?
   * @return true if the current edge is valid
   */
  bool have_edge()
  {
    int n = graph_->vertex_count();
    return ((source_ > 0) && (source_ <= n)
	    && (sink_ > 0) && (sink_ <= n)
	    && (weight() != numeric_limits<W>::infinity()));
  }

  /**
   * find the next valid edge
   */
  void find_next_edge()
  {
    int n = graph_->vertex_count();

    // unrestricted
    if(!restrict_)
      {
	for(; source_ <= n; ++source_, sink_ = 0)
	  {
	    while(++sink_ <= n)
	      {
		if(have_edge())
		  {
		    return;
		  }
	      }
	  }
      }
    else
      {
	if(incoming_)
	  {
	    // only iterate over incoming edges: fixed sink_
	    while(++source_ <= n)
	      {
		if(have_edge())
		  {
		    return;
		  }
	      }
	  }
	else
	  {
	    // only iterator over outgoing edges: fixed source_
	    while(++sink_ <= n)
	      {
		if(have_edge())
		  {
		    return;
		  }
	      }
	  }
      }
  }

public:
  ~directed_edge_iterator() {}

  /**
   * get the current edge weight
   * @return edge weight
   */
  W const weight() const
  {
    return graph_->weight(source_, sink_);
  }

  /**
   * get the current source
   * @return source
   */
  int source() const
  {
    return source_;
  }

  /**
   * get the current sink
   * @return sink
   */
  int sink() const
  {
    return sink_;
  }

  /**
   * advance the iterator
   * @return reference to self
   */
  directed_edge_iterator& operator++()
  {
    find_next_edge();

    if(!have_edge())
      {
	// no more edges, signal end
	valid_ = false;
	graph_ = graph_->end().graph_;
      }

    return *this;
  }

  /**
   * compare two iterators, they are considered equal
   * if the belong to the same graph and have the same
   * state.
   * @return true if the iterators are equivalent
   */
  bool operator==(directed_edge_iterator<W> const&  other) const
  {
    return (valid_ == other.valid_) && (graph_.get() == other.graph_.get());
  }

  /**
   * compare two iterators
   * @return true if the iterators are not equivalent
   */
  bool operator!=(directed_edge_iterator<W> const& other) const
  {
    return !(*this == other);
  }
};

/**
 * print out edge information
 * @param out output stream
 * @paramt e edge iterator
 * @return output stream
 */
template<typename W>
ostream& operator<<(ostream& out, directed_edge_iterator<W> const& e)
{
  using std::endl;

  out << "Edge " << e.source() << " -> " << e.sink()
      << " (weight " << e.weight() << ")" << endl;

  return out;
}

/**
 * Directed graph
 */
template<typename W>
class Directed_Graph : public enable_shared_from_this<Directed_Graph<W> >
{
public:
  typedef directed_edge_iterator<W> iterator;

  /**
   * construct a new directed graph
   * @param vertices number of vertices
   * @param source source vertex
   */
  Directed_Graph(int vertices,
		 int source) : source_(source), vertices_(vertices),
			       adjacency_(shared_array<
					    shared_ptr<W> >
					  (new shared_ptr<W>[vertices]))
  {
    for(int i = 1; i <= vertices; ++i)
      {
	adjacency_.get()[i - 1] = shared_array(new W[vertices]);

	for(int j = 1; j <= vertices; ++j)
	  {
	    adj(i, j) = numeric_limits<W>::infinity();
	  }
      }
  }

  /**
   * return an `end' iterator
   * @return invalid iterator
   */
  iterator& end() const
  {
    static iterator the_end = iterator();

    return the_end;
  }

  /**
   * enumerate all edges
   * @return iterator over all edges
   */
  iterator edges()
  {
    return iterator(this->shared_from_this());
  }

  /**
   * enumerate entering edges
   * @param vertex the vertex
   * @return iterator over edges entering vertex
   */
  iterator entering(int vertex)
  {
    return iterator(this->shared_from_this(), vertex, true);
  }

  /**
   * enumerate leavirg edges
   * @param vertex the vertex
   * @return iterator over edges leaving vertex
   */
  iterator leaving(int vertex)
  {
    return iterator(this->shared_from_this(), vertex, false);
  }

  /**
   * get the number of vertices
   * @return vertex count
   */
  int vertex_count() const
  {
    return vertices_;
  }

  /**
   * return the source vertex
   * @return source vertex
   */
  int source() const
  {
    return source_;
  }

  /**
   * get the edge weight
   * @param source source vertex
   * @param sink sink vertex
   * @return edge weight
   */
  W weight(int source, int sink) const
  {
    return adj(source, sink);
  }

  /**
   * add an edge
   * @param source source vertex
   * @param sink sink vertex
   * @param weight edge weight
   */
  void add_edge(int source, int sink, W weight)
  {
    adj(source, sink) = weight;
  }

  /**
   * print an adjacency matrix
   * @param out output stream
   * @return output stream
   */
  ostream& format_adjacency(ostream& out) const
  {
    using std::endl;

    for(int i = 1; i <= vertices_; ++i)
      {
	for(int j = 1; j <= vertices_; ++j)
	  {
	    out << adj(i, j) << "\t";
	  }
	out << endl;
      }

    return out;
  }

protected:
  /* helpers for adjacency matrix indexing
   * TR1 doesn't specify shared_array which has operator[],
   * so we wouldn't need this .get()[] magic, but we'd have
   * to depend on boost
   */

  /**
   * adjacency matrix entry
   * @param source source vertex
   * @param sink sink vertex
   * @return adjacency matrix entry
   */
  W& adj(int source, int sink)
  {
    return adjacency_.get()[source - 1].get()[sink - 1];
  }

  /**
   * adjacency matrix entry
   * @param source source vertex
   * @param sink sink vertex
   * @return adjacency matrix entry
   */
  W adj(int source, int sink) const
  {
    return adjacency_.get()[source - 1].get()[sink - 1];
  }

private:
  int source_;
  int vertices_;
  shared_ptr<shared_ptr<W> > adjacency_;

  friend class directed_edge_iterator<W>;
};

/**
 * Undirected graph, implemented as a special
 * directed graph
 */
template<typename W>
class Undirected_Graph : public Directed_Graph<W>
{
public:
  /**
   * construct an undirected graph
   * @param vertices number of vertices
   * @param source source vertex
   */
  Undirected_Graph(int vertices, int source)
    : Directed_Graph<W>(vertices, source) {}

  /**
   * add an edge
   * @param source source vertex
   * @param sink sink vertex
   * @param weight edge weight
   */
  void add_edge(int source, int sink, W weight)
  {
    this->adj(source, sink) = weight;
    this->adj(sink, source) = weight;
  }
};

/**
 * print out graph information
 * @param out output stream
 * @param g directed graph
 * @return output stream
 */
template<typename W>
ostream& operator<<(ostream& out, Directed_Graph<W>& g)
{
  using std::endl;
  out << "Graph with " << g.vertex_count()
      << " vertices." << endl
      << "Source is " << g.source()
      << "." << endl;

  out << "Adjacency matrix:" << endl;
  g.format_adjacency(out);

  out << endl << "Edges:" << endl;

  for(typename Directed_Graph<W>::iterator it = g.edges();
      it != g.end(); ++it)
    {
      out << it;
    }

  return out;
}

/**
 * create a graph from an input file
 * @param filename file name
 * @return directed graph
 */
template<typename W>
shared_ptr<Directed_Graph<W> > graph_from_file(string const& filename)
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

  shared_ptr<Directed_Graph<W> > graph(new Directed_Graph<W>(vertices,
							     source));
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

/**
 * create an undirected graph from a directed graph
 * @param graph the directed graph
 * @param return the undirected graph
 */
template<typename W>
shared_ptr<Undirected_Graph<W> >
undirected_from_graph(shared_ptr<Directed_Graph<W> > graph)
{
  shared_ptr<Undirected_Graph<W> > u(new
				     Undirected_Graph<W>(graph->vertex_count(),
							 graph->source()));

  for(typename Directed_Graph<W>::iterator it = graph->edges();
      it != graph->end(); ++it)
    {
      u->add_edge(it.source(), it.sink(), it.weight());
    }

  return u;
}

#endif
