
/// @file libym_techmap/cellmap/CellMgr.cc
/// @brief CellMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "CellMgr.h"
#include "RepFunc.h"
#include "FuncGroup.h"
#include "FFClass.h"
#include "LatchClass.h"

#include "ym_cell/CellLibrary.h"
#include "ym_cell/Cell.h"
#include "ym_utils/BinIO.h"


BEGIN_NAMESPACE_YM_CELLMAP

//////////////////////////////////////////////////////////////////////
// クラス CellGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellGroup::CellGroup() :
  mCellNum(0),
  mCellList(NULL)
{
}

// @brief デストラクタ
CellGroup::~CellGroup()
{
  // mCellList は CellMgr が管理している．
}


//////////////////////////////////////////////////////////////////////
// クラス FuncGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FuncGroup::FuncGroup()
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
  mFuncIdList(NULL)
{
}

// @brief デストラクタ
RepFunc::~RepFunc()
{
  // mFuncIdList は CellMgr が管理している．
}

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
// クラス CellMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
CellMgr::CellMgr() :
  mAlloc(4096),
  mLibrary(NULL),
  mFuncNum(0),
  mFuncArray(NULL),
  mRepNum(0),
  mRepArray(NULL),
  mFFClassNum(0),
  mFFArray(NULL),
  mLatchClassNum(0),
  mLatchArray(NULL),
  mPatMgr(mAlloc)
{
}

// @brief デストラクタ
CellMgr::~CellMgr()
{
  init();
}

// @brief 内容を初期化する．
// @note 以前確保されたメモリは開放される．
void
CellMgr::init()
{
  mAlloc.destroy();
  delete mLibrary;
}

// @brief データを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
CellMgr::load_library(istream& s)
{
  // 以前の内容を捨てる．
  init();

  // ライブラリを読み込む．
  mLibrary = nsYm::nsCell::restore_library(s);

  // 関数の情報を読み込む．
  mFuncNum = BinIO::read_32(s);
  void* p = mAlloc.get_memory(sizeof(FuncGroup) * mFuncNum);
  mFuncArray = new (p) FuncGroup[mFuncNum];
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
  void* q = mAlloc.get_memory(sizeof(RepFunc) * mRepNum);
  mRepArray = new (q) RepFunc[mRepNum];
  for (ymuint i = 0; i < mRepNum; ++ i) {
    load_repfunc(s, i);
  }

  // パタンの情報を読み込む．
  if ( !mPatMgr.load(s) ) {
    return false;
  }

  return true;
}

// @brief RepFunc の内容をロードする．
// @param[in] s 入力ストリーム
// @param[in] id 代表番号
void
CellMgr::load_repfunc(istream& s,
		      ymuint id)
{
  RepFunc& rep = mRepArray[id];
  ymuint n = BinIO::read_32(s);
  rep.mFuncNum = n;
  if ( n > 0 ) {
    void* p = mAlloc.get_memory(sizeof(ymuint32) * n);
    rep.mFuncIdList = new (p) ymuint32[n];
    for (ymuint i = 0; i < n; ++ i) {
      rep.mFuncIdList[i] = BinIO::read_32(s);
    }
  }
  else {
    rep.mFuncIdList = NULL;
  }
}

// @bireif 論理関数の個数を返す．
ymuint
CellMgr::func_num() const
{
  return mFuncNum;
}

// @brief 関数グループを返す．
// @param[in] id 関数番号　( 0 <= id < func_num() )
const FuncGroup&
CellMgr::func_group(ymuint id) const
{
  return mFuncArray[id];
}

// @brief 定数0の関数グループを返す．
const FuncGroup&
CellMgr::const0_func() const
{
  // 決め打ち
  return func_group(0);
}

// @brief 定数1の関数グループを返す．
const FuncGroup&
CellMgr::const1_func() const
{
  // 決め打ち
  return func_group(1);
}

// @brief バッファセルの関数グループを返す．
const FuncGroup&
CellMgr::buf_func() const
{
  // 決め打ち
  return func_group(2);
}

// @brief インバータセルの関数グループを返す．
const FuncGroup&
CellMgr::inv_func() const
{
  // 決め打ち
  return func_group(3);
}

// @brief 代表関数の個数を返す．
ymuint
CellMgr::rep_num() const
{
  return mRepNum;
}

// @brief 代表関数を返す．
// @param[in] id 代表関数番号
const RepFunc&
CellMgr::rep(ymuint id) const
{
  return mRepArray[id];
}

// @brief FFクラス数を返す．
ymuint
CellMgr::ff_class_num() const
{
  return mFFClassNum;
}

// @brief FFクラスを得る．
// @param[in] pos 位置番号 ( 0 <= pos < ff_class_num() )
const FFClass&
CellMgr::ff_class(ymuint pos) const
{
  return mFFArray[pos];
}

// @brief ラッチクラス数を返す．
ymuint
CellMgr::latch_class_num() const
{
  return mLatchClassNum;
}

// @brief ラッチクラスを得る．
// @param[in] pos 位置番号 ( 0 <= pos < latch_class_num() )
const LatchClass&
CellMgr::latch_class(ymuint pos) const
{
  return mLatchArray[pos];
}

// @brief パタンを管理するオブジェクトを得る．
const PatMgr&
CellMgr::pat_mgr() const
{
  return mPatMgr;
}

// @relates CellMgr
// @brief CellMgr の内容を出力する．
// @param[in] s 出力先のストリーム
// @param[in] cell_mgr セルライブラリ
void
dump(ostream& s,
     const CellMgr& cell_mgr)
{
  s << "==== CellMgr dump start ====" << endl;

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

  s << "==== CellMgr dump end ====" << endl;
}

END_NAMESPACE_YM_CELLMAP
