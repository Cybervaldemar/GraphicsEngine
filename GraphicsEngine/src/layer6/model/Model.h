/*
 * Model.h
 *
 *  Created on: 24.05.2011
 *      Author: Norbert Nopper
 */

#ifndef MODEL_H_
#define MODEL_H_

#include "../../UsedLibs.h"

#include "../../layer0/math/Matrix4x4.h"
#include "../../layer0/math/Matrix3x3.h"
#include "../../layer1/collision/BoundingSphere.h"
#include "../../layer2/material/SurfaceMaterial.h"
#include "../../layer5/node/Node.h"

class Model
{

private:

	BoundingSphere boundingSphere;

	NodeSP rootNode;
	boost::int32_t numberJoints;
	bool animated;
	bool skinned;

	std::map<boost::int32_t, NodeSP> allNodesByIndex;
	std::map<std::string, NodeSP> allNodesByName;
	std::map<std::string, SurfaceMaterialSP> allSurfaceMaterialsByName;

	void updateSurfaceMaterialsRecursive(const NodeSP& node);

public:

	Model(const BoundingSphere& boundingSphere, const NodeSP& node, boost::int32_t numberJoints, bool animationData, bool skinned);
	virtual ~Model();

	const BoundingSphere& getBoundingSphere() const;

	const NodeSP& getRootNode() const;

	boost::int32_t getNumberJoints() const;

	bool isAnimated() const;

	bool isSkinned() const;

	SurfaceMaterialSP findSurfaceMaterial(const std::string& name) const;

	boost::int32_t getSurfaceMaterialCount() const;

	SurfaceMaterialSP getSurfaceMaterialAt(boost::int32_t index) const;

};

typedef boost::shared_ptr<Model> ModelSP;

#endif /* MODEL_H_ */
