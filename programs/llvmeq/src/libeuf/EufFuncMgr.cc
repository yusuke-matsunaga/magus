
/// @file EufFuncMgr.cc
/// @brief EufFuncMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "EufFuncMgr.h"
#include "EufFunc.h"


BEGIN_NAMESPACE_YM_LLVMEQ

//////////////////////////////////////////////////////////////////////
// クラス EufFuncMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
EufFuncMgr::EufFuncMgr() :
  mAlloc(sizeof(EufFunc), 1024)
{
  mNum = 0;
  mTableSize = 0;
  mTable = NULL;
  expand(1024);
}

// @brief デストラクタ
EufFuncMgr::~EufFuncMgr()
{
  delete[] mTable;
}

// @brief 関数を探す．
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
// @note 見つからなければ NULL を返す．
EufNode*
EufFuncMgr::find(const string& name,
		 const vector<EufNode*>& arg_list) const
{
  ymuint h = hash(name, arg_list);
  ymuint n = arg_list.size();
  for (EufFunc* f = mTable[h]; f != NULL; f = f->mLink) {
    if ( f->id_name() != name || f->arg_num() != n ) {
      continue;
    }
    bool eq = true;
    for (ymuint i = 0; i < n; ++ i) {
      if ( f->arg(i) != arg_list[i] ) {
	eq = false;
	break;
      }
    }
    if ( eq ) {
      return f;;
    }
  }
  return NULL;
}

// @brief 関数を生成する．
// @param[in] id ID番号
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
EufNode*
EufFuncMgr::new_function(ymuint id,
			 const string& name,
			 const vector<EufNode*>& arg_list)
{
  if ( mNum >= mNextLimit ) {
    expand(mTableSize * 2);
  }

  void* p = mAlloc.get_memory(sizeof(EufFunc));
  EufFunc* f = new (p) EufFunc(id, name, arg_list);

  ymuint h = hash(name, arg_list);
  f->mLink = mTable[h];
  mTable[h] = f;

  ++ mNum;

  return f;
}

// @brief ハッシュテーブルを拡大する．
void
EufFuncMgr::expand(ymuint req_size)
{
  ymuint old_size = mTableSize;
  EufFunc** old_table = mTable;

  mTableSize = req_size;
  mTable = new EufFunc*[mTableSize];
  for (ymuint i = 0; i < mTableSize; ++ i) {
    mTable[i] = NULL;
  }
  mNextLimit = static_cast<ymuint32>(mTableSize * 1.8);

  if ( old_size > 0 ) {
    for (ymuint i = 0; i < old_size; ++ i) {
      for (EufFunc* f = old_table[i]; f != NULL; ) {
	EufFunc* tmp_f = f;
	f = tmp_f->mLink;

	ymuint h = hash(tmp_f->mName, tmp_f->mArgList);
	tmp_f->mLink = mTable[h];
	mTable[h] = tmp_f;
      }
    }
    delete [] old_table;
  }
}

// @brief ハッシュ値を計算する．
// @param[in] name 関数名
// @param[in] arg_list 引数のリスト
ymuint
EufFuncMgr::hash(const string& name,
		 const vector<EufNode*>& arg_list) const
{
  ymuint h = 0;
  for (string::const_iterator p = name.begin();
       p != name.end(); ++ p) {
    ymuint c = *p;
    h = h * 37 + c;
  }
  ymuint n = arg_list.size();
  for (ymuint i = 0; i < n; ++ i) {
    h = h * 13 + arg_list[i]->id();
  }
  return h % mTableSize;
}

END_NAMESPACE_YM_LLVMEQ

