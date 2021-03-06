#ifndef _WORLDSERIALIZER_H_
#define _WORLDSERIALIZER_H_
#include "prerequisites.h"
#include <memory>

class World;
namespace Magnetite 
{
	/**
	 * @class WorldSerializer
	 * 
	 * Handles saving and loading worlds (or portions therof) to binary streams.
	 */
	class WorldSerializer
	{
	protected:
		
		World* mWorld;
		
		Magnetite::String mWorldPath;
		
	public:
		
		WorldSerializer( World* mWorld );
		
		~WorldSerializer();
		
		/** 
		 * Figures out the path for the given region coordinates.
		 */
		String resolveRegion( ChunkScalar x, ChunkScalar y, ChunkScalar z );
		
		/**
		 * Check to see if the serializer has the given chunk.
		 */
		bool hasChunk( ChunkScalar x, ChunkScalar y, ChunkScalar z );
		
		/**
		 * Requests that the serializer load the given chunk into the world.
		 * @return true if the chunk was loaded, false otherwise.
		 */
		bool loadChunk( ChunkScalar x, ChunkScalar y, ChunkScalar z );
		
		/**
		 * Requests that the serializer writes the given chunk into the stream.
		 */
		void saveChunk( ChunkScalar x, ChunkScalar y, ChunkScalar z );
		
	};
};

#endif