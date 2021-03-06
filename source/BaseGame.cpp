#include "BaseGame.h"
#include "BlockFactory.h"
#include "Character.h"
#include "Player.h"
#include "MagnetiteCore.h"
#include "Renderer.h"
#include "Chunk.h"
#include "BaseBlock.h"
#include "Camera.h"
#include "World.h"
#include "Explosion.h"

REG_GAME_TYPE( "default", BaseGame )

BaseGame::BaseGame()
{
	mEngine = MagnetiteCore::Singleton; // Bad design they say? Humbug!
	mPlayer = NULL;
	clickMode = "remove";
}

BaseGame::~BaseGame()
{

}

std::string BaseGame::getName()
{
	return "Default";
}

Magnetite::String BaseGame::getPath()
{
	return "";
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

void BaseGame::_loadGame()
{
	
}

bool BaseGame::isSingleplayer()
{
	return mIsSinglePlayer;
}

void BaseGame::_playerJoined()
{
	if( isSingleplayer() ) {
		mPlayer = new Player();
		mEngine->addPlayer(mPlayer);
		playerJoin( mPlayer );
		playerSpawn( mPlayer );
	}
}

Player* BaseGame::getLocalPlayer()
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
		//if( getLocalPlayer() ) mPlayer->jump();
	}
	if( e.event == Inputs::SPRINT ) {
		//if( getLocalPlayer() ) mPlayer->enableSprint( e.down );
	}
	if( e.event == Inputs::RESPAWN && e.down ) {
		if( getLocalPlayer() ) mPlayer->setPosition( Vector3( 0, 150.f, 0 ) );
	}
}

void BaseGame::_inputMovement( const Vector3& v )
{
	//if( getLocalPlayer() )
		//mPlayer->addMoveDelta( v );
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

void BaseGame::playerJoin( Player* player )
{
	Util::log( "A player just joined!" );
}

void BaseGame::playerSpawn( Player* player )
{
	//if( player == mPlayer )
	//	Util::log( "You just spawned!" );
	player->setPosition( Vector3( 0.f, 120.f, 0.f )  );
}

void BaseGame::playerKilled( Character* player )
{
	//if( player == mPlayer )
	//	Util::log( "You just died! D:" );
}

void BaseGame::characterDamage( Character* player )
{
	//if( player == mPlayer )
	//	Util::log( "You're taking damage" );
}

void BaseGame::playerPrimaryClick( Player* player )
{
	
}

void BaseGame::playerAltClick( Player* player )
{
	
}

void BaseGame::uiPaint(Renderer* r)
{
	r->drawText(clickMode, 10, 50);
}

void BaseGame::think( float dt )
{
	
}
void BaseGame::keyDown( size_t evt )
{
	
}

void BaseGame::keyUp( size_t evt )
{
	
}