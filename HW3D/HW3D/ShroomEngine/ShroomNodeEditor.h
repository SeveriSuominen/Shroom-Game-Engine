#pragma once
//NODE EDITOR
//NODES
#include "ShroomNodes/NodeEditor/imnodes.h"
#include "ShroomNode.h"

class ShroomNodeEditor
{
private:

public:
	void Initialize()
	{
		imnodes::Initialize();
	};

	void Destroy()
	{
		imnodes::Shutdown();
	};
};
