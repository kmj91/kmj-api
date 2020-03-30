#pragma once
class Event
{
public:
	Event();
	virtual ~Event();

	virtual void KeyProcess() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;

public:
	WORD m_type;
};

