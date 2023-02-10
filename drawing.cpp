#include "drawing.h"
#include "Interfaces.h"

CDrawingWrapper::CDrawingWrapper(ImDrawList* pDrawList)
{
	this->DrawList = pDrawList;
}

void CDrawingWrapper::DrawLine(float x1, float y1, float x2, float y2, ImColor col, float thickness)
{
	DrawLine({ x1, y1 }, { x2, y2 }, col, thickness);
}

void CDrawingWrapper::DrawLine(const ImVec2& start, const ImVec2& end, ImColor col, float thickness)
{
	DrawList->AddLine(start, end, col, thickness);
}

void CDrawingWrapper::DrawWorldLine(const Vector& start, const Vector& end, ImColor col, float thickness)
{
	Vector screenStart;
	Interface->DebugOverlay->ScreenPosition(start, screenStart);

	Vector screenEnd;
	Interface->DebugOverlay->ScreenPosition(start, screenEnd);

	DrawLine(screenStart, screenEnd, col, thickness);
}