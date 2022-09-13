#include "pch.h"
#include "D2DConsole.h"

using namespace V6;

D2DConsole::D2DConsole()
{

}

void D2DConsole::CreateControl(D2DWindow* parent, D2DControls* pacontrol, const FRectF& rc, DWORD stat, LPCWSTR name, int local_id)
{
	D2DTextbox::CreateControl(parent, pacontrol, MULTILINE, rc, stat, name, local_id );

	this->back_ = ColorF::Black;
	this->fore_ = ColorF::White;
}