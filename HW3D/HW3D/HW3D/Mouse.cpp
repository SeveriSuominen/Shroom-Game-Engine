#include "Shroom.h"
#include "Mouse.h"

//--------------------------------------------------------------

//CONTROL INTERFACE

//--------------------------------------------------------------
std::pair<int, int> Mouse::GetPos() const noexcept
{
	return {x , y};
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return {};
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Mouse::Event>();
}

void Mouse::EnableRaw() noexcept
{
	rawEnabled = true;
}

void Mouse::DisableRaw() noexcept
{
	rawEnabled = false;
}

bool Mouse::RawEnabled() const noexcept
{
	return rawEnabled;
}
//--------------------------------------------------------------

//ON WINDOW EVENTS

//--------------------------------------------------------------

//--------------------------------------------------------------
//MOUSE POSITION
//--------------------------------------------------------------
void Mouse::OnMouseMove(int xi, int yi) noexcept
{
	//Map current pos
	x = xi;
	y = yi;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}
//--------------------------------------------------------------
//MOUSE PRESS
//--------------------------------------------------------------

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::LeftPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::LeftRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::RightPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::RightRelease, *this));
	TrimBuffer();
}
//--------------------------------------------------------------
//WHEEL
//--------------------------------------------------------------
void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept 
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >=   WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= - WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
//--------------------------------------------------------------
//TRIMS
//--------------------------------------------------------------
void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
//--------------------------------------------------------------