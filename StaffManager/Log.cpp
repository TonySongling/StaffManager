#include "StdAfx.h"
#include "Log.h"


Log::Log(void)
	: staff_no(_T(""))
	, staff_name(_T(""))
	, result(_T(""))
	, time(_T(""))
{
}


Log::~Log(void)
{
}


CString Log::GetStaffNo(void)
{
	return this->staff_no;
}


CString Log::GetStaffName(void)
{
	return this->staff_name;
}


CString Log::GetResult(void)
{
	return this->result;
}


CString Log::GetTime(void)
{
	return this->time;
}

void Log::SetStaffNo(CString staff_no)
{
	this->staff_no = staff_no;
}

void Log::SetStaffName(CString staff_name)
{
	this->staff_name = staff_name;
}

void Log::SetResult(CString result)
{
	this->result = result;
}

void Log::SetTime(CString time)
{
	this->time = time;
}
