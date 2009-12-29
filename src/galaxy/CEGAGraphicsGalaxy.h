/*
 * CEGAGraphics.h
 *
 *  Created on: 27.12.2009
 *      Author: gerstrong
 */

#ifndef CGALAXY_EGAGRAPHICS_H_
#define CGALAXY_EGAGRAPHICS_H_

#include <string>
#include <vector>

#include "EGAStructs.h"

class CEGAGraphicsGalaxy
{
public:
	CEGAGraphicsGalaxy(short episode, const std::string& path);

	bool loadData( int version, unsigned char *p_exedata );
	bool begin( int version, unsigned char *p_exedata );
	bool exportBMP();

	virtual ~CEGAGraphicsGalaxy();

private:

	std::string m_path;
	short m_episode;
	std::vector<unsigned long> m_egahead;
	std::vector<ChunkStruct> m_egagraph;
};

#endif /* CGALAXY_EGAGRAPHICS_H_ */
