
/// @file SatDimacsHandler.cc
/// @brief SatDimacsHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "logic/SatDimacsHandler.h"


BEGIN_NAMESPACE_YM_SAT

// @brief コンストラクタ
SatDimacsHandler::SatDimacsHandler(SatSolver& solver) :
  mSolver(solver)
{
}

// @brief デストラクタ
SatDimacsHandler::~SatDimacsHandler()
{
}

// @brief 初期化
bool
SatDimacsHandler::init()
{
  return true;
}

// @brief p 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] nv 変数の数
// @param[in] nc 節の数
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_p(const FileRegion& loc,
			 ymuint nv,
			 ymuint nc)
{
  for (ymuint i = 0; i < nv; ++ i) {
    mSolver.new_var();
  }

  return true;
}

// @brief clause 行の読込み
// @param[in] loc ファイル上の位置情報
// @param[in] lits リテラルの配列．最後の0は含まない
// @retval true 処理が成功した．
// @retval false エラーが起こった．
bool
SatDimacsHandler::read_clause(const FileRegion& loc,
			      const vector<int>& lits)
{
  vector<Literal> tmp;
  for (vector<int>::const_iterator p = lits.begin();
       p != lits.end(); ++ p) {
    int i = *p;
    VarId vid;
    tPol pol;
    if ( i > 0 ) {
      vid = VarId(i - 1);
      pol = kPolPosi;
    }
    else {
      vid = VarId(- i - 1);
      pol = kPolNega;
    }
    tmp.push_back(Literal(vid, pol));
  }
  mSolver.add_clause(tmp);

  return true;
}

// @brief 終了処理
// @param[in] loc 位置情報
bool
SatDimacsHandler::end()
{
  return true;
}

// @brief エラー終了時の処理
void
SatDimacsHandler::error_exit()
{
}

END_NAMESPACE_YM_SAT
