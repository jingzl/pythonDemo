#pragma once
#include "stCommon.h"


ZDWORD CPGetTickCount();

ZVOID CPGetLocalTime(ZSYSTEMTIME* lpSystemTime);

ZBOOL GetTextSize(ZINT32 iFontSize, ZINT32 num, ZSIZE* lpsz);

ZFLOAT64 GetPixelPerMeter();
