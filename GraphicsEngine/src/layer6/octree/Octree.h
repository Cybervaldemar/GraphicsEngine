/*
 * Octree.h
 *
 *  Created on: 30.04.2011
 *      Author: Norbert Nopper
 */

#ifndef OCTREE_H_
#define OCTREE_H_

#include "../../UsedLibs.h"

#include "../../layer4/entity/EntityList.h"

#include "Octant.h"
#include "OctreeEntity.h"

class Octree
{

	friend class Octant;
	friend class OctreeFactory;
	friend void boost::checked_delete<Octree>(Octree* x);

private:

	Octant* pool;

	Octant* root;

	EntityListSP entityExcludeList;

	Octree(boost::uint32_t maxLevels, boost::uint32_t maxElements, const Point4& center, float halfWidth, float halfHeight, float halfDepth);

	virtual ~Octree();

	Octant* createOctant(Octant* parent, boost::uint32_t level, boost::uint32_t maxLevels, const Point4& center, float halfWidth, float halfHeight, float halfDepth);

	void recycleOctant(Octant* octant);

public:

	bool updateEntity(const OctreeEntitySP& octreeEntity) const;

	void removeEntity(const OctreeEntitySP& octreeEntity) const;

	void removeAllEntities() const;

	void sort() const;

	void update() const;

	void render(bool force = false) const;

	void setDebug(bool debug);

	void setEntityExcludeList(const EntityListSP& entityExcludeList);

	bool isEntityExcluded(const OctreeEntitySP& octreeEntity) const;

};

typedef boost::shared_ptr<Octree> OctreeSP;

#endif /* OCTREE_H_ */
