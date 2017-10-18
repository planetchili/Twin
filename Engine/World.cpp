#include "World.h"

// these are the layout strings for the scenery (background tilemaps)
const std::string layer1 =
"AAAAAAAAAADEEAAAAAAAAAAAA"
"AIHIHIHIHICDBIHIHIHIHIHIA"
"AKJKJKJKJKEEDKJKJKJKJKJKA"
"AEEBBFEDEFBCDCCECCCEBDDCA"
"ADCCCCCGBEFEEDEECBCDEEDBA"
"ABBBBBCEBCBCBGBCBDEBDEBCA"
"ABCGFDEEBGEDBCDDEFECBEEBA"
"ADCCCBCCEEEEEBEEGDEGBEDEA"
"ACEECEBCCDEDBBEDDBDBFBDDA"
"ACBCDDFBGFDDEFEDEEDBCCBEA"
"ABBDEEEECECDECFBEDCBEBDBA"
"AEBGEEEFEDCCEECFDCBDDBDDA"
"ACEDDBBCBFDBBCCBEGCCCBEEA"
"ACBBBBEBEGBDDBDCCDECBBBBA"
"AGEDCCEDEDCCCBCEECCDCEDDA"
"AGDDEGCBBCBEEEBCFCDCEGGCA"
"ABEBFBCFBBEDCEDBDGBFBCGBA"
"ADECBCDDCCCDCDDDECECDDDBA"
"AAAAAAAAAAAAAAAAAAAAAAAAA";

const std::string layer2 =
"LLLLLLLLLLAAALLLLLLLLLLLL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LLLLLLLLLLLLLLLLLLLLLLLLL";

World::World( const RectI& screenRect )
	:
	bg1( screenRect,25,19,layer1 ),
	bg2( screenRect,25,19,layer2 )
{
	bgm.Play( 1.0f,0.6f );
	std::uniform_real_distribution<float> xd( 0,800 );
	std::uniform_real_distribution<float> yd( 0,600 );
	for( int n = 0; n < 12; n++ )
	{
		poos.emplace_back( Vec2{ xd( rng ),yd( rng ) } );
	}
}
void World::HandleInput( Keyboard& kbd,Mouse& mouse )
{
	// process mouse messages while any remain
	while( !mouse.IsEmpty() )
	{
		const auto e = mouse.Read();
		// only interested in left mouse presses
		// fire in the hole! (here is our attack logic)
		if( e.GetType() == Mouse::Event::Type::LPress )
		{
			// bullet spawn location
			const Vec2 bspawn = chili.GetPos() + Vec2{ 0.0f,-15.0f };
			// get direction of firing
			auto delta = (Vec2)e.GetPos() - bspawn;
			// process delta to make it direction
			// if delta is 0 set to straight down
			if( delta == Vec2{ 0.0f,0.0f } )
			{
				delta = { 0.0f,1.0f };
			}
			// else normalize
			else
			{
				delta.Normalize();
			}
			// now spawn bullet!
			// (eventually wanna handle this stuff in Chili or some Chili controller... maybe?)
			SpawnBullet( { bspawn,delta } );
		}
	}
	// process arrow keys state to set direction
	Vec2 dir = { 0.0f,0.0f };
	if( kbd.KeyIsPressed( VK_UP ) )
	{
		dir.y -= 1.0f;
	}
	if( kbd.KeyIsPressed( VK_DOWN ) )
	{
		dir.y += 1.0f;
	}
	if( kbd.KeyIsPressed( VK_LEFT ) )
	{
		dir.x -= 1.0f;
	}
	if( kbd.KeyIsPressed( VK_RIGHT ) )
	{
		dir.x += 1.0f;
	}
	chili.SetDirection( dir );
	// independent poo that don't need no World to tell her what to do!
	for( auto& poo : poos )
	{
		poo.ProcessLogic( *this );
	}
}

void World::Update( float dt )
{
	// update chili
	chili.Update( dt );
	// adjust chili to boundary
	bounds.Adjust( chili );
	
	// update all bullets
	for( auto& b : bullets )
	{
		b.Update( dt );
	}

	// update the poos and do poo collision with chili and bullets
	// AND do bullet removal due to collision with poo
	// (I don't like that we are doing so many collisions in here, but we'll see...
	// also mixing cleanup in here when most of it is done at the end)
	for( auto& poo : poos )
	{
		// update those poos
		poo.Update( *this,dt );

		// here we have tests for collision between poo and bullet/chili
		// only do tests if poo is alive
		if( !poo.IsDead() )
		{
			// calculate the poo hitbox once here
			const auto poo_hitbox = poo.GetHitbox();
			// chili take damage if collide with poo
			// a little redundancy here in generating same chili hitbox for each poo
			// but do we really care? (naw son)
			if( !chili.IsInvincible() && chili.GetHitbox().IsOverlappingWith( poo_hitbox ) )
			{
				chili.ApplyDamage();
			}

			// remove all bullets colliding with current poo
			remove_erase_if( bullets,
				// capture poo so we can damage it on bullet collision
				[&poo,&poo_hitbox]( const Bullet& b )
				{
					if( b.GetHitbox().IsOverlappingWith( poo_hitbox ) )
					{
						// this lambda predicate has side effect of damaging poos
						// when a collision is detected
						poo.ApplyDamage( 35.0f );
						return true;
					}
					return false;
				}
			);
		}
	}

	// remove all poos ready for removal
	remove_erase_if( poos,std::mem_fn( &Poo::IsReadyForRemoval ) );

	// remove all oob fballs
	remove_erase_if( bullets,
		// precalculate oob box
		// offset upwards to account for bullet 'height' (nasty hack?)
		[bound_rect = bounds.GetRect().GetDisplacedBy( { 0.0f,-10.0f } )]
		( const Bullet& b )
		{
			return !b.GetHitbox().IsOverlappingWith( bound_rect );
		}
	);
}

void World::Draw( Graphics& gfx ) const
{
	// draw scenery underlayer
	bg1.Draw( gfx );

	// draw dem poos
	for( const auto& poo : poos )
	{
		poo.Draw( gfx );
	}

	chili.Draw( gfx );

	for( const auto& b : bullets )
	{
		b.Draw( gfx );
	}

	// draw scenery overlayer
	bg2.Draw( gfx );
}

void World::SpawnBullet( Bullet bullet )
{
	bullets.push_back( bullet );
}

const std::vector<Poo>& World::GetPoosConst() const
{
	return poos;
}

const Chili& World::GetChiliConst() const
{
	return chili;
}

const std::vector<Bullet>& World::GetBulletsConst() const
{
	return bullets;
}

const Boundary& World::GetBoundsConst() const
{
	return bounds;
}
