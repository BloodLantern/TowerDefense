#include "bee_projectile_dart.hpp"
#include "globals.hpp"

BeeProjectileDart::BeeProjectileDart(Point2 position, Vector2 velocity)
	: Projectile(2, 1, 5, 1.f)
{
	SetPixelPosition(position);
	mVelocity = velocity;
	mScale = .1f;
	mTexture = Globals::gResources->GetTexture("projectiles\\dart");
}

void BeeProjectileDart::OnUpdate()
{
	Projectile::OnUpdate();
	mRotation = mVelocity.Angle();
}

void BeeProjectileDart::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, mScale, mRotation);
}

