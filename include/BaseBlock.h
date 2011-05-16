#ifndef _BASEBLOCK_H_
#define _BASEBLOCK_H_
#include "prerequisites.h"

struct GLvertex;
typedef unsigned short GLedge;

enum {
	FACE_NONE	= 0,
	FACE_TOP		= (1<<0),
	FACE_BOTTOM	= (1<<1),
	FACE_LEFT	= (1<<2),
	FACE_RIGHT	= (1<<3),
	FACE_FORWARD	= (1<<4),
	FACE_BACK	= (1<<5)
};

class WorldChunk;

class BaseBlock
{
protected:
	size_t mDamage;
	long mX;
	long mY;
	long mZ;
	short mBlockX;
	short mBlockY;

	WorldChunk* mChunk;
public:
	BaseBlock(long x = 0, long y = 0, long z = 0);
	~BaseBlock(void);

	void _setChunk( WorldChunk* chnk );

	short vertexIndex;
	short mViewFlags;

	virtual void created() = 0;
	virtual void hit() = 0;

	/**
	 * Returns true if this object inherits FluidBlock
	 */
	virtual bool isFluid() { return false; }
	
	/**
	 * Returns true if block should stop occlusion
	 */
	virtual bool isOpaque() { return true; }


	/**
	 * Called when a nearby block changes.
	 * @param face The direction of the changed block.
	 */
	virtual void connectedChange( short face );

	virtual void texture(int x, int y);
	virtual void getTextureCoords( short face, short &x, short &y );

	/**
	 * Returns the Verex Data for this block
	 */
	virtual void buildCubeData(size_t& ind, size_t& eInd, GLvertex* data, GLedge* edges);

	virtual std::string getType() = 0;

	long getX();
	long getY();
	long getZ();

	void setPosition( long x, long y, long z );
	void setPosition( const Vector3& vec );
};

#endif