#pragma once
#include "mftransform.h"
#include "StandardNotation.h"

/**
 * \brief This object decodes raw H.264 streams when used as an IMFTransform within the
 * Microsoft Media Foundation framework
 * 
 */
class CH264Decoder : public IMFTransform
{
public:
	/**
	 * \brief Constructs a CH264Decoder object
	 */
	CH264Decoder(void);

	/**
	 * \brief Deconstructs a CH264Decoder object
	 */
	~CH264Decoder(void);

	/* IMFTransform implementations */

	/* IUnknown implementations */
};