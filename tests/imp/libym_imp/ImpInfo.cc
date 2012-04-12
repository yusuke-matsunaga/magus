
/// @file ImpInfo.cc
/// @brief ImpInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpInfo.h"
#include "ImpMgr.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
ImpInfo::ImpInfo()
{
  mArraySize = 0;
  mArray = NULL;
}

// @brief デストラクタ
ImpInfo::~ImpInfo()
{
  delete [] mArray;
}

// @brief 含意情報のリストを取り出す．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
const vector<ImpVal>&
ImpInfo::get(ymuint src_id,
	     ymuint src_val) const
{
  return mArray[src_id * 2 + src_val];
}

// @brief 含意の総数を得る．
ymuint
ImpInfo::size(const ImpMgr& imp_mgr) const
{
  ymuint n = 0;
  for (ymuint i = 0; i < mArraySize; ++ i) {
    ymuint src_id = i / 2;
    if ( imp_mgr.is_const(src_id) ) {
      continue;
    }
    const vector<ImpVal>& imp_list = mArray[i];
    for (vector<ImpVal>::const_iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ymuint dst_id = p->id();
      if ( !imp_mgr.is_const(dst_id) ) {
	++ n;
      }
    }
  }
  return n;
}

// @brief 実際に保持している含意の総数を返す．
ymuint
ImpInfo::imp_num() const
{
  ymuint n = 0;
  for (ymuint i = 0; i < mArraySize; ++ i) {
    n += mArray[i].size();
  }
  return n;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
ImpInfo::print(ostream& s) const
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    const vector<ImpVal>& imp_list = mArray[i];
    if ( imp_list.empty() ) continue;
    ymuint src_id = i / 2;
    ymuint src_val = i % 2;
    s << "Node#" << src_id << ": " << src_val << endl;
    for (vector<ImpVal>::const_iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      ymuint dst_id = p->id();
      ymuint dst_val = p->val();
      s << "   => Node#" << dst_id << ": " << dst_val << endl;
    }
    s << endl;
  }
}

// @brief 統計情報を出力する．
void
ImpInfo::print_stats(ostream& s) const
{
  s << "Total " << imp_num() << " implications ("
    << static_cast<double>(imp_num()) / (mArraySize * mArraySize) * 100
    << "%)" << endl;
}

// @brief 内容をクリアする．
void
ImpInfo::clear()
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    mArray[i].clear();
  }
  delete [] mArray;
  mArray = NULL;
}

// @brief サイズを設定する．
// @param[in] max_id ID番号の最大値
void
ImpInfo::set_size(ymuint max_id)
{
  clear();

  mArraySize = max_id * 2;
  mArray = new vector<ImpVal>[mArraySize];
}

// @brief 内容をセットする．
void
ImpInfo::set(vector<vector<ImpVal> >& imp_list_array)
{
  for (ymuint i = 0; i < mArraySize; ++ i) {
    vector<ImpVal>& imp_list = imp_list_array[i];
    sort(imp_list.begin(), imp_list.end());
    vector<ImpVal>::iterator p = unique(imp_list.begin(), imp_list.end());
    imp_list.erase(p, imp_list.end());
    mArray[i] = imp_list;
  }
}

END_NAMESPACE_YM_NETWORKS
