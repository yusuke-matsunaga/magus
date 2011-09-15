
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
    NpnVmap imap = map.imap(i);
    ymuint j = imap.pos();
    LogExpr expr;
    if ( imap.pol() == kPolPosi) {
      expr = LogExpr::make_posiliteral(j);
    }
    else {
      expr = LogExpr::make_negaliteral(j);
    }
    vlm.insert(make_pair(i, expr));
  }
  LogExpr cexpr = expr.compose(vlm);
  if ( map.omap(0).pol() == kPolNega ) {
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
LibComp::compile(const CellLibrary& library)
{
  mGroupList.clear();
  mClassList.clear();

  mLogicMgr.init();
  mFFMgr.init();
  mLatchMgr.init();
  mPatMgr.init();

  // XOR のパタンを登録しておく．
  // これはちょっとしたハック
  {
    LogExpr lit0 = LogExpr::make_posiliteral(0);
    LogExpr lit1 = LogExpr::make_posiliteral(1);
    LogExpr xor_ex = lit0 ^ lit1;
    reg_expr(xor_ex);
  }

  ymuint nc = library.cell_num();
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library.cell(i);

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
      reg_expr(expr);
    }
    else if ( cell->is_ff() ) {
      mFFMgr.add_cell(cell);
    }
    else if ( cell->is_latch() ) {
      mLatchMgr.add_cell(cell);
    }
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
void
LibComp::reg_expr(const LogExpr& expr)
{
  // expr に対応する LcGroup を求める．
  TvFunc f = expr.make_tv();
  LcGroup* fgroup = mLogicMgr.find_group(TvFuncM(f));
  const LcClass* fclass = fgroup->parent();

  // fclass->rep_func() を用いる理由は論理式に現れる変数が
  // 真のサポートとは限らないから
  if ( fclass->repfunc().ni() > 1 ) {
    // expr を変換したパタンを登録する．
    LogExpr cexpr = xform_expr(expr, fgroup->map());
    assert_cond( !cexpr.is_constant(), __FILE__, __LINE__);

    mPatMgr.reg_pat(cexpr, fclass->id());
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
LcClass*
LibComp::new_class(const TvFuncM& repfunc)
{
  ymuint new_id = mClassList.size();
  LcClass* fclass = new LcClass(new_id, repfunc);
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
    const vector<const Cell*>& cell_list = group->cell_list();
    for (vector<const Cell*>::const_iterator p = cell_list.begin();
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
    cclass->repfunc().dump(s, 2);
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

#if 0
  // 論理セルグループの情報を出力する．
  s << "*** LogicGroupMgr BEGIN ***" << endl;
  mLogicMgr.display(s);
  s << "*** LogicGroupMgr END ***" << endl
    << endl;

  // FFグループの情報を出力する．
  s << "*** FFGroupMgr BEGIN ***" << endl;
  mFFMgr.display(s);
  s << "*** FFGroupMgr END ***" << endl
    << endl;

  // ラッチグループの情報を出力する．
  s << "*** LatchGroupMgr BEGIN ***" << endl;
  mLatchMgr.display(s);
  s << "*** LatchGroupMgr END ***" << endl
    << endl;
#endif

  // パタングラフの情報を出力する．
  mPatMgr.display(s);
}

END_NAMESPACE_YM_CELL_LIBCOMP
