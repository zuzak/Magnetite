#include "ChunkGenerator.h"
#include "OpencraftCore.h"
#include "WorldChunk.h"
#include "BlockFactory.h"
#include "BaseBlock.h"

ChunkGenerator::ChunkGenerator(long seed)
: mSeed( seed )
{
	srand(seed);
}

ChunkGenerator::~ChunkGenerator()
{

}

float ChunkGenerator::noise( long x, long z )
{
	long n = x + z * 57;
	n = (n<<13) ^ n;
	return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float ChunkGenerator::interpolateCosine( float a, float b, float x )
{
	float ft = x * 3.1415827f;
	ft = (1-cos(ft)) * .5f;
	return (a*(1-ft) + b*ft);
}

float ChunkGenerator::interpolatedNoise( float x, float y )
{
	int X = x;
	float frac_x = x - X;
	int Y = y;
	float frac_y = y - Y;

	float v1 = smooth( X, Y );
	float v2 = smooth( X + 1, Y );
	float v3 = smooth( X, Y + 1 );
	float v4 = smooth( X + 1, Y + 1 );

	float i1 = interpolateCosine(v1, v2, frac_x);
	float i2 = interpolateCosine(v3, v4, frac_x);

	return interpolateCosine( i1, i2, frac_y );
}

float ChunkGenerator::smooth( float x, float z )
{
	float corners = (noise( x - 1, z - 1 ) + noise( x + 1, z - 1 ) + noise( x - 1, z + 1 ) + noise( x + 1, z + 1 ) ) / 16.f;
	float sides = ( noise( x - 1, z ) + noise( x+1, z ) + noise( x, z -1 ) + noise( x, z +1 ) ) / 8.f;
	float center = ( noise( x, z ) ) / 4.f;
	
	return center + sides + corners;
}

 void ChunkGenerator::fillChunk(WorldChunk *chunk)
{
	float p = 0.25f;
	float octs = 10;
	for( int x = 0; x < CHUNK_WIDTH; x++ ) {
		for( int z = 0; z < CHUNK_WIDTH; z++ ) {
			float total = 0.f;
			for( float i = 0; i < octs; i++ ) {
				float freq = pow(2.f, i);
				float amp = pow(p, i);
				total = total + interpolatedNoise((float)((chunk->getX()*CHUNK_WIDTH) + x) * freq * 0.05f , (float)((chunk->getZ() * CHUNK_WIDTH) + z) *freq * 0.05f ) * amp * 64.f;
			}
			std::string type = "stone";
			for( int y = 0; y < 64+total; y++ ) {
				if( y > total - 5 )
					type = "dirt";
				if( y > total -1 )
					type = "grass";
				BaseBlock* block = FactoryManager::createBlock(type);
				if( block ) {
					block->setPosition( x, y, z );
					chunk->addBlockToChunk( block );
				}
			}
			if( 64 + total < 64 )
			{
				for( int y = 64+total; y < 64; y++ ) {
					BaseBlock* block = FactoryManager::createBlock("water");
					if( block ) {
						block->setPosition( x, y, z );
						chunk->addBlockToChunk( block );
					}
				}
			}
		}
	}
}