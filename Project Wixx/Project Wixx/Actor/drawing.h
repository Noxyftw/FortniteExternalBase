#pragma once

class drawing_v
{
public:
	void Draw_Text(const std::string& text, const ImVec2& pos, float size, uint32_t color, bool center)
	{

		constexpr float fStrokeVal1 = 1.0f;
		uint32_t EdgeColor = 0xFF000000;

		float Edge_a = (EdgeColor >> 24) & 0xff;
		float Edge_r = (EdgeColor >> 16) & 0xff;
		float Edge_g = (EdgeColor >> 8) & 0xff;
		float Edge_b = (EdgeColor) & 0xff;
		std::stringstream steam(text);
		std::string line;
		float y = 0.0f;
		int i = 0;
		while (std::getline(steam, line))
		{
			ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
			if (center)
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x - textSize.x / 2.0f) - fStrokeVal1, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x - textSize.x / 2.0f) + fStrokeVal1, pos.y + textSize.y * i), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * i) - fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x - textSize.x / 2.0f), (pos.y + textSize.y * i) + fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), color, line.c_str());
			}
			else
			{
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x) - fStrokeVal1, (pos.y + textSize.y * i)), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x) + fStrokeVal1, (pos.y + textSize.y * i)), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x), (pos.y + textSize.y * i) - fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2((pos.x), (pos.y + textSize.y * i) + fStrokeVal1), ImGui::GetColorU32(ImVec4(Edge_r / 255, Edge_g / 255, Edge_b / 255, Edge_a / 255)), line.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x, pos.y + textSize.y * i), color, line.c_str());
			}
			y = pos.y + textSize.y * (i + 1);
			i++;
		}
	}

}; drawing_v drawing;