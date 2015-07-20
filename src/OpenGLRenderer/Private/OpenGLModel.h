#pragma once

#include <Engine.h>
#include <ModelData.h>
#include <Model.h>

#include "OpenGLMaterialInstance.h"

class OpenGLModelData;

class OpenGLModel final : public Model
{
public:

	friend class OpenGLRenderer;

	explicit OpenGLModel(OpenGLRenderer& renderer);
	virtual ~OpenGLModel();

	virtual void init(std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, MeshComponent& ownerComp) override;

	virtual MeshComponent& getOwnerComponent() override;	
	virtual const MeshComponent& getOwnerComponent() const override;



	void draw();
private:

	std::shared_ptr<OpenGLModelData> modelData;

	MeshComponent* parent;

	OpenGLRenderer& renderer;

	std::shared_ptr<OpenGLMaterialInstance> material;

};