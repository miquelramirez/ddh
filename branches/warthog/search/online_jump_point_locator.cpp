#include "gridmap.h"
#include "jps.h"
#include "online_jump_point_locator.h"

#include <climits>

warthog::online_jump_point_locator::online_jump_point_locator(warthog::gridmap* map)
	: map_(map), jumplimit_(UINT32_MAX)
{
}

warthog::online_jump_point_locator::~online_jump_point_locator()
{
}


// Finds a jump point successor of node (x, y) in Direction d.
// Also given is the location of the goal node (goalx, goaly) for a particular
// search instance. If encountered, the goal node is always returned as a 
// jump point successor.
//
// @return: the id of a jump point successor or warthog::INF if no jp exists.
void
warthog::online_jump_point_locator::jump(warthog::jps::direction d,
	   	uint32_t node_id, uint32_t goal_id, uint32_t& jumpnode_id, 
		double& jumpcost)
{
	switch(d)
	{
		case warthog::jps::NORTH:
			jump_north(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::SOUTH:
			jump_south(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::EAST:
			jump_east(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::WEST:
			jump_west(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::NORTHEAST:
			jump_northeast(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::NORTHWEST:
			jump_northwest(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::SOUTHEAST:
			jump_southeast(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		case warthog::jps::SOUTHWEST:
			jump_southwest(node_id, goal_id, jumpnode_id, jumpcost);
			break;
		default:
			break;
	}
}

void
warthog::online_jump_point_locator::jump_north(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump one step at a time
	while(true)
	{
		// check if we can step north
		if((neis & 514) != 514) // bits 9 and 1
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step north
		jumpcost += 1;
		next_id -= mapw;
		neis <<= 8;
		map_->get_tripleh(next_id-mapw, ((uint8_t*)&neis)[0]);

		// stop if we hit the goal or find any forced neighbours
		if(next_id == goal_id) { break; }
		if((neis & 65792) == 256 || (neis & 263168) == 1024) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_south(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump one step at a time
	while(true)
	{
		// check if we can step south
		if((neis & 131584) != 131584) // bits 9 and 17
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step south
		jumpcost += 1;
		next_id += mapw;
		neis >>= 8;
		map_->get_tripleh(next_id+mapw, ((uint8_t*)&neis)[2]); 

		// stop if we hit the goal or find any forced neighbours
		if(next_id == goal_id) { break; }
		if((neis & 257) == 256 || (neis & 1028) == 1024) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_east(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;

	// jump a single step 
	while(true)
	{
		map_->get_neighbours(next_id, (uint8_t*)&neis);

		// check if we can step east
		if((neis & 1536) != 1536) // bits 9 and 10
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step east
		jumpcost += 1;
		next_id++;
		neis >>= 1; 

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 3) == 2 || (neis & 196608) == 131072) { break; }

		// check if we can step east again
		if((neis & 1536) != 1536) // bits 9 and 10
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step east #2
		jumpcost += 1;
		next_id++;
		neis >>= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 3) == 2 || (neis & 196608) == 131072) { break; }

		// check if we can step east again
		if((neis & 1536) != 1536) // bits 9 and 10
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step east #3
		jumpcost += 1;
		next_id++;
		neis >>= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 3) == 2 || (neis & 196608) == 131072) { break; }

		// check if we can step east again
		if((neis & 1536) != 1536) // bits 9 and 10
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step east #4
		jumpcost += 1;
		next_id++;
		neis >>= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 3) == 2 || (neis & 196608) == 131072) { break; }

		// check if we can step east again
		if((neis & 1536) != 1536) // bits 9 and 10
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}

		// step east #5
		jumpcost += 1;
		next_id++;
		neis >>= 1;

		// stop jumping if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 3) == 2 || (neis & 196608) == 131072) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_west(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// **LAST** 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;

	// jump a single step at a time
	while(true)
	{
		map_->get_neighbours_upper(next_id, (uint8_t*)&neis);

		// check if we can step west
		if((neis & 24576) != 24576) // bits 14 and 13
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		 
		// step west
		jumpcost += 1;
		next_id--;
		neis <<= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 192) == 64 || (neis & 12582912) == 4194304) { break; }

		// check if we can step west again
		if((neis & 24576) != 24576) // bits 14 and 13
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		 
		// step west #2
		jumpcost += 1;
		next_id--;
		neis <<= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 192) == 64 || (neis & 12582912) == 4194304) { break; }

		// check if we can step west again
		if((neis & 24576) != 24576) // bits 14 and 13
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		 
		// step west #3
		jumpcost += 1;
		next_id--;
		neis <<= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 192) == 64 || (neis & 12582912) == 4194304) { break; }

		// check if we can step west again
		if((neis & 24576) != 24576) // bits 14 and 13
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		 
		// step west #4
		jumpcost += 1;
		next_id--;
		neis <<= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 192) == 64 || (neis & 12582912) == 4194304) { break; }

		// check if we can step west again
		if((neis & 24576) != 24576) // bits 14 and 13
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		 
		// step west #5
		jumpcost += 1;
		next_id--;
		neis <<= 1;

		// check if we hit the goal or if we have any forced neis
		if(next_id == goal_id) { break; }
		if((neis & 192) == 64 || (neis & 12582912) == 4194304) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_northeast(uint32_t node_id,
	   	uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump a single step at a time (no corner cutting)
	while(true)
	{
		// stop jumping if we hit an obstacle or take an invalid step
		if((neis & 1542) != 1542) // bits 9, 10, 1 and 2
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		jumpcost += warthog::ROOT_TWO;
		next_id = next_id - mapw + 1;
		map_->get_neighbours(next_id, (uint8_t*)&neis);

		// stop jumping if we hit the goal
		if(next_id == goal_id) { break; }

		// recurse straight before stepping again diagonally;
		// (ensures we do not miss any optimal turning points)
		uint32_t jp_id; 
		double jp_cost;
		jump_north(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
		jump_east(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_northwest(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump a single step at a time (no corner cutting)
	while(true)
	{
		// stop jumping if we hit an obstacle or take an invalid step
		if((neis & 771) != 771) // bits 9, 8, 0 and 1
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		jumpcost += warthog::ROOT_TWO;
		next_id = next_id - mapw - 1;
		map_->get_neighbours(next_id, (uint8_t*)&neis);

		// stop jumping if we hit the goal
		if(next_id == goal_id) { break; }

		// recurse straight before stepping again diagonally;
		// (ensures we do not miss any optimal turning points)
		uint32_t jp_id; 
		double jp_cost;
		jump_north(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
		jump_west(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_southeast(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump a single step at a time (no corner cutting)
	while(true)
	{
		// stop jumping if we hit an obstacle or take an invalid step
		if((neis & 394752) != 394752) // bits 9, 10, 17 and 18
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		jumpcost += warthog::ROOT_TWO;
		next_id = next_id + mapw + 1;
		map_->get_neighbours(next_id, (uint8_t*)&neis);

		// stop jumping if we hit the goal
		if(next_id == goal_id) { break; }

		// recurse straight before stepping again diagonally;
		// (ensures we do not miss any optimal turning points)
		uint32_t jp_id; 
		double jp_cost;
		jump_south(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
		jump_east(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
	}
	jumpnode_id = next_id;
}

void
warthog::online_jump_point_locator::jump_southwest(uint32_t node_id, 
		uint32_t goal_id, uint32_t& jumpnode_id, double& jumpcost)
{
	jumpcost = 0;

	// first 3 bits of first 3 bytes represent a 3x3 cell of tiles
	// from the grid. Assume little endian format.
	uint32_t neis;
	uint32_t next_id = node_id;
	uint32_t mapw = map_->width();
	map_->get_neighbours(next_id, (uint8_t*)&neis);

	// jump a single step (no corner cutting)
	while(true)
	{
		// stop jumping if we hit an obstacle or take an invalid step
		if((neis & 197376) != 197376) // bits 9, 8, 16 and 17
		{
			jumpcost = warthog::INF;
			next_id = warthog::INF;
			break;
		}
		jumpcost += warthog::ROOT_TWO;
		next_id = next_id + mapw - 1;
		map_->get_neighbours(next_id, (uint8_t*)&neis);

		// stop jumping if we hit the goal
		if(next_id == goal_id) { break; }

		// recurse straight before stepping again diagonally;
		// (ensures we do not miss any optimal turning points)
		uint32_t jp_id; 
		double jp_cost;
		jump_south(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
		jump_west(next_id, goal_id, jp_id, jp_cost);
		if(jp_id != warthog::INF) { break; }
	}
	jumpnode_id = next_id;
}
