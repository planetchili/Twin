#pragma once

#include "Animation.h"
#include "Vec2.h"
#include "SpriteEffect.h"
#include "Codex.h"
#include "SoundEffect.h"
#include "Bullet.h"

class Chili
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
	enum class AnimationSequence
	{
		Walking,
		Standing,
		Count
	};
public:
	Chili( const Vec2& pos );
	void Draw( Graphics& gfx ) const;
	// process input (can cause spawn of bullet, which is a little B.S.)
	void HandleInput( class Keyboard& kbd,class Mouse& mouse,const class World& world );
	void Update( class World& world,float dt );
	void ApplyDamage();
	const Vec2& GetPos() const;
	RectF GetHitbox() const;
	bool IsInvincible() const;
	void DisplaceBy( const Vec2& d );
private:
	void SetDirection( const Vec2& dir );
	void ProcessBullet( World& world );
private:
	const Surface* pHeadSurface = Codex<Surface>::Retrieve( L"Images\\chilihead.bmp" );
	const SoundEffect* pHurtSfx = Codex<SoundEffect>::Retrieve( L"Sounds\\chili_hurt.sfx" );
	Vec2 pos;
	// this flag is set during input processing to indicate a bullet should
	// be spawned on update (would love optional for this and the data)
	bool isFiring = false;
	// this is data for the bullet that will be fired
	Vec2 bulletDir;
	Vec2 bulletSpawnPos;
	// hitbox dimensions
	float hitbox_halfwidth = 10.0f;
	float hitbox_halfheight = 9.0f;
	// this value give the offset from the actual base of the
	// character to its drawing base
	Vec2 draw_offset = { -21.0f,-67.0f };
	Vec2 vel = { 0.0f,0.0f };
	std::vector<Animation> animations;
	AnimationSequence iCurSequence = AnimationSequence::Standing;
	// used to keep track of graphical facing (for sprite mirroring)
	bool facingRight = true;
	float speed = 110.0f;
	DamageEffectController dec = { *this };
};