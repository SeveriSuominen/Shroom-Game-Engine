#pragma once


class Shader 
{
	virtual ID3DBlob* GetBytecode() const noexcept = 0;
protected: 
	
};