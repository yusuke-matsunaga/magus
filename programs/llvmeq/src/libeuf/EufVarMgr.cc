
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

// @brief 変数を探す．
// @param[in] name 変数名
EufNode*
EufVarMgr::find(const string& name) const
{
  ymuint h = hash(name);
  for (EufVar* var = mTable[h]; var != NULL; var = var->mLink) {
    if ( var->id_name() == name ) {
      return var;
    }
  }
  return NULL;
}

// @brief 変数を生成する．
// @param[in] id ID番号
// @param[in] name 変数名
// @note 同じ名前の変数が存在したらそれを返す．
EufNode*
EufVarMgr::new_variable(ymuint id,
			const string& name)
{
  if ( mNum >= mNextLimit ) {
    expand(mTableSize * 2);
  }

  void* p = mAlloc.get_memory(sizeof(EufVar));
  EufVar* var = new (p) EufVar(id, name);

  ymuint h = hash(name);
  var->mLink = mTable[h];
  mTable[h] = var;

  ++ mNum;

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

// @brief 名前からハッシュ値を計算する．
ymuint
EufVarMgr::hash(const string& name) const
{
  ymuint h = 0;
  for (string::const_iterator p = name.begin();
       p != name.end(); ++ p) {
    ymuint c = *p;
    h = h * 37 + c;
  }
  return h % mTableSize;
}

END_NAMESPACE_YM_LLVMEQ
