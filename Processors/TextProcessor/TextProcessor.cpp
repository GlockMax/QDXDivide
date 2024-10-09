// TextProcessor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "targetver.h"
#include "afx.h"
#include <vector>

int main(int argc, char* argv[])
{
	if (argc <= 1)
		return -1;
	// read input file
	CString path = argv[0];
	auto ind = path.ReverseFind('\\');
	if (ind >= 0)
		path = path.Left(ind + 1);
	CStdioFile f;
	if (!f.Open(path + argv[1], CFile::typeText | CFile::modeRead))
		return -2;
	char buf[256];
	std::vector<CString> text;
	while (f.ReadString(buf, 256))
	{
		for (int i = 0; buf[i] == '\x00'; ++i)
			buf[i] = ' ';
		CString str(buf);
		str.Remove('\n');
		str.Remove('\x0D');
		if (str.IsEmpty())
			continue;
		text.push_back(str);
	}
	f.Close();

	// process file content
	for (int i = 0; i < text.size(); ++i)
		text[i].MakeUpper();

	// write output file
	CStdioFile fo(path + argv[1], CFile::typeText | CFile::modeWrite | CFile::modeCreate);
	for (int i = 0; i < text.size(); ++i)
		fo.WriteString(text[i] + "\n");
	fo.Close();

}
