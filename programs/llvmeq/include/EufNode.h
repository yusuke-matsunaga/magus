#ifndef EUFNODE_H
#define EUFNODE_H

/// @file EufNode.h
/// @brief EufNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"
#include "logic/VarId.h"


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
    kCon  = 1,
    /// @brief disjunctoin
    kDis  = 2,
    /// @brief negation
    kNeg  = 3,
    /// @brief equality
    kEq   = 4,
    /// @brief function
    kFunc = 5,
    /// @brief variable
    kVar  = 6,
    /// @brief boolean variable
    kBVar = 7,
  };


protected:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  EufNode(ymuint id,
	  VarId vid);

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

  /// @biref SatSolver 用の変数番号を得る．
  VarId
  var_id() const;

  /// @brief 型を得る．
  virtual
  tType
  type() const = 0;

  /// @brief Boolean 型の時 true を返す．
  /// @note 具体的には kCon, kDis, kNeg, kEq, kBVar の時 true を返す．
  virtual
  bool
  is_boolean() const = 0;

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

  // SatSolver 用の変数番号
  VarId mVarId;

};

/// @relates EufNode
/// @brief EufNode の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] node 対象のノード
extern
void
display(ostream& s,
	const EufNode* node);

/// @relates EufNode
/// @brief 複数の EufNode の内容を出力する．
/// @param[in] s 出力先のストリーム
/// @param[in] node_list 対象のノードのリスト
extern
void
display(ostream& s,
	const vector<const EufNode*>& node_list);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] vid SatSolver 用の変数番号
inline
EufNode::EufNode(ymuint id,
		 VarId vid) :
  mId(id),
  mVarId(vid)
{
}

// @brief ユニークIDを得る．
inline
ymuint
EufNode::id() const
{
  return mId;
}

// @biref SatSolver 用の変数番号を得る．
inline
VarId
EufNode::var_id() const
{
  return mVarId;
}

END_NAMESPACE_YM_LLVMEQ

#endif // EUFNODE_H
