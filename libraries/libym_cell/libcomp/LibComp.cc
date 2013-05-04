
/// @file LibComp.cc
/// @brief LibComp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LibComp.h"
#include "LcClass.h"
#include "LcGroup.h"
#include "LcPatNode.h"
#include "LcPatHandle.h"
#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_cell/CellPin.h"
#include "ym_logic/LogExpr.h"
#include "ym_logic/NpnMap.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

BEGIN_NONAMESPACE

// 論理式の変数を map にしたがって変換する．
LogExpr
xform_expr(const LogExpr& expr,
	   const NpnMapM& map)
{
  ymuint ni = map.ni();
  ymuint no = map.no();
  assert_cond( no == 1, __FILE__, __LINE__);
  VarLogExprMap vlm;
  for (ymuint i = 0; i < ni; ++ i) {
    VarId src_var(i);
    NpnVmap imap = map.imap(src_var);
    VarId dst_var = imap.var();
    LogExpr expr = LogExpr::make_literal(dst_var, imap.pol());
    vlm.insert(make_pair(src_var, expr));
  }
  LogExpr cexpr = expr.compose(vlm);
  if ( map.omap(VarId(0)).pol() == kPolNega ) {
    cexpr = ~cexpr;
  }
  return cexpr;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス LibComp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LibComp::LibComp() :
  mLogicMgr(*this),
  mFFMgr(*this),
  mLatchMgr(*this)
{
}

// @brief デストラクタ
LibComp::~LibComp()
{
  for (vector<LcGroup*>::iterator p = mGroupList.begin();
       p != mGroupList.end(); ++ p) {
    delete *p;
  }
  for (vector<LcClass*>::iterator p = mClassList.begin();
       p != mClassList.end(); ++ p) {
    delete *p;
  }
}

// @brief 論理セルグループの情報を取り出す．
const LcGroupMgr&
LibComp::logic_group_mgr() const
{
  return mLogicMgr;
}

// @brief FFセルグループの情報を取り出す．
const LcGroupMgr&
LibComp::ff_group_mgr() const
{
  return mFFMgr;
}

// @brief ラッチセルグループの情報を取り出す．
const LcGroupMgr&
LibComp::latch_group_mgr() const
{
  return mLatchMgr;
}

// @brief パタングラフの情報を取り出す．
const LcPatMgr&
LibComp::pat_mgr() const
{
  return mPatMgr;
}

// @brief セルのグループ化，クラス化を行う．
void
LibComp::compile(CellLibrary& library)
{
  mGroupList.clear();
  mClassList.clear();

  mLogicMgr.init();
  mFFMgr.init();
  mLatchMgr.init();
  mPatMgr.init();

  // AND2 〜 AND8 のパタンを登録しておく．
  for (ymuint ni = 2; ni <= 8; ++ ni) {
    LogExpr and_expr = LogExpr::make_posiliteral(VarId(0));
    for (ymuint i = 1; i < ni; ++ i) {
      and_expr &= LogExpr::make_posiliteral(VarId(i));
    }
    reg_expr(and_expr, true);
  }

  // XOR2 〜 XOR4 のパタンを登録しておく．
  for (ymuint ni = 2; ni <= 4; ++ ni) {
    LogExpr xor_expr = LogExpr::make_posiliteral(VarId(0));
    for (ymuint i = 1; i < ni; ++ i) {
      xor_expr ^= LogExpr::make_posiliteral(VarId(i));
    }
    reg_expr(xor_expr, true);
  }

  // MUX2 のパタンを登録しておく．
  {
    LogExpr lit0 = LogExpr::make_posiliteral(VarId(0));
    LogExpr lit1 = LogExpr::make_posiliteral(VarId(1));
    LogExpr lit2 = LogExpr::make_posiliteral(VarId(2));
    LogExpr mux2_ex = lit0 & ~lit2 | lit1 & lit2;
    reg_expr(mux2_ex, true);
  }

  // MUX4 のパタンを登録しておく．
  {
    LogExpr lit0 = LogExpr::make_posiliteral(VarId(0));
    LogExpr lit1 = LogExpr::make_posiliteral(VarId(1));
    LogExpr lit2 = LogExpr::make_posiliteral(VarId(2));
    LogExpr lit3 = LogExpr::make_posiliteral(VarId(3));
    LogExpr lit4 = LogExpr::make_posiliteral(VarId(4));
    LogExpr lit5 = LogExpr::make_posiliteral(VarId(5));
    LogExpr mux4_ex =
      lit0 & ~lit4 & ~lit5 |
      lit1 &  lit4 & ~lit5 |
      lit2 & ~lit4 &  lit5 |
      lit3 &  lit4 &  lit5;
    reg_expr(mux4_ex, true);
  }

  // MUX8 のパタンを登録しておく．
  if ( 0 ) {
    LogExpr lit0 = LogExpr::make_posiliteral(VarId(0));
    LogExpr lit1 = LogExpr::make_posiliteral(VarId(1));
    LogExpr lit2 = LogExpr::make_posiliteral(VarId(2));
    LogExpr lit3 = LogExpr::make_posiliteral(VarId(3));
    LogExpr lit4 = LogExpr::make_posiliteral(VarId(4));
    LogExpr lit5 = LogExpr::make_posiliteral(VarId(5));
    LogExpr lit6 = LogExpr::make_posiliteral(VarId(6));
    LogExpr lit7 = LogExpr::make_posiliteral(VarId(7));
    LogExpr lit8 = LogExpr::make_posiliteral(VarId(8));
    LogExpr lit9 = LogExpr::make_posiliteral(VarId(9));
    LogExpr lit10 = LogExpr::make_posiliteral(VarId(10));
    LogExpr mux8_ex =
      lit0 & ~lit8 & ~lit9 & ~lit10 |
      lit1 &  lit8 & ~lit9 & ~lit10 |
      lit2 & ~lit8 &  lit9 & ~lit10 |
      lit3 &  lit8 &  lit9 & ~lit10 |
      lit4 & ~lit8 & ~lit9 &  lit10 |
      lit5 &  lit8 & ~lit9 &  lit10 |
      lit6 & ~lit8 &  lit9 &  lit10 |
      lit7 &  lit8 &  lit9 &  lit10;
    reg_expr(mux8_ex, true);
  }

  ymuint nc = library.cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    Cell* cell = library.cell(i);

    if ( cell->is_logic() ) {
      mLogicMgr.add_cell(cell);

      // パタンを作る．
      ymuint no2 = cell->output_num2();
      if ( no2 != 1 ) {
	// 出力ピンが複数あるセルは対象外
	continue;
      }
      if ( !cell->has_logic(0) ) {
	// 論理式を持たないセルも対象外
	continue;
      }
      if ( cell->has_tristate(0) ) {
	// three_state 属性を持つセルも対象外
	continue;
      }

      LogExpr expr = cell->logic_expr(0);
      reg_expr(expr, false);
    }
    else if ( cell->is_ff() ) {
      mFFMgr.add_cell(cell);
    }
    else if ( cell->is_latch() ) {
      mLatchMgr.add_cell(cell);
    }
  }

  if ( 0 ) {
    display(cout);
  }
}

