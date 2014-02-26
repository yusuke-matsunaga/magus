#ifndef GDS_GDSDATA_H
#define GDS_GDSDATA_H

/// @file gds/GdsData.h
/// @brief GdsData のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "gds/gds_nsdef.h"


BEGIN_NAMESPACE_YM_GDS

//////////////////////////////////////////////////////////////////////
/// @class GdsData GdsData.h "gds/GdsData.h"
/// @brief GDS-II のデータを表すクラス
//////////////////////////////////////////////////////////////////////
class GdsData
{
  friend class GdsParser;

private:

  /// @brief コンストラクタ
  /// @param[in] version バージョン番号
  /// @param[in] date 2つの日時の配列
  /// @param[in] libdirsize LIBDIRSIZE の値
  /// @param[in] srfname SRFNAME の値
  /// @param[in] acl LIBSECURE の値
  /// @param[in] libname LIBNAME の値
  /// @param[in] reflibs REFLIBS の値
  /// @param[in] fonts FONTS の値
  /// @param[in] attrtable ATTRTABLE の値
  /// @param[in] generations GENERATIONS の値
  /// @param[in] format フォーマット情報
  /// @param[in] units UNITS の値
  GdsData(ymint16 version,
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
	  GdsUnits* units);

  /// @brief デストラクタ
  ~GdsData();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief バージョン番号を返す．
  int
  version() const;

  /// @brief 最終更新日時を返す．
  const GdsDate&
  last_modification_time() const;

  /// @brief 最終アクセス日時を返す．
  const GdsDate&
  last_access_time() const;

  /// @brief LIBDIRSIZE を返す．
  int
  lib_dir_size() const;

  /// @brief SRFNAME(spacing rule file name) を返す．
  const char*
  srf_name() const;

  /// @brief ACL リストを返す．
  GdsACL*
  acl_list() const;

  /// @brief ライブラリ名を返す．
  const char*
  lib_name() const;

  /// @brief 参照しているライブラリ名のリストを返す．
  const char*
  reflibs() const;

  /// @brief フォント名のリストを返す．
  const char*
  fonts() const;

  /// @brief 属性定義ファイル名を返す．
  const char*
  attrtable() const;

  /// @brief 世代を返す．
  int
  generations() const;

  /// @brief フォーマット情報を返す．
  const GdsFormat*
  format() const;

  /// @brief user unit を返す．
  double
  user_unit() const;

  /// @brief unit in meters を返す．
  double
  meter_unit() const;

  /// @brief GdsStruct の先頭を返す．
  const GdsStruct*
  struct_top() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バージョン番号
  ymint16 mVersion;

  // 最終更新日時/最終アクセス日時
  GdsDate* mDate;

  // LIBDIRSIZE
  ymint16 mLibDirSize;

  // SRFNAME(spacing rule file name)
  GdsString* mSrfName;

  // LIBSECRUE
  GdsACL* mAclList;

  // ライブラリ名
  GdsString* mLibName;

  // reference libraries
  GdsString* mRefLibs;

  // フォント名
  GdsString* mFonts;

  // 属性定義ファイル名
  GdsString* mAttrTable;

  // 世代 ( 2 - 99 )
  ymuint8 mGenerations;

  // フォーマット情報
  GdsFormat* mFormat;

  // UNITS
  GdsUnits* mUnits;

  // GdsStruct の先頭
  GdsStruct* mStruct;

};

END_NAMESPACE_YM_GDS

#endif // GDS_GDSDATA_H
