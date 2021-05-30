#pragma once

#ifndef Defs_h
#define Defs_h


enum Mission_Type
{
	Polar,
	Emergency,
	Mission_Type_CNT
};

enum Mission_Status
{
	Waiting,
	in_Execution,
	Completed,
	Mission_status_CNT
};

enum Event_Type
{
	Formulation
};

enum SystemMode
{
	Interactive_mode = 1,
	Silent_mode,
	Step_by_Step
};
#endif // !1
