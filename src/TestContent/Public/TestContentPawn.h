#pragma once

#include "TestContentConfig.h"

#include <Pawn.h>
#include <Runtime.h>
#include <MaterialInstance.h>
#include <Texture.h>
#include <ModelData.h>
#include <PhysicsShape.h>
#include <PhysicsComponent.h>
#include <Window.h>
#include <CameraComponent.h>

class TestContentPawn : public Pawn
{
public:
	TestContentPawn()
	{
		auto& inputManager = Runtime::get().inputManager;

		inputManager.bindAxisMapping("moveLeft", [this](float amount)
		{
			this->moveLeft(amount);
		});
		inputManager.bindAxisMapping("moveUp", [this](float amount)
		{
			this->moveUp(amount);
		});

		uvec2 windowSize = Runtime::get().moduleManager.getRenderer().getWindow().getWindowProps().size;

		float aspectRatio = float(windowSize.y) / float(windowSize.x);

		camera = std::make_unique<CameraComponent>(*this, Transform{}, aspectRatio, .1f);
		Runtime::get().moduleManager.getRenderer().setCurrentCamera(*camera);


		std::array<vec2, 4> locations =
		{
			vec2{ -1.f, -1.f },
			vec2{ -1.f, +1.f },
			vec2{ +1.f, -1.f },
			vec2{ +1.f, +1.f }
		};
		std::array<vec2, 4> UVs =
		{
			vec2{ 0.f, 1.f },
			vec2{ 0.f, 0.f },
			vec2{ 1.f, 1.f },
			vec2{ 1.f, 0.f }
		};
		std::array<uvec3, 2> tris =
		{
			uvec3{ 0, 1, 2 },
			uvec3{ 1, 2, 3 }

		};

		auto mat = Runtime::get().moduleManager.getRenderer().newMaterial(
			Runtime::get().moduleManager.getRenderer().getMaterialSource("boilerplate"));

		auto tex = Runtime::get().moduleManager.getRenderer().getTexture("4");


		mat->setTexture(0, tex);

		auto meshData = Runtime::get().moduleManager.getRenderer().newModelData();
		meshData->init(locations.data(), UVs.data(), locations.size(), tris.data(), tris.size());
		mesh = std::make_unique<MeshComponent>(*this, Transform{}, std::move(mat), std::move(meshData));


		auto shape = Runtime::get().moduleManager.getPhysicsSystem().newPhysicsShape();
		shape->asCircle(1.f);

		phyMesh = std::make_unique<PhysicsComponent>(*this, *shape, Transform{});
		setPhysicsType(PhysicsType::DYNAMIC);
		phyMesh->setDensity(1.f);
	}

	void moveLeft(float amount)
	{
		if (amount)
			applyLocalForce(vec2{ -amount*100.f, 0.f }, vec2{ 0.f, 0.f });
	}
	void moveUp(float amount)
	{
		if (amount)
			applyLocalForce(vec2{ 0.f, amount*100.f }, vec2{ 0.f, 0.f });

	}

private:
	std::unique_ptr<MeshComponent> mesh;
	std::unique_ptr<PhysicsComponent> phyMesh;
	std::unique_ptr<CameraComponent> camera;
};