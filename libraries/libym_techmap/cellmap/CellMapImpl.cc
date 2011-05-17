
/// @file libym_techmap/cellmap/CellMapImpl.cc
/// @brief CellMapImpl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellMapImpl.h"
#include "RepFunc.h"
#include "FuncGroup.h"
#include "AreaCover.h"

#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_utils/BinIO.h"

#include "patgen/patgen_nsdef.h"
#include "patgen/PgFuncMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス FuncGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncGroup::FuncGroup() :
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
FuncGroup::~FuncGroup()
{
}


//////////////////////////////////////////////////////////////////////
// クラス RepFunc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
RepFunc::RepFunc() :
  mFuncNum(0U),
  mFuncArray(NULL)
{
}

// @brief デストラクタ
RepFunc::~RepFunc()
{
  delete [] mFuncArray;
}

// @brief バイナリ形式のファイルを読み込む．
// @param[in] s 入力ストリーム
// @return 読み込みが成功したら true を返す．
bool
RepFunc::load(istream& s)
{
  mFuncNum = BinIO::read_32(s);
  if ( mFuncNum > 0 ) {
    mFuncArray = new ymuint32[mFuncNum];
    for (ymuint i = 0; i < mFuncNum; ++ i) {
      mFuncArray[i] = BinIO::read_32(s);
    }
  }
  else {
    mFuncArray = NULL;
  }

  return true;
}

END_NAMESPACE_YM_TECHMAP_CELLMAP


BEGIN_NAMESPACE_YM_TECHMAP

BEGIN_NONAMESPACE

void
read_map(istream& s,
	 NpnMap& map)
{
  ymuint32 tmp = BinIO::read_32(s);
  ymuint ni = (tmp >> 1);
  map.resize(ni);
  tPol opol = (tmp & 1U) ? kPolNega : kPolPosi;
  map.set_opol(opol);
  for (ymuint i = 0; i < ni; ++ i) {
    ymuint32 tmp = BinIO::read_32(s);
    ymuint pos = (tmp >> 1);
    tPol ipol = (tmp & 1U) ? kPolNega : kPolPosi;
    map.set(i, pos, ipol);
  }
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// クラス CellMapImpl
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellMapImpl::CellMapImpl() :
  mLibrary(NULL)
{
}

// @brief デストラクタ
CellMapImpl::~CellMapImpl()
{
  init();
}

// @brief 内容を初期化する．
// @note 以前確保されたメモリは開放される．
void
CellMapImpl::init()
{
  delete mLibrary;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
CellMapImpl::load_library(istream& s)
{
  // 以前の内容を捨てる．
  init();

  // ライブラリを読み込む．
  mLibrary = nsYm::nsCell::restore_library(s);

  // 関数の情報を読み込む．
  mFuncNum = BinIO::read_32(s);
  mFuncArray = new FuncGroup[mFuncNum];
  for (ymuint i = 0; i < mFuncNum; ++ i) {
    FuncGroup& func = mFuncArray[i];
    read_map(s, func.mNpnMap);
    ymuint n = BinIO::read_32(s);
    func.mCellNum = n;
    if ( n > 0 ) {
      func.mCellList = new const Cell*[n];
      for (ymuint j = 0; j < n; ++ j) {
	ymuint id = BinIO::read_32(s);
	func.mCellList[j] = mLibrary->cell(id);
      }
    }
    else {
      func.mCellList = NULL;
    }
  }

  // 代表関数の情報を読み込む．
  mRepNum = BinIO::read_32(s);
  mRepArray = new RepFunc[mRepNum];
  for (ymuint i = 0; i < mRepNum; ++ i) {
    if ( !mRepArray[i].load(s) ) {
      return false;
    }
  }

  // パタンの情報を読み込む．
  if ( !mPatMgr.load(s) ) {
    return false;
  }

  return true;
}

// @brief セルライブラリの内容(+パタングラフ)をバイナリファイルに書き出す．
void
CellMapImpl::dump_library(ostream& s,
			  const CellLibrary& library)
{
  using namespace nsPatgen;

  PgFuncMgr pgf_mgr;
  pgf_mgr.set_library(&library);

  pg_dump(s, pgf_mgr);
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
void
CellMapImpl::area_map(const BdnMgr& sbjgraph,
		      ymuint mode,
		      CnGraph& mapnetwork)
{
  nsCellmap::AreaCover area_cover;

  area_cover(sbjgraph, *this, mapnetwork);
}

// @brief 関数グループを返す．
// @param[in] id 関数番号　( 0 <= id < func_num() )
const FuncGroup&
CellMapImpl::func_group(ymuint id) const
{
  return mFuncArray[id];
}

// @brief 代表関数を返す．
// @param[in] id 代表関数番号
const RepFunc&
CellMapImpl::rep(ymuint id) const
{
  return mRepArray[id];
}

// @relates CellMapImpl
// @brief CellMapImpl の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cell_mgr セルライブラリ
void
dump(ostream& s,
     const CellMapImpl& cell_mgr)
{
  s << "==== CellMapImpl dump start ====" << endl;

  // 関数情報の出力
  const FuncGroup& func0 = cell_mgr.const0_func();
  const FuncGroup& func1 = cell_mgr.const1_func();
  const FuncGroup& func2 = cell_mgr.buf_func();
  const FuncGroup& func3 = cell_mgr.inv_func();
  ymuint nf = cell_mgr.func_num();
  for (ymuint i = 0; i < nf; ++ i) {
    const FuncGroup& func = cell_mgr.func_group(i);
    if ( &func == &func0 ) {
      s << "Const0" << endl;
    }
    else if ( &func == &func1 ) {
      s << "Const1" << endl;
    }
    else if ( &func == &func2 ) {
      s << "Buffer" << endl;
    }
    else if ( &func == &func3 ) {
      s << "Inverter" << endl;
    }
    else {
      const NpnMap& map = func.npn_map();
      s << "Func#" << i << ": " << map << endl;
    }
    ymuint nc = func.cell_num();
    for (ymuint j = 0; j < nc; ++ j) {
      const Cell* cell = func.cell(j);
      s << "  " << cell->name() << endl;
    }
  }
  s << endl;

  // 代表関数情報の出力
  ymuint nr = cell_mgr.rep_num();
  for (ymuint i = 0; i < nr; ++ i) {
    const RepFunc& rep = cell_mgr.rep(i);
    s << "Rep#" << i << endl;
    s << "  equivalence = ";
    ymuint nf = rep.func_num();
    for (ymuint j = 0; j < nf; ++ j) {
      s << " Func#" << rep.func_id(j);
    }
    s << endl;
  }

  // パタンの情報の出力
  dump(s, cell_mgr.pat_mgr());

  s << "==== CellMapImpl dump end ====" << endl;
}

END_NAMESPACE_YM_TECHMAP
