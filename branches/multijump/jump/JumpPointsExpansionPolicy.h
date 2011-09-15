#ifndef JUMPPOINTSEXPANSIONPOLICY_H
#define JUMPPOINTSEXPANSIONPOLICY_H

// JumpPointsExpansionPolicy.h
//
// This expansion policy reduces the branching factor
// of a node n during search by ignoring any neighbours which
// could be reached by an equivalent (or shorter) path that visits
// the parent of n but not n itself.
//
// An extension of this idea is to generate jump nodes located in the
// same direction as the remaining neighbours. 
//
// Theoretical details:
// [Harabor D. and Grastien A., 2011, Online Node Pruning for Pathfinding
// On Grid Maps, AAAI] 
//
// @author: dharabor
// @created: 06/01/2010

#include "ExpansionPolicy.h"
#include "Jump.h"
#include <vector>
#include <stdexcept>

class JumpPointLocator;
class JumpPointsExpansionPolicy : public ExpansionPolicy
{

	public:
		JumpPointsExpansionPolicy(JumpPointLocator* jpl);
		virtual ~JumpPointsExpansionPolicy();

		virtual void expand(node* t) throw(std::logic_error);
		virtual node* first();
		virtual node* next();
		virtual node* n();
		virtual double cost_to_n();
		virtual bool hasNext();

		inline void setMaxDepth(int _maxdepth) { this->maxdepth = _maxdepth; }
		inline int getMaxDepth() { return maxdepth; }

		int jumplimit; 

	private:
		Jump::Direction computeDirection(int px, int py, int x, int y);
		void computeNeighbourSet();
		void computeNeighbours(Jump::Direction which, int x, int y);
		node* findJumpNode(Jump::Direction d, int x, int y);

		std::vector<node*> neighbours;
		unsigned int neighbourIndex; 
		JumpPointLocator* jpl;
		int maxdepth;
};

#endif
