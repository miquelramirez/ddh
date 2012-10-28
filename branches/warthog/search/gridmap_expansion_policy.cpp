#include "blocklist.h"
#include "gridmap_expansion_policy.h"
#include "helpers.h"
#include "problem_instance.h"

warthog::gridmap_expansion_policy::gridmap_expansion_policy(
		warthog::gridmap* map) : map_(map)
{
	nodepool_ = new warthog::blocklist(map->height(), map->width());
}

warthog::gridmap_expansion_policy::~gridmap_expansion_policy()
{
	delete nodepool_;
}

void 
warthog::gridmap_expansion_policy::expand(warthog::search_node* current,
		warthog::problem_instance* problem)
{
	reset();

	// get terrain type of each tile in the 3x3 square around (x, y)
	char tiles[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t nodeid = current->get_id();
	map_->get_neighbours(nodeid, tiles);

	#ifndef NDEBUG
	uint32_t cx_, cy_;
	warthog::helpers::index_to_xy(nodeid, map_->width(), cx_, cy_);
	assert(tiles[0] == map_->get_label(cx_-1, cy_-1));
	assert(tiles[1] == map_->get_label(cx_, cy_-1));
	assert(tiles[2] == map_->get_label(cx_+1, cy_-1));
	assert(tiles[3] == map_->get_label(cx_-1, cy_));
	assert(tiles[4] == map_->get_label(cx_, cy_));
	assert(tiles[5] == map_->get_label(cx_+1, cy_));
	assert(tiles[6] == map_->get_label(cx_-1, cy_+1));
	assert(tiles[7] == map_->get_label(cx_, cy_+1));
	assert(tiles[8] == map_->get_label(cx_+1, cy_+1));
	#endif

	if(!tiles[4]) { return; }

	// calculate transition costs; from (x, y) to each adjacent tile
	// this value is just an average of terrain costs weighted by the 
	// transition type (straight or diagonal)
	// NB: no corner cutting or squeezing between obstacles!

	uint32_t nid_m_w = nodeid - map_->width();
	uint32_t nid_p_w = nodeid + map_->width();
	if(tiles[0] && tiles[1] && tiles[3]) // NW
	{ 
		neis_[num_neis_] = nodepool_->generate(nid_m_w - 1);
		costs_[num_neis_] =  (tiles[0] + tiles[1] + tiles[3] + tiles[4])
			 * warthog::ROOT_TWO_OVER_FOUR;
		num_neis_++;
	}

	if(tiles[1]) // N
	{  
		neis_[num_neis_] = nodepool_->generate(nid_m_w);
		costs_[num_neis_] = (tiles[1] + tiles[4]) * 0.5;
		num_neis_++;
	} 

	if(tiles[2] && tiles[1] && tiles[5]) // NE
	{ 
		neis_[num_neis_] = nodepool_->generate(nid_m_w + 1);
		costs_[num_neis_] = (tiles[2] + tiles[1] + tiles[5] + tiles[4])
			* warthog::ROOT_TWO_OVER_FOUR;
		num_neis_++;
	}

	if(tiles[3]) // W
	{ 
		neis_[num_neis_] = nodepool_->generate(nodeid - 1);
		costs_[num_neis_] = (tiles[3] + tiles[4]) * 0.5;
		num_neis_++;
	}

	if(tiles[5]) // E
	{
		neis_[num_neis_] = nodepool_->generate(nodeid + 1);
		costs_[num_neis_] = (tiles[5] + tiles[4]) * 0.5;
		num_neis_++;
	}
	
	if(tiles[6] && tiles[3] && tiles[7]) // SW
	{	
		neis_[num_neis_] = nodepool_->generate(nid_p_w - 1);
		costs_[num_neis_] = (tiles[6] + tiles[3] + tiles[7] + tiles[4]) 
			* warthog::ROOT_TWO_OVER_FOUR;
		num_neis_++;
	}

	if(tiles[7]) // S
	{ 
		neis_[num_neis_] = nodepool_->generate(nid_p_w);
		costs_[num_neis_] = (tiles[7] + tiles[4]) * 0.5;
		num_neis_++;
	}

	if(tiles[8] && tiles[7] && tiles[5]) // SE
	{ 
		neis_[num_neis_] = nodepool_->generate(nid_p_w + 1);
		costs_[num_neis_] = (tiles[8] + tiles[7] + tiles[5] + tiles[4]) 
			* warthog::ROOT_TWO_OVER_FOUR;
		num_neis_++;
	}
}
