
/// @file Expr.sip
/// @brief Expr の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class Expr
{
%TypeHeaderCode
#include "logic/Expr.h"
using nsYm::Expr;
using nsYm::ExprVector;
using nsYm::VarExprMap;
using nsYm::VarVarMap;
%End

public:

  /// @brief 空のデフォルトコンストラクタ
  ///
  /// - 値は定数0になる
  /// - ただし意識して定数0を作りたいなら MakeConst0() を使うこと．
  Expr();

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
  make_and(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  ExprVector chd_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    if ( !sipCanConvertToType(obj1, sipType_Expr, 0) ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    void* p = sipConvertToType(obj1, sipType_Expr, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    Expr* expr = reinterpret_cast<Expr*>(p);
    chd_list[i] = *expr;
  }
  sipRes = new Expr(Expr::make_and(chd_list));
  %End

  /// @brief OR 式の生成
  /// @param[in] chd_list オペランドのベクタ
  /// @return chd_list を部分論理式に持つ OR 式を生成し，返す．
  static
  Expr
  make_or(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  ExprVector chd_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    if ( !sipCanConvertToType(obj1, sipType_Expr, 0) ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    void* p = sipConvertToType(obj1, sipType_Expr, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    Expr* expr = reinterpret_cast<Expr*>(p);
    chd_list[i] = *expr;
  }
  sipRes = new Expr(Expr::make_or(chd_list));
  %End

  /// @brief XOR 式の生成
  /// @param[in] chd_list オペランドのベクタ
  /// @return chd_list を部分論理式に持つ XOR 式を生成し，返す．
  static
  Expr
  make_xor(SIP_PYLIST);
  %MethodCode
  ymuint n = PyList_Size(a0);
  ExprVector chd_list(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    if ( !sipCanConvertToType(obj1, sipType_Expr, 0) ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    void* p = sipConvertToType(obj1, sipType_Expr, NULL, 0, NULL, &sipIsErr);
    if ( sipIsErr ) {
      // エラー
      Py_INCREF(Py_None);
      return Py_None;
    }
    Expr* expr = reinterpret_cast<Expr*>(p);
    chd_list[i] = *expr;
  }
  sipRes = new Expr(Expr::make_xor(chd_list));
  %End

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
  compose(SIP_PYDICT) const;
  %MethodCode
  PyObject* item_list = PyMapping_Items(a0);
  ymuint n = PyList_Size(item_list);
  VarExprMap varmap;
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* tuple = PyList_GetItem(item_list, i);
    PyObject* obj1;
    PyObject* obj2;
    if ( !PyArg_ParseTuple(tuple, "(O!O!)",
			   &sipType_VarId, &obj1,
			   &sipType_Expr, &obj2) ) {
      // エラー
    }
    void* p = sipConvertToType(obj1, sipType_VarId, NULL, 0, NULL, &sipIsErr);
    VarId& var = *reinterpret_cast<VarId*>(p);
    void* q = sipConvertToType(obj2, sipType_Expr, NULL, 0, NULL, &sipIsErr);
    Expr& expr = *reinterpret_cast<Expr*>(q);
    varmap.insert(make_pair(var, expr));
  }
  sipRes = new Expr(sipCpp->compose(varmap));
  %End

  /// @brief 変数番号を再マップする．
  /// @param[in] varmap 置き換え元の変数番号をキーとして
  /// 置き換え先の変数番号を値とした連想配列
  /// @return 置き換えた論理式
  ///
  /// varmap に登録されていない場合には不変とする．
  Expr
  remap_var(SIP_PYDICT) const;
  %MethodCode
  PyObject* item_list = PyMapping_Items(a0);
  ymuint n = PyList_Size(item_list);
  VarVarMap varmap;
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* tuple = PyList_GetItem(item_list, i);
    PyObject* obj1;
    PyObject* obj2;
    if ( !PyArg_ParseTuple(tuple, "(O!O!)",
			   &sipType_VarId, &obj1,
			   &sipType_VarId, &obj2) ) {
      // エラー
    }
    void* p1 = sipConvertToType(obj1, sipType_VarId, NULL, 0, NULL, &sipIsErr);
    void* p2 = sipConvertToType(obj2, sipType_VarId, NULL, 0, NULL, &sipIsErr);
    VarId& var1 = *reinterpret_cast<VarId*>(p1);
    VarId& var2 = *reinterpret_cast<VarId*>(p2);
    varmap.insert(make_pair(var1, var2));
  }
  sipRes = new Expr(sipCpp->remap_var(varmap));
  %End

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
  unsigned long
  eval(SIP_PYLIST lib_obj,
       unsigned long maskL) const;
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<ymulong> vals(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    unsigned long val;
    if ( !PyArg_ParseTuple(obj1, "l", &val) ) {
      // エラー
    }
    vals[i] = val;
  }
  sipRes = sipCpp->eval(vals, a1);
  %End

  /// @brief 真理値表の作成
  /// @param[in] ni 入力数
  /// @note ni が省略された場合には input_size() が用いられる．
  TvFunc
  make_tv(unsigned int ni = 0) const;

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
  unsigned int
  child_num() const;

  /// @brief オペランドの取得
  /// @param[in] pos 取り出すオペランドの位置 (最初の位置は 0)
  /// @return pos 番目のオペランドを返す．
  /// ただし pos が範囲外の場合と演算子ノードでなかった場合には
  /// 0を表す式を返す．
  Expr
  child(unsigned int pos) const;

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

  /// @brief リテラル数の取得
  /// @return リテラル数
  unsigned int
  litnum() const;

  /// @brief 変数の出現回数の取得
  /// @param[in] varid 変数番号
  /// @return varid 番めの変数のリテラルの出現回数を得る．
  unsigned int
  litnum(VarId varid) const;

  /// @brief リテラルの出現回数の取得
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return varid 番めの変数の極性が inv のリテラルの出現回数を得る．
  unsigned int
  litnum(VarId varid,
	 bool inv) const;

  /// @brief リテラルの出現回数の取得
  /// @param[in] lit リテラル
  /// @return lit のリテラルの出現回数を得る．
  unsigned int
  litnum(const Literal& lit) const;

  /// @brief 使われている変数の最大の番号 + 1を得る．
  unsigned int
  input_size() const;

  /// @brief SOP形式に展開したときのキューブ数の見積もり
  /// @return SOP形式のキューブ (積項) 数
  unsigned int
  sop_cubenum() const;

  /// @brief SOP形式に展開した時のリテラル数の見積もり
  /// @return SOP形式のリテラル数
  unsigned int
  sop_litnum() const;

  /// @brief SOP形式に展開した時の変数の出現回数の見積もり
  /// @param[in] varid 変数番号
  /// @return SOP形式の varid 番めの変数のリテラルの出現回数
  unsigned int
  sop_litnum(VarId varid) const;

  /// @brief SOP形式に展開した時のテラルの出現回数の見積もり
  /// @param[in] varid 変数番号
  /// @param[in] inv 極性
  ///                - false: 反転なし (正極性)
  ///                - true:  反転あり (負極性)
  /// @return SOP形式に展開した時の varid 番めの変数の極性が
  /// inv のリテラルの出現回数
  unsigned int
  sop_litnum(VarId varid,
	     bool inv) const;

  /// @brief SOP形式に展開したときのリテラルの出現回数の見積もり
  /// @param[in] lit リテラル
  /// @return SOP形式に展開したときの lit のリテラルの出現回数
  unsigned int
  sop_litnum(const Literal& lit) const;

  /// @brief 論理否定
  /// @return 自分自身を否定した形の論理式を返す．
  Expr
  operator~() const;

  Expr
  operator&(const Expr& src2);

  Expr
  operator|(const Expr& src2);

  Expr
  operator^(const Expr& src2);

};
