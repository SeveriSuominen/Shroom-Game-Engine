#pragma once

#include <map>
#include <boost\filesystem.hpp>


class ShroomAssetResolver
{
public:
	static void ResolveAssetEditor(boost::filesystem::path path, std::string& type)
	{
		std::map<std::string, unsigned int> asset_type_map
		{
			{".shroom_mat", 0}
		};

		std::map<std::string, unsigned int>::iterator it = asset_type_map.find(type);
		unsigned int type_id;
		bool found = false;

		if (it != asset_type_map.end())
		{
			type_id = it->second;
			found = true;
		}

		std::stringstream ss;
		ss << type_id;

		if (found)
		{
			if (type_id == asset_type_map[".shroom_mat"])
			{
				MaterialEditor(path);
				return;
			}
		}
	}

private:
	static void MaterialEditor(boost::filesystem::path path)
	{
		//static ImNodes::CanvasState canvas;

		if (ImGui::Begin("Shroom Material"))
		{
			/*ImNodes::BeginCanvas(&canvas);

			struct Node
			{
				ImVec2 pos{};
				bool selected{};
				ImNodes::Ez::SlotInfo inputs[2];
				ImNodes::Ez::SlotInfo outputs[1];
			};

			static Node nodes[3] = {
				{{50, 100}, false,  {{"In", 1},  {"yolonki", true}}, {{"Out", 1}}},
				{{250, 50}, false,  {{"In", 1}}, {{"Out", 1}}},
				{{250, 100}, false, {{"In", 1}}, {{"Out", 1}}},
			};

			for (Node& node : nodes)
			{
				if (ImNodes::Ez::BeginNode(&node, "Node Title", &node.pos, &node.selected))
				{
					ImNodes::Ez::InputSlots(node.inputs, 1);
					ImNodes::Ez::OutputSlots(node.outputs, 1);
					ImNodes::Ez::EndNode();
				}
			}

			ImNodes::Connection(&nodes[1], "In", &nodes[0], "Out");
			ImNodes::Connection(&nodes[2], "In", &nodes[0], "Out");

			ImNodes::EndCanvas();
			*/
			ImGui::End();
		}
	}
};