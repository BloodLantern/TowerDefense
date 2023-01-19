#include "spray_projectile.hpp"
#include "globals.hpp"

SprayProjectile::SprayProjectile()
	: Projectile(3.f, 5, 500, 5.f)
{
	mHitboxRadius = 60.f;
	mTexture = Globals::gResources->GetTexture("projectiles\\spray_fog");
	mScale = 0.1f;
}

Projectile* SprayProjectile::Clone() const
{
	SprayProjectile* proj = new SprayProjectile();
	proj->mScale = mScale;
	proj->mSpeed = mSpeed;

	return proj;
}

void SprayProjectile::OnRender()
{
	ImVec2 pos(GetPixelPosition().x + Globals::gGridX, GetPixelPosition().y + Globals::gGridY);

	ImGuiUtils::DrawTextureEx(*Globals::gDrawList, *GetTexture(), pos, mScale, mRotation);
}
