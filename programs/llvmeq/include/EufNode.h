#ifndef EUFNODE_H
#define EUFNODE_H

/// @file EufNode.h
/// @brief EufNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
/// @class EufNode EufNode.h "EufNode.h"
/// @brief EUF 式を表す構成要素の基底クラス
//////////////////////////////////////////////////////////////////////
class EufNode
{
  friend class EufMgr;

public:
  //////////////////////////////////////////////////////////////////////
  // 型の定義
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードのタイプを表す列挙型
  enum tType {
    /// @brief conjunction
    kCon,
    /// @brief disjunctoin
    kDis,
    /// @brief negation
    kNeg,
    /// @brief equality
    kEq,
    /// @brief function
    kFunc,
    /// @brief variable
    kVar
  };


protected:

  /// @brief デストラクタ
  virtual
  ~EufNode() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ユニークIDを得る．
  ymuint
  id() const;

  /// @brief 型を得る．
  virtual
  tType
  type() const = 0;

  /// @brief 左辺の式を得る．
  /// @note type() が kCon, kDis, kNeg, kEq の時のみ有効
  virtual
  EufNode*
  left() const;

  /// @brief 右辺の式を得る．
  /// @note type() が kCon, kDis, kEq の時のみ有効
  virtual
  EufNode*
  right() const;

  /// @brief 識別子名を得る．
  /// @note type() が kFunc, kVar の時のみ有効
  virtual
  string
  id_name() const;

  /// @brief 引数の数を得る．
  /// @note type() が kFunc の時のみ有効
  virtual
  ymuint
  arg_num() const;

  /// @brief 引数を得る．
  /// @param[in] pos 位置番号 ( 0 <= pos < arg_num() )
  /// @note type() が kFunc の時のみ有効
  virtual
  EufNode*
  arg(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint32 mId;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ユニークIDを得る．
inline
ymuint
EufNode::id() const
{
  return mId;
}

END_NAMESPACE_YM_LLVMEQ

#endif // EUFNODE_H
