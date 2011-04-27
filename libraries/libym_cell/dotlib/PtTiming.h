#ifndef LIBYM_CELL_DOTLIB_PTTIMING_H
#define LIBYM_CELL_DOTLIB_PTTIMING_H

/// @file libym_cell/dotlib/PtTiming.h
/// @brief PtTiming のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtTiming PtTiming.h "PtTiming.h"
/// @brief タイミング情報を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtTiming :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  PtTiming();

  /// @brief デストラクタ
  virtual
  ~PtTiming();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルを追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] table テーブル
  bool
  add_table(const ShString& attr_name,
	    PtTable* table);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する PtNode の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief simple attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_simple_attr(const ShString& attr_name,
		  const PtValue* value);

  /// @brief complex attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_complex_attr(const ShString& attr_name,
		   const vector<const PtValue*>& value_list);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 属性名をキーにしてテーブルを格納する連想配列
  hash_map<ShString, const PtTable*> mTableMap;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTTIMING_H
