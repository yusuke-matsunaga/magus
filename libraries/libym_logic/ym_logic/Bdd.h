#ifndef YM_LOGIC_BDD_H
#define YM_LOGIC_BDD_H

/// @file ym_logic/Bdd.h
/// @brief Bdd のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"
#include "ym_logic/BddEdge.h"
#include "ym_logic/VarId.h"
#include "ym_logic/LogExpr.h"
#include "ym_utils/BinIO.h"
#include "gmpxx.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
/// @name 定数の定義
/// @{

/// @brief 葉のノードの仮想的なレベル
/// @ingroup Bdd
const ymuint kLevelMax = 0xFFFFFFFF;

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/// @class Bdd Bdd.h "ym_logic/Bdd.h"
/// @ingroup Bdd
/// @brief BDDを表すクラス
/// @sa BddMgr
//////////////////////////////////////////////////////////////////////
class Bdd
{
  friend class BddMgr;
  friend class BddMgrImpl;

public:

  /// @brief デフォルトのコンストラクタ
  /// @note デフォルトの BDD マネージャの定数0となる
  Bdd();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Bdd(const Bdd& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const Bdd&
  operator=(const Bdd& src);

  /// @brief デストラクタ
  /// @note 根の枝のリンク数を一つ減らして，0になった時には実際に
  /// ノードの解放を行なう．
  /// @note エラーは起こらない．
  ~Bdd();


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

  /// @brief 正リテラルのみのキューブのチェック
  /// @return 表している関数が正リテラルのみのキューブ (積項)
  /// の場合に true を返す．
  bool
  is_posi_cube() const;

  /// @brief キューブのチェック
  /// @return 表している関数がキューブ (積項) の場合に true を返す．
  bool
  is_cube() const;

  /// @brief 対称性のチェック
  /// @param[in] x, y 変数番号
  /// @param[in] pol 極性
  /// @return 変数xとyが対称(交換可能)な時に true を返す．
  /// ただし pol = kPolNega の時には x(または y) を反転して
  /// 交換したときに等しくなるかどうかチェックする．
  bool
  check_symmetry(VarId x,
		 VarId y,
		 tPol pol = kPolPosi) const;

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

  /// @brief 否定の論理を計算する
  /// @return 否定した論理
  Bdd
  operator~() const;

  /// @brief 否定を計算して代入する．
  /// @return 自分自身
  const Bdd&
  negate();

  /// @brief AND 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator&=(const Bdd& src);

  /// @brief OR 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator|=(const Bdd& src);

  /// @brief XOR 付き代入
  /// @param[in] src オペランド
  /// @return 自分自身
  const Bdd&
  operator^=(const Bdd& src);

  /// @brief 変数の拘束付き代入
  /// @param[in] r 固定する変数を表したリテラルの集合
  /// @return 自分自身
  const Bdd&
  operator/=(const BddLitSet& r);

  /// @brief generalized cofactor 付き代入
  /// @param[in] c 制約を表す BDD
  /// @return 自分自身
  /// @warning c を定数0にしてはいけない
  const Bdd&
  operator/=(const Bdd& c);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name BDD の構造に関係したメソッド
  /// @{

  /// @brief 根の節点の変数に基づいてShannon展開を行なう．
  /// @param[out] f0 0枝のコファクターを入れる BDD
  /// @param[out] f1 1枝のコファクターを入れる BDD
  /// @return 根の節点の変数番号を返す．
  /// @note もともと定数値(葉)のBDDの場合，kVarIdMax を返し，
  /// f0, f1 には自分自身を代入する．
  VarId
  root_decomp(Bdd& f0,
	      Bdd& f1) const;

  /// @brief 根の変数番号の取得
  /// @retval 根の変数番号 内部節点の場合
  /// @retval kVarIdMax 終端節点の場合
  VarId
  root_var() const;

  /// @brief 0枝の指しているコファクターの取得
  /// @retval 0枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Bdd
  edge0() const;

  /// @brief 1枝の指しているコファクターの取得
  /// @retval 1枝の指しているコファクター 内部節点の場合
  /// @retval 自分自身 終端節点の場合
  Bdd
  edge1() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name その他の演算
  /// @{

  /// @brief コファクター演算
  /// @param[in] var 変数番号
  /// @param[in] pol 極性
  /// @return 変数 var の極性 pol 側のコファクターを返す．
  Bdd
  cofactor(VarId var,
	   tPol pol) const;

  /// @brief コファクター演算
  /// @param[in] literal リテラル
  /// @return literal によるコファクターを返す．
  Bdd
  cofactor(const Literal& literal) const;

  /// @brief Davio展開のモーメント項の計算
  /// @param[in] idx 変数番号
  /// @return Davio 展開のモーメント項 (\f$f_{\overline{x}} \oplus f_x\f$)
  /// を返す．
  Bdd
  xor_moment(VarId idx) const;

  /// @brief Smallest Cube Containg F を求める．
  /// @return SCC を返す．
  Bdd
  SCC() const;

  /// @brief compose 演算
  /// @param[in] var 置き換える対象の変数
  /// @param[in] g 置き換え先の BDD
  /// @return 演算結果
  Bdd
  compose(VarId var,
	  const Bdd& g) const;

  /// @brief 複数変数の compose 演算
  /// @param[in] comp_map 置き換える対象の変数から置き換え先の BDD への写像
  /// @return 演算結果
  Bdd
  compose(const VarBddMap& comp_map) const;

  /// @brief 変数インデックスの置き換えを行う．
  /// @param[in] var_map 置き換え元の変数から置き換え先の変数への写像
  /// @return 演算結果
  Bdd
  remap_var(const VarVarMap& var_map) const;

  /// @brief e-smoothing(elimination)
  /// @param[in] svars 消去する変数の集合
  /// @return 演算結果
  Bdd
  esmooth(const BddVarSet& svars) const;

  /// @brief a-smoothing(elimination)
  /// @param[in] svars 消去する変数の集合
  /// @return 演算結果
  Bdd
  asmooth(const BddVarSet& svars) const;

  /// @brief x_level の変数を y_level まで「押し込む」
  /// @param[in] x_level 元の変数のレベル
  /// @param[in] y_level 移動先の変数のレベル
  /// @param[in] pol 極性
  /// @return 演算結果
  /// @note pol が kPolNega の場合には 0-枝と 1-枝を取り替える．
  /// @note 他の関数と異なり変数番号ではなくレベルで指定する．
  /// @warning x_level < y_level でないときはエラーとなる．
  Bdd
  push_down(ymuint x_level,
	    ymuint y_level,
	    tPol pol = kPolPosi) const;

  /// @brief BDD の内容を積和形論理式に変換する．
  /// @return 等価な積和形論理式
  LogExpr
  sop() const;

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
  display(ostream& s) const;

  /// @brief BDD が表す関数のカルノー図を表示する
  /// @param[in] s 主力ストリーム
  /// @warning ただし4変数以内
  void
  display_map(ostream& s) const;

  /// @brief BDD の内容を積和形論理式の形で出力する．
  /// @param[in] s 出力ストリーム
  void
  display_sop(ostream& s) const;

  /// @brief 内容のダンプ
  /// @param[in] s 出力ストリーム
  void
  dump(BinO& s) const;

  /// @brief BDD が使っているノード数を数える．
  /// @return BDD が使っているノード数
  ymuint64
  size() const;

  /// @brief 真理値表密度の計算
  /// @param[in] nvar 入力数
  double
  density(ymuint nvar) const;

  /// @brief 最小項の数の計算
  /// @param[in] nvar 入力数
  /// @return 最小項の数
  mpz_class
  minterm_count(ymuint nvar) const;

  /// @brief Walsh変換の 0次係数の計算
  /// @param[in] nvar 入力数
  /// @return Walsh変換の 0次係数
  mpz_class
  walsh0(ymuint nvar) const;

  /// @brief Walsh変換の 1次係数の計算
  /// @param[in] var 変数番号
  /// @param[in] nvar 入力数
  /// @return 変数 var の 1次係数
  mpz_class
  walsh1(VarId var,
	 ymuint nvar) const;

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

  /// @brief サポート変数集合の計算 (BddVarSet)
  /// @return サポート変数集合
  BddVarSet
  support() const;

  /// @brief サポート変数集合の要素数の計算
  /// @return サポート変数集合の要素数
  ymuint
  support_size() const;

  /// @brief 1パスの探索
  /// @return 1パス ('1' の終端へ至る経路) を表すリテラル集合
  BddLitSet
  onepath() const;

  /// @brief 最短の 1パスの探索
  /// @return 最短の 1パス ('1' の終端へ至る経路) を表すリテラル集合
  BddLitSet
  shortest_onepath() const;

  /// @brief 最短の 1パスの長さの取得
  /// @return 最短の 1パスの長さ
  ymuint
  shortest_onepath_len() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 比較演算
  /// @{

  /// @brief 等価比較演算
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身と src2 が等価な論理関数を表している場合
  /// @retval false 上記以外
  bool
  operator==(const Bdd& src2) const;

  /// @brief 2つの関数の交叉チェック
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身と src2 が交わっている時
  /// @retval false 上記以外
  bool
  operator&&(const Bdd& src2) const;

  /// @brief 包含関係のチェック
  /// @param[in] src2 比較対象の BDD
  /// @retval true 自分自身が minterms の集合として src2 を含む時
  /// @retval false 上記以外
  bool
  operator>=(const Bdd& src2) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name リテラル集合関係の関数
  /// @{

  /// @brief BddLitSet からリテラルベクタへの変換
  /// @param[out] dst リテラルを格納するベクタ
  /// @return 要素数
  /// @note 自分自身がリテラル集合を表している場合に内容をベクタに変換する．
  ymuint
  to_literalvector(LiteralVector& dst) const;

  /// @brief BddLitSet からリテラルリストへの変換
  /// @param[out] dst リテラルを格納するリスト
  /// @return 要素数
  /// @note 自分自身がリテラル集合を表している場合に内容をリストに変換する．
  ymuint
  to_literallist(LiteralList& dst) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  //  friend 関数の宣言
  //////////////////////////////////////////////////////////////////////
  friend
  Bdd
  ite_op(const Bdd& cond,
	 const Bdd& s,
	 const Bdd& t);

  friend
  Bdd
  and_exist(const Bdd& src1,
	    const Bdd& src2,
	    const BddVarSet& svars);

  friend
  Bdd
  isop(const Bdd& lower,
       const Bdd& upper,
       LogExpr& cover);

  friend
  LogExpr
  prime_cover(const Bdd& lower,
	      const Bdd& upper);

  friend
  Bdd
  minimal_support(const Bdd& lower,
		  const Bdd& upper);

  friend
  ymuint64
  display(const BddVector& array,
	  ostream& s);

  friend
  ymuint64
  display(const BddList& array,
	  ostream& s);

  friend
  void
  dump(BinO& s,
       const BddVector& array);

  friend
  void
  dump(BinO& s,
       const BddList& array);

  friend
  ymuint64
  size(const BddVector& array);

  friend
  ymuint64
  size(const BddList& array);

  friend
  ymuint
  support(const BddVector& bdd_array,
	  VarVector& sup);

  friend
  ymuint
  support(const BddVector& bdd_array,
	  VarList& sup);

  friend
  BddVarSet
  support(const BddVector& bdd_array);

  friend
  ymuint
  support_size(const BddVector& bdd_array);

  friend
  ymuint
  support(const BddList& bdd_array,
	  VarVector& sup);

  friend
  ymuint
  support(const BddList& bdd_array,
	  VarList& sup);

  friend
  BddVarSet
  support(const BddList& bdd_array);

  friend
  ymuint
  support_size(const BddList& bdd_array);

  friend
  Bdd
  vscap(const Bdd& src1,
	const Bdd& src2);

  friend
  Bdd
  vsdiff(const Bdd& src1,
	 const Bdd& src2);

  friend
  bool
  vsintersect(const Bdd& src1,
	      const Bdd& src2);

  friend
  Bdd
  lscap(const Bdd& src1,
	const Bdd& src2);

  friend
  Bdd
  lsdiff(const Bdd& src1,
	 const Bdd& src2);

  friend
  bool
  lsintersect(const Bdd& src1,
	      const Bdd& src2);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD マネージャと根の枝を引数とするコンストラクタ
  /// @param[in] mgr BDD マネージャ
  /// @param[in] e 根の枝
  Bdd(BddMgrImpl* mgr,
      BddEdge root);

  // 根の枝をとり出す
  BddEdge
  root() const;

  /// @brief mgr, root をセットする時に呼ばれる関数
  /// @param[in] mgr BDD マネージャ
  /// @param[in] root 根の枝
  void
  set(BddMgrImpl* mgr,
      BddEdge root);

  /// @brief (mMgr, mRoot) への参照をなくす時に呼ばれる関数
  void
  reset();

  /// @brief 根の枝を new_e に置き換える．
  /// @param[in] new_e 新しい枝
  /// @warning new_e も同一の BddMgr に属していると仮定する．
  void
  assign(BddEdge new_e);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の BddMgr
  BddMgrImpl* mMgr;

  // 根のノードを指す枝
  BddEdge mRoot;

  // おなじ BddMgr の BDD をつなぐためのリンクポインタ
  Bdd* mPrev;
  Bdd* mNext;

};


//////////////////////////////////////////////////////////////////////
/// @name BDD 関係の non-member functions
/// @{

/// @relates Bdd
/// @ingroup Bdd
/// @brief AND 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の AND
Bdd
operator&(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief OR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の OR
Bdd
operator|(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief XOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の XOR
Bdd
operator^(const Bdd& src1,
	  const Bdd& src2);

/// @relates Bdd
/// @ingroup Bdd
/// @brief 変数への値の代入
/// @param[in] src もとの BDD
/// @param[in] r 固定する変数を表したリテラルの集合
/// @return r に従って変数を固定した BDD
Bdd
operator/(const Bdd& src,
	  const BddLitSet& r);

/// @relates Bdd
/// @ingroup Bdd
/// @brief generalized cofactor の計算
/// @param[in] src もとの BDD
/// @param[in] c 制約を表す BDD
/// @return c に従ってコファクタリングした BDD
/// @warning c を定数0にしてはいけない
Bdd
operator/(const Bdd& src,
	  const Bdd& c);

/// src1 と src2 が等価でない場合 true を返す．
bool
operator!=(const Bdd& src1,
	   const Bdd& src2);

// src1 が minterms の集合として src2 に含まれるとき true を返す．
bool
operator<=(const Bdd& src1,
	   const Bdd& src2);

// src1 が真に src2 を含むとき true を返す．
bool
operator>(const Bdd& src1,
	  const Bdd& src2);

// src1 が真に src2 に含まれるとき true を返す．
bool
operator<(const Bdd& src1,
	  const Bdd& src2);

// if-then-else 演算．
// condが真の時はsを，偽の時はtを選ぶ
Bdd
ite_op(const Bdd& cond,
       const Bdd& s,
       const Bdd& t);

// src1 と src2 の論理積を計算しつつスムージングを行なう．
Bdd
and_exist(const Bdd& src1,
	  const Bdd& src2,
	  const BddVarSet& svars);

// ISOP の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// 非冗長積和形表現を一つ求め, cover にセットする．
// その論理関数を返す．
Bdd
isop(const Bdd& lower,
     const Bdd& upper,
     LogExpr& cover);

// prime-cover の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// すべての prime implicant からなる prime cover を求める．
LogExpr
prime_cover(const Bdd& lower,
	    const Bdd& upper);

// minimum support の生成
// lower と upper で指定された不完全指定論理関数に含まれる関数の
// 極小サポート集合をもとめる．
Bdd
minimal_support(const Bdd& lower,
		const Bdd& upper);

// BDDの配列の内容を書き出す
// と同時にノード数を返す．
ymuint64
display(const BddVector& array,
	ostream& s);
ymuint64
display(const BddList& array,
	ostream& s);

// BDDの配列の内容を保存用に書き出す
void
dump(BinO& s,
     const BddVector& array);

// BDDの配列の内容を保存用に書き出す
void
dump(BinO& s,
     const BddList& array);

// BDDの配列のノード数を数える
ymuint64
size(const BddVector& array);

// BDDの配列のノード数を数える
ymuint64
size(const BddList& array);

// BDD のベクタのサポートを求める．
ymuint
support(const BddVector& bdd_array,
	VarVector& sup);

// BDD のベクタのサポートを求める．
ymuint
support(const BddVector& bdd_array,
	VarList& sup);

// BDD のベクタのサポートを求める．
BddVarSet
support(const BddVector& bdd_array);

// BDD のベクタのサポートを求める．
ymuint
support_size(const BddVector& bdd_array);

// BDD のリストのサポートを求める．
ymuint
support(const BddList& bdd_array,
	VarVector& sup);

// BDD のリストのサポートを求める．
ymuint
support(const BddList& bdd_array,
	VarList& sup);

// BDD のリストのサポートを求める．
BddVarSet
support(const BddList& bdd_array);

// BDD のリストのサポートを求める．
ymuint
support_size(const BddList& bdd_array);

// src1 と src2 が変数集合の時に共通部分を求める．
Bdd
vscap(const Bdd& src1,
      const Bdd& src2);

// src1 と src2 が変数集合の時に集合差を求める．
Bdd
vsdiff(const Bdd& src1,
       const Bdd& src2);

// src1 と src2 が変数集合の時のインターセクションチェック
bool
vsintersect(const Bdd& src1,
	    const Bdd& src2);

// src1 と src2 がリテラル集合の時に共通部分を求める．
Bdd
lscap(const Bdd& src1,
      const Bdd& src2);

// src1 と src2 がリテラル集合の時に集合差を求める．
Bdd
lsdiff(const Bdd& src1,
       const Bdd& src2);

// src1 と src2 がリテラル集合の時のインターセクションチェック
bool
lsintersect(const Bdd& src1,
	    const Bdd& src2);

/// @}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 根の枝をとり出す
// @return 根の枝
inline
BddEdge
Bdd::root() const
{
  return mRoot;
}

// @brief 定数0 のチェック
// @return 定数0の時 true を返す．
inline
bool
Bdd::is_zero() const
{
  return mRoot.is_zero();
}

// @brief 定数1 のチェック
// @return 定数1 の時 true を返す．
inline
bool
Bdd::is_one() const
{
  return mRoot.is_one();
}

// @brief 定数のチェック
// @return 定数の時 true を返す．
inline
bool
Bdd::is_const() const
{
  return mRoot.is_const();
}

// @brief オーバーフローのチェック
// @return 演算結果がオーバーフローしたとき true を返す．
inline
bool
Bdd::is_overflow() const
{
  return mRoot.is_overflow();
}

// @brief エラーのチェック
// @return 演算結果がエラーになったとき true を返す．
inline
bool
Bdd::is_error() const
{
  return mRoot.is_error();
}

// @brief オーバーフローとエラーのチェック
// @return 演算結果がオーバーフローかエラーのとき true を返す．
inline
bool
Bdd::is_invalid() const
{
  return mRoot.is_invalid();
}

// @brief 終端ノードのチェック
// @return 終端ノードのとき true を返す．
inline
bool
Bdd::is_leaf() const
{
  return mRoot.is_leaf();
}

// @brief 定数0に設定する．
inline
void
Bdd::set_zero()
{
  set(mMgr, BddEdge::make_zero());
}

// @brief 定数1に設定する．
inline
void
Bdd::set_one()
{
  set(mMgr, BddEdge::make_one());
}

// @brief エラー値に設定する．
inline
void
Bdd::set_error()
{
  set(mMgr, BddEdge::make_error());
}

// @brief オーバーフロー値に設定する．
inline
void
Bdd::set_overflow()
{
  set(mMgr, BddEdge::make_overflow());
}

// @brief 等価比較
// @return 2つのBDDが等しいとき true を返す．
inline
bool
Bdd::operator==(const Bdd& src2) const
{
  // 実はただのポインタ（スカラ値）比較でわかる．
  return root() == src2.root();
}

// @brief 不等価比較
// @return 2つのBDDが等しくないとき true を返す．
inline
bool
operator!=(const Bdd& src1,
	   const Bdd& src2)
{
  return ! src1.operator==(src2);
}

// @brief 被包含チェック
// @return src1 が src2 に minterm 集合として包含されているときに
// true を返す．
inline
bool
operator<=(const Bdd& src1,
	   const Bdd& src2)
{
  return src2 >= src1;
}

// @brief 真包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含しているときに
// true を返す．
inline
bool
operator>(const Bdd& src1,
	  const Bdd& src2)
{
  return (src1 != src2) && (src1 >= src2);
}

// @brief 真被包含チェック
// @param src1, src2 オペランド
// @return src1 が src2 を minterm 集合として真に包含されているときに
// true を返す．
inline
bool
operator<(const Bdd& src1,
	  const Bdd& src2)
{
  return src2 > src1;
}

// @brief リテラルによるコファクター演算
// @param literal コファクタリングの変数番号と極性を組にしたもの
// @return コファクター
inline
Bdd
Bdd::cofactor(const Literal& literal) const
{
  return cofactor(literal.varid(), literal.pol());
}

// @brief ハッシュ関数
// @return ハッシュ値
inline
ymuint
Bdd::hash() const
{
  ympuint r = mRoot.mBody;
  return (r * r) >> 8;
}

// @brief 積和系論理式の出力
// @param s 出力ストリーム
// BDD の内容を積和形論理式の形で s に出力する．
inline
void
Bdd::display_sop(ostream& s) const
{
  s << sop() << endl;
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH

// BDDをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::Bdd>
{
  ymuint
  operator()(const nsYm::nsBdd::Bdd& bdd) const
  {
    return bdd.hash();
  }
};

END_NAMESPACE_HASH

#endif // YM_LOGIC_BDD_H
