#pragma once
//type Date
//(not real date-time type for simplicity, assume that the start point of time is 0)
typedef long Date;
typedef long TimeStep;
inline Date mkDate(TimeStep time_step){ return time_step; }
static const Date time0 = mkDate(0);
