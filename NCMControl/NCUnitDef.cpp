#include "StdAfx.h"
#include "NCUnitDef.h"
#include <list>
#include <tchar.h>
#include <algorithm>

CNCUnitDef::CNCUnitDef(void)
{
}

CNCUnitDef::~CNCUnitDef(void)
{
}

void CNCUnitDef::Clear()
{
	while(!sectionList.empty())
	{
		sectionList.pop_back();
	}//while
}//void CMCUnitDef::Clear()

void CNCUnitDef::Reset()
{
	class local
	{
	public:
		static bool notKnown(Definition d)
		{
			if(d.GetType() != D_COMMON)
				return true;

			if(!d.def.known)
				return true;
			else
				return false;
		};//static bool notKnown(Definition d)
	};//class local

	for(unsigned int i = 0; i < sectionList.size(); i++)
	{
		for(unsigned int j = 0; j < sectionList[i].defList.size(); j++)
		{
			switch (sectionList[i].defList[j].GetType())
			{
			case D_COMMON:
				sectionList[i].defList[j].def.param = sectionList[i].defList[j].def.defParam;
				sectionList[i].defList[j].def.presence = false;
				break;
			case D_WREPL:
				sectionList[i].defList.erase(sectionList[i].defList.begin() + j);
				break;
			default:
				ASSERT(false);
			}//switch
		}//for
	}//for

	/// Удаление всех определений, которых нет в файле - прототипе
	for(unsigned int i = 0; i < sectionList.size(); i++)
	{
		vector <Definition>::iterator new_end = remove_if(sectionList[i].defList.begin(), sectionList[i].defList.end(), local::notKnown);
		sectionList[i].defList.erase(new_end, sectionList[i].defList.end());
	}//for
}//void CNCUnitDef::Reset()

