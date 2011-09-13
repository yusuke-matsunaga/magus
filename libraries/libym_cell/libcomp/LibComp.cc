
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
LibComp::LibComp()
{
}

// @brief デストラクタ
LibComp::~LibComp()
{
}

// @brief セルのグループ化，クラス化を行う．
void
LibComp::compile(const CellLibrary& library)
{
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
      if ( !cell->has_logic() || cell->output_num() == 0 ) {
	continue;
      }
      mLogicMgr.add_cell(cell);

      ymuint no = cell->output_num();
      ymuint nio = cell->inout_num();
      if ( no + nio > 1 ) {
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

// @brief グラフ構造全体をダンプする．
// @param[in] s 出力先のストリーム
void
LibComp::display(ostream& s)
{
  // 論理セルグループの情報を出力する．
  mLogicMgr.display(s);

  // FFグループの情報を出力する．
  mFFMgr.display(s);

  // ラッチグループの情報を出力する．
  mLatchMgr.display(s);

  // パタングラフの情報を出力する．
  mPatMgr.display(s);
}

// @brief グラフ構造全体をダンプする．
// @param[in] bos 出力先のストリーム
// @note ダンプされた情報はそのまま PatGraph で読み込むことができる．
void
LibComp::dump(BinO& bos)
{
  // 論理セルグループの情報をダンプする．
  mLogicMgr.dump(bos);

  // FFグループの情報を出力する．
  mFFMgr.dump(bos);

  // ラッチグループの情報を出力する．
  mLatchMgr.dump(bos);

  // パタングラフの情報をダンプする．
  mPatMgr.dump(bos);
}

END_NAMESPACE_YM_CELL_LIBCOMP
