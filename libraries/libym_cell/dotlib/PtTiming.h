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

  /// @brief 子供を追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] node 追加する子供のノード
  virtual
  void
  add_child(const ShString& attr_name,
	    PtNode* node);

  /// @brief テーブルを追加する．
  /// @param[in] attr_name 属性名
  /// @param[in] table テーブル
  void
  add_table(const ShString& attr_name,
	    PtTable* table);


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を参照する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の数を返す．
  /// @note このクラスでは常に 0
  virtual
  ymuint
  value_num() const;

  /// @brief 値を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
  /// @note このクラスでの呼び出しはエラーになる．
  virtual
  const PtValue*
  value(ymuint pos) const;


public:
  //////////////////////////////////////////////////////////////////////
  // PtTiming に独自の関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイミング情報のリスト(配列)
  vector<const PtNode*> mTimingList;

  // タイミング情報以外の子供のリスト(配列)
  vector<const PtNode*> mChildList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTTIMING_H
