#ifndef MISLIBNOT_H
#define MISLIBNOT_H

/// @file MislibNot.h
/// @brief MislibNot のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibNot MislibNot.h "MislibNot.h"
/// @brief NOT論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibNot :
  public MislibNodeImpl
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  MislibNot(const FileRegion& loc,
	    const MislibNode* child1);

  /// @brief デストラクタ
  virtual
  ~MislibNot();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  const MislibNode*
  child1() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  Expr
  to_expr(const NameMap& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子供
  const MislibNode* mChild1;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBNOT_H
