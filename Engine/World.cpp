#include "World.h"
#include <iterator>

// these are the layout strings for the scenery (background tilemaps)
const std::string layer1 =
"ALLLLLLLLLDEELLLLLLLLLLLA"
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
"LAAAAAAAAAAAAAAAAAAAAAAAL"
"LLLLLLLLLLLLLLLLLLLLLLLLL";

World::World( const RectI& screenRect,Keyboard& kbd,Mouse& mouse )
	:
	chili( Vec2( screenRect.GetCenter() ),kbd,mouse ),
	shia( chili.GetPos() ),
	bg1( screenRect,25,19,layer1 ),
	bg2( screenRect,25,19,layer2 )
{
	bgm.Play( 1.0f,0.6f );
	std::uniform_real_distribution<float> xd( 0,800 );
	std::uniform_real_distribution<float> yd( 0,600 );
	//for( int n = 0; n < 12; n++ )
	//{
	//	poos.emplace_back( Vec2{ xd( rng ),yd( rng ) } );
	//}
}
void World::ProcessLogic()
{
	chili.ProcessLogic( *this );
	shia.ProcessLogic( *this );
	// independent poo that don't need no World to tell her what to do!
	for( auto& poo : poos )
	{
		poo.ProcessLogic( *this );
	}
}

void World::Update( float dt )
{
	chili.Update( *this,dt );

	shia.Update( *this,dt );
	
	for( auto& b : bullets )
	{
		b.Update( *this,dt );
	}

	// update the poos and do poo collision with chili and bullets
	// AND do bullet removal due to collision with poo
	// (I don't like that we are doing so many collisions in here, but we'll see...
	// also mixing cleanup in here when most of it is done at the end)
	for( auto& poo : poos )
	{
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
				chili.ApplyDamage( 0.0f );
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

	// build vector of entity references for sort and draw
	std::vector<std::reference_wrapper<const Entity>> entSort;
	entSort.insert( entSort.end(),bullets.begin(),bullets.end() );
	entSort.insert( entSort.end(),poos.begin(),poos.end() );
	entSort.emplace_back( chili );
	entSort.emplace_back( shia );

	// sort entities by y coord
	std::sort( entSort.begin(),entSort.end(),Entity::LessY{} );

	// draw all entities
	for( const Entity& e : entSort )
	{
		e.Draw( gfx );
		gfx.DrawRectThin( RectI( e.GetHitbox() ),Colors::Blue );
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
