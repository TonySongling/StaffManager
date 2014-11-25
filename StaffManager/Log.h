#pragma once
class Log
{
public:
	Log(void);
	~Log(void);
private:
	CString staff_no;
	CString staff_name;
	CString result;
	CString time;
public:
	CString GetStaffNo(void);
	CString GetStaffName(void);
	CString GetResult(void);
	CString GetTime(void);

	void SetStaffNo(CString staff_no);
	void SetStaffName(CString staff_name);
	void SetResult(CString result);
	void SetTime(CString time);
};

