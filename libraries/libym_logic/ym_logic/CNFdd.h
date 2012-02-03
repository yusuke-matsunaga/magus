#ifndef YM_LOGIC_CNFDD_H
#define YM_LOGIC_CNFDD_H

/// @file ym_logic/CNFdd.h
/// @brief CNFdd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/cnfdd_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_logic/Literal.h"
#include "ym_utils/BinIO.h"
#include "gmpxx.h"


BEGIN_NAMESPACE_YM_CNFDD

class CNFddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @name 定数の定義
/// @{

/// @brief 葉のノードの仮想的なレベル
/// @ingroup CNFdd
const ymuint kCNFddLevelMax = 0xFFFFFFFF;

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class CNFdd CNFdd.h "ym_logic/CNFdd.h"
/// @ingroup CNFdd
/// @brief CNFDDを表すクラス
/// @sa CNFddMgr
//////////////////////////////////////////////////////////////////////
class CNFdd
{
  friend class CNFddMgr;
  friend class CNFddMgrImpl;

public:

  /// @brief デフォルトのコンストラクタ
  /// @note デフォルトの CNFDD マネージャの定数0となる
  CNFdd();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  CNFdd(const CNFdd& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const CNFdd&
  operator=(const CNFdd& src);

  /// @brief デストラクタ
  /// @note 根の枝のリンク数を一つ減らして，0になった時には実際に
  /// ノードの解放を行なう．
  /// @note エラーは起こらない．
  ~CNFdd();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 特別な値を持つときのチェック
  /// @{

  /// @brief 定数0(A.K.A. empty set)のチェック
  /// @return 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1(A.K.A. base set)のチェック
  /// @return 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数のチェック
  /// @return 定数0か定数1の時 true を返す．
  bool
  is_const() const;

  /// @brief メモリオーバーフローのチェック
  /// @return メモリオーバーフローで処理が完了していないとき true を返す．
  bool
  is_overflow() const;

  /// @brief エラーのチェック
  /// @return 演算結果がエラーになったとき true を返す．
  bool
  is_error() const;

  /// @brief オーバーフローかエラーのチェック
  /// @return オーバーフローかエラーの時 true を返す．
  bool
  is_invalid() const;

  /// @brief 終端ノードのチェック
  /// @return 終端ノード (定数0, 定数1, オーバーフロー, エラー)
  /// のとき true を返す．
  bool
  is_leaf() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 特別な値の設定
  /// @{

  /// @brief 定数0(A.K.A. empty set)に設定する．
  void
  set_zero();

  /// @brief 定数1(A.K.A. base set)に設定する．
  void
  set_one();

  /// @brief エラー値に設定する．
  void
  set_error();

  /// @brief オーバーフロー値に設定する．
  void
  set_overflow();

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算＋代入演算子
  /// @{

  /// @brief conjunction (\f$\wedge\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const CNFdd&
  operator&=(const CNFdd& src);

  /// @brief disjunction (\f$\vee\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const CNFdd&
  operator|=(const CNFdd& src);

  /// @brief diff (\f$\setdiff\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const CNFdd&
  operator-=(const CNFdd& src);

  /// @brief 要素ごとのユニオン付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const CNFdd&
  operator*=(const CNFdd& src);

  /// @brief 他の節に支配されている節を取り除く
  /// @return 自分自身
  const CNFdd&
  make_minimal();

  /// @brief 要素数が limit 以下の要素のみを残す．
  /// @param[in] limit 要素数の制限値
  /// @return 結果の CNFdd
  CNFdd
  cut_off(ymuint limit) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の演算
  /// @{

  /// @brief 指定したリテラルを加える．
  /// @param[in] literal リテラル
  /// @return 自分自身を返す．
  const CNFdd&
  add_literal(Literal literal);

  /// @brief 指定した変数の肯定リテラルを加える．
  /// @param[in] var 変数番号
  /// @return 自分自身を返す．
  const CNFdd&
  add_posiliteral(VarId var);

  /// @brief 指定した変数の否定リテラルを加える．
  /// @param[in] var 変数番号
  /// @return 自分自身を返す．
  const CNFdd&
  add_negaliteral(VarId var);

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数を含まないコファクターを返す．
  CNFdd
  cofactor_0(VarId var) const;

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数の肯定リテラルを含むコファクターを返す．
  CNFdd
  cofactor_p(VarId var) const;

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数の否定リテラルを含むコファクターを返す．
  CNFdd
  cofactor_n(VarId var) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name CNFDD の構造に関係したメソッド
  /// @{

  /// @brief 根の節点の変数に基づいて展開を行なう．
  /// @param[out] f_0 0枝のコファクターを入れる CNFDD
  /// @param[out] f_p p枝のコファクターを入れる CNFDD
  /// @param[out] f_n n枝のコファクターを入れる CNFDD
  /// @return 根の節点の変数番号を返す．
  /// @note もともと定数値(葉)のCNFDDの場合動作は不定
  VarId
  root_decomp(CNFdd& f_0,
	      CNFdd& f_p,
	      CNFdd& f_n) const;

  /// @brief 根の変数番号の取得
  /// @retval 根の変数番号 内部節点の場合
  /// @retval kVarIdMax 終端節点の場合
  VarId
  root_var() const;

  /// @brief 0枝の指しているコファクターの取得
  /// @retval 0枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  CNFdd
  edge_0() const;

  /// @brief p枝の指しているコファクターの取得
  /// @retval p枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  CNFdd
  edge_p() const;

  /// @brief n枝の指しているコファクターの取得
  /// @retval n枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  CNFdd
  edge_n() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ハッシュ用の関数
  /// @{

  /// @brief ハッシュ値の計算
  /// @return ハッシュ値を返す．
  ymuint
  hash() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ノード数の計数などの関数
  /// @{

  /// @brief CNFDD が使っているノード数を数える．
  ymuint64
  node_count() const;

  /// @brief 項数を返す．
  mpz_class
  count() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{

  /// @brief 等価比較演算
  /// @param[in] right 比較対象の CNFDD
  /// @retval true 自分自身と right が等価な場合
  /// @retval false 上記以外
  bool
  operator==(const CNFdd& right) const;

#if 0
  /// @brief 2つの関数の交叉チェック
  /// @param[in] right 比較対象の CNFDD
  /// @retval true 自分自身と right が交わっている時
  /// @retval false 上記以外
  bool
  operator&&(const CNFdd& right) const;

  /// @brief 包含関係のチェック
  /// @param[in] right 比較対象の CNFDD
  /// @retval true 自分自身が minterms の集合として right を含む時
  /// @retval false 上記以外
  bool
  operator>=(const CNFdd& right) const;
#endif

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name サポート関係の関数
  /// @{

  /// @brief サポート変数集合の計算 (VarVector)
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  ymuint
  support(VarVector& support) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] support サポート変数集合を格納するリスト
  /// @return サポートの要素数
  ymuint
  support(VarList& support) const;

  /// @brief サポート変数集合の要素数の計算
  /// @param[in] bdd 対象の BDD
  /// @return サポート変数集合の要素数
  ymuint
  support_size() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 出力用の関数
  /// @{

  /// @brief 内容を書き出す．
  /// @param[in] s 出力ストリーム
  /// @return ノード数を返す．
  ymuint64
  print(ostream& s) const;

  /// @brief 内容を節ごとに書き出す．
  /// @param[in] s 出力ストリーム
  void
  print_clause(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name バイナリダンプ関係
  /// @{

  /// @brief CNFDD の内容をダンプする．
  /// @param[in] s 出力ストリーム
  void
  dump(BinO& s) const;

  /// @brief バイナリファイルに保存されたCNFDDを読み込む．
  /// @param[in] s 入力ストリーム
  void
  restore(BinI& s);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CNFDD マネージャと根の枝を引数とするコンストラクタ
  /// @param[in] mgr CNFDD マネージャ
  /// @param[in] root 根の枝
  CNFdd(CNFddMgrImpl* mgr,
	ympuint root);

  /// @brief mgr, root をセットする時に呼ばれる関数
  /// @param[in] mgr CNFDD マネージャ
  /// @param[in] root 根の枝
  void
  set(CNFddMgrImpl* mgr,
      ympuint root);

  /// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
  void
  reset();

  /// @brief 根の枝を new_e に置き換える．
  /// @param[in] new_e 新しい枝
  /// @warning new_e も同一の CNFddMgr に属していると仮定する．
  void
  assign(ympuint new_e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の CNFddMgr
  CNFddMgrImpl* mMgr;

  // 根のノードを指す枝
  ympuint mRoot;

  // おなじ CNFddMgr の CNFDD をつなぐためのリンクポインタ
  CNFdd* mPrev;
  CNFdd* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @name CNFDD 関係の non-member functions
/// @{

/// @relates CNFdd
/// @ingroup CNFdd
/// @brief conjunction (\f$wedge\f$)演算
/// @param[in] left, right オペランド
/// @return left と right の intersection
CNFdd
operator&(const CNFdd& left,
	  const CNFdd& right);

/// @relates CNFdd
/// @ingroup CNFdd
/// @brief disjunction (\f$vee\f$演算
/// @param[in] left, right オペランド
/// @return left と right の union
CNFdd
operator|(const CNFdd& left,
	  const CNFdd& right);

/// @relates CNFdd
/// @ingroup CNFdd
/// @brief diff (\f$\setdiff\f$)演算
/// @param[in] left, right オペランド
/// @return left と right の diff
CNFdd
operator-(const CNFdd& left,
	  const CNFdd& right);

/// @relates CNFdd
/// @ingroup CNFdd
/// @brief 要素ごとのユニオン)演算
/// @param[in] left, right オペランド
/// @return left と right の要素ごとのユニオン
CNFdd
operator*(const CNFdd& left,
	  const CNFdd& right);

/// @relates CNFdd
/// @ingroup CNFdd
/// @brief left と right が等価でない場合 true を返す．
/// @param[in] left, right オペランド
bool
operator!=(const CNFdd& left,
	   const CNFdd& right);

#if 0
// left が minterms の集合として right に含まれるとき true を返す．
bool
operator<=(const CNFdd& left,
	   const CNFdd& right);

// left が真に right を含むとき true を返す．
bool
operator>(const CNFdd& left,
	  const CNFdd& right);

// left が真に right に含まれるとき true を返す．
bool
operator<(const CNFdd& left,
	  const CNFdd& right);
#endif

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 不等価比較
// @return 2つのCNFDDが等しくないとき true を返す．
inline
bool
operator!=(const CNFdd& left,
	   const CNFdd& right)
{
  return ! left.operator==(right);
}

// @brief intersection 演算
inline
CNFdd
operator&(const CNFdd& left,
	  const CNFdd& right)
{
  return CNFdd(left).operator&=(right);
}

// @brief union 演算
inline
CNFdd
operator|(const CNFdd& left,
	  const CNFdd& right)
{
  return CNFdd(left).operator|=(right);
}

// @brief diff 演算
inline
CNFdd
operator-(const CNFdd& left,
	  const CNFdd& right)
{
  return CNFdd(left).operator-=(right);
}

// @brief 要素ごとのユニオン)演算
inline
CNFdd
operator*(const CNFdd& left,
	  const CNFdd& right)
{
  return CNFdd(left).operator*=(right);
}

// @brief 指定したリテラルを加える．
// @param[in] literal リテラル
// @return 自分自身を返す．
inline
const CNFdd&
CNFdd::add_literal(Literal literal)
{
  if ( literal.pol() == kPolPosi ) {
    return add_posiliteral(literal.varid());
  }
  else {
    return add_negaliteral(literal.varid());
  }
}

#if 0
// @brief 被包含チェック
// @return left が right に minterm 集合として包含されているときに
// true を返す．
inline
bool
operator<=(const CNFdd& left,
	   const CNFdd& right)
{
  return right >= left;
}

// @brief 真包含チェック
// @param left, right オペランド
// @return left が right を minterm 集合として真に包含しているときに
// true を返す．
inline
bool
operator>(const CNFdd& left,
	  const CNFdd& right)
{
  return (left != right) && (left >= right);
}

// @brief 真被包含チェック
// @param left, right オペランド
// @return left が right を minterm 集合として真に包含されているときに
// true を返す．
inline
bool
operator<(const CNFdd& left,
	  const CNFdd& right)
{
  return right > left;
}
#endif

// @brief ハッシュ関数
// @return ハッシュ値
inline
ymuint
CNFdd::hash() const
{
  ympuint r = mRoot;
  return (r * r) >> 8;
}

END_NAMESPACE_YM_CNFDD

BEGIN_NAMESPACE_HASH

// CNFDDをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsCNFdd::CNFdd>
{
  ymuint
  operator()(const nsYm::nsCNFdd::CNFdd& bdd) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_LOGIC_CNFDD_H