bool CNCUnitDef::LoadXML(TiXmlDocument *doc)
{
	Clear();
	TiXmlNode *rootNode = doc->FirstChild("NCMUnit");

	TiXmlNode *firstSection = rootNode->FirstChild("section");
	
	/// Перебираем секции
	for(TiXmlNode* cursection = firstSection;
		cursection != 0;
		cursection = cursection->NextSibling())
	{
		Section section;
		section.name = cursection->ToElement()->Attribute("name");

		/// В секции Word Replacement нет определений в Xml файле
		//if(section.name != _T("Word Replacement"))
		/// Для Word Replacement считываем хмл элемент Description
		{

			TiXmlNode *defXmlNode = cursection->FirstChild();
			/// Перебираем определения, все определения здесь имеют конструктор данных def.
			for(TiXmlNode * curDefXmlNode = defXmlNode;
				curDefXmlNode != 0;
				curDefXmlNode = curDefXmlNode->NextSibling())
			{
				CString st = curDefXmlNode->Value();
				if(st == _T("Definition"))
				{
					Def a;
					Definition definition(a);
					Param param;

					/// Считываем аттрибуты "type" и "defaultvalue" в локальную переменную param
					CString typeStr; typeStr = curDefXmlNode->ToElement()->Attribute("type");
					/// По умолчанию определения имеют строковый тип
					if(typeStr == _T(""))
					{
						param.SetType(P_S);
						CString value;
						value = curDefXmlNode->ToElement()->Attribute("defaultvalue");
						if(value != _T(""))
						{
							param.s = value;
						}//if
						else
						{
							param.s = "";
						}//else
					}//if
					else if (typeStr == _T("bool"))
					{
						param.SetType(P_B);
						CString value;
						value = curDefXmlNode->ToElement()->Attribute("defaultvalue");
						if(value != _T(""))
						{
							if(value == _T("Yes") || value == _T("YES") || value == _T("yes"))
							{
								param.b = true;
							}//if
							else if (value == _T("no") || value == _T("No") || value == _T("NO"))
							{
								param.b = false;
							}//if
							else
							{
								/// Логическое значение не удалось распознать, скорее всего ошибка
								CString str; str.LoadString(IDS_ERROR_UNKNOWN_TYPE);
								AfxMessageBox(str+typeStr);
								return false;
							}//else
						}
					}//if
					else if (typeStr == _T("string"))
					{
						param.SetType(P_S);
						CString value;
						value = curDefXmlNode->ToElement()->Attribute("defaultvalue");
						if(value != _T(""))
						{
							param.s = value;
						}//if
						else
						{
							param.s = "";
						}//else
					}//if
					else if (typeStr == _T("int"))
					{
						param.SetType(P_I);
						int i;
						if(curDefXmlNode->ToElement()->Attribute("defaultvalue",&i) != NULL)
						{
							param.i = i;
						}//if
						else
						{
							param.i = 0;
						}//else
					}//if
					else if (typeStr == _T("double"))
					{
						param.SetType(P_D);
						double d;
						if(curDefXmlNode->ToElement()->Attribute("defaultvalue",&d) != NULL)
						{
							param.d = d;
						}//if
						else
						{
							param.d = 0;
						}//else
					}//if
					else if (typeStr == _T("eenum"))
					{
						param.SetType(P_EE);
						CString value;
						value = curDefXmlNode->ToElement()->Attribute("defaultvalue");
						if(value != _T(""))
						{
							param.s = value;
						}//if
						else
						{
							param.s = "";
						}//else
					}//if
					else if (typeStr == _T("senum"))
					{
						param.SetType(P_SE);
						CString value;
						value = curDefXmlNode->ToElement()->Attribute("defaultvalue");
						if(value != _T(""))
						{
							param.s = value;
						}//if
						else
						{
							param.s = "";
						}//else
					}//if
					else
					{
						/// Неизвестный тип, скорее всего ошибка
						CString str; str.LoadString(IDS_ERROR_UNKNOWN_TYPE);
						AfxMessageBox(str+typeStr);
						return false;
					}//else

					/// Считываем параметр Name
					TiXmlElement *nameXmlEl = curDefXmlNode->FirstChildElement("Name");
					
					if(nameXmlEl != NULL)
					{
						definition.def.defWord = nameXmlEl->GetText();
					}//if
					else
					{
						/// Name - обязательный параметр
						CString str; str.LoadString(IDS_ERROR_XML_NAME_ELEMENT);
						AfxMessageBox(str);
						return false;
					}//else
					/// Параметр briefDesctiption
					TiXmlElement *briefDescrXmlEl = curDefXmlNode->FirstChildElement("briefDescription");
					if(briefDescrXmlEl != NULL)
						definition.def.briefDescr = briefDescrXmlEl->GetText();
					/// Параметр description
					TiXmlElement *fullDescrXmlEl = curDefXmlNode->FirstChildElement("FullDescription");
					if(fullDescrXmlEl != NULL)
					{
						TiXmlElement *pictXmlEl = fullDescrXmlEl->FirstChildElement("Picture");
						if(pictXmlEl != NULL)
							definition.def.fullDescr.picName = pictXmlEl->GetText();

						TiXmlElement *textXmlEl = fullDescrXmlEl->FirstChildElement("Description");
						if(textXmlEl != NULL)
							definition.def.fullDescr.text = textXmlEl->GetText();
					}//if

					/// Параметр group
					TiXmlElement *groupXmlEl = curDefXmlNode->FirstChildElement("group");
					if(groupXmlEl != NULL)
						definition.def.group = groupXmlEl->GetText();

					/// Если определение имеет один из типов enum, считываем возможные значения
					vector <CString> enumerate;
					if(param.GetType() == P_EE || param.GetType() == P_SE)
					{
						TiXmlElement *enumXmlEl = curDefXmlNode->FirstChildElement("Enum");
						if(enumXmlEl == NULL)
						{
							CString str; str.LoadString(IDS_ERROR_XML_ENUM_WITHOUT_VARIANTS);
							AfxMessageBox(str);
							return false;
						}//if
						else
						{
							TiXmlElement *enumItemXmlEl = enumXmlEl->FirstChildElement("element");
							if(enumItemXmlEl == NULL)
							{
								CString str; str.LoadString(IDS_ERROR_XML_ENUM_WRONG_VARIANTS);
								AfxMessageBox(str);
								return false;
							}//if
							else
							{
								for(TiXmlElement* curEnumItemXmlEl = enumItemXmlEl;
									curEnumItemXmlEl != 0;
									curEnumItemXmlEl = curEnumItemXmlEl->NextSiblingElement())
								{
									CString s;
									//s = curEnumItemXmlEl->GetText();
									s = curEnumItemXmlEl->Attribute("val");
									enumerate.push_back(s);
								}//for
							}//else
						}//else
					}//if

					/// В стойке определение не присутствует
					definition.def.presence = false;
					definition.def.known = true;

					definition.def.defParam = param;
					definition.def.param = param;
					definition.def.enumerate = enumerate;

					section.defList.push_back(definition);
				}//if
				else
				if(st == _T("Description"))
				{
					TiXmlElement *brDescXmlEl = curDefXmlNode->FirstChildElement("BriefDescription");
					if(brDescXmlEl != NULL)
						section.briefDescr = brDescXmlEl->GetText();
					TiXmlElement *flDescXmlEl = curDefXmlNode->FirstChildElement("FullDescription");
					if(flDescXmlEl != NULL)
					{
						TiXmlElement *picXmlEl = flDescXmlEl->FirstChildElement("Picture");
						if(picXmlEl != NULL)
							section.fullDescr.picName = picXmlEl->GetText();

						TiXmlElement *textXmlEl = flDescXmlEl->FirstChildElement("Description");
						if(textXmlEl != NULL)
							section.fullDescr.text = textXmlEl->GetText();
					}//if
				}//if
				else
				if(st == _T("Group"))
				{
					Group group;
					TiXmlElement *grNameXmlEl = curDefXmlNode->FirstChildElement("Name");
					if(grNameXmlEl  == NULL)
					{
						CString str; str.LoadString(IDS_ERROR_XML_GROUP_NAME);
						AfxMessageBox(str);
					}//if
						
					group.name = grNameXmlEl->GetText();

					TiXmlElement *grBrDescXmlEl = curDefXmlNode->FirstChildElement("briefDescription");
					if(grBrDescXmlEl != NULL)
						group.briefDescr = grBrDescXmlEl->GetText();

					TiXmlElement *grFlDescXmlEl = curDefXmlNode->FirstChildElement("FullDescription");
					if(grFlDescXmlEl != NULL)
					{
						TiXmlElement *PicXmlEl = grFlDescXmlEl->FirstChildElement("Picture");
						if(PicXmlEl != NULL)
							group.fullDescr.picName = PicXmlEl->GetText();
						TiXmlElement *textXmlEl = grFlDescXmlEl->FirstChildElement("Description");
						if(textXmlEl != NULL)
							group.fullDescr.text = textXmlEl->GetText();
					}//if
					section.groupList.push_back(group);
				}//if
				else
				{
					CString str; str.LoadString(IDS_ERROR_XML_UNKNOWN_ELEMENT);
					AfxMessageBox(str + st);
				}//else
			}//for
		}//if
		sectionList.push_back(section);
	}//for
	return true;
}//bool CNCUnitDef::LoadXML(TiXmlDocument *doc)

