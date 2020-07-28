#pragma once

#include "Include.h"

class CFileInfo
{
public:
	std::wstring	ConvertRelativePath(wstring szFullPath);

	void			DirInfoExtraction(const wstring& wstrPath,
		list<IMGPATH*>& rPathList);

	void			DirInfoExtraction(const wstring& wstrPath,
		list<SINGLE_IMGPATH*>& rPathList);

	int				DirFileCount(const wstring& wstrPath);


public:
	CFileInfo();
	~CFileInfo();
};

