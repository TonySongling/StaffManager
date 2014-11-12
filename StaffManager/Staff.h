#pragma once
class Staff
{
public:
	Staff(void);
	~Staff(void);
private:
	int staff_id;
	CString name;
	CString no;
	CString sex;
	CString duty;
	CString tel;	
public:
	
	int getStaffId(void);
	CString getName(void);
	CString getNo(void);
	CString getSex(void);
	CString getDuty(void);
	CString getTel(void);

	void setStaffId(int staff_id);
	void setName(CString name);
	void setNo(CString no);
	void setSex(CString sex);
	void setDuty(CString duty);
	void setTel(CString tel);

};

