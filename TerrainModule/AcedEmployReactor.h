#pragma once
#include <aced.h>
class AcedEmployReactor :public AcEditorReactor
{
	virtual void commandWillStart(const ACHAR* cmdStr);
	virtual void commandEnded(const ACHAR* cmdStr);
	virtual void viewChanged();

};