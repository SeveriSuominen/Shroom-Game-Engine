#pragma once

#include "Drawable.h"
#include "IndexBuffer.h"
//Static version of "Drawable" to allow sharing bind between Drawable instances

template<class T>
class DrawableStatic : public Drawable
{
public:
	bool IsStaticInitialized() const noexcept
	{
		return !staticBinds.empty();
	}
	void AddStaticBind(std::unique_ptr<Bindable> bind) noexcept
	{
		assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept
	{
		assert(pIndexBuffer == nullptr);
		pIndexBuffer = ibuf.get();
		staticBinds.push_back(std::move(ibuf));
	}

	//Use this method to set index buffer for drawable when StaticInitialized
	void SetIndexBufferFromStaticBinds() noexcept
	{
		for (const auto& sb : staticBinds)
		{
			if (const auto p = dynamic_cast<IndexBuffer*>(sb.get()))
			{
				pIndexBuffer = p;
				return;
			}
		}
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

//ALL STATIC BINDS
template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableStatic<T>::staticBinds;
