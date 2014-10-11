#ifndef MISLIBBOP_H
#define MISLIBBOP_H

/// @file MislibBop.h
/// @brief MislibBop の派生クラスのヘッダファイル(その2)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "MislibNodeImpl.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibBop MislibBop.h "MislibBop.h"
/// @brief 2項演算子を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibBop :
  public MislibNodeImpl
{
protected:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibBop(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibBop();


public:

  /// @brief 論理式を表す型のときに true を返す．
  virtual
  bool
  is_expr() const;

  /// @brief 1番目の子供を取り出す．
  virtual
  const MislibNode*
  child1() const;

  /// @brief 2番目の子供を取り出す．
  virtual
  const MislibNode*
  child2() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 1番目の子供
  const MislibNode* mChild1;

  // 2番目の子供
  const MislibNode* mChild2;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibAnd MislibBop.h "MislibBop.h"
/// @brief AND論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibAnd :
  public MislibBop
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibAnd(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibAnd();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  Expr
  to_expr(const HashMap<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibOr MislibBop.h "MislibBop.h"
/// @brief OR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibOr :
  public MislibBop
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibOr(const FileRegion& loc,
	   const MislibNode* child1,
	   const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibOr();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  Expr
  to_expr(const HashMap<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};


//////////////////////////////////////////////////////////////////////
/// @class MislibXor MislibBop.h "MislibBop.h"
/// @brief XOR論理式を表すクラス
//////////////////////////////////////////////////////////////////////
class MislibXor :
  public MislibBop
{
  friend class MislibMgrImpl;

private:

  /// @brief コンストラクタ
  /// @param[in] loc 位置情報
  /// @param[in] child1 1番目の子供
  /// @param[in] child2 2番目の子供
  MislibXor(const FileRegion& loc,
	    const MislibNode* child1,
	    const MislibNode* child2);

  /// @brief デストラクタ
  virtual
  ~MislibXor();


public:

  /// @brief 種類を取り出す．
  virtual
  tType
  type() const;

  /// @brief 対応する論理式を生成する．
  /// @param[in] name_map 端子名をキーにして端子番号を取り出す連想配列
  virtual
  Expr
  to_expr(const HashMap<ShString, ymuint>& name_map) const;

  /// @brief 内容を出力する．
  /// デバッグ用
  virtual
  void
  dump(ostream& s) const;

};

END_NAMESPACE_YM_MISLIB

#endif // MISLIBBOP_H
