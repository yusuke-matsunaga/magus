#ifndef EXPRNODE_H
#define EXPRNODE_H

/// @file ExprNode.h
/// @brief ExprNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Expr.h"
#include "expr_types.h"


BEGIN_NAMESPACE_YM_EXPR

class SopLit;

//////////////////////////////////////////////////////////////////////
/// @class ExprNode ExprNode.h "ExprNode.h"
/// @brief 論理式を形作るノードのクラス
/// 効率化のためにコピーはポインタのコピーを用いる．
/// そのために参照回数を持つ．
//////////////////////////////////////////////////////////////////////
class ExprNode
{
  friend class Expr;
  friend class ExprNodePtr;
  friend class ExprMgr;

public:

  //////////////////////////////////////////////////////////////////////
  /// @name 内部の情報を読み出す関数
  /// @{

  /// @brief 型を返す
  tType
  type() const;

  /// @brief 恒偽関数を表している時に真となる．
  bool
  is_zero() const;

  /// @brief 恒真関数を表している時に真となる．
  bool
  is_one() const;

  /// @brief 定数関数を表している時に真となる．
  bool
  is_constant() const;

  /// @brief 肯定のリテラルを表している時に真となる．
  bool
  is_posiliteral() const;

  /// @brief 否定のリテラルを表している時に真となる．
  bool
  is_negaliteral() const;

  /// @brief リテラルを表している時に真となる．
  bool
  is_literal() const;

  /// @brief type がリテラルで極性が inv の時に真となる．
  /// @param[in] inv 極性
  bool
  is_literal(bool inv) const;

  /// @brief リテラルノードの場合に変数番号を取り出す．
  VarId
  varid() const;

  /// @brief AND ノードの時に真となる．
  bool
  is_and() const;

  /// @brief OR ノードの時に真となる．
  bool
  is_or() const;

  /// @brief XOR ノードの時に真となる．
  bool
  is_xor() const;

  /// @brief 演算子ノードの時に真となる．
  ///
  /// enum の順番に依存するよくないコード
  bool
  is_op() const;

  /// @brief 演算子ノードの場合に項の数を返す．
  ymuint
  child_num() const;

  /// @brief 演算子ノードの場合に子供のノードを返す．
  /// @param[in] pos 取り出す子供の位置
  /// @note 演算子ノード以外の場合と pos が範囲外の場合には定数0を返す．
  /// 通常は定数ノードが子供に含まれることはないのでエラーとわかる．
  const ExprNode*
  child(ymuint pos) const;

  /// @brief vals の値にしたがった評価を行う．
  ymulong
  eval(const vector<ymulong>& vals,
       ymulong mask) const;

  /// @brief 真理値表を作成する．
  /// @param[in] ni 入力数
  TvFunc
  make_tv(ymuint ni) const;

  /// @brief 定数,リテラルもしくは子供がリテラルのノードの時に true を返す．
  bool
  is_simple() const;

  /// @brief 子供がすべてリテラルの AND ノードの時に true を返す．
  bool
  is_simple_and() const;

  /// @brief 子供がすべてリテラルの OR ノードの時に true を返す．
  bool
  is_simple_or() const;

  /// @brief 子供がすべてリテラルの XOR ノードの時に true を返す．
  bool
  is_simple_xor() const;

  /// @brief SOP 形式の時 true を返す．
  bool
  is_sop() const;

  /// @brief リテラル数を返す．
  ymuint
  litnum() const;

  /// @brief 特定の変数のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  ymuint
  litnum(VarId varid) const;

  /// @brief 特定の変数の特定の極性のリテラルの出現回数を返す．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] inv 計測対象の極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  ymuint
  litnum(VarId varid,
	 bool inv) const;

  /// @brief 使われている変数の最大の番号 + 1を得る．
  ymuint
  input_size() const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  SopLit
  soplit(bool inverted) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  SopLit
  soplit(bool inverted,
	 VarId varid) const;

  /// @brief SOP 形式に展開したときの積項数とリテラル数を見積もる．
  /// @param[in] inverted true の時否定のSOPに対する見積を行う．
  /// @param[in] varid 計測対象の変数番号
  /// @param[in] inv 計測対象の極性
  SopLit
  soplit(bool inverted,
	 VarId varid,
	 bool inv) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  /// @name 参照回数の制御
  /// @{

