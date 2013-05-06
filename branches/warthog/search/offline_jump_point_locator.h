#ifndef WARTHOG_OFFLINE_JUMP_POINT_LOCATOR_H
#define WARTHOG_OFFLINE_JUMP_POINT_LOCATOR_H

// offline_jump_point_locator.h
//
// Identifies jump points using a pre-computed database that stores
// distances from each node to jump points in every direction.
//
// @author: dharabor
// @created: 05/05/2013
//

#include "jps.h"

namespace warthog
{

class gridmap;
class offline_jump_point_locator
{
	public:
		offline_jump_point_locator(warthog::gridmap* map);
		~offline_jump_point_locator();

		void
		jump(warthog::jps::direction d, uint32_t node_id, uint32_t goalid, 
				uint32_t& jumpnode_id, double& jumpcost);

	private:

		void
		preproc();

		void
		jump_northwest(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_northeast(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_southwest(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_southeast(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_north(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_south(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_east(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);
		void
		jump_west(uint32_t node_id, uint32_t goal_id, 
				uint32_t& jumpnode_id, double& jumpcost);

		warthog::gridmap* map_;
		uint32_t dbsize_;
		uint8_t* db_;	
};

}

#endif

