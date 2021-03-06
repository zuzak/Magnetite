#ifndef _WORLD_H_
#define _WORLD_H_
#include "prerequisites.h"
#include <deque>
#include <mutex>
#include <thread>
#include <glm/core/type.hpp>
#include "Region.h"
#include "MovingBlock.h"
#include "paging/PagingContext.h"

namespace Magnetite {
class WorldSerializer;class BaseEntity;
}

class BaseTriangulator;
class Chunk;
typedef Chunk* ChunkPtr;
typedef Chunk** ChunkArray;

/**
 * @struct Raycast result structure.
 */
struct raycast_r
{
	/**
	 * First intersection length
	 */
	float i0;
	/**
	 * Second intersection length
	 */
	float i1;
	/**
	 * Maximum ray length
	 */
	float maxDistance;
	/**
	 * Unused
	 */
	float length;
	/**
	 * True if the ray hit something
	 */
	bool  hit;

	/**
	 * Origin position of the ray
	 */
	Vector3 orig;
	/**
	 * Ray Direction
	 */
	Vector3 dir;
	/**
	 * World Hit Position
	 */
	Vector3 worldHit;
	/**
	 * Hit normal
	 */
	Vector3 hitNormal;

	/**
	 * pointer to block the ray hit, if applicable
	 */
	BaseBlock* block;
	/**
	 * Index of the block that was hit (relative to it's chunk)
	 */
	size_t blockIndex;
	/**
	 * Position of the block, relative to the chunk
	 */
	Vector3 blockPosition;
	/**
	 * Chunk containing the hit block
	 */
	Chunk* chunk;

	raycast_r(void) { 
		i0 = std::numeric_limits<float>::max();
		i1 = -std::numeric_limits<float>::max();
		maxDistance = 100.f;
		length = std::numeric_limits<float>::max();
		hit = false;
		block = NULL;
		chunk = NULL;
		blockIndex = 0;
	}
};

/**
 * @struct EntitySearch
 * 
 * Struct for entity search option
 */
struct EntitySearch 
{
	enum SearchFlags {
		SF_None = 0,
		SF_Position = 1,
		SF_Type = 2,
		SF_MaxDistance = 4
	};
	
	/**
	 * Flags for which search options to enable
	 */
	uint8_t flags;
	
	/**
	 * Type of entity to find.
	 */
	Magnetite::String type;
	
	/**
	 * Center to search from
	 */
	Vector3 center;
	
	/**
	 * maximum distance to find entities.
	 */
	float maxDistance;
	
	void setDefaults() {
		flags = SF_None;
		maxDistance = 0.f;
	}
};

class Sky;
class ChunkGenerator;
class Camera;

struct ChunkRequest
{
	ChunkScalar x, y, z;
	bool unload;
};

typedef std::deque<ChunkRequest> ChunkLoadList;
/**
 * ChunkArray - an array of Chunks
 */
typedef Chunk** ChunkArray;

class World : public Magnetite::PagingContext
{
protected:
	
	//ChunkArray	mChunks;
	
	Magnetite::ChunkRegionArray mRegions;
	
	// Size of the world, in regions.
	size_t 		mWorldSize;

	Sky*		mSky;
	ChunkGenerator* mGenerator;
	std::string mWorldName;
	ChunkLoadList mChunkRequests;
	
	MovingBlockList mMovingBlocks;
	
	/**
	 * List of entities in the world.
	 */
	Magnetite::EntityList mEntities;
	
	/**
	 * The Triangulator to use for creating the world geometry
	 */
	BaseTriangulator* mTriangulator;
	
	/**
	 * Serializer, for writing chunks to disk
	 */
	Magnetite::WorldSerializer* mSerializer;
	
	/**
	 * Mutex for thread-orientated work.
	 */
	std::mutex mWorldMutex;
	
	/**
	 * Internal function to add entities to the mEntities list.
	 */
	void addEntity( Magnetite::BaseEntity* ent );
	
public:
	
	/**
	 * World thread id.
	 */
    std::thread::id mThreadID;
	
	/** 
	 * Constructor: -
	 */
	World( size_t edgeSize );
	
	/**
	 * Destructor:- 
	 */
	~World( void );

	/**
	 * Returns the number of regions.
	 */
	size_t getRegionCount() const;

	/**
	 * Returns the triangulator to use for generating meshes.
	 */
	BaseTriangulator* getTriangulator();

	/**
	 * Returns the color of a brightness level
	 */
	inline static float getLightColor( LightIndex light )
	{
		return ( 0.1f + ( 0.9f * ( (float)light/256 ) ) );
	}
	
	/**
	 * Returns the block at the world index specified
	 */
	BlockPtr getBlockAt( long x, long y, long z );
	
	/**
	 * Removes any block at the given position
	 */
	void removeBlockAt( long x, long y, long z );
	
	/**
	 * Places a block at the world coordinates
	 */
	void setBlockAt( BaseBlock* b, long x, long y, long z );
	
	/**
	 * Starts a block moving if there is a block at the given coordinates
	 */
	void moveBlock( long x, long y, long z, float time, long ex, long ey, long ez );
	
