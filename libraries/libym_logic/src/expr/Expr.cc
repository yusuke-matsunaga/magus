
/// @file Expr.cc
/// @brief Expr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Expr.h"

#include "ExprMgr.h"
#include "ExprNode.h"
#include "ExprParser.h"
#include "SopLit.h"
#include "YmLogic/TvFunc.h"


BEGIN_NAMESPACE_YM_EXPR

// 根のノードを指定したコンストラクタ
Expr::Expr(const ExprNode* node) :
  mRootPtr(NULL)
{
  set_root(node);
}

// デフォルトコンストラクタ
Expr::Expr() :
  mRootPtr(NULL)
{
  set_root(ExprMgr::the_obj().make_zero());
}

// コピーコンストラクタ
Expr::Expr(const Expr& src) :
  mRootPtr(NULL)
{
  set_root(src.root());
}

// @brief 根のノートのスマートポインタを得る．
const ExprNode*
Expr::root() const
{
  return mRootPtr;
}

// 根のノードをセットする．
void
Expr::set_root(const ExprNode* node)
{
  if ( node ) {
    node->inc_ref();
  }
  if ( mRootPtr ) {
    mRootPtr->dec_ref();
  }
  mRootPtr = node;
}

// 代入演算子
const Expr&
Expr::operator=(const Expr& src)
{
  set_root(src.root());
  return *this;
}

// デストラクタ
Expr::~Expr()
{
  set_root(NULL);
}

// 定数 0 の論理式を作る
Expr
Expr::make_zero()
{
  return Expr(ExprMgr::the_obj().make_zero());
}

// 定数 1 の論理式を作る
Expr
Expr::make_one()
{
  return Expr(ExprMgr::the_obj().make_one());
}

// 肯定のリテラルを作る．
Expr
Expr::make_posiliteral(VarId varid)
{
  return Expr(ExprMgr::the_obj().make_posiliteral(varid));
}

// 否定のリテラルを作る．
Expr
Expr::make_negaliteral(VarId varid)
{
  return Expr(ExprMgr::the_obj().make_negaliteral(varid));
}

// 与えられた論理式を部分論理式に持つ n 入力ANDの論理式を作るクラス・メソッド
Expr
Expr::make_and(const ExprVector& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprVector::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_and(begin));
}

Expr
Expr::make_and(const ExprList& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprList::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_and(begin));
}

// 与えられた論理式を部分論理式に持つ n 入力ORの論理式を作るクラス・メソッド
Expr
Expr::make_or(const ExprVector& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprVector::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_or(begin));
}

Expr
Expr::make_or(const ExprList& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprList::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_or(begin));
}

// 与えられた論理式を部分論理式に持つ n 入力XORの論理式を作るクラス・メソッド
Expr
Expr::make_xor(const ExprVector& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprVector::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_xor(begin));
}

Expr
Expr::make_xor(const ExprList& chd_list)
{
  assert_cond(chd_list.size() > 0, __FILE__, __LINE__);
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  for (ExprList::const_iterator p = chd_list.begin();
       p != chd_list.end(); ++ p) {
    mgr.nodestack_push((*p).root());
  }
  return Expr(mgr.make_xor(begin));
}

// @brief 確保していたメモリを開放する．
// @note メモリリークチェックのための関数なので通常は使用しない．
void
Expr::clear_memory()
{
  ExprMgr::clear_memory();
}

// 否定の論理式を与える演算子
Expr
Expr::operator~() const
{
  return Expr(ExprMgr::the_obj().complement(root()));
}

// src1 の論理式と src2 の論理式の論理積を計算する．
Expr
operator&(const Expr& src1,
	  const Expr& src2)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(src1.root());
  mgr.nodestack_push(src2.root());
  return Expr(mgr.make_and(begin));
}

// 自分の論理式と src の論理式の論理積を計算し自分に代入する．
const Expr&
Expr::operator&=(const Expr& src)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_and(begin));
  return *this;
}

