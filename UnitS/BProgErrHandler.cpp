#include "stdafx.h"
#include "NCMProject.h"
#include "NProgError.h"
#include "BProgErrHandler.h"

BProgErrHandler::BProgErrHandler(int ReactType) noexcept
{
	// level 0 is highest level
	for (size_t i = 0; i < MAX_ERRORS; ++i)
		ErrLevels[i] = EL_LINE_ERROR;

	const int Level0Nums[] = { 1000, 1001, 1102, 1036, 1086, 1171, 1168, 1169 };
	for (int i = 0; i < sizeof(Level0Nums) / sizeof(Level0Nums[0]); ++i)
		SetErrorLevel(Level0Nums[i], EL_CRITICAL_ERROR);

	const int Level2Nums[] = 
		{ 1003, 1014, 1015, 1019, 1026, 1028, 1029, 1030, 1034, 1037
		, 1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1049
		, 1050, 1051, 1052, 1053, 1054, 1059, 1060, 1061, 1062, 1064
		, 1072, 1080, 1083, 1084, 1085, 1100, 1103, 1104, 1105, 1113
		, 1132, 1138, 1142, 1143, 1150, 1162, 1187, 1198, 1199, 1200
		, 1201, 1202, 1203};
	for (int i = 0; i < sizeof(Level2Nums) / sizeof(Level2Nums[0]); ++i)
		SetErrorLevel(Level2Nums[i], EL_LEXEM_ERROR);

	const int Level3Nums[] = 
		{ 1011, 1012, 1013, 1031, 1055, 1056, 1057, 1069, 1073, 1074
		, 1075, 1076, 1077, 1078, 1088, 1089, 1094, 1095, 1112, 1117
		, 1120, 1121, 1122, 1123, 1124, 1125, 1148, 1152, 1153, 1154, 1155
		, 1157, 1158, 1159, 1172, 1173, 1174, 1176, 1180, 1183, 1184
		, 1185, 1208 };
	for (int i = 0; i < sizeof(Level3Nums) / sizeof(Level3Nums[0]); ++i)
		SetErrorLevel(Level3Nums[i], EL_CHECK_ERROR);

	CurReactType = -1;
	SetReactType(ReactType);
}

BProgErrHandler::~BProgErrHandler() noexcept
{
}

BProgErrHandler::ErrorReaction BProgErrHandler::GetErrorReaction(NCUnitStatePrev::EvaluateCadrResult ret, int ErrCode) const noexcept
{
	if (ret != NCUnitStatePrev::ECR_ERROR)
		return ER_IGNORE;
	ErrCode -= ERROR_NUM_BASE;
	if (ErrCode < 0 || ErrCode >= MAX_ERRORS)
		return ER_BREAK;
	return ErrReactions[ErrLevels[ErrCode]];
}

BProgErrHandler::ErrorLevel BProgErrHandler::GetErrorLevel(int ErrCode) const noexcept
{
	return ErrLevels[ErrCode - ERROR_NUM_BASE];
}

CString BProgErrHandler::MakeErrorMessage(const NProgError& ProgError) const
{
	CString Message;
	switch (GetErrorLevel(ProgError.GetError()))
	{
	case BProgErrHandler::EL_CRITICAL_ERROR:
		Message = _T("0:");
		break;
	case BProgErrHandler::EL_LINE_ERROR:
		Message = _T("1:");
		break;
	case BProgErrHandler::EL_LEXEM_ERROR:
		Message = _T("2:");
		break;
	case BProgErrHandler::EL_CHECK_ERROR:
		Message = _T("3:");
		break;
	default:
		Message = _T("4:");
		break;
	}
	Message += ProgError.GetErrorString();
	return Message;
}

int BProgErrHandler::SetReactType(int ReactType)
{
	if (CurReactType == ReactType)
		return CurReactType;

	for (size_t i = 0; i < MAX_ERROR_LEVELS; ++i)
		ErrReactions[i] = ER_BREAK;
	switch (ReactType)
	{
	case 0:
		break;
	case 1:
		// Don't change ErrReactions[0]
		ErrReactions[1] = ER_SKIP_LINE;
		ErrReactions[2] = ER_SKIP_LINE;
		ErrReactions[3] = ER_SKIP_LINE;
		break;
	case 2:
	default:
		// Don't change ErrReactions[0]
		ErrReactions[1] = ER_SKIP_LINE;
		ErrReactions[2] = ER_CONTINUE;
		ErrReactions[3] = ER_CONTINUE;
		break;
	}
	auto Ret = CurReactType;
	CurReactType = ReactType;
	return Ret;
}

void BProgErrHandler::SetErrorLevel(int ErrCode, BProgErrHandler::ErrorLevel ErrLevel) noexcept
{
	ErrLevels[ErrCode - ERROR_NUM_BASE] = ErrLevel;
}