bool CNCUnitDef::SetString(CString setString)
{
	///Локальные функции
	class local
	{
	public:
		/// Разбиение входного файла на строки по символам 13 10
		static void breakString(list<CString> &listString,CString inString)
		{
			/// 0 если конец строки,
			if(inString.GetLength() == 0)
				return;

			for(int i = 0; i < inString.GetLength()-1; i++)
			{
				if( inString[i] == 13 && inString[i+1] == 10)
				{
					CString nextString = inString.Mid(0, i);
					listString.push_back(nextString);
					
					inString.Delete(0, i+2);
					breakString(listString,inString); 
					return;
				}//if
			}//if
			/// Больше нет символов конца строки
			listString.push_back(inString);
			return;
		};//static void breakString(list<CString> *listString,CString inString)
		/// Проверка сигнатуры начала файла (2 строки "//")
		static bool isFileBegin(CString string1, CString string2)
		{
			if(string1.GetLength() >= 2 && string2.GetLength() >= 2)
				if(string1[0] == _T('/') && string1[1] == _T('/'))
					if(string2[0] == _T('/') && string2[1] == _T('/'))
						return true;
			return false;
		};//static bool isFileBegin(CString string1, CString string2)
		/// Проверка, содержит ли строка только пробелы
		static bool isBlankString(CString str)
		{
			for( int i = 0; i < str.GetLength(); i++)
			{
				if(str[i] != ' ' && str[i] != '\t')
					return false;
			}//for
			return true;
		};//static bool isSpaceString(CString str)
		/// Удаление всех пробелов в начале строки
		static bool deleteBlank(CString &str)
		{
			while(!str.IsEmpty() && (str[0] == _T(' ') || str[0] == _T('\t')))
			{
				str.Delete(0);
			}//while
			return true;
		};//static bool deleteBlank(CString str)
		/// Проверка, является ли строка правильным заголовком секции с именем section,
		/// получение имени секции в строку
		static bool isSectionString(CString str, CString &section)
		{
			if(!str.Mid(0,2) == CString(_T("//")))
				return false;
			str.Delete(0,2);
			deleteBlank(str);

			if(!str.Mid(0,7) == CString(_T("Section")))
				return false;
			str.Delete(0,7);
			deleteBlank(str);

			section = str;
			while(section[section.GetLength()-1] == _T(' ') || section[section.GetLength()-1] == _T('\t'))
				section.Delete(section.GetLength()-1);
			//if(!str.Mid(0,section.GetLength()) == section)
				//return false;
			//str.Delete(0, section.GetLength());
			return true;
		};//static bool isSectionString(CString str, CString section)
		/// Проверка, является ли строка завершением секции
		static bool isSectionEnd(CString str)
		{
			if(!(str.Mid(0,2) == CString(_T("//"))))
				return false;
			str.Delete(0,2);
			deleteBlank(str);

			if(!(str.Mid(0,3) == CString(_T("end"))))
				return false;
			str.Delete(0,3);
			deleteBlank(str);
			
			if(!(str.Mid(0,7) == CString(_T("section"))))
				return false;
			str.Delete(0,7);
			deleteBlank(str);

			return true;
		};//static bool isSectionEnd(CString str)
		/// Считывание слова из строки, вырезание этого слова из строки
		static CString GetWord(CString &str)
		{
			deleteBlank(str);
			if(str.GetLength() == 0)
				return CString(_T(""));

			for(int i = 0; i < str.GetLength(); i++)
			{
				if(str[i] == _T(' ') || str[i] == _T('\t'))
				{
					CString str2 = str.Mid(0,i);
					str.Delete(0,i+1);
					return str2;
					break;
				}//if
			}//for
			CString str2 = str;
			str.Delete(0, str.GetLength());
			return str2;
		}//static CString GetWord(CString &str)
		/// Считывание определения из строки
		static CString WRepl(CString &str)
		{
			deleteBlank(str);
			if(str.GetLength() == 0)
				return CString(_T(""));
			if(str[0] != '"')
				return CString(_T(""));
			str.Delete(0,1);

			CString res;
			while(str.GetLength() > 0)
			{
				if(str[0] != '"')
				{
					res = res+str[0];
					str.Delete(0,1);
				}//if
				else
				{
					str.Delete(0,1);
					return res;
				}//else
			}//for

#ifdef _DEBUG
			CString str1; str1.LoadString(IDS_ERROR_PARSER_BACKSLASH_EXPECTED);
			AfxMessageBox(str1);
#endif
			return CString(_T(""));
		};//static CString GetWReplString(CString &str)
		static bool GetDef(Def &def, CString str)
		{
			CString defword = GetWord(str);
			if(defword == CString(_T("")))
				return false;
			def.defWord = defword;

			deleteBlank(str);

			if(!str.IsEmpty())
			{
				/// При считывании все параметры имеют тип string
				Param param(CString(_T("")));
				param.s = GetWord(str);
				//if(param.s != CString(_T("")))
					def.param = param;
			}//if
			else
			{
				Param param(CString(_T("")));
				param.s = _T(""); 
				def.param = param;
			}//else
			return true;
		}//static bool GetDefinition(Definition &def, CString str)
		/// Считывание word replacement из строки
		static bool GetWRepl(WRep &wrep, CString str)
		{
			CString replace = GetWord(str);
			if(replace != CString(_T("Replace")))
				return false;
			
			CString srcString = WRepl(str);
			CString dstString = WRepl(str);

			wrep.srcString = srcString;
			wrep.destString = dstString;
			
			return true;
		}//static bool GetWRepl(WRep &wrep, CString str)
	};//class local

	Reset();

	/// Разбиваем входной файл на строки
	list<CString> stringList;
	local::breakString(stringList, setString);

	/// Перебираем строки входного файла

	/// Сигнатура начала файла ("//\n//\n") 
	if(stringList.size() < 2)
	{
#ifdef _DEBUG
		CString str; str.LoadString(IDS_ERROR_PARSER_LESS_2STRINGS);
		AfxMessageBox(str);
#endif
		return false;
	}//if

	/// Начало файла
	CString str1, str2;
	str1 = stringList.front(); stringList.pop_front();
	str2 = stringList.front(); stringList.pop_front();
	if(! local::isFileBegin(str1, str2)) //   "//", "//"
	{
#ifdef _DEBUG
		CString str; str.LoadString(IDS_ERROR_PARSER_NO_BEGIN_SIGNATURE);
		AfxMessageBox(str);
#endif
		return false;
	}//if

	if(stringList.empty())
	{
#ifdef _DEBUG
		CString str; str.LoadString(IDS_ERROR_PARSER_UNEXPECTED_END);
		AfxMessageBox(str);
#endif
		return false;
	}//if

	/// Одна пустая строка
	str1 = stringList.front(); stringList.pop_front();
	if(! local::isBlankString(str1))
	{
#ifdef _DEBUG
		CString str; str.LoadString(IDS_ERROR_PARSER_BLANK_LINE_EXPECTED);
		AfxMessageBox(str);
#endif
		return false;
	}//if

	/// Обрабатываем секции
	while(1)
	{
		/// Проверяем, не закончился ли список
		if(stringList.empty())
		{
#ifdef _DEBUG
			CString str; str.LoadString(IDS_ERROR_PARSER_UNEXPECTED_END);
			AfxMessageBox(str);
#endif
			return false;
		}//if
		/// Получаем следующую строку
		str1 = stringList.front(); stringList.pop_front();
		CString sSecName;
		/// Проверяем, является ли строка заголовком секции и получаем название секции
		if(local::isSectionString(str1, sSecName))
		{
			/// Создаём секцию
			Section section;
			section.name = sSecName;
			/// Организуем цикл, перебираем входные строки
			while(1)
			{
				///Если список строк кончился - ошибка
				if(stringList.empty())
				{
#ifdef _DEBUG
					CString str; str.LoadString(IDS_ERROR_PARSER_UNEXPECTED_END);
					AfxMessageBox(str);
#endif
					return false;
				}//if

				/// Получаем следующую строку из начала списка
				str1 = stringList.front(); stringList.pop_front();

				/// Если эта строка - сигнатура конца секции, выходим из цикла
				if( local::isSectionEnd(str1))
					break;

				/// Считываем определение из строки
				Definition definition;
				if(section.name != _T("Word Replacement"))
				{
					definition.SetType(D_COMMON);

					Def def;
					if(! local::GetDef(def,str1))
					{
#ifdef _DEBUG
						CString str; str.LoadString(IDS_ERROR_PARSER_DEFINITION);
						AfxMessageBox(str);
#endif
						return false;
					}//if
					definition.def = def;
				}//if
				else
				{
					definition.SetType(D_WREPL);

					WRep wrep;
					if(! local::GetWRepl(wrep, str1))
					{
#ifdef _DEBUG
						CString str; str.LoadString(IDS_ERROR_PARSER_WREPL);
						AfxMessageBox(str);
#endif
						return false;
					}//if
					definition.wRep = wrep;
				}//else
				/// Добавляем определение в список определений секции
				section.defList.push_back(definition);
			}//while
			/// добавляем секцию в список секций
			if(!AddSection(section))
			{
#ifdef _DEBUG
				CString str; str.LoadString(IDS_ERROR_PARSER_SECTION_ADD);
				AfxMessageBox(str);
#endif
				return false;
			}//
			//sectionList.push_back(section);
		}//if
		else
		{
#ifdef _DEBUG
			CString str; str.LoadString(IDS_ERROR_PARSER_SECTION_NAME_EXPECTED);
			AfxMessageBox(str);
#endif
			return false;
		}//else
		
		/// Если больше строк нет - выходим
		if(stringList.empty())
		{
			return true;
		}//if

		/// Пропускаем все пустые строки
		str1 = stringList.front();
		while(local::isBlankString(str1))
		{
			/// Если больше строк нет - выходим
			stringList.pop_front();
			if(stringList.empty())
			{
				return true;
			}//if
			str1 = stringList.front();
		}//while
	}

	return false;
}//bool CNCUnitDef::SetString(CString setString)

