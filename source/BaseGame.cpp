#include "BaseGame.h"
#include "BlockFactory.h"
#include "Character.h"
#include "MagnetiteCore.h"
#include "WorldChunk.h"
#include "BaseBlock.h"
#include "Camera.h"
#include "World.h"

REG_GAME_TYPE( "default", BaseGame )

BaseGame::BaseGame()
{
	mEngine = MagnetiteCore::Singleton; // Bad design they say? Humbug!
	mPlayer = NULL;
}

BaseGame::~BaseGame()
{

}

std::string BaseGame::getName()
{
	return "Default";
}

void BaseGame::_startGameSingle()
{
	mIsSinglePlayer = true;
	_startGame();
}

void BaseGame::_startGame()
{
	Util::log("#== Sandbox Game v0.1 ==================");
	Util::log("# is multiplayer: " + Util::toString( !isSingleplayer() ) );
}

bool BaseGame::isSingleplayer()
{
	return mIsSinglePlayer;
}

void BaseGame::_playerJoined()
{
	if( isSingleplayer() ) {
		mPlayer = createCharacter();
		playerJoin( mPlayer );
		playerSpawn( mPlayer );
	}
}

Character* BaseGame::getLocalPlayer()
{
	return mPlayer;
}

Character* BaseGame::createCharacter()
{
	return mEngine->createCharacter();
}

void BaseGame::_inputEvents( const InputEvent& e )
{
	if( e.event == Inputs::FORWARD ) {
		if( e.down )
			_inputMovement( Vector3( 0.f, 0.f, -1.f ) );
		else
			_inputMovement( Vector3( 0.f, 0.f, 1.f ) );
	}
	if( e.event == Inputs::LEFT ) {
		if( e.down )
			_inputMovement( Vector3( -1.f, 0.f, 0.f ) );
		else
			_inputMovement( Vector3( 1.f, 0.f, 0.f ) );
	}
	if( e.event == Inputs::RIGHT ) {
		if( e.down )
			_inputMovement( Vector3( 1.f, 0.f, 0.f ) );
		else
			_inputMovement( Vector3( -1.f, 0.f, 0.f ) );
	}
	if( e.event == Inputs::BACK ) {
		if( e.down )
			_inputMovement( Vector3( 0.f, 0.f, 1.f ) );
		else
			_inputMovement( Vector3( 0.f, 0.f, -1.f ) );
	}
	if( e.event == Inputs::JUMP && e.down ) {
		if( getLocalPlayer() ) mPlayer->jump();
	}
	if( e.event == Inputs::SPRINT ) {
		if( getLocalPlayer() ) mPlayer->enableSprint( e.down );
	}
	if( e.event == Inputs::FLY && e.down ) {
		if( getLocalPlayer() ) mPlayer->enableFlying( !mPlayer->isFlying() );
	}
	if( e.event == Inputs::RESPAWN && e.down ) {
		if( getLocalPlayer() ) mPlayer->setPosition( Vector3( 0, 150.f, 0 ) );
	}
}

void BaseGame::_inputMovement( const Vector3& v )
{
	if( getLocalPlayer() )
		mPlayer->addMoveDelta( v );
}

void BaseGame::_mouseMoved( const float x, const float y )
{
	if( getLocalPlayer() ) {
		mPlayer->getCamera()->pitch( y );
		mPlayer->getCamera()->yaw( x );
	}
}

void BaseGame::_primary()
{
	if( getLocalPlayer() ) {
		playerPrimaryClick( mPlayer );
	}
}

void BaseGame::_secondary()
{
	if( getLocalPlayer() ) {
		playerAltClick( mPlayer );
	}
}

//========= Events

void BaseGame::playerJoin( Character* player )
{
	Util::log( "A player just joined!" );
}

void BaseGame::playerSpawn( Character* player )
{
	if( player == mPlayer )
		Util::log( "You just spawned!" );
	player->setPosition( Vector3( 0.f, 120.f, 0.f )  );
}

void BaseGame::playerKilled( Character* player )
{
	if( player == mPlayer )
		Util::log( "You just died! D:" );
}

void BaseGame::characterDamage( Character* player )
{
	if( player == mPlayer )
		Util::log( "You're taking damage" );
}

void BaseGame::playerPrimaryClick( Character* player )
{
	raycast_r ray = player->getEyeCast();
	ray = mEngine->getWorld()->raycastWorld(ray);
	if(ray.hit)
	{
		Vector3 cIndex = mEngine->getWorld()->worldToChunks( ray.worldHit );
		Vector3 bIndex = mEngine->getWorld()->worldToBlock( ray.worldHit - (ray.hitNormal/2) );
		WorldChunk* chunk = mEngine->getWorld()->getChunk( cIndex.x, cIndex.y, cIndex.z );
		Util::log("Ray Hit: " + Util::toString( cIndex ) + " Normal: " + Util::toString( ray.hitNormal ) + " block: " + Util::toString(bIndex) );
		if(chunk && ray.block) {
			chunk->removeBlockAt( bIndex.x, bIndex.y, bIndex.z );
		}
	}
}

void BaseGame::playerAltClick( Character* player )
{
	raycast_r ray = player->getEyeCast();
	ray = mEngine->getWorld()->raycastWorld(ray);
	if(ray.hit && ray.block)
	{
		Util::log( Util::toString( ray.worldHit + ray.hitNormal ) );
		Vector3 cIndex = mEngine->getWorld()->worldToChunks( ray.worldHit + ray.hitNormal );
		Vector3 bIndex = mEngine->getWorld()->worldToBlock( ray.worldHit + (ray.hitNormal/2) );
		Util::log("Ray Hit: " + Util::toString( cIndex ) + " Normal: " + Util::toString( ray.hitNormal ) + " block: " + Util::toString(bIndex) );
		WorldChunk* chunk = mEngine->getWorld()->getChunk( cIndex.x, cIndex.y, cIndex.z );
		if(chunk) {
			BaseBlock* block = FactoryManager::getManager().createBlock( mEngine->getRenderer()->blockType );
			if( block != NULL ) {
				chunk->addBlockToChunk( block, bIndex.x, bIndex.y, bIndex.z );
			}
		}
	}
}