// src1 の論理式と src2 の論理式の論理和を計算する．
Expr
operator|(const Expr& src1,
	  const Expr& src2)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(src1.root());
  mgr.nodestack_push(src2.root());
  return Expr(mgr.make_or(begin));
}

// 自分の論理式と src の論理式の論理和を計算し自分に代入する．
const Expr&
Expr::operator|=(const Expr& src)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_or(begin));
  return *this;
}

// src1 の論理式と src2 の論理式の排他的論理和を計算する．
Expr
operator^(const Expr& src1,
	  const Expr& src2)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(src1.root());
  mgr.nodestack_push(src2.root());
  return Expr(mgr.make_xor(begin));
  ExprNodeList node_list;
}

// 自分の論理式と src の論理式の排他的論理和を計算し自分に代入する．
const Expr&
Expr::operator^=(const Expr& src)
{
  ExprMgr& mgr = ExprMgr::the_obj();
  ymuint begin = mgr.nodestack_top();
  mgr.nodestack_push(root());
  mgr.nodestack_push(src.root());
  set_root(mgr.make_xor(begin));
  return *this;
}

// pos 番目のリテラルを src の論理式に置き換える．
Expr
Expr::compose(VarId varid,
	      const Expr& src) const
{
  return Expr(ExprMgr::the_obj().compose(root(), varid, src.root()));
}

// comp_map にしたがって複数のリテラルの置き換えを行う．
Expr
Expr::compose(const HashMap<VarId, Expr>& comp_map) const
{
  return Expr(ExprMgr::the_obj().compose(root(), comp_map));
}

// 与えられた論理式のリテラル番号を再マップする．
Expr
Expr::remap_var(const HashMap<VarId, VarId>& varmap) const
{
  return Expr(ExprMgr::the_obj().remap_var(root(), varmap));
}

// 重複した式を取り除く
// expr + expr = expr
// expr + ~expr = 1 のようなもの
// 自分自身が簡単化された式に置き換わる．
// 新しい式を返す．
const Expr&
Expr::simplify()
{
  set_root(ExprMgr::the_obj().simplify(root()));
  return *this;
}

// @brief 値の評価
// @param[in] vals 変数の値割り当て
// @return 評価値
ymulong
Expr::eval(const vector<ymulong>& vals,
	      ymulong mask) const
{
  return root()->eval(vals, mask);
}

// @brief 真理値表の作成
TvFunc
Expr::make_tv(ymuint ni) const
{
  ymuint ni2 = input_size();
  if ( ni < ni2 ) {
    ni = ni2;
  }
  return root()->make_tv(ni);
}
#if 0
  // とりあえずベタなやり方．
  ymuint ni = input_size();
  ymuint nv = 1 << ni;
  const ymuint BPW = sizeof(ymulong) * 8;
  ymuint nt = (nv + BPW - 1) / BPW;
  tv.clear();
  tv.resize(nt);

  const ExprNode* node = root();
  ymuint b = 0;
  ymulong s = 1UL;
  ymulong mask = 0UL;
  vector<ymulong> ivec(ni, 0UL);
  for (ymuint p = 0; p < nv; ++ p) {
    for (ymuint i = 0; i < ni; ++ i) {
      if ( p & (1 << i) ) {
	ivec[i] |= s;
      }
    }
    mask |= s;
    s <<= 1;
    if ( s == 0UL ) {
      tv[b] = node->eval(ivec, ~0UL);
      ++ b;
      s = 1UL;
      for (ymuint i = 0; i < ni; ++ i) {
	ivec[i] = 0UL;
      }
    }
  }
  if ( s != 1UL ) {
    tv[b] = node->eval(ivec, mask);
  }
}
#endif

// 恒偽関数を表している時に真となる．
bool
Expr::is_zero() const
{
  return root()->is_zero();
}

// 恒真関数を表している時に真となる．
bool
Expr::is_one() const
{
  return root()->is_one();
}

// 定数関数を表している時に真となる．
bool
Expr::is_constant() const
{
  return root()->is_constant();
}

