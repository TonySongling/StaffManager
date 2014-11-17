#pragma once
class Face
{
public:
	Face(void);
	~Face(void);
	// 工号
	CString staff_no;
	// 脸部特征资源文件路径
	CString face_path;
	int face_id;
	void SetStaffNo(CString staff_no);
	void SetFacePath(CString face_path);
	void SetFaceId(int face_id);

	CString GetStaffNo();
	CString GetFacePath();
	int GetFaceId();
};

