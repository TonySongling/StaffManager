#include "StdAfx.h"
#include "Face.h"


Face::Face(void)
	: staff_no(_T(""))
	, face_path(_T(""))
	, face_id(0)
{
}


Face::~Face(void)
{
}


void Face::SetStaffNo(CString staff_no)
{
	this->staff_no = staff_no;
}


void Face::SetFacePath(CString face_path)
{
	this->face_path = face_path;
}


CString Face::GetStaffNo()
{
	return this->staff_no;
}


CString Face::GetFacePath()
{
	return this->face_path;
}


void Face::SetFaceId(int face_id)
{
	this->face_id = face_id;
}


int Face::GetFaceId()
{
	return this->face_id;
}