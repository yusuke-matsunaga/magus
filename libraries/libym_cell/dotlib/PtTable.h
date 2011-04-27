#ifndef LIBYM_CELL_DOTLIB_PTTABLE_H
#define LIBYM_CELL_DOTLIB_PTTABLE_H

/// @file libym_cell/dotlib/PtTable.h
/// @brief PtTable のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtTable PtTable.h "PtTable.h"
/// @brief タイミング情報を表す PtNode の継承クラス
//////////////////////////////////////////////////////////////////////
class PtTable :
  public PtNode
{
  friend class PtMgr;
  friend class TableHandler;

private:

  /// @brief コンストラクタ
  /// @param[in] name テンプレート名
  PtTable(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~PtTable();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief index_1 属性の値を返す．
  const PtValue*
  index_1() const;

  /// @brief index_2 属性の値を返す．
  const PtValue*
  index_2() const;

  /// @brief index_3 属性の値を返す．
  const PtValue*
  index_3() const;

  /// @brief values 属性の値を返す．
  const PtValue*
  values() const;

  /// @brief domain グループを返す．
  const PtDomain*
  domain() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テンプレート名
  ShString mName;

  // index_1 の値
  PtValue* mIndex1;

  // index_2 の値
  PtValue* mIndex2;

  // index_3 の値
  PtValue* mIndex3;

  // vaues の値
  PtValue* mValues;

  // domain グループ
  PtDomain* mDomain;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTTABLE_H
