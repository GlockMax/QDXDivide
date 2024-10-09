#pragma once
#include "StdAfx.h"
#include <vector>
#include <set>
#include "MTAxis.h"
#include "BMatr.h"
#include "tinyxml.h"
#include "BMTLinkage.h"
#include "MTCollisionArray.h"

//#define LOG_FILE "..\\temp\\NCM.log"

/// Базовый класс для станка
class AFX_EXT_CLASS MTBase : public CObject
{
public:
	DECLARE_SERIAL(MTBase)
	MTBase();
	MTBase(const MTBase &MT);
	virtual ~MTBase();
	///Загрузка станка из xml файла
	/** на входе LoadXML - путь к каталогу со станками, имя станка
	 в классе MTBase - считывание кинематической информации
	 \sa ReadXMLObject
	 */
	void Regularize();///< упорядочение массива.
	void Reset();
	void ResetConfigs();
	void ResetComp();

	/// Считывание станка из xml файла
	/** на входе ReadXMLObject - путь к каталогу со станками. Имя уже хранится в объекте
	\sa LoadXML
	*/
	virtual bool ReadXMLObject(TiXmlDocument *doc);
	MTAxis* ReadXML1Axis(TiXmlNode* curaxis);
public:
	const MTBase& operator = (const MTBase &B);
	void CopyKinematics(const MTBase &Source);   ///< !!!!!!!!!!!!!!!дописать

	virtual bool RemoveBranch(int AxisIndex);
	class MTConfig& GetCurConfig();
	const class MTConfig& GetCurConfig() const;
	void RenameAxis(const CString& OldName, const CString& NewName);
	virtual void RenameAxis(int index, const CString& NewName);
	bool SwapAxisNames(int First, int Second);
	virtual void RemoveAll();
	const std::set<std::pair<int, int>>& GetEnemies(void) const;
	BMTLinkage GetMTLink() const { return MTLink; }
	const MTCollisionArray& GetCollisions() const { return Collisions; }
	void FullCopyWOVis(const MTBase& MT);
	void DelFullCopyWOVis();
	void SetCurConf(BMTLinkage MTLink);

	class MTConfig& AddNewConfig();
	int GetAxisColNum(const CString& AxisName);///< Collisions number for specified axis
	void AddColToMT(SCollision& Col);///< Add a collision four either for MTCollisionArray and MachineTool
	void AddColToMT(int MyNum, BOOL MyChildren, int AxisNum, BOOL AxisChildren);
	const MTAxis* GetParentAxis(const MTAxis* pAxis) const;
	const MTAxis* GetNearestMoveRotAxis(const MTAxis* pAxis) const;
	int GetNearestMoveRotAxisInd(int AxisInd) const;
	void ClearNow();
	const std::set<int>& GetStPosSet() const { return StPositionsSet; }
	int Conv2StPos(class BStockGrID StGr) const;

#ifdef _DEBUG
	/// Debug function. There is usefull GPL tool for it http://tailforwin32.sourceforge.net
	void DebugPrint(const CString& label = CString(""), BOOL level = 1, BOOL parents = 1);
#endif
protected:
	BMTLinkage MTLink;
	MTCollisionArray Collisions; ///< Список объектов контроля столкновений, по которому генерируется Enemies
	CooArray Now;
	/// Имя станка
	/** Устанавливается только в методе LoadXML
	*/
	CString FileName; 
	/// Номер выделяемой оси.
	/** При отрисовке стрелка этой оси выделяется габаритным параллелепипедом */
	CString SelectedAxis;
	/// Вектор конфигураций.
	std::vector<MTConfig> Configs;
	/// Stock positions set
	std::set<int> StPositionsSet;
};


