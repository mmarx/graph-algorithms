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

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

using boost::shared_ptr;
using boost::shared_array;

template<typename W>
class edge_iterator : public boost::noncopyable
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
  directed_edge_iterator(int start, int end, int pos, shared_ptr<Directed_Graph<W> > graph)
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
  virtual ~Graph() = 0;

  virtual int vertex_count() = 0;
  virtual int edge_count() = 0;
  virtual weight_type weight(int edge) = 0;
  virtual edge_iterator<W> edges() = 0;
  virtual edge_iterator<W> entering(int vertex) = 0;
  virtual edge_iterator<W> leaving(int vertex) = 0;
};

template<typename W>
class Directed_Graph : public Graph<W>
{
  typedef directed_edge_iterator<W> iterator_type;

public:
  Directed_Graph(int vertices) : edges_(0), vertices_(vertices),
				 adjacency_(new shared_array<shared_array<W> >[vertices])
  {
    for(int i = 0; i < vertices; ++i)
      {
	adjacency_[i] = new shared_array<W>[vertices];
      }
  }

  int vertex_count()
  {
    return vertices_;
  }

  int edge_count()
  {
    return edges_;
  }

  

private:
  int edges_;
  int vertices_;
  shared_array<shared_array<W> > adjacency_;
};

template<typename W>
class Undirected_Graph : public Graph<W>
{
};

#endif
