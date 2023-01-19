#include "particle_emitter.hpp"
#include "globals.hpp"

#define RANDOM_FLOAT(max) (((float_t)rand()/(float_t)(RAND_MAX)) * (max))

ParticleEmitter::ParticleEmitter(Point2 position, Texture& texture)
	: mPosition(position), mTexture(texture), mParticleLimit(1), mFrequency(0.f), mFrequencyMax(0.f)
{
	mActive = false;
	mConfigured = false;

	mAverageLifetime = 0.f;
	mDeltaLifetime = 0.f;
}

void ParticleEmitter::ConfigureEmitter(
	uint16_t limit, float_t avgLifetime, float_t deltaLifetime,
	float_t frequency, uint32_t color, Vector2 direction,
	float_t avgSpeed, float_t deltaSpeed,
	bool start
)
{
	mParticleLimit = limit;
	mParticles.resize(limit);

	mAverageLifetime = avgLifetime;
	mDeltaLifetime = deltaLifetime;

	mFrequency = frequency;
	mFrequencyMax = frequency;

	mColor = color;
	mDirection = direction.Normalize();
	mDirectionIntensity = direction.GetNorm() / 5.f;

	mAverageSpeed = avgSpeed;
	mDeltaSpeed = deltaSpeed;

	mConfigured = true;

	if (start)
		Start();
}

void ParticleEmitter::Update()
{
	if (!mConfigured)
		return;

	float_t deltaTime = Globals::gGame->GetPlayingSpeedDeltaTime();
	ImDrawList* dl = Globals::gDrawList;

	if (mActive)
	{
		mFrequency -= deltaTime;
		if (mFrequency < 0)
		{
			mFrequency = mFrequencyMax;
			TrySpawnParticle();
		}
	}

	// Iterate through the particles
	for (uint32_t i = 0; i < mParticles.size(); i++)
	{
		Particle& particle = mParticles[i];

		// Check if lifetime is over, no need to update the particle if it is
		if (particle.lifetime <= 0)
			continue;

		// Update lifetime
		particle.lifetime -= deltaTime;
		particle.position = particle.position + particle.velocity * deltaTime; // TODO: operator Point2 += Vector2

		ImVec2 posMin(Globals::gGridX + particle.position.x, Globals::gGridY + particle.position.y);
		ImVec2 posMax(posMin.x + mTexture.width, posMin.y + mTexture.height);

		dl->AddImage(mTexture.id, posMin, posMax, ImVec2(0, 0), ImVec2(1, 1), mColor);
	}
}

void ParticleEmitter::SpawnParticles(uint32_t count)
{
	assert(mConfigured && "ParticleEmitter must be configured to spawn particles.");

	for (uint32_t i = 0; i < count; i++)
		TrySpawnParticle();
}

/// <summary>
/// Tries to spawn a new particle
/// </summary>
void ParticleEmitter::TrySpawnParticle()
{
	// Iterate through the particles
	for (uint32_t i = 0; i < mParticles.size(); i++)
	{
		Particle& particle = mParticles[i];

		// Check is dead
		if (particle.lifetime <= 0)
		{
			Vector2 velocity;
			if (mDirection.x == 0 && mDirection.y == 0)
				velocity = Vector2(ApplyDeltaToValue(0.f, 1.f), ApplyDeltaToValue(0.f, 1.f));
			else
			{
				velocity = mDirection;
				float_t deltaAngle = ApplyDeltaToValue(0, 1.f / mDirectionIntensity);
				velocity = velocity.Rotate(deltaAngle);
			}

			float_t speed = ApplyDeltaToValue(mAverageSpeed, mDeltaSpeed);
			velocity = velocity * speed;

			// Replace with new particle
			particle = Particle(mPosition, velocity, ApplyDeltaToValue(mAverageLifetime, mDeltaLifetime));
			break;
		}
	}
}

/// <summary>
/// Gets a random value of an average with a range [avg-delta;avg+delta]
/// </summary>
/// <returns>Result</returns>
float_t ParticleEmitter::ApplyDeltaToValue(float_t average, float_t delta)
{
	// Get a lifetime value between [avg-delta;avg+delta]

	// Get the maximum value possible
	float_t max = average + delta;

	// Get a random number between 0 and twice the lifetime
	float_t _delta = RANDOM_FLOAT(delta * 2);

	// Substract max and the random number
	return max - _delta;
}


void ParticleEmitter::SetParticleLimit(uint16_t limit)
{
	if (!mConfigured)
		return;

	mParticleLimit = limit;
	mParticles.resize(limit);
}

void ParticleEmitter::Start()
{
	if (!mConfigured)
		return;

	mActive = true;
}

void ParticleEmitter::Stop()
{
	mActive = false;
}

#undef RANDOM_FLOAT
