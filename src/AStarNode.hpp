/**
 * AStarNode.hpp
 * 
 * Author   :   Fabian Gehri
 * Date     :   22.05.2011
 *
 * All rights reserved.
 *
 */


#ifndef GRAPHICSLAB_ASTARNODE_HPP
#define GRAPHICSLAB_ASTARNODE_HPP

// Includes
#include <queue>

GRAPHICSLAB_NAMESPACE_BEGIN

/** AStarNode */
class AStarNode
{
public:
	float cost_so_far, cost_to_goal;
	int i, j; // the field this node represents
	int i_aim, j_aim; // the farthest visible field in this node's path (aim at this field)
	bool isVisible; // whether the field of this node is visible
	AStarNode* parent;

	AStarNode(float cost_so_far_, float cost_to_goal_, int i_, int j_, int i_aim_, int j_aim_, bool isVisible_, AStarNode* parent_)
		: cost_so_far(cost_so_far_), cost_to_goal(cost_to_goal_), i(i_), j(j_), i_aim(i_aim_), j_aim(j_aim_), isVisible(isVisible_), parent(parent_)
	{
	}

	//friend bool operator> (const AStarNode &me, const AStarNode &other);

	bool hasVisited(int i_, int j_);

}; // class AStarNode

//bool operator> (const AStarNode &me, const AStarNode &other)
//{
//	return me.cost_so_far + me.cost_to_goal > other.cost_so_far + other.cost_to_goal;
//	//return me.cost_so_far + std::pow(me.cost_to_goal, AI_COST_TO_GOAL_EXP) > other.cost_so_far + std::pow(me.cost_to_goal, AI_COST_TO_GOAL_EXP);
//	//return me.cost_so_far + me.cost_to_goal * me.cost_to_goal > other.cost_so_far + other.cost_to_goal * other.cost_to_goal;
//	//return me.cost_to_goal > other.cost_to_goal;
//}

bool AStarNode::hasVisited(int i_, int j_)
{
	if (i == i_ && j == j_)
		return true;
	else if (parent != NULL)
		return parent->hasVisited(i_, j_);
	else return false;
}

struct greater_by_pointer
{
	bool operator() (const AStarNode* me, const AStarNode* other)
	{
		return me->cost_so_far + me->cost_to_goal > other->cost_so_far + other->cost_to_goal;
	}
};

typedef std::priority_queue<AStarNode*, std::vector<AStarNode*>, greater_by_pointer> Fringe;

GRAPHICSLAB_NAMESPACE_END


#endif  // GRAPHICSLAB_ASTARNODE_HPP