// @brief セルグループの数を返す．
ymuint
LibComp::group_num() const
{
  return mGroupList.size();
}

// @brief セルグループを返す．
// @param[in] id グループ番号 ( 0 <= id < group_num() )
LcGroup*
LibComp::group(ymuint id) const
{
  assert_cond( id < group_num(), __FILE__, __LINE__);
  return mGroupList[id];
}

// @brief NPN同値クラスの数を返す．
ymuint
LibComp::npn_class_num() const
{
  return mClassList.size();
}

// @brief NPN同値クラスを返す．
// @param[in] id クラス番号 ( 0 <= id < npn_class_num() )
LcClass*
LibComp::npn_class(ymuint id) const
{
  assert_cond( id < npn_class_num(), __FILE__, __LINE__);
  return mClassList[id];
}

// @brief 定義済みの論理グループ番号を返す．
// @param[in] id 番号
// - 0: 定数0
// - 1: 定数1
// - 2: バッファ
// - 3: インバーター
ymuint
LibComp::logic_group(ymuint id) const
{
  return mLogicMgr.logic_group(id);
}

// @brief 定義済みのFFクラス番号を返す．
// @param[in] id 番号
// - 0: クリアなし，プリセットなし
// - 1: クリアあり，プリセットなし
// - 2: クリアなし，プリセットあり
// - 3: クリアあり，プリセットあり
ymuint
LibComp::ff_class(ymuint id) const
{
  return mFFMgr.ff_class(id);
}

