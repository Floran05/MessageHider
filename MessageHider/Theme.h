#pragma once
#include <wtypes.h>
class Theme
{
public:
	HBRUSH BackgroundColor;
	HBRUSH ButtonColor;
	HBRUSH TextColor;
	HBRUSH MidGroundColor;

	Theme() {
		BackgroundColor = CreateSolidBrush(RGB(255, 255, 255));
		ButtonColor = CreateSolidBrush(RGB(255, 255, 255));
		TextColor = CreateSolidBrush(RGB(255, 255, 255));
		MidGroundColor = CreateSolidBrush(RGB(255, 255, 255));
	};
};

