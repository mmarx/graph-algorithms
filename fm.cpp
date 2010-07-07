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

#include <string>
#include <iostream>

#include "graph.hpp"
#include "prim.hpp"
#include "kruskal.hpp"
#include "ford_moore.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::string;

// shortcuts
typedef double weight_type;
typedef Directed_Graph<weight_type> DG;
typedef Undirected_Graph<weight_type> UG;

/**
 * print usage information
 * @param name program name
 */
void usage(string name)
{
  cerr << "usage: " << name << " <mode> <input file>" << endl
       << "where mode is one of:" << endl
       << "\t--shortest\t\tFord/Moore shortest paths" << endl
       << "\t--longest\t\tFord/Moore longest paths" << endl
       << "\t--kruskal\t\tKruskal minimal spanning tree" << endl
       << "\t--prim\t\tPrim minimal spanning tree" << endl;
}

/**
 * main function
 * @param argc number of CLI arguments
 * @param argv array of CLI arguments
 * @return 0 in case of success, 1 otherwise
 */
int main(int argc, char** argv)
{
  if(argc < 3)
    {
      usage(argv[0]);
      return 1;
    }

  // read the graph
  shared_ptr<DG> graph = graph_from_file<weight_type>(argv[2]);
  cout << (*graph); // ...and print it

  // what are we supposed to do with it?
  string mode = string(argv[1]);

  // shortest paths
  if(mode == "--shortest")
    {
      cout << endl << "Shortest paths:" << endl;

      list<path<weight_type> > sp =
	ford_moore<weight_type, shortest_path_policy<weight_type> >(graph);

      for(list<path<weight_type> >::iterator i = sp.begin();
	  i != sp.end(); ++i)
	{
	  cout << (*i);
	}
    }
  // longest paths
  else if(mode == "--longest")
    {
      cout << endl << "Longest paths:" << endl;

      list<path<weight_type> > lp =
	ford_moore<weight_type, longest_path_policy<weight_type> >(graph);

      for(list<path<weight_type> >::iterator i = lp.begin();
	  i != lp.end(); ++i)
	{
	  cout << (*i);
	}
    }
  // kruskal
  else if(mode == "--kruskal")
    {
      cout << endl << "Minimal spanning tree (Kruskal):" << endl;

      shared_ptr<UG> k = kruskal(undirected_from_graph(graph));

      cout << (*k);
    }
  // prim
  else if(mode == "--prim")
    {
      cout << endl << "Minimal spanning tree (Prim):" << endl;

      shared_ptr<UG> p = prim(undirected_from_graph(graph));

      cout << (*p);
    }
  // don't know, report error
  else
    {
      usage(argv[0]);
      return 1;
    }

  return 0;
}
