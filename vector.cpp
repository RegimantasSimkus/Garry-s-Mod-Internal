#include "vector.h"
#include "ImGui/imgui_internal.h"

Vector::operator ImVec2()
{
	return ImVec2(x, y);
}