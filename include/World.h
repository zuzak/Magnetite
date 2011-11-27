#ifndef _WORLD_H_
#define _WORLD_H_
#include "prerequisites.h"
#include "WorldNode.h"
#include "Collision.h"

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
	WorldChunk* chunk;

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
 * @struct AABB->AABB collision test 
 */
struct collision_r
{
	Vector3 response;

	bool collision;

	collision_r() {
		collision = false;
	}
};

class Sky;
class ChunkGenerator;
class Camera;

struct ChunkRequest
{
	int x, y, z;
};

typedef std::vector<ChunkRequest> ChunkLoadList;

/**
* World Stage ENUM, used for determining certain behaviours
* During WORLD_GEN, adjacent chunks are NOT updated if an edge update occurs
*/
enum WorldStage {
	WORLD_GEN = 1, // World is being mass-generated
	WORLD_NORMAL = 2 // World is being run as normal
};

class World
{
protected:
	ChunkList	mChunks;
	NodeList	mQuadTrees;
	Sky*		mSky;
	ChunkGenerator* mGenerator;
	Camera*		mPagingCamera;
	std::string mWorldName;
	WorldStage mWorldStage;
	ChunkLoadList mChunksToLoad;
public:
	/** 
	 * Constructor: -
	 */
	World( void );
	
	/**
	 * Destructor:- 
	 */
	~World( void );

	/**
	 * Returns the world's current stage
	 */
	WorldStage getCurrentStage();

	/**
	 * Returns the color of a brightness level
	 */
	static float getLightColor( LightIndex light );

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
	std::string getName();

	/**
	 * Returns the path to the current world
	 */
	std::string getSavePath();

	/**
	 * Creates a new empty world.
	 */
	void createWorld();

	/**
	 * Returns a reference to the chunk list.
	 */
	ChunkList& getChunks();

	/**
	 * Requests that the engine load or generate the chunk at the given index
	 */
	void requestChunk( int x, int y, int z );
	
	/**
	 *	Converts world coordinates into chunk indexes.
	 */
	static Vector3 worldToChunks( const Vector3& vec );

	/**
	 *	Converts world coordinates into block offset ( in the chunk at that location )
	 */
	static Vector3 worldToBlock( const Vector3& vec );

	/**
	 * Returns the top-level Quadtree node at the given coordinates or NULL if it's not in memory
	 * @param Vector3 pos The position to use
	 * @param safe In the case that the leaf doesn't exist and this is true, the leaf will be created for you.
	 */
	WorldNode* getWorldNode( const Vector3& pos, bool safe = false );

	/**
	 * Returns the chunk node at the given position
	 */
	WorldNode* getChunkNode( const Vector3& pos, bool safe = false );
	void _populateTree( WorldNode* parent, const NodeIndex &ppos, int depth );

	std::string printTree();
	std::string _printTree(WorldNode* node, int depth);

	/**
	 * Returns the list of top-level nodes
	 */
	NodeList& getTopNodes();

	/**
	 * Creates a chunk at the given coordinates and fills it with test data
	 * @param x Coordinate
	 * @param y Coordinate
	 * @param z Coordinate
	 */
	WorldChunk* createChunk(long x, long y, long z);

	/**
	 * Removes the chunk at the given offset
	 * @param x Coordinate
	 * @param y Coordinate
	 * @param z Coordinate
	 */
	void removeChunk(long x, long y, long z);

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
	void deativateChunk( long x, long y, long z );

	/**
	 * Saves all chunks to disk
	 */
	void saveAllChunks();

	/**
	 * Creates the folder structure for the current world
	 */
	void createWorldFolder();

	/**
	 * Sets the camera used for paging chunks
	 */
	void setPagingCamera( Camera* _c );

	/**
	 * Destroys all chunks in the world.
	 */
	void destoryWorld();

	/**
	 * Updates all loaded chunks
	 */
	void update( float dt );

	/**
	 * Performs a raycast test against a single cube
	 * @param ray Raycast object.
	 * @param min Cube minimum.
	 * @param max Cube maximum.
	 */
	raycast_r raycastCube(const raycast_r &ray, Vector3& min, Vector3& max);

	/**
	 * Performs a raytest against the world.
	 */
	raycast_r raycastWorld(const raycast_r &inray, bool solidOnly = false);

	/**
	 * Returns the chunk at the given indexes.
	 */
	WorldChunk* getChunk(const long x, const long y, const long z);

	/**
	 * Performs an AABB test against the world.
	 */
	CollisionResponse AABBWorld( Vector3& min, Vector3& max );

	collision_r AABBCube( Vector3& min, Vector3& max, Vector3& minb, Vector3& maxb );

	/**
	 * Creates a new sky object 
	 */
	void createSky( size_t time );

	/**
	 * @returns a Pointer to the sky object.
	 */
	Sky* getSky();

};

#endif // _WORLD_H_
