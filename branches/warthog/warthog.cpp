#ifndef WARTHOG_H
#define WARTHOG_H

#include "gridmap.h"
#include "heap.h"

#include "getopt.h"

void gridmap_access_test();
void heap_insert_test();
int main(int argc, int** argv)
{
	heap_insert_test();
}

void heap_insert_test()
{
	std::cout << "heap_insert_test...\n";
	int heapnodes = 5;
	warthog::heap myheap(heapnodes, false);
	warthog::search_node** nodes = new warthog::search_node*[heapnodes];
	for(int i=0; i < heapnodes; i++)
	{
		nodes[i] = new warthog::search_node(i);
		nodes[i]->update(0, i, 0);
		myheap.push(nodes[i]);
		std::cout << "iteration "<<i<<std::endl;
		myheap.print(std::cout);
	}
	myheap.print(std::cout);
	for(int i=0; i < heapnodes; i++)
	{
		delete myheap.pop();
	}
	delete [] nodes;
	std::cout << "/heap_insert_test...\n";
}

void gridmap_access_test()
{
	const char* file = "orz700d.map";
	std::cout << "loading map..."<<file<<std::endl;
	warthog::gridmap mymap(file, false);
//	std::cout << "map\n";
	//mymap.print(std::cout);
//	std::cout << "done."<<std::endl;

	for(int i=0; i < 1<<28; i++)
	{
		int x = (rand()/RAND_MAX)*mymap.width();
		int y = (rand()/RAND_MAX)*mymap.height();
		for(int nx = x-1; nx < x+2; nx++)
		{
			for(int ny = y-1; ny < y+2; ny++)
			{
				mymap.get_label(nx, ny);
			}
		}
		//std::cout << i << "\r" << std::flush;
	}
}

#endif

