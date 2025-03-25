#include "generation.h"

FS::Genome::Generation::Generation()
{
	FluidumScene_Log_Constructor(::FS::Genome::Generation);
}

FS::Genome::Generation::~Generation() {
	FluidumScene_Log_Destructor(::FS::Genome::Generation);
}

void FS::Genome::Generation::call() {
	//ImGui::ShowDemoWindow();
	this->table();
}

void FS::Genome::Generation::table() {

	struct Item {
		Size ID;
		float val;
	};

	static std::vector<Item> items(100);
	static bool a = false;

	if (!a) {
		a = true;
		for (Size i = 0; i < 100; i++) {
			items[99-i] = Item{ i + 453, (float)i + 1262 + (i + 1.0f / 20.0f) * 3.24f / 2.9f + 1 - i * 3.245f };
		}
	}


	static const char* template_items_names[] =
	{
		"Banana", "Apple", "Cherry", "Watermelon", "Grapefruit", "Strawberry", "Mango",
		"Kiwi", "Orange", "Pineapple", "Blueberry", "Plum", "Coconut", "Pear", "Apricot"
	};

	static ImGuiTableFlags flags =
		ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti
		| ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_NoBordersInBody
		| ImGuiTableFlags_ScrollY;

	if (ImGui::BeginTable("table_sorting", 4, flags, ImVec2(0.0f, ImGui::GetFontSize() * 15), 0.0f))
	{

		ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_DefaultSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
		ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 2);
		ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_PreferSortDescending | ImGuiTableColumnFlags_WidthStretch, 0.0f, 3);
		ImGui::TableSetupScrollFreeze(0, 1); // Make row always visible
		ImGui::TableHeadersRow();

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin(items.size());
		while (clipper.Step())
			for (int row_n = clipper.DisplayStart; row_n < clipper.DisplayEnd; row_n++)
			{
				// Display a data item
				ImGui::PushID(items[row_n].ID);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%04d", items[row_n].ID);
				ImGui::TableNextColumn();
				ImGui::TextUnformatted("aa");
				ImGui::TableNextColumn();
				ImGui::SmallButton("None");
				ImGui::TableNextColumn();
				ImGui::Text("%.4f", items[row_n].val);
				ImGui::PopID();
			}
		ImGui::EndTable();
	}

}
