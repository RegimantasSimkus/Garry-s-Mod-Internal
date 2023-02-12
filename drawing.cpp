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

void CDrawingWrapper::DrawTextA(const char* text, const ImVec2& pos, ImColor col, TextAlign alignx, TextAlign aligny)
{
	const char* textend = text + strlen(text);

	ImVec2 size = ImGui::CalcTextSize(text, textend, false);
	ImVec2 adjustedPos = pos;

	if (alignx == TEXT_ALIGN_RIGHT)
		adjustedPos.x -= size.x;
	else if (alignx == TEXT_ALIGN_CENTER)
		adjustedPos.x -= size.x / 2.f;
	if (aligny == TEXT_ALIGN_BOTTOM)
		adjustedPos.y -= size.y;
	else if (aligny == TEXT_ALIGN_CENTER)
		adjustedPos.y -= size.y / 2.f;

	DrawList->AddText(adjustedPos, col, text, textend);
}

void CDrawingWrapper::DrawTextScreenA(const char* text, const Vector& pos, ImColor col, TextAlign alignx, TextAlign aligny)
{
	Vector screenpos;
	Interface->DebugOverlay->ScreenPosition(pos, screenpos);
	DrawTextA(text, screenpos, col, alignx, aligny);
}