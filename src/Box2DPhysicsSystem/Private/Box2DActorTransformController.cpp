#include "Box2DActorTransformController.h"
#include "Box2DPhysicsSystem.h"

Box2DActorTransformController::Box2DActorTransformController(Actor& owner, Box2DPhysicsSystem& system)
	: owner(owner)
	, system(system)
	, body(std::unique_ptr<b2Body, decltype(&bodyDeleter)>(nullptr, &bodyDeleter))
{
	system.bodies.insert(std::map<Actor*, Box2DActorTransformController*>::value_type(&owner, this));

	b2BodyDef bodyDef;
	bodyDef.userData = this;
	bodyDef.type = b2_staticBody;
	bodyDef.position = b2Vec2(0.f, 0.f);
	bodyDef.angle = 0.f;
	bodyDef.fixedRotation = false;

	body = std::unique_ptr<b2Body, decltype(&bodyDeleter)>(system.world->CreateBody(&bodyDef), &bodyDeleter);
}
Box2DActorTransformController::~Box2DActorTransformController() { system.bodies.erase(&owner); }

Transform Box2DActorTransformController::getTransform() const
{
	Transform ret;
	ret.location = convertVec(body->GetPosition());
	ret.rotation = body->GetAngle();

	return ret;
}

vec2 Box2DActorTransformController::getVelocity() const { return convertVec(body->GetLinearVelocity()); }

void Box2DActorTransformController::setTransform(const Transform& newTrans)
{
	body->SetTransform(convertVec(newTrans.location), newTrans.rotation);
}

void Box2DActorTransformController::setVelocity(vec2 newVelocity)
{
	body->SetLinearVelocity(convertVec(newVelocity));
}

PhysicsType Box2DActorTransformController::getType()
{
	b2BodyType type = body->GetType();

	switch (type)
	{
	case b2BodyType::b2_dynamicBody: return PhysicsType::DYNAMIC;
	case b2BodyType::b2_kinematicBody: return PhysicsType::KINEMATIC;
	case b2BodyType::b2_staticBody: return PhysicsType::STATIC;
	}

	return PhysicsType::UNKNOWN;
}

void Box2DActorTransformController::setType(PhysicsType newType)
{
	switch (newType)
	{
	case PhysicsType::DYNAMIC: body->SetType(b2BodyType::b2_dynamicBody); return;
	case PhysicsType::KINEMATIC: body->SetType(b2BodyType::b2_kinematicBody); return;
	case PhysicsType::STATIC: body->SetType(b2BodyType::b2_staticBody); return;
	}

	MFLOG(Warning) << "trying to set PhysicsType to an unknown type. Using previous type";
}

void Box2DActorTransformController::applyLocalForce(vec2 localForce, vec2 localPoint)
{
	body->ApplyForce(
		body->GetWorldVector(convertVec(localForce)), body->GetWorldPoint(convertVec(localPoint)), true);
}
void Box2DActorTransformController::applyWorldForce(vec2 localForce, vec2 localPoint)
{
	body->ApplyForce(convertVec(localForce), convertVec(localPoint), true);
}

void Box2DActorTransformController::applyTorque(float magnituede) { body->ApplyTorque(magnituede, true); }

Actor& Box2DActorTransformController::getOwner() const { return owner; }

void Box2DActorTransformController::setAngularVelocity(float newVelocity)
{
	body->SetAngularVelocity(newVelocity);
}
float Box2DActorTransformController::getAngularVelocity() { return body->GetAngularVelocity(); }