#ifndef LIBYM_CELL_DOTLIB_PTBUNDLE_H
#define LIBYM_CELL_DOTLIB_PTBUNDLE_H

/// @file libym_cell/dotlib/PtBundle.h
/// @brief PtBundle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class PtBundle PtBundle.h "PtBundle.h"
/// @brief bundle グループを表すパース木の要素
//////////////////////////////////////////////////////////////////////
class PtBundle :
  public PtNode
{
  friend class PtMgr;

private:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  PtBundle(const ShString& name);

  /// @brief デストラクタ
  virtual
  ~PtBundle();


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ピンを追加する．
  /// @param[in] pin 対象のピン
  void
  add_pin(PtPin* pin);


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

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief pin グループの数を返す．
  ymuint
  pin_num() const;

  /// @brief pin グループのパース木を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  const PtPin*
  pin(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ピンのリスト(配列)
  vector<const PtPin*> mPinList;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_PTBUNDLE_H
