#pragma once
#include "ImGui/imgui_internal.h"
#include "vector.h"

enum TextAlign
{
	TEXT_ALIGN_LEFT,
	TEXT_ALIGN_TOP,
	TEXT_ALIGN_RIGHT,
	TEXT_ALIGN_BOTTOM,
	TEXT_ALIGN_CENTER,
};

class CDrawingWrapper
{
public:
	ImDrawList* DrawList;

	CDrawingWrapper(ImDrawList* pDrawList);

	void DrawLine(float x1, float y1, float x2, float y2, ImColor color, float thickness = 1.f);
	void DrawLine(const ImVec2& start, const ImVec2& end, ImColor col, float thickness = 1.f);
	void DrawWorldLine(const Vector& start, const Vector& end, ImColor col, float thickness = 1.f);
	void DrawTextA(const char* text, const ImVec2& pos, ImColor col, TextAlign alignx = TEXT_ALIGN_LEFT, TextAlign aligny = TEXT_ALIGN_TOP, float* outx = nullptr, float* outy = nullptr);
	void DrawTextScreenA(const char* text, const Vector& pos, ImColor col, TextAlign alignx = TEXT_ALIGN_LEFT, TextAlign aligny = TEXT_ALIGN_TOP, float* outx = nullptr, float* outy = nullptr);
};