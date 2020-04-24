#include "Keyboard.h"

//--------------------------------------------------------------

//CONTROL INTERFACE

//--------------------------------------------------------------
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode] == true;
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

Keyboard::Event Keyboard::ReadKey()
{
	if (keybuffer.size() > 0u)
	{
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else 
	{
		return Keyboard::Event();
	}
}

void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u)
	{
		unsigned char charc = charbuffer.front();
		charbuffer.pop();
		return charc;
	}
	else
	{
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey ();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeat = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeat = false;
}

bool Keyboard::AutorepeatIsEnabled() noexcept
{
	return autorepeat;
}
//--------------------------------------------------------------

//ON WINDOW EVENTS

//--------------------------------------------------------------
void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;
	keybuffer.push( Keyboard::Event( Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}
//--------------------------------------------------------------

//CLEAR
void Keyboard::ClearState() noexcept
{
	keystates.reset();
}

//TRIMS
template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept 
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
