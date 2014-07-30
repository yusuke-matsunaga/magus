#ifndef YMYMLOGIC_EXPR_H
#define YMYMLOGIC_EXPR_H

/// @file YmLogic/Expr.h
/// @brief Expr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/expr_nsdef.h"
#include "YmLogic/Literal.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM

// クラス名の先行宣言
class TvFunc;

END_NAMESPACE_YM

BEGIN_NAMESPACE_YM_EXPR

// クラス名の先行宣言
class ExprNode;

//////////////////////////////////////////////////////////////////////
/// @class Expr Expr.h "YmLogic/Expr.h"
/// @ingroup ExprGroup
/// @brief 論理式を表すクラス
///
/// 論理式は
/// - 定数0, 定数1
/// - リテラル (変数番号 + 極性)
///
/// および, 複数の論理式を AND/OR/XOR でまとめたものから構成される
///
/// このクラスでは表現としての論理式を扱っているので論理関数としての
/// 処理は行っていない．
/// @sa VarId, Literal
//////////////////////////////////////////////////////////////////////
class Expr
{
  friend class ExprMgr;

public:
  //////////////////////////////////////////////////////////////////////
  /// @name コンストラクタ/デストラクタ/代入演算子/生成用クラスメソッド
  /// @{

  /// @brief 空のデフォルトコンストラクタ
  ///
  /// - 値は定数0になる
  /// - ただし意識して定数0を作りたいなら MakeConst0() を使うこと．
  Expr();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  Expr(const Expr& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const Expr&
  operator=(const Expr& src);

  /// @brief デストラクタ
  ~Expr();

  /// @brief 入力ストリームを読んで論理式に変換する．
  /// @param[in] istr 入力ストリーム
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 変換が成功したら true を返す．
  /// @note 結果を自分自身に代入する．
  /// @note エラーが起きたら err_msg にエラーメッセージを設定して false を返す．
  bool
  read_from_stream(istream& istr,
		   string& err_msg);

  /// @brief 論理式をパーズして Expr オブジェクトを作る．
  /// @param[in] in 入力ストリーム
  /// @param[out] err_msg エラーメッセージを格納する文字列
  /// @return 変換された Expr オブジェクト
  /// @note エラーが起きたら msg にエラーメッセージをセットする．
  /// @note このインターフェイスでは err_msg が空かどうかでエラーを判断するしかない．
  static
  Expr
  stream_to_expr(istream& in,
		 string& err_msg);

  /// @brief 恒偽関数の生成
  /// @return 生成したオブジェクト
  static
  Expr
  make_zero();

  /// @brief 恒真関数の生成
  /// @return 生成したオブジェクト
  static
  Expr
  make_one();

  /// @brief リテラル式の生成
  /// @param[in] varid 変数番号
  /// @param[in] int 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return 生成したオブジェクト
  static
  Expr
  make_literal(VarId varid,
	       bool inv);

  /// @brief リテラル式の生成
  /// @param[in] lit リテラル
  /// @return 生成したオブジェクト
  static
  Expr
  make_literal(const Literal& lit);

  /// @brief 正(肯定)リテラル式の生成
  /// @param[in] varid 変数番号
  /// @return 生成したオブジェクト
  static
  Expr
  make_posiliteral(VarId varid);

  /// @brief 負(否定)リテラル式の生成
  /// @param[in] varid 変数番号
  /// @return 生成したオブジェクト
  static
  Expr
  make_negaliteral(VarId varid);

  /// @brief AND 式の生成
  /// @param[in] chd_list オペランドのベクタ
  /// @return chd_list を部分論理式に持つ AND 式を生成し，返す．
  static
  Expr
  make_and(const ExprVector& chd_list);

  /// @brief AND 式の生成
  /// @param[in] chd_list オペランドのリスト
  /// @return chd_list を部分論理式に持つ AND 式を生成し，返す．
  static
  Expr
  make_and(const ExprList& chd_list);

  /// @brief OR 式の生成
  /// @param[in] chd_list オペランドのベクタ
  /// @return chd_list を部分論理式に持つ OR 式を生成し，返す．
  static
  Expr
  make_or(const ExprVector& chd_list);

  /// @brief OR 式の生成
  ///
  /// chd_list を部分論理式に持つ OR を返す．
  /// @param[in] chd_list オペランドのリスト
  /// @return chd_list を部分論理式に持つ OR 式を生成し，返す．
  static
  Expr
  make_or(const ExprList& chd_list);

  /// @brief XOR 式の生成
  /// @param[in] chd_list オペランドのベクタ
  /// @return chd_list を部分論理式に持つ XOR 式を生成し，返す．
  static
  Expr
  make_xor(const ExprVector& chd_list);

  /// @brief XOR 式の生成
  /// @param[in] chd_list オペランドのリスト
  /// @return chd_list を部分論理式に持つ XOR 式を生成し，返す．
  static
  Expr
  make_xor(const ExprList& chd_list);

  /// @brief 確保していたメモリを開放する．
  /// @note メモリリークチェックのための関数なので通常は使用しない．
  static
  void
  clear_memory();

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 論理演算および論理演算＋代入演算
  /// @{

  /// @brief 論理否定
  /// @return 自分自身を否定した形の論理式を返す．
  Expr
  operator~() const;

  /// @brief AND つき代入
  /// @param[in] src オペランド
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の AND を計算し自分に代入する．
  const Expr&
  operator&=(const Expr& src);

  /// @brief OR つき代入
  /// @param[in] src オペランド
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の OR を計算し自分に代入する．
  const Expr&
  operator|=(const Expr& src);

  /// @brief XOR つき代入
  /// @param[in] src オペランド
  /// @return 自分自身
  ///
  /// 自分の論理式と src の論理式の XOR を計算し自分に代入する．
  const Expr&
  operator^=(const Expr& src);

  /// @brief compose 演算
  /// @param[in] varid 置き換え対象の変数番号
  /// @param[in] sub varid を置き換える先の論理式
  /// @return varid 番目の終端ノードを sub に置き換えたものを返す．
  ///
  /// - sub の論理式の中に varid 番目のリテラルがあっても
  /// 展開はされない．
  /// -もしも自分自身の論理式の中に varid 番目のリテラル
  /// が含まれない場合にはなにも変わらない．
  Expr
  compose(VarId varid,
	  const Expr& sub) const;

  /// @brief 複数変数の compose 演算
  /// @param[in] comp_map 置き換える変数をキーにして置き換える先の
  /// 論理式を値とした連想配列
  /// @return comp_map にしたがって置き換えを行った論理式
  ///
  /// 一度に複数の置き換えを行う
  Expr
  compose(const VarExprMap& comp_map) const;

  /// @brief 変数番号を再マップする．
  /// @param[in] varmap 置き換え元の変数番号をキーとして
  /// 置き換え先の変数番号を値とした連想配列
  /// @return 置き換えた論理式
  ///
  /// varmap に登録されていない場合には不変とする．
  Expr
  remap_var(const VarVarMap& varmap) const;

  /// @brief 簡単化
  /// - expr + expr = expr
  /// - expr + ~expr = 1
  /// のような簡単なルールで簡単かを行う
  /// 自分自身が簡単化された式に置き換わる．
  /// @return 自分自身
  const Expr&
  simplify();

  /// @brief 値の評価
  /// @param[in] vals 変数の値割り当て
  /// @param[in] mask 使用するビットのためのマスク
  /// @return 評価値
  /// @note 演算はビット毎に独立に行われる．
  ymulong
  eval(const vector<ymulong>& vals,
       ymulong mask = ~0UL) const;

  /// @brief 真理値表の作成
  /// @param[in] ni 入力数
  /// @note ni が省略された場合には input_size() が用いられる．
  TvFunc
  make_tv(ymuint ni = 0) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name 根本の演算子の情報を得る．
  /// @{

  /// @brief 恒偽関数のチェック
  /// @return 恒偽関数を表している時に true を返す．
  bool
  is_zero() const;

  /// @brief 恒真関数のチェック
  /// @return 恒真関数を表している時に true を返す．
  bool
  is_one() const;

  /// @brief 定数関数のチェック
  /// @return 定数関数)(恒偽/恒真)を表している時に true を返す．
  bool
  is_constant() const;

  /// @brief 肯定のリテラルのチェック
  /// @return 肯定のリテラルを表している時に true を返す．
  bool
  is_posiliteral() const;

  /// @brief 否定のリテラルのチェック
  /// @return 否定のリテラルを表している時に true を返す．
  bool
  is_negaliteral() const;

  /// @brief リテラルのチェック
  /// @return リテラルを表している時に true を返す．
  bool
  is_literal() const;

  /// @brief リテラルの変数番号の取得
  /// @retval 変数番号 リテラルの場合
  /// @retval kVarMaxId リテラル以外
  VarId
  varid() const;

  /// @brief AND 式のチェック
  /// @return トップが AND の時に true を返す．
  bool
  is_and() const;

  /// @brief OR 式のチェック
  /// @return トップが OR の時に true を返す．
  bool
  is_or() const;

  /// @brief XOR 式のチェック
  /// @return トップが XOR の時に true を返す．
  bool
  is_xor() const;

  /// @brief 演算子のチェック
  /// @return トップが演算子の時に true を返す．
  bool
  is_op() const;

  /// @brief オペランドの個数の取得
  /// @retval オペランドの個数 AND/OR/XOR の場合
  /// @retval 0 上記以外の場合
  ymuint
  child_num() const;

  /// @brief オペランドの取得
  /// @param[in] pos 取り出すオペランドの位置 (最初の位置は 0)
  /// @return pos 番目のオペランドを返す．
  /// ただし pos が範囲外の場合と演算子ノードでなかった場合には
  /// 0を表す式を返す．
  Expr
  child(ymuint pos) const;

  /// @brief "シンプル"な論理式のチェック
  /// @return シンプルな論理式のときに true を返す．
  ///
  /// より具体的には 0, 1, リテラルとすべての子供が
  /// リテラルであるようなの AND/OR/XOR 式を"シンプル"と
  /// 定義している．
  bool
  is_simple() const;

  /// @brief "シンプル"な AND 式のチェック
  /// @return すべての子供がリテラルであるような AND 式のときに
  /// true を返す．
  bool
  is_simple_and() const;

  /// @brief "シンプル"な OR 式のチェック
  /// @return すべての子供がリテラルであるような OR 式のときに
  /// true を返す．
  bool
  is_simple_or() const;

  /// @brief "シンプル"な XOR 式のチェック
  /// @return すべての子供がリテラルであるような XOR 式のときに
  /// true を返す．
  bool
  is_simple_xor() const;

  /// @brief 積和形論理式のチェック
  /// @return 1段目が OR, 2段目が AND の論理式のときに true を返す．
  bool
  is_sop() const;

  /// @}
  //////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////
  /// @name リテラル数の計算
  /// @{

  /// @brief リテラル数の取得
  /// @return リテラル数
  ymuint
  litnum() const;

  /// @brief 変数の出現回数の取得
  /// @param[in] varid 変数番号
  /// @return varid 番めの変数のリテラルの出現回数を得る．
  ymuint
  litnum(VarId varid) const;

  /// @brief リテラルの出現回数の取得
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return varid 番めの変数の極性が inv のリテラルの出現回数を得る．
  ymuint
  litnum(VarId varid,
	 bool inv) const;

  /// @brief リテラルの出現回数の取得
  /// @param[in] lit リテラル
  /// @return lit のリテラルの出現回数を得る．
  ymuint
  litnum(const Literal& lit) const;

  /// @brief 使われている変数の最大の番号 + 1を得る．
  ymuint
  input_size() const;

  /// @brief SOP形式に展開したときのキューブ数の見積もり
  /// @return SOP形式のキューブ (積項) 数
  ymuint
  sop_cubenum() const;

  /// @brief SOP形式に展開した時のリテラル数の見積もり
  /// @return SOP形式のリテラル数
  ymuint
  sop_litnum() const;

  /// @brief SOP形式に展開した時の変数の出現回数の見積もり
  /// @param[in] varid 変数番号
  /// @return SOP形式の varid 番めの変数のリテラルの出現回数
  ymuint
  sop_litnum(VarId varid) const;

  /// @brief SOP形式に展開した時のテラルの出現回数の見積もり
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return SOP形式に展開した時の varid 番めの変数の極性が
  /// inv のリテラルの出現回数
  ymuint
  sop_litnum(VarId varid,
	     bool inv) const;

  /// @brief SOP形式に展開したときのリテラルの出現回数の見積もり
  /// @param[in] lit リテラル
  /// @return SOP形式に展開したときの lit のリテラルの出現回数
  ymuint
  sop_litnum(const Literal& lit) const;

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 内部状態を得るクラスメソッド
  /// @{

  /// @brief 使用されているメモリ量を返す．
  static
  ymuint
  used_size();

  /// @brief 現在使用中のノード数を返す．
  static
  ymuint
  node_num();

  /// @brief used_size() の最大値を返す．
  static
  ymuint
  max_used_size();

  /// @brief nodenum() の最大値を返す．
  static
  ymuint
  max_node_num();

  /// @brief 実際に確保したメモリ量を返す．
  static
  ymuint
  allocated_size();

  /// @brief 実際に確保した回数を返す．
  static
  ymuint
  allocated_count();

  /// @brief 内部状態を出力する．
  static
  void
  print_stats(ostream& s);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  // friend 関数の宣言
  //////////////////////////////////////////////////////////////////////

  friend
  Expr
  operator&(const Expr& src1,
	    const Expr& src2);

  friend
  Expr
  operator|(const Expr& src1,
	    const Expr& src2);

  friend
  Expr
  operator^(const Expr& src1,
	    const Expr& src2);

  friend
  bool
  compare_type(const Expr& src1,
	       const Expr& src2);


private:

  // @brief 内部で用いるコンストラクタ
  // @param[in] node 根のノード
  // node が 0 の場合には abort する．
  Expr(const ExprNode* node);

  // 根のノードをセットする．
  void
  set_root(const ExprNode* node);

  /// @brief 根のノートを得る．
  const ExprNode*
  root() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const ExprNode* mRootPtr;

};


//////////////////////////////////////////////////////////////////////
// クラスに属さない関数の宣言
//////////////////////////////////////////////////////////////////////

/// @name 論理演算
/// @{

/// @relates Expr
/// @brief AND 演算
/// @param[in] src1, src2 オペランド
/// @return src1 の論理式と src2 の論理式の AND を返す．
Expr
operator&(const Expr& src1,
	  const Expr& src2);

/// @relates Expr
/// @brief OR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 の論理式と src2 の論理式の OR を返す．
Expr
operator|(const Expr& src1,
	  const Expr& src2);

/// @relates Expr
/// @brief XOR 演算
/// @param[in] src1, src2 オペランド
/// @return src1 の論理式と src2 の論理式の XOR を返す．
Expr
operator^(const Expr& src1,
	  const Expr& src2);

/// @relates Expr
/// @brief 根の演算タイプの比較
/// @param[in] src1, src2 オペランド
/// @return src1 と src2 の根のタイプが同じとき true を返す．
bool
compare_type(const Expr& src1,
	     const Expr& src2);

/// @}

/// @relates Expr
/// @brief 論理式の内容のストリーム出力
/// @param[in] s 出力ストリーム
/// @param[in] expr 論理式
/// @return s
ostream&
operator<<(ostream& s,
	   const Expr& expr);

/// @relates Expr
/// @brief 論理式の内容のバイナリ出力
/// @param[in] s 出力ストリーム
/// @param[in] expr 論理式
/// @return s
ODO&
operator<<(ODO& s,
	   const Expr& expr);

/// @relates Expr
/// @brief 論理式の内容のバイナリ入力
/// @param[in] s 入力ストリーム
/// @param[out] expr 論理式
/// @return s
IDO&
operator>>(IDO& s,
	   Expr& expr);


//////////////////////////////////////////////////////////////////////
// inline 関数の定義
//////////////////////////////////////////////////////////////////////

inline
Expr
Expr::make_literal(VarId varid,
		      bool inv)
{
  return inv ? make_negaliteral(varid) : make_posiliteral(varid);
}

inline
Expr
Expr::make_literal(const Literal& lit)
{
  return make_literal(lit.varid(), lit.is_negative());
}

inline
ymuint
Expr::litnum(const Literal& lit) const
{
  return litnum(lit.varid(), lit.is_negative());
}

inline
ymuint
Expr::sop_litnum(const Literal& lit) const
{
  return sop_litnum(lit.varid(), lit.is_negative());
}

END_NAMESPACE_YM_EXPR

#endif // YMYMLOGIC_EXPR_H
