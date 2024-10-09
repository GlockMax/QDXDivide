
// NCEditor2Doc.h : interface of the CNCEditor2Doc class
//


#pragma once
#include "NCUnitDef.h"
#include "NCUnitTreePane.h"
#include "PropertiesWnd.h"
#include "tinyxml.h"
#include <stack>
#include "NCUnitSnapshot.h"
#include "NCUnitHelpPane.h"

using namespace std;

/// Документ
/**
Действия, выполняемые над документов в процессе редактирования:
  - Изменение параметра Definition
  - Изменение присутствия определения в стойке (presence)
  - Добавление элемента Word Replacement
  - Изменение srcString и/или dstString элемента Word Replacment
  - Удаление элемента Word Replacement
  - Перетаскивание элемента Word Replacement вдоль списка
  - Добавление определения, которого нет в файле - прототипе
*/
class CNCEditor2Doc : public CDocument
{
protected: // create from serialization only
	CNCEditor2Doc();
	DECLARE_DYNCREATE(CNCEditor2Doc)

// Attributes
public:
	CString Content;

// Operations
public:

// Overrides
public:
	/// Создание пустого документа
	virtual BOOL OnNewDocument();
	/// Загрузка/сохранение
	virtual void Serialize(CArchive& ar);

	/// Перезагрузка файла-прототипа
	void ReloadXML(CString fileName);

	/// Отображение объектов стойки
	void ShowSectionProp(CString sectionName);
	void ShowDefinitionProp(CString sectionName, CString defName);
	void ShowDefinitionUnknownProp(CString sectionName, CString defName);
	void ShowWReplProp(CString srcString, CString dstString);
	void ShowGroupProp(CString sectName, CString groupName);

	/// Перерисовать документ
	void RefreshTextView();

	/// Редактирование параметра определения
	void ChangeDef(CString secName, CString defName, Param newValue);
	void ChangeDef(Def *def, Param newValue);
	/// Изменение флага присутствия в стойке для определения
	void ChangeDefPresence(CString secName, CString defName);
	void ChangeDefPresence(Def *def, bool presence = 1);
	/// Добавление элемента Word Replacement
	void AddWRepl(CString srcString, CString dstString);
	/// Изменение элемента Word Replacement
	void ChangeWRepl(unsigned int wreplIndex, CString srcString, CString dstString);
	void ChangeWRepl(CString srcString, CString dstString, CString newSrcString, CString newDstString);
	/// Удаление элемента Word Replacement
	void DelWRepl(CString srcString, CString dstString);
	/// Перемещение элемента Word Replacement на одну позицию вверх или вниз
	void MoveWRepl(CString srcString, CString dstString, bool up);
	/// Перемещение элемента Word Replacement
	void MoveWRepl(CString srcsrcString, CString srcdstString, CString dstsrcString, CString dstdstString);
	/// Добавление определения в секцию, которого нет в файле- прототипе
	void AddDef(CString secName, Def definition);

	/// Фиксирование изменения.
	/**
	Очистка redoStack, добавление состояния редактора в undoStack
	*/
	void CommitChange(CNCUnitSnapshot &snapshot);
	/// Создание снимка состояния редактора для хранения в истории undo/redo
	void MakeUnitSnapshot(CNCUnitSnapshot &snapshot);
	/// Восставновление редактора из снимка
	void ApplyUnitSnapshot(CNCUnitSnapshot &snapshot);
	/// Undo
	/// Удаление элемента из undo stack, добавление элемента в redo stack
	void Undo();
	/// Redo
	/// Удаление элемента из redo stack, добавление элемента в undo stack
	void Redo();
	/// Очистка стеков для undo и redo
	void ClearHistory();
	/// Очистка стека redo
	void ClearRedoStack();
	/// стек undo не пустой?
	bool isUndoActive();
	/// стек redo не пустой?
	bool isRedoActive();

	/// Установка флага в TreePane, чтобы не перерисовывать свойства
	void SetInternalSelect(bool IS);

// Implementation
public:
	virtual ~CNCEditor2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	/// Получение указателя на панель с деревом
	CNCUnitTreePane * GetTreeView();
	/// Получение указателя на окно свойств
	CPropertiesWnd * GetPropertiesWnd();
	/// Получение указателя на окно помощи
	CNCUnitHelpPane * GetHelpPane();

	CString GetCurFile();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	
	/// Описание стойки, главная структура данных
	CNCUnitDef unitDef;
	/// Стеки истории изменений
	stack <CNCUnitSnapshot> undoStack, redoStack;

	/// Имя текущего файла-прототипа
	CString curFile;

	bool successLoad;
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