  // 参照回数を1つ増やす．
  void
  inc_ref() const;

  // 参照回数を1つ減らす．
  // 結果0になれば自分自身を削除する．
  void
  dec_ref() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // コンストラクタ/デストラクタはむやみに使ってはだめなので
  // private にしている．
public:

  // コンストラクタ
  ExprNode();

  // デストラクタ
  ~ExprNode();


private:

  // 参照回数を得る．
  ymuint
  ref() const;

  // 参照回数をセットする．
  void
  ref(ymuint ref) const;

  // 自殺する．
  void
  suicide();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 参照回数＋ノードタイプ(3ビット)
  ymuint32 mRefType;

  // 子供の数 もしくは 変数番号
  ymuint32 mNc;

  // 子を指すポインタの配列
  const ExprNode* mChildArray[1];

};

/// @relates ExprNode
/// @brief node0 と node1 が式として等価のときに true を返す．
bool
posi_equiv(const ExprNode* node0,
	   const ExprNode* node1);


/// @relates ExprNode
/// @brief node0 と node1 が式として否定の関係にあるときに true を返す．
bool
nega_equiv(const ExprNode* node0,
	   const ExprNode* node1);



//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// コンストラクタ
inline
ExprNode::ExprNode()
{
}

// デストラクタ
inline
ExprNode::~ExprNode()
{
}

inline
tType
ExprNode::type() const
{
  return static_cast<tType>(mRefType & 7U);
}

inline
bool
ExprNode::is_zero() const
{
  return type() == kConst0;
}

inline
bool
ExprNode::is_one() const
{
  return type() == kConst1;
}

inline
bool
ExprNode::is_constant() const
{
  // ちょっとキタナイコード．
  return (int(type()) & ~1) == kConst0;
}

inline
bool
ExprNode::is_posiliteral() const
{
  return type() == kPosiLiteral;
}

inline
bool
ExprNode::is_negaliteral() const
{
  return type() == kNegaLiteral;
}

inline
bool
ExprNode::is_literal() const
{
  // ちょっとキタナイコード．
  return (static_cast<int>(type()) & ~1) == kPosiLiteral;
}

inline
bool
ExprNode::is_literal(bool inv) const
{
  return inv ? is_negaliteral() : is_posiliteral();
}

inline
VarId
ExprNode::varid() const
{
  return is_literal() ? VarId(mNc) : VarId();
}

inline
bool
ExprNode::is_and() const
{
  return type() == kAnd;
}

inline
bool
ExprNode::is_or() const
{
  return type() == kOr;
}

inline
bool
ExprNode::is_xor() const
{
  return type() == kXor;
}

inline
bool
ExprNode::is_op() const
{
  return type() >= kAnd;
}

inline
ymuint
ExprNode::child_num() const
{
  return is_op() ? mNc : 0;
}

inline
const ExprNode*
ExprNode::child(ymuint pos) const
{
  assert_cond(pos < child_num(), __FILE__, __LINE__);
  return mChildArray[pos];
}

inline
ymuint
ExprNode::ref() const
{
  return static_cast<ymuint>(mRefType >> 3);
}

inline
void
ExprNode::ref(ymuint ref) const
{
  ExprNode* node = const_cast<ExprNode*>(this);
  // 昔の参照回数を落とす．
  node->mRefType &= 7;
  // ref をセットする．
  node->mRefType |= (ref << 3);
}

inline
void
ExprNode::inc_ref() const
{
  // MAX の時は増やさない．
  if ( ref() < kRefMax ) {
    ExprNode* node = const_cast<ExprNode*>(this);
    node->mRefType += 8;
  }
}

inline
void
ExprNode::dec_ref() const
{
  ymuint r = ref();
  // MAX の時は減らさない．
  if ( r < kRefMax ) {
    ExprNode* node = const_cast<ExprNode*>(this);
    node->mRefType -= 8;
    if ( r == 1 ) {
      node->suicide();
    }
  }
}

END_NAMESPACE_YM_EXPR

#endif // EXPRNODE_H
