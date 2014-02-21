#ifndef MISLIBGATE_H
#define MISLIBGATE_H

/// @file MislibGate.h
/// @brief MislibGate のヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibGate MislibGate.h "MislibGate.h"
/// @brief ゲート(セル)を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibGate :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] name 名前を表すパース木
  /// @param[in] area 面積を表すパース木
  /// @param[in] opin_name 出力ピン名を表すパース木
  /// @param[in] opin_expr 出力の論理式を表すパース木
  /// @param[in] ipin_list 入力ピンのリストを表すパース木
  MislibGate(const FileRegion& loc,
	     const MislibNode* name,
	     const MislibNode* area,
	     const MislibNode* opin_name,
	     const MislibNode* opin_expr,
	     const MislibNode* ipin_list);

  /// @brief デストラクタ
  virtual
  ~MislibGate();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief ピン名/ゲート名を表すオブジェクトを取り出す．
  virtual
  const MislibNode*
  name() const;

  /// @brief 面積を表すオブジェクトを返す．
  virtual
  const MislibNode*
  area() const;

  /// @brief 出力ピン名を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_name() const;

  /// @brief 出力の論理式を表すオブジェクトを返す．
  virtual
  const MislibNode*
  opin_expr() const;

  /// @brief 入力ピンのリストを表すオブジェクトを返す．
  virtual
  const MislibNode*
  ipin_list() const;

  /// @brief 次の要素を設定する．
  virtual
  void
  set_next(MislibNodeImpl* next);

  /// @brief 次の要素を取り出す．
  virtual
  const MislibNode*
  next() const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  const MislibNode* mName;

  // 面積
  const MislibNode* mArea;

  // 出力ピン名
  const MislibNode* mOpinName;

  // 出力の論理式
  const MislibNode* mOpinExpr;

  // 入力ピンのリスト
  const MislibNode* mIpinList;

  // 次の要素
  MislibNodeImpl* mNext;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBGATE_H
