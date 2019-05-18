#pragma once

#include <new>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include <strsafe.h>
#include <assert.h>

#include <ks.h>
#include <ksmedia.h>
#include <Dbt.h>

extern int flag ;
class Process
{
	 RGBQUAD * pframe;
	UINT width;
	UINT height;
public:
	Process(RGBQUAD* p, UINT w, UINT h) :pframe(p), width(w), height(h) { ; }
	Process() { ; }
	~Process() { ; }
	void Nagation();
	void Black_and_white();
	void Save();
};
