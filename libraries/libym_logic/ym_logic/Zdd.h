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
#include "ym_logic/ZddEdge.h"
#include "ym_utils/BinIO.h"
#include "gmpxx.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @name 定数の定義
/// @{

/// @brief 葉のノードの仮想的なレベル
/// @ingroup Zdd
const tLevel kLevelMax = 0xFFFFFFFF;

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class Zdd Zdd.h "ym_logic/Zdd.h"
/// @ingroup Zdd
/// @brief BDDを表すクラス
/// @sa ZddMgr
//////////////////////////////////////////////////////////////////////
class Zdd
{
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

  /// @brief 定数0のチェック
  /// @return 定数0の時 true を返す．
  bool
  is_zero() const;

  /// @brief 定数1のチェック
  /// @return 定数1の時 true を返す．
  bool
  is_one() const;

  /// @brief 定数のチェック
  /// @return 定数の時 true を返す．
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

  /// @brief 定数0に設定する．
  void
  set_zero();

  /// @brief 定数1に設定する．
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

  /// @brief intersection 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator&=(const Zdd& src);

  /// @brief union 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator|=(const Zdd& src);

  /// @brief diff 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Zdd&
  operator-=(const Zdd& src);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ZDD の構造に関係したメソッド
  /// @{

  /// @brief 根の節点の変数に基づいてShannon展開を行なう．
  /// @param[out] f0 0枝のコファクターを入れる ZDD
  /// @param[out] f1 1枝のコファクターを入れる ZDD
  /// @return 根の節点の変数番号を返す．
  /// @note もともと定数値(葉)のZDDの場合，kVarIdMax を返し，
  /// f0, f1 には自分自身を代入する．
  tVarId
  root_decomp(Zdd& f0,
	      Zdd& f1) const;

  /// @brief 根の変数番号の取得
  /// @retval 根の変数番号 内部節点の場合
  /// @retval kVarIdMax 終端節点の場合
  tVarId
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
  /// @name その他の演算
  /// @{

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数 var を含まないコファクターを返す．
  Zdd
  cofactor0(tVarId var) const;

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @return 変数 var を含むコファクターを返す．
  Zdd
  cofactor1(tVarId var) const;

  /// @brief 変数インデックスの置き換えを行う．
  /// @param[in] var_map 置き換え元の変数から置き換え先の変数への写像
  /// @return 演算結果
  Zdd
  remap_var(const VarVarMap& var_map) const;

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
  /// @name 表示/ノード数の計数など
  /// @{

  /// @brief 内容を書き出す
  /// @param[in] s 出力ストリーム
  /// @return ノード数を返す．
  ymuint64
  print(ostream& s) const;

  /// @brief 内容のダンプ
  /// @param[in] s 出力ストリーム
  void
  dump(BinO& s) const;

  /// @brief ZDD が使っているノード数を数える．
  /// @return ZDD が使っているノード数
  ymuint64
  size() const;

  /// @brief 要素数の計算
  mpz_class
  count() const;

  /// @brief サポート変数集合の計算 (VarVector)
  /// @param[out] support サポート変数集合を格納するベクタ
  /// @return サポートの要素数
  tVarSize
  support(VarVector& support) const;

  /// @brief サポート変数集合の計算 (VarList)
  /// @param[out] support サポート変数集合を格納するリスト
  /// @return サポートの要素数
  tVarSize
  support(VarList& support) const;

