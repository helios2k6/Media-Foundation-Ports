#pragma once
#include "stdafx.h"
#include "mftransform.h"

/**
 * This object decodes raw H.264 streams when used as an IMFTransform within the
 * Microsoft Media Foundation framework
 * 
 */
class CH264Decoder : public IMFTransform
{
public:

	/**
	 * Constructs a CH264Decoder object
	 */
	CH264Decoder(void);

	/**
	 * Deconstructs a CH264Decoder object
	 */
	~CH264Decoder(void);

	/* IMFTransform implementations */

	/* IUnknown implementations */
};