CString CNCUnitDef::GetString()
{
	class local
	{
	public:
		static CString ScreenWRepl(CString wrepl)
		{
			wrepl = wrepl + _T('"');
			wrepl.Insert(0,_T('"'));
			return wrepl;
		};//static CString ScreenWRepl(CString wrepl)
		static CString sections(vector<Section> & sList)
		{
			CString res;
			for(unsigned int i = 0; i < sList.size(); i++)
			{
				res = res + _T("// Section ") + sList[i].name + _T("\r\n");

				for(unsigned int j = 0; j < sList[i].defList.size(); j++)
				{
					switch (sList[i].defList[j].GetType())
					{
					case D_COMMON:
						if(sList[i].defList[j].def.presence)
						{
							res = res + sList[i].defList[j].def.defWord;
							res = res + _T("\t\t");
							res = res + sList[i].defList[j].def.param.Show();
							if(sList[i].name == _T("Function Definition"))
							{
								res = res + _T("\t1\tN");
							}//if
							res = res + _T("\r\n");
						}//if
						break;
					case D_WREPL:
						res = res + _T("Replace");
						res = res + _T("\t") + ScreenWRepl(sList[i].defList[j].wRep.srcString);
						res = res + _T("\t") + ScreenWRepl(sList[i].defList[j].wRep.destString) + _T("\r\n");
						break;
					}//switch
				}//for

				res = res + _T("// end section\r\n\r\n");
			}//for

			return res;
		};//static CString sections(vector <Section> & sList)
	};//class local

	return CString(_T("// \r\n")) +
		   CString(_T("// \r\n")) +
		   CString(_T(" \r\n")) +
		   local::sections(sectionList) +
		   CString(_T("// Section Cycle Definition\r\n")) +
		   CString(_T("// end section\r\n"));
}//bool CNCUnitDef::GetString(CString getString)

