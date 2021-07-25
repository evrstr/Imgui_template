#pragma once

#include <windows.h>
#include <string>

//�����ຯ��
//��������ת��

//UTF-8��GB2312��ת��
char* U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

//GB2312��UTF-8��ת��
char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

/*
string ת��Ϊ wstring
*/
std::wstring c2w(const char* pc)
{
	std::wstring val = L"";

	if (nullptr == pc)
	{
		return val;
	}
	size_t i;
	size_t size_of_ch = strlen(pc);
	size_t size_of_wc = size_of_ch + 1;
	wchar_t* pw = nullptr;
	pw = new wchar_t[size_of_wc];
	errno_t flag = mbstowcs_s(&i, pw, size_of_wc, pc, size_of_wc);
	//cout << "the lenght of converted:" << i << endl;
	if (flag != 0)
	{
		//cout << "convert failed." << endl;
		return val;
	}
	else {
		val = pw;
		delete[] pw;
		return val;
	}
}

/*
wstring ת��Ϊ string
*/
std::string w2c(const wchar_t* pw)
{
	std::string val = "";
	if (!pw)
	{
		return val;
	}
	size_t convert;
	size_t size_of_ch = wcslen(pw) + 1;
	char* pc = nullptr;
	if (!(pc = (char*)malloc(size_of_ch)))
	{
		return val;
	}
	errno_t flag = wcstombs_s(&convert, pc, size_of_ch, pw, size_of_ch);
	/*ת���ɹ�ʱ������ֵΪ0��ʧ�ܣ���������ֵ  errno_t��������*/
	if (flag != 0)
	{
		return val;
	}
	else {
		val = pc;
		free(pc);
		return val;
	}
}
