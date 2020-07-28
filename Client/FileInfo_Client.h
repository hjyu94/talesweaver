#pragma once
class CFileInfo_Client
{
	DECLARE_SINGLETON(CFileInfo_Client);

private:
	CFileInfo_Client();
	~CFileInfo_Client();

public:
	int	Get_TxtFileLine(wstring* arrFile, int iCount);
};

