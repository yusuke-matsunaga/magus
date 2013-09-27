
/// @file EufVarMgr.cc
/// @brief EufVarMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2011 Yusuke Matsunaga
/// All rights reserved.


#include "EufVarMgr.h"
#include "EufVar.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufVarMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EufVarMgr::EufVarMgr() :
  mAlloc(sizeof(EufVar), 1024)
{
  mNum = 0;
  mTableSize = 0;
  mTable = NULL;

  expand(1024);
}

// @brief デストラクタ
EufVarMgr::~EufVarMgr()
{
  delete [] mTable;
}

// @brief 変数を生成する．
// @param[in] name 変数名
// @note 同じ名前の変数が存在したらそれを返す．
EufVar*
EufVarMgr::new_variable(const string& name)
{
  ymuint h = hash(name);
  for (EufVar* var = mTable[h]; var != NULL; var = var->mLink) {
    if ( var->id_name() == name ) {
      return var;
    }
  }

  if ( mNum >= mNextLimit ) {
    expand(mTableSize * 2);
    h = hash(name);
  }

  EufVar* var = new EufVar(name);
  ++ mNum;
  var->mLink = mTable[h];
  mTable[h] = var;

  return var;
}

// @brief ハッシュテーブルを拡大する．
void
EufVarMgr::expand(ymuint req_size)
{
  ymuint old_size = mTableSize;
  EufVar** old_table = mTable;

  mTableSize = req_size;
  mTable = new EufVar*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (EufVar* var = old_table[i]; var != NULL; ) {
	EufVar* tmp_var = var;
	var = tmp_var->mLink;

	ymuint h = hash(tmp_var->id_name());
	tmp_var->mLink = mTable[h];
	mTable[h] = tmp_var;
      }
    }
    delete [] old_table;
  }
}

END_NAMESPACE_YM_LLVMEQ