// 肯定のリテラルを表している時に真となる．
bool
Expr::is_posiliteral() const
{
  return root()->is_posiliteral();
}

// 否定のリテラルを表している時に真となる．
bool
Expr::is_negaliteral() const
{
  return root()->is_negaliteral();
}

// リテラルを表している時に真となる．
bool
Expr::is_literal() const
{
  return root()->is_literal();
}

// リテラルの時に変数番号を返す．
// リテラルでなければ kVarMaxId を返す．
VarId
Expr::varid() const
{
  return root()->varid();
}

// トップがANDの時に真となる．
bool
Expr::is_and() const
{
  return root()->is_and();
}

// トップがORの時に真となる．
bool
Expr::is_or() const
{
  return root()->is_or();
}

// トップがXORの時に真となる．
bool
Expr::is_xor() const
{
  return root()->is_xor();
}

// トップが演算子の時に真となる．
bool
Expr::is_op() const
{
  return root()->is_op();
}

// src1 と src2 の根のタイプが同じとき true を返す．
bool
compare_type(const Expr& src1,
	     const Expr& src2)
{
  return src1.root()->type() == src2.root()->type();
}

// AND/OR/XOR の時に子供の項の数を返す．
// それ以外のノードの時には 0 を返す．
ymuint
Expr::child_num() const
{
  return root()->child_num();
}

// pos 番目の子供を返す．
Expr
Expr::child(ymuint pos) const
{
  return Expr(root()->child(pos));
}

// 一種類の演算子のみからなる式のとき true を返す．
// より具体的には zero, one, literal とすべての子供が
// リテラルの and, or, xor
bool
Expr::is_simple() const
{
  return root()->is_simple();
}

// is_simple() && is_and()
bool
Expr::is_simple_and() const
{
  return root()->is_simple_and();
}

// is_simple() && is_or()
bool
Expr::is_simple_or() const
{
  return root()->is_simple_or();
}

// is_simple() && is_xor()
bool
Expr::is_simple_xor() const
{
  return root()->is_simple_xor();
}

// SOP形式の時 true を返す．
bool
Expr::is_sop() const
{
  return root()->is_sop();
}

// リテラル数を得る．
ymuint
Expr::litnum() const
{
  return root()->litnum();
}

// 特定の変数のリテラル数を得る．
ymuint
Expr::litnum(VarId varid) const
{
  return root()->litnum(varid);
}

// 特定の変数の特定の極性のリテラル数を得る．
ymuint
Expr::litnum(VarId varid,
		bool inv) const
{
  return root()->litnum(varid, inv);
}

// @brief 使われている変数の最大の番号を得る．
ymuint
Expr::input_size() const
{
  return root()->input_size();
}

// SOP形式に展開したときのキューブ数を得る．
ymuint
Expr::sop_cubenum() const
{
  SopLit l = root()->soplit(false);
  return l.np();
}

// SOP形式に展開した時のリテラル数を見積もる．
ymuint
Expr::sop_litnum() const
{
  SopLit l = root()->soplit(false);
  return l.nl();
}

// SOP形式に展開した時の varid 番めの変数のリテラルの出現回数を得る．
ymuint
Expr::sop_litnum(VarId varid) const
{
  SopLit l = root()->soplit(false, varid);
  return l.nl();
}

// SOP形式に展開した時の varid 番めの変数の極性が pol のリテラル
// の出現回数を得る．
ymuint
Expr::sop_litnum(VarId varid,
		    bool inv) const
{
  SopLit l = root()->soplit(false, varid, inv);
  return l.nl();
}

// @brief 使用されているメモリ量を返す．
ymuint
Expr::used_size()
{
  return ExprMgr::the_obj().used_size();
}

// 現在使用中のノード数を返す．
ymuint
Expr::node_num()
{
  return ExprMgr::the_obj().node_num();
}

// @brief used_size() の最大値を返す．
ymuint
Expr::max_used_size()
{
  return ExprMgr::the_obj().max_used_size();
}

