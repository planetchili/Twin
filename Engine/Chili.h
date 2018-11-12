#pragma once

#include "Animation.h"
#include "Codex.h"
#include "SoundEffect.h"
#include "Surface.h"
#include "Entity.h"

class Chili : public Entity
{
private:
	class DamageEffectController
	{
	public:
		DamageEffectController( Chili& parent );
		// update damage effect time
		void Update( float dt );
		// draw chili based on damage effect state
		void DrawChili( Graphics& gfx ) const;
		// activate damage effect
		void Activate();
		bool IsActive() const;
	private:
		Chili& parent;
		static constexpr float RedDuration = 0.045f;
		static constexpr float totalDuration = 1.5f;
		static constexpr float blinkHalfPeriod = 0.18f;
		float time;
		bool active = false;
	};
	class DashEffectController
	{
	private:
		struct Shadow
		{
			Vec2 pos;
			float t;
			float tDeath;
		};
	public:
		DashEffectController( Chili& parent );
		// update shadow
		void Update( float dt );
		// draw chili as per effect
		void DrawChili( Graphics& gfx ) const;
		// activate shadow
		void Activate();
		bool IsActive() const;
	private:
		Chili& parent;
		std::vector<Shadow> shadows;
		static constexpr float dashDuration = 0.15f;
		static constexpr float dashSpeed = 900.0f;
		static constexpr float kShadowDuration = 2.5f;
		static constexpr float shadowPeriod = 0.024f;
		float shadowTime;
		float time;
		bool active = false;
	};
	enum class AnimationSequence
	{
		Walking,
		Standing,
		Count
	};
public:
	Chili( const Vec2& pos,class Keyboard& kbd,class Mouse& mouse );
	void Draw( Graphics& gfx ) const override;
	// process logic (set direction and queues up actions) (uses ptrs to kbd/mouse)
	void ProcessLogic( const class World& world ) override;
	void Update( class World& world,float dt ) override;
	void ApplyDamage( float damage ) override;
	bool IsInvincible() const;
	float GetHealth() const
	{
		return health;
	}
private:
	void SetDirection( const Vec2& dir );
	void ProcessBullet( World& world );
private:
	static constexpr float chiliSpeed = 110.0f;
	float health = 4.0f;
	class Keyboard* pKbd = nullptr;
	class Mouse* pMouse = nullptr;
	const Surface* pHeadSurface = Codex<Surface>::Retrieve( L"Images\\chilihead.bmp" );
	const Sound* pDodgeSnd = Codex<Sound>::Retrieve( L"Sounds\\dodge.wav" );
	const SoundEffect* pHurtSfx = Codex<SoundEffect>::Retrieve( L"Sounds\\chili_hurt.sfx" );
	// this flag is set during input processing to indicate a bullet should
	// be spawned on update (would love optional for this and the data)
	bool isFiring = false;
	// this is data for the bullet that will be fired
	Vec2 bulletDir;
	Vec2 bulletSpawnPos;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -21.0f,-67.0f };
	std::vector<Animation> animations;
	AnimationSequence iCurSequence = AnimationSequence::Standing;
	DamageEffectController dec = { *this };
	DashEffectController dshec = { *this };
};