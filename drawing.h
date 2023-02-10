#pragma once
#include "ImGui/imgui_internal.h"
#include "vector.h"

class CDrawingWrapper
{
public:
	ImDrawList* DrawList;

	CDrawingWrapper(ImDrawList* pDrawList);

	void DrawLine(float x1, float y1, float x2, float y2, ImColor color, float thickness = 1.f);
	void DrawLine(const ImVec2& start, const ImVec2& end, ImColor col, float thickness = 1.f);
	void DrawWorldLine(const Vector& start, const Vector& end, ImColor col, float thickness = 1.f);
};