unsigned int CNCUnitDef::FindWRepl(CString srcString, CString dstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(sectionList[1].name == str);

	for(unsigned int i = 0; i < sectionList[1].defList.size(); i++)
	{
		ASSERT(sectionList[1].defList[i].GetType() == D_WREPL);
		if(sectionList[1].defList[i].wRep.srcString == srcString && sectionList[1].defList[i].wRep.destString == dstString)
		{
			return i;
		}//if
	}//for
	return -1;
}//int CNCUnitDef::FindWRepl(CString srcString, CString dstString)

bool CNCUnitDef::MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString)
{
	CString str; str.LoadString(IDS_WORD_REPLACEMENT);
	ASSERT(sectionList[1].name == str);
	
	/// 1 - Находим позицию src wrepl
	unsigned int srcPos;
	bool srcExists = false;
	for(unsigned int i = 0; i < sectionList[1].defList.size(); i++)
	{
		ASSERT(sectionList[1].defList[i].GetType() == D_WREPL);
		if(sectionList[1].defList[i].wRep.srcString == srcsrcString && sectionList[1].defList[i].wRep.destString == srcdstString)
		{
			srcExists = true;
			srcPos = i;
		}//if
	}//for
	if(!srcExists)
		return false;

	/// 2 - Находим позицию target wrepl
	unsigned int dstPos;
	bool dstExists = false;
	for(unsigned int i = 0; i < sectionList[1].defList.size(); i++)
	{
		ASSERT(sectionList[1].defList[i].GetType() == D_WREPL);
		if(sectionList[1].defList[i].wRep.srcString == dstsrcString && sectionList[1].defList[i].wRep.destString == dstdstString)
		{
			dstExists = true;
			dstPos = i;
		}//if
	}//for
	if(!dstExists)
		return false;
	
	if(srcPos < dstPos)
	{
		/// 3 - делаем копию src wrepl
		Definition copy;
		copy = sectionList[1].defList[srcPos];
		/// 4 - Вставляем копию в позицию target wrepl
		sectionList[1].defList.insert(sectionList[1].defList.begin() + dstPos, copy);
		/// 5 - Удаляем src wrepl по его позиции
		sectionList[1].defList.erase(sectionList[1].defList.begin() + srcPos);
	}//if
	else
	{
		/// 3 - Делаем копию src wrepl
		Definition copy;
		copy = sectionList[1].defList[srcPos];
		/// 4 - Удаляем src wrepl по номеру позиции
		sectionList[1].defList.erase(sectionList[1].defList.begin() + srcPos);
		/// 5 - Вставляем копию src wrepl в позицию target wrepl
		sectionList[1].defList.insert(sectionList[1].defList.begin() + dstPos, copy);
	}//else

	return true;
}//void CNCUnitDef::MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString)

