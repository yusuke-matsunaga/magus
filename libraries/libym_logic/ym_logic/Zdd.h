#ifndef YM_LOGIC_ZDD_H
#define YM_LOGIC_ZDD_H

/// @file ym_logic/Zdd.h
/// @brief Zdd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/zdd_nsdef.h"
#include "ym_logic/VarId.h"
#include "ym_utils/IDO.h"
#include "ym_utils/ODO.h"
#include "gmpxx.h"


BEGIN_NAMESPACE_YM_ZDD

class ZddMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @name 定数の定義
/// @{

/// @brief 葉のノードの仮想的なレベル
/// @ingroup Zdd
const ymuint kZddLevelMax = 0xFFFFFFFF;

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class Zdd Zdd.h "ym_logic/Zdd.h"
/// @ingroup Zdd
/// @brief ZDDを表すクラス
/// @sa ZddMgr
//////////////////////////////////////////////////////////////////////
class Zdd
{
  friend class ZddVector;
  friend class ZddList;
  friend class ZddMgr;
  friend class ZddMgrImpl;

public:

  /// @brief デフォルトのコンストラクタ
  /// @note デフォルトの BDD マネージャの定数0となる
  Zdd();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Zdd(const Zdd& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const Zdd&
  operator=(const Zdd& src);

  /// @brief デストラクタ
  /// @note 根の枝のリンク数を一つ減らして，0になった時には実際に
  /// ノードの解放を行なう．
  /// @note エラーは起こらない．
  ~Zdd();


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

  /// @brief intersection (\f$\cap\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator&=(const Zdd& src);

  /// @brief union (\f$\cup\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator|=(const Zdd& src);

  /// @brief diff (\f$\setminus\f$)付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator-=(const Zdd& src);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の演算
  /// @{

  /// @brief 指定した変数の0枝と1枝を交換する．
  /// @param[in] var 交換を行う変数番号
  /// @return 自分自身を返す．
  const Zdd&
  swap(VarId var);

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数 var を含まないコファクターを返す．
  Zdd
  cofactor0(VarId var) const;

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数 var を含むコファクターを返す．
  Zdd
  cofactor1(VarId var) const;

  /// @brief 変数インデックスの置き換えを行う．
  /// @param[in] var_map 置き換え元の変数から置き換え先の変数への写像
  /// @return 演算結果
  Zdd
  remap_var(const VarVarMap& var_map) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ZDD の構造に関係したメソッド
  /// @{

  /// @brief 根の節点の変数に基づいて展開を行なう．
  /// @param[out] f0 0枝のコファクターを入れる ZDD
  /// @param[out] f1 1枝のコファクターを入れる ZDD
  /// @return 根の節点の変数番号を返す．
  /// @note もともと定数値(葉)のZDDの場合動作は不定
  VarId
  root_decomp(Zdd& f0,
	      Zdd& f1) const;

  /// @brief 根の変数番号の取得
  /// @retval 根の変数番号 内部節点の場合
  /// @retval kVarIdMax 終端節点の場合
  VarId
  root_var() const;

  /// @brief 0枝の指しているコファクターの取得
  /// @retval 0枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Zdd
  edge0() const;

  /// @brief 1枝の指しているコファクターの取得
  /// @retval 1枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Zdd
  edge1() const;

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

  /// @brief ZDD が使っているノード数を数える．
  ymuint64
  node_count() const;

  /// @brief 集合の要素数を数える．
  mpz_class
  count() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{

  /// @brief 等価比較演算
  /// @param[in] right 比較対象の ZDD
  /// @retval true 自分自身と right が等価な場合
  /// @retval false 上記以外
  bool
  operator==(const Zdd& right) const;

#if 0
  /// @brief 2つの関数の交叉チェック
  /// @param[in] right 比較対象の ZDD
  /// @retval true 自分自身と right が交わっている時
  /// @retval false 上記以外
  bool
  operator&&(const Zdd& right) const;

  /// @brief 包含関係のチェック
  /// @param[in] right 比較対象の ZDD
  /// @retval true 自分自身が minterms の集合として right を含む時
  /// @retval false 上記以外
  bool
  operator>=(const Zdd& right) const;
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

  /// @brief 内容を集合の形で書き出す．
  /// @param[in] s 出力ストリーム
  void
  print_set(ostream& s) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name バイナリダンプ関係
  /// @{

  /// @brief ZDD の内容をダンプする．
  /// @param[in] s 出力ストリーム
  void
  dump(ODO& s) const;

  /// @brief バイナリファイルに保存されたZDDを読み込む．
  /// @param[in] s 入力ストリーム
  void
  restore(IDO& s);

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ZDD マネージャと根の枝を引数とするコンストラクタ
  /// @param[in] mgr ZDD マネージャ
  /// @param[in] root 根の枝
  Zdd(ZddMgrImpl* mgr,
      ympuint root);

  /// @brief mgr, root をセットする時に呼ばれる関数
  /// @param[in] mgr ZDD マネージャ
  /// @param[in] root 根の枝
  void
  set(ZddMgrImpl* mgr,
      ympuint root);

  /// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
  void
  reset();

  /// @brief 根の枝を new_e に置き換える．
  /// @param[in] new_e 新しい枝
  /// @warning new_e も同一の ZddMgr に属していると仮定する．
  void
  assign(ympuint new_e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の ZddMgr
  ZddMgrImpl* mMgr;

  // 根のノードを指す枝
  ympuint mRoot;

  // おなじ ZddMgr の ZDD をつなぐためのリンクポインタ
  Zdd* mPrev;
  Zdd* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @name ZDD 関係の non-member functions
/// @{

/// @relates Zdd
/// @ingroup Zdd
/// @brief intersection (\f$cap\f$)演算
/// @param[in] left, right オペランド
/// @return left と right の intersection
Zdd
operator&(const Zdd& left,
	  const Zdd& right);

/// @relates Zdd
/// @ingroup Zdd
/// @brief union (\f$cup\f$演算
/// @param[in] left, right オペランド
/// @return left と right の union
Zdd
operator|(const Zdd& left,
	  const Zdd& right);

/// @relates Zdd
/// @ingroup Zdd
/// @brief diff (\f$\setminus\f$)演算
/// @param[in] left, right オペランド
/// @return left と right の diff
Zdd
operator-(const Zdd& left,
	  const Zdd& right);

/// @relates Zdd
/// @ingroup Zdd
/// @brief left と right が等価でない場合 true を返す．
/// @param[in] left, right オペランド
bool
operator!=(const Zdd& left,
	   const Zdd& right);

#if 0
// left が minterms の集合として right に含まれるとき true を返す．
bool
operator<=(const Zdd& left,
	   const Zdd& right);

// left が真に right を含むとき true を返す．
bool
operator>(const Zdd& left,
	  const Zdd& right);

// left が真に right に含まれるとき true を返す．
bool
operator<(const Zdd& left,
	  const Zdd& right);
#endif

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 不等価比較
// @return 2つのZDDが等しくないとき true を返す．
inline
bool
operator!=(const Zdd& left,
	   const Zdd& right)
{
  return ! left.operator==(right);
}

// @brief intersection 演算
inline
Zdd
operator&(const Zdd& left,
	  const Zdd& right)
{
  return Zdd(left).operator&=(right);
}

// @brief union 演算
inline
Zdd
operator|(const Zdd& left,
	  const Zdd& right)
{
  return Zdd(left).operator|=(right);
}

// @brief diff 演算
inline
Zdd
operator-(const Zdd& left,
	  const Zdd& right)
{
  return Zdd(left).operator-=(right);
}

#if 0
// @brief 被包含チェック
// @return left が right に minterm 集合として包含されているときに
// true を返す．
inline
bool
operator<=(const Zdd& left,
	   const Zdd& right)
{
  return right >= left;
}

// @brief 真包含チェック
// @param left, right オペランド
// @return left が right を minterm 集合として真に包含しているときに
// true を返す．
inline
bool
operator>(const Zdd& left,
	  const Zdd& right)
{
  return (left != right) && (left >= right);
}

// @brief 真被包含チェック
// @param left, right オペランド
// @return left が right を minterm 集合として真に包含されているときに
// true を返す．
inline
bool
operator<(const Zdd& left,
	  const Zdd& right)
{
  return right > left;
}
#endif

// @brief ハッシュ関数
// @return ハッシュ値
inline
ymuint
Zdd::hash() const
{
  ympuint r = mRoot;
  return (r * r) >> 8;
}

END_NAMESPACE_YM_ZDD

BEGIN_NAMESPACE_HASH

// ZDDをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsZdd::Zdd>
{
  ymuint
  operator()(const nsYm::nsZdd::Zdd& bdd) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_LOGIC_ZDD_H
