#pragma once
#include "SECS.h"

#include "StandardAssets.h"
#include  <filesystem>

void SECS::Scene::Save(SECS& secs, std::filesystem::path savepath)
{
	std::ofstream savefile;
	savefile.open(savepath);
	savefile.clear();

	StringBuffer sb;
	SHROOM_JSON_WRITER writer(sb);

	//------------------------------------------------------------
	//ENTITIES
	//------------------------------------------------------------
	writer.StartArray();
	for (size_t i = 0; i < secs.entities.size(); i++)
	{
		//MessageBox(nullptr, "terve", "Loading parse error", MB_OK);

		writer.StartObject();

		writer.Key("Name");
		writer.String(secs.entities[i].get()->entityName.c_str());

		//COMPONENTS
		writer.Key("Components");
		writer.StartArray();

		//Transform, special case
		writer.StartObject();
		secs.entities[i].get()->GetComponent<Transform>()->Serialize(writer);
		writer.EndObject();

		writer.StartObject();
		for (size_t j = 0; j < secs.entities[i].get()->componentNames.size(); j++)
		{
			//secs.entities[i].get()->components[j]->Serialize(writer);
			auto componentName = secs.entities[i].get()->componentNames[j];

			if ("MeshRenderer" == componentName)
			{
				secs.entities[i].get()->GetComponent<MeshRenderer>()->Serialize(writer);
				continue;
			}

			if ("PointLight" == componentName)
			{
				secs.entities[i].get()->GetComponent<PointLight>()->Serialize(writer);
				continue;
			}
			//secs.entities[i].get()->components[j]->Serialize(writer);
		}
		writer.EndObject();

		writer.EndArray();
		writer.EndObject();
	}
	writer.EndArray();
	//------------------------------------------------------------

	savefile << sb.GetString() << std::endl;
	savefile.close();
}

void SECS::Scene::Load(SECS& secs, std::filesystem::path savepath)
{
	std::ifstream savefile;
	savefile.open(savepath);

	//try
	{
		IStreamWrapper isw(savefile);

		Document document;
		document.ParseStream(isw);

		if (document.HasParseError())
		{
			std::stringstream error;
			error << document.GetParseError() << " & offset: " << document.GetErrorOffset();

			MessageBox(nullptr, error.str().c_str(), "Loading parse error", MB_OK);
		}

		//------------------------------------------------------------
		//ENTITIES
		//------------------------------------------------------------
		for (size_t i = 0; i < document.Size(); i++)
		{
			//std::stringstream test;
			//test << document.Size();

			//MessageBox(nullptr, test.str().c_str(), "hou", MB_OK);
			auto& entity = SECS::Entity::Create(std::string(document[i]["Name"].GetString()), secs);

			entity.get()->GetComponent<Transform>()->Deserialize
			(
				document[i]["Components"][0]
			);

			for (size_t j = 0; j < document[i]["Components"].Size(); j++)
			{
				auto& locator = document[i]["Components"][j];
				auto  componentName = std::string(locator["Name"].GetString());
		
				std::stringstream ss;
				ss << "MeshRenderer" << " == " << componentName;

				//MessageBox(nullptr, componentName, "Loading parse error", MB_OK);
				
				if ("MeshRenderer" == componentName)
				{
					entity.get()->AssignComponent<MeshRenderer>()->Deserialize(locator);
					continue;
				}

				if ("PointLight" == componentName)
				{
					entity.get()->AssignComponent<PointLight>(secs.gfx)->Deserialize(locator);
					continue;
				}
			}
		}
	}
	/*catch (std::exception& exc)
	{
		MessageBox(nullptr, "Trying to load corrupted save file", "Loading parse error", MB_OK);
	}*/

	savefile.close();
}