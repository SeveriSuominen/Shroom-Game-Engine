#pragma once

#include <queue>
#include <bitset>

class Keyboard 
{
	//Exposing private parts to Window ;)
	friend class Window;
public: 
	class Event 
	{

	public:
		enum class Type
		{
			Press, Release, Undefined
		};

		//Event costructors
		Event() noexcept : type(Type::Undefined), code(0u) {}
		Event(Type type, unsigned char code) noexcept : type(type), code(code) {}

		bool IsPress()   const noexcept { return type == Type::Press;	  }
		bool IsRelease() const noexcept { return type == Type::Release;   }
		bool IsValid()   const noexcept { return type != Type::Undefined; }
		
		unsigned char GetCode() const noexcept 
		{
			return code;
		}

	private:
		Type type;
		unsigned char code;
	
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	// E KEYS
	bool  KeyIsPressed(unsigned char keycode) const noexcept;
	bool  KeyIsEmpty  () const noexcept;
	Event ReadKey();
	void  FlushKey    () noexcept;
	
	// E CHARS
	char  ReadChar	  () noexcept;
	bool  CharIsEmpty () const noexcept;
	void  FlushChar   () noexcept;
	void  Flush		  () noexcept;
	
	// Autorepeat control
	void EnableAutorepeat    () noexcept;
	void DisableAutorepeat   () noexcept;
	bool AutorepeatIsEnabled () noexcept;
private:
	//E 
	void OnKeyPressed  (unsigned char keycode) noexcept;
	void OnKeyReleased (unsigned char keycode) noexcept;
	void OnChar		   (char character)		   noexcept;
	
	void ClearState() noexcept;
	
	template<typename T> static void TrimBuffer(std::queue<T>& buffer) noexcept;
	
	static constexpr unsigned int nKeys      = 256u;
	static constexpr unsigned int bufferSize = 16u;
	
	bool autorepeat = false;

	std::bitset<nKeys> keystates;
	std::queue <Event> keybuffer;
	std::queue <char>  charbuffer;
};