	/**
	 * Returns the list of moving blocks
	 */
	MovingBlockList& getMovingBlocks();
	
	/** 
	 * Returns the light level at the world coordinates
	 * @param x X Coordinate
	 * @param y Y Coordinate
	 * @param z Z Coordinate
	 */
	LightIndex getLightLevel( long x, long y, long z );

	/**
	 * Loads a world of the specified name
	 */
	void loadWorld( std::string name );

	/**
	 * Makes a NEW WORLD!
	 */
	void newWorld( std::string name );

	/**
	 * Returns the name of the world
	 */
	Magnetite::String getName();
	
	/**
	 * Sets the name of the world.
	 */
	void setName( const Magnetite::String& name );

	/**
	 * Returns the path to the current world
	 */
	std::string getSavePath();

	/**
	 * Creates a new empty world.
	 */
	void createWorld();
	
	/**
	 * Creates a new entity of the given type and adds it to the entity pool.
	 */
	template<class T> T* createEntity()
	{
		T* e = new T(this);
		addEntity(e);
		return e;
	};
	
	Magnetite::EntityList getEntities();
	
	/**
	 * Search for a single Entities
	 */
	Magnetite::BaseEntity* findEntity( const EntitySearch& es );
	
	bool printDbg;

	/**
	 * Requests that the engine load or generate the chunk at the given index
	 */
	void requestChunk( ChunkScalar x, ChunkScalar y, ChunkScalar z );
	
	/**
	 * Requests that the engine unload the given chunk.
	 */
	void requestChunkUnload( ChunkScalar x, ChunkScalar y, ChunkScalar z );
	
	/**
	 * Returns the number of queued chunk requests.
	 */
	size_t getNumChunkRequests() {
		return mChunkRequests.size();
	}
	
	/**
	 * Creates a chunk at the given coordinates.
	 * @param x Coordinate
	 * @param y Coordinate
	 * @param z Coordinate
	 */
	Chunk* createChunk(long x, long y, long z);
	
	/**
	 * Generates data for the chunk at the given coordinates
	 * @param x Coordinate.
	 * @param y Coordinate.
	 * @param z Coordinate.
	 */
	Chunk* generateChunk( ChunkScalar x, ChunkScalar y, ChunkScalar z );

	/**
	 * Removes the chunk at the given offset
	 * @param x Coordinate
	 * @param y Coordinate
	 * @param z Coordinate
	 */
	void removeChunk(long x, long y, long z);
	
	/**
	 * Creates a Region at the given coordinates
	 */
	Magnetite::ChunkRegionPtr createRegion( const ChunkScalar x, const ChunkScalar y, const ChunkScalar z );
	
	/**
	 * Destroys a region
	 */
	void removeRegion( const ChunkScalar x, const ChunkScalar y, const ChunkScalar z );
	
	/**
	 * Returns all of the Regions.
	 */
	Magnetite::ChunkRegionArray getRegions() const;

	/**
	 * Creates a series of test chunks of radius size.
	 * @param size Radius
	 */
	void createTestChunks( int size );

	/**
	 * Activates a chunk, loading it if it is stored on disk, or generating it if necassery
	 */
	void activateChunk( long x, long y, long z );

	/**
	 * Deactivates a chunk
	 */
	void deactivateChunk( long x, long y, long z );
	
	/**
	 * Causes all adjacent chunks to update
	 */
	void updateAdjacent( ChunkScalar x, ChunkScalar y, ChunkScalar z );

	/**
	 * Destroys all chunks in the world.
	 */
	void destoryWorld();

	/**
	 * Updates all terrain related matters.
	 */
	void update( float dt );
	
	/**
	 * Updates entities in the world.
	 */
	void updateEntities( float dt );
	
	/**
	 * Updates all of the moving blocks
	 */
	void updateMovingBlocks( float dt );
	
	/**
	 * Updates paging 
	 */
	void updatePages();

	/**
	 * Performs a raytest against the world.
	 */
	raycast_r raycastWorld(const raycast_r &inray, bool solidOnly = false);

	/**
	 * Returns the chunk at the given indexes.
	 */
	Chunk* getChunk(const long x, const long y, const long z);
	
	/**
	 * Returns the Region at the given indexes 
	 */
	Magnetite::ChunkRegionPtr getRegion(const ChunkScalar x, const ChunkScalar y, const ChunkScalar z);

	/**
	 * Creates a new sky object 
	 */
	void createSky( size_t time );

	/**
	 * @returns a Pointer to the sky object.
	 */
	Sky* getSky();
	
	/**
	 * Returns true if there is a block surrounding this point
	 */
    bool hasNeighbours(short int x, short int y, short int z);
	
	/**
	 * Called by the PagingContext when a new chunk is needed to be loaded.
	 */
	virtual void onPageEntered(const Magnetite::PageInfo& pageinfo);
	
	/**
	 * Called by the PagingContext when a page should be unloaded.
	 */
	virtual void onPageExit(const Magnetite::PageInfo& pageinfo);

};

#endif // _WORLD_H_
