
/// @file GdsData.cc
/// @brief GdsData の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_gds/GdsData.h"
#include "ym_gds/GdsDate.h"
#include "ym_gds/GdsString.h"
#include "ym_gds/GdsUnits.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
// クラス GdsData
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] version バージョン番号
// @param[in] date 2つの日時の配列
// @param[in] libdirsize LIBDIRSIZE の値
// @param[in] srfname SRFNAME の値
// @param[in] acl LIBSECURE の値
// @param[in] libname LIBNAME の値
// @param[in] reflibs REFLIBS の値
// @param[in] fonts FONTS の値
// @param[in] attrtable ATTRTABLE の値
// @param[in] generations GENERATIONS の値
// @param[in] format フォーマット情報
// @param[in] units UNITS の値
GdsData::GdsData(ymint16 version,
		 GdsDate* date,
		 ymint16 libdirsize,
		 GdsString* srfname,
		 GdsACL* acl,
		 GdsString* libname,
		 GdsString* reflibs,
		 GdsString* fonts,
		 GdsString* attrtable,
		 ymint16 generations,
		 GdsFormat* format,
		 GdsUnits* units) :
  mVersion(version),
  mDate(date),
  mLibDirSize(libdirsize),
  mSrfName(srfname),
  mAclList(acl),
  mLibName(libname),
  mRefLibs(reflibs),
  mFonts(fonts),
  mAttrTable(attrtable),
  mGenerations(generations),
  mFormat(format),
  mUnits(units),
  mStruct(NULL)
{
}

// @brief デストラクタ
GdsData::~GdsData()
{
}

// @brief バージョン番号を返す．
int
GdsData::version() const
{
  return mVersion;
}

// @brief 最終更新日時を返す．
const GdsDate&
GdsData::last_modification_time() const
{
  return mDate[0];
}

// @brief 最終アクセス日時を返す．
const GdsDate&
GdsData::last_access_time() const
{
  return mDate[1];
}

// @brief LIBDIRSIZE を返す．
int
GdsData::lib_dir_size() const
{
  return mLibDirSize;
}

// @brief SRFNAME(spacing rule file name) を返す．
const char*
GdsData::srf_name() const
{
  return mSrfName->str();
}

// @brief ACL リストを返す．
GdsACL*
GdsData::acl_list() const
{
  return mAclList;
}

// @brief ライブラリ名を返す．
const char*
GdsData::lib_name() const
{
  return mLibName->str();
}

// @brief 参照しているライブラリ名のリストを返す．
const char*
GdsData::reflibs() const
{
  return mRefLibs->str();
}

// @brief フォント名のリストを返す．
const char*
GdsData::fonts() const
{
  return mFonts->str();
}

// @brief 属性定義ファイル名を返す．
const char*
GdsData::attrtable() const
{
  return mAttrTable->str();
}

// @brief 世代を返す．
int
GdsData::generations() const
{
  return mGenerations;
}

// @brief フォーマット情報を返す．
const GdsFormat*
GdsData::format() const
{
  return mFormat;
}

// @brief user unit を返す．
double
GdsData::user_unit() const
{
  return mUnits->user_unit();
}

// @brief unit in meters を返す．
double
GdsData::meter_unit() const
{
  return mUnits->meter_unit();
}

// @brief GdsStruct の先頭を返す．
const GdsStruct*
GdsData::struct_top() const
{
  return mStruct;
}

END_NAMESPACE_YM_GDS
