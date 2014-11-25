#include "StdAfx.h"
#include "Staff.h"


Staff::Staff(void)
	: staff_id(0)
	, name(_T(""))
	, no(_T(""))
	, sex(_T(""))
	, duty(_T(""))
	, tel(_T(""))
{
}


Staff::~Staff(void)
{
}


int Staff::getStaffId(void)
{
	return this->staff_id;
}

CString Staff::getName(void)
{
	return this->name;
}


CString Staff::getNo(void)
{
	return this->no;
}


CString Staff::getSex(void)
{
	return this->sex;
}


CString Staff::getDuty(void)
{
	return this->duty;
}


CString Staff::getTel(void)
{
	return this->tel;
}

int Staff::getFeatureFlag(void)
{
	return this->feature_flag;
}


void Staff::setStaffId(int staff_id)
{
	this->staff_id = staff_id;
}


void Staff::setName(CString name)
{
	this->name = name;
}


void Staff::setNo(CString no)
{
	this->no = no;
}

void Staff::setSex(CString sex)
{
	this->sex = sex;
}

void Staff::setDuty(CString duty)
{
	this->duty = duty;
}

void Staff::setTel(CString tel)
{
	this->tel = tel;
}

void Staff::setFeatureFlag(int feature_flag)
{
	this->feature_flag = feature_flag;
}

