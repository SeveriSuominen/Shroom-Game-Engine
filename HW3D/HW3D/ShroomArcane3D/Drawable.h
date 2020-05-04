#pragma once
#include "ShroomArcaneGraphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableStatic;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	//virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(ShroomArcaneGraphics& gfx) const noexcept;
	
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf) noexcept;
	virtual ~Drawable() = default;
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};