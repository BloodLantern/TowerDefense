#pragma once

#include "Point2.h"
#include "Vector2.h"
#include "imgui_utils.hpp"
#include <vector>

class ParticleEmitter
{
private:
	struct Particle
	{
		Point2 position;
		Vector2 velocity;

		float_t lifetime;

		Particle()
			: position(0.f), velocity(0.f), lifetime(0.f)
		{ }

		Particle(Point2 pos, Vector2 vel, float_t lt)
			: position(pos), velocity(vel), lifetime(lt)
		{ }
	};

	Point2 mPosition;

	Texture& mTexture;
	std::vector<Particle> mParticles;

	uint16_t mParticleLimit;

	float_t mAverageLifetime;
	float_t mDeltaLifetime;

	float_t mFrequency;
	float_t mFrequencyMax;

	uint32_t mColor;
	Vector2 mDirection;
	float_t mDirectionIntensity;

	float_t mAverageSpeed;
	float_t mDeltaSpeed;

	bool mConfigured;
	bool mActive;

public:
	ParticleEmitter(Point2 position, Texture& texture);

	/// <summary>
	/// Configures the emitter with data on how the particles should behave
	/// </summary>
	/// <param name="limit">The maximum amount of particles it can emit</param>
	/// <param name="avgLifetime">The average lifetime of a particle</param>
	/// <param name="deltaLifetime">The range of the lifetime variation [-delta;+delta]</param>
	/// <param name="frequency">The frequency at which the particles will spawn (in seconds)</param>
	/// <param name="color">An additional color to be added to the particle</param>
	/// <param name="direction">A direction in which the particles will tend to go (use 0 for full random)</param>
	/// <param name="avgSpeed">The average speed of the particles</param>
	/// <param name="deltaSpeed">The range of the speed variation [-delta;+delta]</param>
	/// <param name="start">Wheter the emitter should start emitting straight away or not</param>
	void ConfigureEmitter(
		uint16_t limit, float_t avgLifetime, float_t deltaLifetime,
		float_t frequency, uint32_t color, Vector2 direction,
		float_t avgSpeed, float_t deltaSpeed,
		bool start = true
	);

	void Update();

	void SetTexture(Texture& texture) { mTexture = texture; }
	void SetParticleLimit(uint16_t limit);
	void SetPosition(Point2 pos) { mPosition = pos; }

	void Start();
	void Stop();

private:
	void TrySpawnParticle();

	float_t ApplyDeltaToValue(float_t average, float_t delta);
};