  /// @brief サポート変数集合の要素数の計算
  /// @return サポート変数集合の要素数
  tVarSize
  support_size() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{

  /// @brief 等価比較演算
  /// @param[in] src2 比較対象の ZDD
  /// @retval true 自分自身と src2 が等価な場合
  /// @retval false 上記以外
  bool
  operator==(const Zdd& src2) const;

#if 0
  /// @brief 2つの関数の交叉チェック
  /// @param[in] src2 比較対象の ZDD
  /// @retval true 自分自身と src2 が交わっている時
  /// @retval false 上記以外
  bool
  operator&&(const Zdd& src2) const;

  /// @brief 包含関係のチェック
  /// @param[in] src2 比較対象の ZDD
  /// @retval true 自分自身が minterms の集合として src2 を含む時
  /// @retval false 上記以外
  bool
  operator>=(const Zdd& src2) const;
#endif

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  //  friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  ymuint64
  print(const ZddVector& array,
	ostream& s);

  friend
  ymuint64
  print(const ZddList& array,
	ostream& s);

  friend
  void
  dump(const ZddVector& array,
       BinO& s);

  friend
  void
  dump(const ZddList& array,
       BinO& s);

  friend
  ymuint64
  size(const ZddVector& array);

  friend
  ymuint64
  size(const ZddList& array);

  friend
  tVarSize
  support(const ZddVector& bdd_array,
	  VarVector& sup);

  friend
  tVarSize
  support(const ZddVector& bdd_array,
	  VarList& sup);

  friend
  tVarSize
  support_size(const ZddVector& bdd_array);

  friend
  tVarSize
  support(const ZddList& bdd_array,
	  VarVector& sup);

  friend
  tVarSize
  support(const ZddList& bdd_array,
	  VarList& sup);

  friend
  tVarSize
  support_size(const ZddList& bdd_array);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ZDD マネージャと根の枝を引数とするコンストラクタ
  /// @param[in] mgr ZDD マネージャ
  /// @param[in] e 根の枝
  Zdd(ZddMgrImpl* mgr,
      ZddEdge root);

  // 根の枝をとり出す
  ZddEdge
  root() const;

  /// @brief mgr, root をセットする時に呼ばれる関数
  /// @param[in] mgr ZDD マネージャ
  /// @param[in] root 根の枝
  void
  set(ZddMgrImpl* mgr,
      ZddEdge root);

  /// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
  void
  reset();

  /// @brief 根の枝を new_e に置き換える．
  /// @param[in] new_e 新しい枝
  /// @warning new_e も同一の ZddMgr に属していると仮定する．
  void
  assign(ZddEdge new_e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の ZddMgr
  ZddMgrImpl* mMgr;

  // 根のノードを指す枝
  ZddEdge mRoot;

  // おなじ ZddMgr の ZDD をつなぐためのリンクポインタ
  Zdd* mPrev;
  Zdd* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @name ZDD 関係の non-member functions
/// @{

/// @relates Zdd
/// @ingroup Zdd
/// @brief intersection 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の intersection
Zdd
operator&(const Zdd& src1,
	  const Zdd& src2);

/// @relates Zdd
/// @ingroup Zdd
/// @brief union 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の union
Zdd
operator|(const Zdd& src1,
	  const Zdd& src2);

/// @relates Zdd
/// @ingroup Zdd
/// @brief diff 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の diff
Zdd
operator-(const Zdd& src1,
	  const Zdd& src2);

/// @relates Zdd
/// @ingroup Zdd
/// @brief 変数への値の代入
/// @param[in] src もとの ZDD
/// @param[in] r 固定する変数を表したリテラルの集合
/// @return r に従って変数を固定した ZDD
Zdd
operator/(const Zdd& src,
	  const ZddLitSet& r);

/// src1 と src2 が等価でない場合 true を返す．
bool
operator!=(const Zdd& src1,
	   const Zdd& src2);

#if 0
// src1 が minterms の集合として src2 に含まれるとき true を返す．
bool
operator<=(const Zdd& src1,
	   const Zdd& src2);

// src1 が真に src2 を含むとき true を返す．
bool
operator>(const Zdd& src1,
	  const Zdd& src2);

// src1 が真に src2 に含まれるとき true を返す．
bool
operator<(const Zdd& src1,
	  const Zdd& src2);
#endif

// ZDDの配列の内容を書き出す
// と同時にノード数を返す．
ymuint64
print(const ZddVector& array,
      ostream& s);
ymuint64
print(const ZddList& array,
      ostream& s);

// ZDDの配列の内容を保存用に書き出す
void
dump(const ZddVector& array,
     BinO& s);

// ZDDの配列の内容を保存用に書き出す
void
dump(const ZddList& array,
     BinO& s);

// ZDDの配列のノード数を数える
ymuint64
size(const ZddVector& array);

// ZDDの配列のノード数を数える
ymuint64
size(const ZddList& array);

// ZDD のベクタのサポートを求める．
tVarSize
support(const ZddVector& bdd_array,
	VarVector& sup);

// ZDD のベクタのサポートを求める．
tVarSize
support(const ZddVector& bdd_array,
	VarList& sup);

// ZDD のベクタのサポートを求める．
tVarSize
support_size(const ZddVector& bdd_array);

// ZDD のリストのサポートを求める．
tVarSize
support(const ZddList& bdd_array,
	VarVector& sup);

// ZDD のリストのサポートを求める．
tVarSize
support(const ZddList& bdd_array,
	VarList& sup);

// ZDD のリストのサポートを求める．
tVarSize
support_size(const ZddList& bdd_array);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根の枝をとり出す
// @return 根の枝
inline
ZddEdge
Zdd::root() const
{
  return mRoot;
}

// @brief 定数0 のチェック
// @return 定数0の時 true を返す．
inline
bool
Zdd::is_zero() const
{
  return mRoot.is_zero();
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
inline
bool
Zdd::is_one() const
{
  return mRoot.is_one();
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
inline
bool
Zdd::is_const() const
{
  return mRoot.is_const();
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
inline
bool
Zdd::is_overflow() const
{
  return mRoot.is_overflow();
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
inline
bool
Zdd::is_error() const
{
  return mRoot.is_error();
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
inline
bool
Zdd::is_invalid() const
{
  return mRoot.is_invalid();
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
inline
bool
Zdd::is_leaf() const
{
  return mRoot.is_leaf();
}

// @brief 定数0に設定する．
inline
void
Zdd::set_zero()
{
  set(mMgr, ZddEdge::make_zero());
}

// @brief 定数1に設定する．
inline
void
Zdd::set_one()
{
  set(mMgr, ZddEdge::make_one());
}

// @brief エラー値に設定する．
inline
void
Zdd::set_error()
{
  set(mMgr, ZddEdge::make_error());
}

// @brief オーバーフロー値に設定する．
inline
void
Zdd::set_overflow()
{
  set(mMgr, ZddEdge::make_overflow());
}

// @brief 等価比較
// @return 2つのZDDが等しいとき true を返す．
inline
bool
Zdd::operator==(const Zdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return root() == src2.root();
}

// @brief 不等価比較
// @return 2つのZDDが等しくないとき true を返す．
inline
bool
operator!=(const Zdd& src1,
	   const Zdd& src2)
{
  return ! src1.operator==(src2);
}

#if 0
// @brief 被包含チェック
// @return src1 が src2 に minterm 集合として包含されているときに
// true を返す．
inline
bool
operator<=(const Zdd& src1,
	   const Zdd& src2)
{
  return src2 >= src1;
}

// @brief 真包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含しているときに
// true を返す．
inline
bool
operator>(const Zdd& src1,
	  const Zdd& src2)
{
  return (src1 != src2) && (src1 >= src2);
}

// @brief 真被包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含されているときに
// true を返す．
inline
bool
operator<(const Zdd& src1,
	  const Zdd& src2)
{
  return src2 > src1;
}
#endif

// @brief リテラルによるコファクター演算
// @param literal コファクタリングの変数番号と極性を組にしたもの
// @return コファクター
inline
Zdd
Zdd::cofactor(const Literal& literal) const
{
  return cofactor(literal.varid(), literal.pol());
}

// @brief ハッシュ関数
// @return ハッシュ値
inline
ymuint
Zdd::hash() const
{
  ympuint r = mRoot.mBody;
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