// @brief nodenum() の最大値を返す．
ymuint
Expr::max_node_num()
{
  return ExprMgr::the_obj().max_node_num();
}

// @brief 実際に確保したメモリ量を返す．
ymuint
Expr::allocated_size()
{
  return ExprMgr::the_obj().allocated_size();
}

// @brief 実際に確保した回数を返す．
ymuint
Expr::allocated_count()
{
  return ExprMgr::the_obj().allocated_count();
}

// @brief 内部状態を出力する．
void
Expr::print_stats(ostream& s)
{
  ExprMgr::the_obj().print_stats(s);
}

// 論理式をパーズしてファクタードフォームを作る．
// エラーが起きたら msg にエラーメッセージをセットし, false を返す．
bool
Expr::read_from_stream(istream& in,
			  string& err_msg)
{
  Expr expr = stream_to_expr(in, err_msg);
  if ( err_msg != string() ) {
    return false;
  }
  else {
    operator=(expr);
    return true;
  }
}

// 論理式をパーズしてファクタードフォームを作る．
// エラーが起きたら msg にエラーメッセージをセットする．
Expr
Expr::stream_to_expr(istream& in,
			string& err_msg)
{
  err_msg = string();
  try {
    ExprParser p(&in);
    return p.get_expr(kTokenEND);
  }
  catch ( SyntaxError e ) {
    err_msg = e.mMsg;
    return make_zero();
  }
}


BEGIN_NONAMESPACE

// 論理式をバイナリダンプする．
void
write_expr(ODO& s,
	   const Expr& expr)
{
  if ( expr.is_zero() ) {
    s << static_cast<ymuint8>(0);
    return;
  }
  if ( expr.is_one() ) {
    s << static_cast<ymuint8>(1);
    return;
  }
  if ( expr.is_posiliteral() ) {
    s << static_cast<ymuint8>(2)
      << expr.varid();
    return;
  }
  if ( expr.is_negaliteral() ) {
    s << static_cast<ymuint8>(3)
      << expr.varid();
    return;
  }

  // 残りは論理演算ノード
  ymuint8 type = 0;
  if ( expr.is_and() ) {
    type = 4;
  }
  else if ( expr.is_or() ) {
    type = 5;
  }
  else if ( expr.is_xor() ) {
    type = 6;
  }
  else {
    assert_not_reached(__FILE__, __LINE__);
  }

  ymuint32 nc = expr.child_num();
  s << type
    << nc;
  for (ymuint i = 0; i < nc; ++ i) {
    write_expr(s, expr.child(i));
  }
}

// バイナリストリームから論理式を作る．
Expr
read_expr(IDO& s)
{
  ymuint8 type;
  s >> type;
  switch ( type ) {
  case 0:
    return Expr::make_zero();

  case 1:
    return Expr::make_one();

  case 2:
    {
      VarId var;
      s >> var;
      return Expr::make_posiliteral(var);
    }

  case 3:
    {
      VarId var;
      s >> var;
      return Expr::make_negaliteral(var);
    }
  }

  // 残りは論理演算
  ymuint32 nc;
  s >> nc;
  vector<Expr> child_list(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    child_list[i] = read_expr(s);
  }

  switch ( type ) {
  case 4:
    return Expr::make_and(child_list);

  case 5:
    return Expr::make_or(child_list);

  case 6:
    return Expr::make_xor(child_list);

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  // ダミー
  return Expr::make_zero();
}

END_NONAMESPACE

// @relates Expr
// @brief 論理式の内容のバイナリ出力
// @param[in] s 出力ストリーム
// @param[in] expr 論理式
// @return s
ODO&
operator<<(ODO& s,
	   const Expr& expr)
{
  write_expr(s, expr);
  return s;
}

// @relates Expr
// @brief 論理式の内容のバイナリ入力
// @param[in] s 入力ストリーム
// @param[out] expr 論理式
// @return s
IDO&
operator>>(IDO& s,
	   Expr& expr)
{
  expr = read_expr(s);
  return s;
}

END_NAMESPACE_YM_EXPR