// @brief 定義済みのラッチクラス番号を返す．
// @param[in] id 番号
// - 0: クリアなし，プリセットなし
// - 1: クリアあり，プリセットなし
// - 2: クリアなし，プリセットあり
// - 3: クリアあり，プリセットあり
ymuint
LibComp::latch_class(ymuint id) const
{
  return mLatchMgr.latch_class(id);
}

// @brief expr から生成されるパタンを登録する．
// @param[in] expr 論理式
// @param[in] builtin 組み込みクラスの時 true にするフラグ
void
LibComp::reg_expr(const LogExpr& expr,
		  bool builtin)
{
  // expr に対応する LcGroup を求める．
  TvFunc f = expr.make_tv();
  LcGroup* fgroup = mLogicMgr.find_group(TvFuncM(f), builtin);
  const LcClass* fclass = fgroup->parent();

  // 組み込みクラスなら新たなパタンの登録は行わない．
  if ( !builtin && fclass->builtin() ) {
    return;
  }

  // fclass->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから

  ymuint ni = fclass->repfunc().ni();

  if ( ni <= 1 ) {
    // 定数関数およびバッファ，インバータは別に処理する．
    return;
  }

  // expr を変換したパタンを登録する．
  LogExpr cexpr = xform_expr(expr, fgroup->map());
  assert_cond( !cexpr.is_constant(), __FILE__, __LINE__);

  if ( ni <= 8 ) {
    mPatMgr.reg_pat(cexpr, fclass->id());
  }
  else {
    // 登録できなかったことを通知する？
  }
}

// @brief 新しいグループを作る．
LcGroup*
LibComp::new_group()
{
  ymuint new_id = mGroupList.size();
  LcGroup* fgroup = new LcGroup(new_id);
  mGroupList.push_back(fgroup);

  return fgroup;
}

// @brief 新しいクラスを作る．
// @param[in] repfunc 代表関数
// @param[in] builtin 組み込みクラスの時 true にするフラグ
LcClass*
LibComp::new_class(const TvFuncM& repfunc,
		   bool builtin)
{
  ymuint new_id = mClassList.size();
  LcClass* fclass = new LcClass(new_id, builtin, repfunc);
  mClassList.push_back(fclass);

  return fclass;
}

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
void
LibComp::display(ostream& s) const
{
  // セルグループの情報を出力する．
  s << "*** Cell Group BEGIN ***" << endl;
  for (ymuint i = 0; i < group_num(); ++ i) {
    const LcGroup* group = this->group(i);
    assert_cond( group->id() == i, __FILE__, __LINE__);
    s << "GROUP#" << i
      << ": CLASS#" << group->parent()->id()
      << ": " << group->map()
      << endl;
    s << "  CELL:";
    const vector<Cell*>& cell_list = group->cell_list();
    for (vector<Cell*>::const_iterator p = cell_list.begin();
	 p != cell_list.end(); ++ p) {
      const Cell* cell = *p;
      s << " " << cell->name();
    }
    s << endl;
  }
  s << "*** Cell Group END ***" << endl
    << endl;

  // NPN同値クラスの情報を出力する．
  s << "*** NPN Class BEGIN ***" << endl;
  for (ymuint i = 0; i < npn_class_num(); ++ i) {
    const LcClass* cclass = npn_class(i);
    assert_cond( cclass->id() == i, __FILE__, __LINE__);
    s << "CLASS#" << i << ": ";
    cclass->repfunc().print(s, 2);
    s << endl;
    s << "  equivalence = ";
    const vector<LcGroup*>& group_list = cclass->group_list();
    for (ymuint j = 0; j < group_list.size(); ++ j) {
	s << " GROUP#" << group_list[j]->id();
    }
    s << endl;
  }
  s << "*** NPN Class END ***" << endl
    << endl;

  // パタングラフの情報を出力する．
  mPatMgr.display(s);
}

END_NAMESPACE_YM_CELL_LIBCOMP