bool CNCUnitDef::GetGroup(CString sectName, CString groupName, Group &group)
{
	for(unsigned int i = 0; i < sectionList.size(); i++)
	{
		if(sectName == sectionList[i].name)
		{
			for(unsigned int j = 0 ; j < sectionList[i].groupList.size() ; j++)
			{
				if(groupName == sectionList[i].groupList[j].name)
				{
					group = sectionList[i].groupList[j];
					return true;
				}//if
			}//for
			return false;
		}
	}//for
	return false;
}//bool CNCUnitDef::GetGroup(CString sectName, CString groupName, Group &group)

bool CNCUnitDef::AddSection(Section section)
{
	unsigned int sectionNumber = 1;
	/// Определяем номер секции в списке секций
	for(unsigned int i = 0; i < sectionList.size(); i++)
	{
		if(section.name == sectionList[i].name)
		{
			sectionNumber = i;
			break;
		}//if
	}//for
	/// Если секция не найдена, ошибка
	if(sectionNumber == -1)
	{
		//sectionList.push_back(section);
		return false;
	}//if
	/// Секция найдена
	else
	{
		/// Для каждого определения во входной секции выполняем
		for(unsigned int i =0; i < section.defList.size(); i++)
		{
			/// Если текущее определение - Word Replacement, добавляем его в любом случае
			if( section.defList[i].GetType() == D_WREPL)
			{
				sectionList[sectionNumber].defList.push_back(section.defList[i]);
			}//if
			else
			{
				unsigned int definitionNumber = -1;
				/// Определяем номер определения в найденной секции
				for(unsigned int j = 0; j < sectionList[sectionNumber].defList.size(); j++)
				{
					if(section.defList[i].def.defWord == sectionList[sectionNumber].defList[j].def.defWord)
					{
						definitionNumber = j;
						break;
					}//if
				}//for
				/// Если определение в найденной секции не найдено, добавляем его туда
				if(definitionNumber == -1)
				{
					/// В текущий момент времени параметр считан из файла в строковом виде в поле param.s
					/// Инициализируем Definition для использования в стойке
					section.defList[i].def.defParam.SetType(P_S);
					/// Ставим флаг, параметр не присутствует в файле-прототипе
					section.defList[i].def.known = false;
					section.defList[i].def.presence = true;
					section.defList[i].def.defParam.s = section.defList[i].def.param.s;
					section.defList[i].def.param = section.defList[i].def.defParam;
					CString str; str.LoadString(IDS_MESSAGE_DOESNT_DEFINED);
					section.defList[i].def.briefDescr = str;
					sectionList[sectionNumber].defList.push_back(section.defList[i]);
				}//if
				/// Определение найдено, помечаем его как присутствующее в стойке и копируем параметры
				else
				{
					sectionList[sectionNumber].defList[definitionNumber].def.presence = true;
					/// Проверка и преобразование типа
					switch (sectionList[sectionNumber].defList[definitionNumber].def.param.GetType())
					{
					case P_B:
						if(section.defList[i].def.param.s.MakeUpper() == _T("YES"))
						{
							sectionList[sectionNumber].defList[definitionNumber].def.param.b = true;
						}//if
						else
						{
							sectionList[sectionNumber].defList[definitionNumber].def.param.b = false;
						}//else
						break;
					case P_I:
						sectionList[sectionNumber].defList[definitionNumber].def.param.i = _ttoi(section.defList[i].def.param.s); 
						break;
					case P_D:
						sectionList[sectionNumber].defList[definitionNumber].def.param.d = _tstof(section.defList[i].def.param.s); 
						break;
					case P_S:
						sectionList[sectionNumber].defList[definitionNumber].def.param = section.defList[i].def.param;
						break;
					case P_SE:
						/// Варианты значений уже внутри структуры данных
						sectionList[sectionNumber].defList[definitionNumber].def.param.s = section.defList[i].def.param.s;
						break;
					case P_EE:
						/// Варианты значений уже внутри структуры данных
						sectionList[sectionNumber].defList[definitionNumber].def.param.s = section.defList[i].def.param.s;
						break;
					case P_EMPTY:
						ASSERT(false);
						break;
					}//switch
				}//else
			}//else
		}//for
	}//else
	return true;
}//bool CNCUnitDef::AddSection(Section section)