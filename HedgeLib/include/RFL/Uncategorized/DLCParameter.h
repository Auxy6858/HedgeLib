#ifndef HDLC_PARAMETER_H_INCLUDED
#define HDLC_PARAMETER_H_INCLUDED
#include "IO/BINA.h"
#include "IO/offsets.h"
#include <cstdint>

namespace HedgeLib::RFL
{
	struct DLCSlot
	{
		std::uint8_t StartIndex;
		std::uint8_t Length;
	};

	struct DLCParameter
	{
		HedgeLib::IO::BINA::DBINAV2DataNode Header;
		DLCSlot GadgetDatabase;
		DLCSlot CustomizeBody;
		DLCSlot CustomizeHead;
		DLCSlot CustomizeGlass;
		DLCSlot CustomizeFace;
		DLCSlot CustomizeGlove;
		DLCSlot CustomizeShoes;
		DLCSlot CustomizePattern;
		DLCSlot EventMovie;
		DLCSlot GameScore;
		DLCSlot StageScore;

		HedgeLib::IO::BINA::BINAString64 MiscPac;
		HedgeLib::IO::BINA::BINAString64 StgMissionLua;
	};
}
#endif