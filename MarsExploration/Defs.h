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
#endif // !1
