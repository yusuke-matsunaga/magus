
/// @file ImpInfo.cc
/// @brief ImpInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ImpInfo.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
// クラス ImpInfo
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
ImpInfo::ImpInfo()
{
  mSize = 0;
}

// @brief デストラクタ
ImpInfo::~ImpInfo()
{
}

// @brief 含意情報のリストを取り出す．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
const list<ImpCell>&
ImpInfo::get(ymuint src_id,
	     ymuint src_val) const
{
  return mArray[src_id * 2 + src_val];
}

// @brief 含意情報のリストを取り出す．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
list<ImpCell>&
ImpInfo::get(ymuint src_id,
	     ymuint src_val)
{
  return mArray[src_id * 2 + src_val];
}

// @brief 該当する含意情報が含まれているか調べる．
bool
ImpInfo::check(ymuint src_id,
	       ymuint src_val,
	       ymuint dst_id,
	       ymuint dst_val) const
{
  const list<ImpCell>& imp_list = mArray[src_id * 2 + src_val];
  for (list<ImpCell>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    const ImpCell& imp = *p;
    if ( imp.dst_id() == dst_id && imp.dst_val() == dst_val ) {
      return true;
    }
  }
  return false;
}

// @brief 含意の総数を得る．
ymuint
ImpInfo::size() const
{
  ymuint c = 0;
  ymuint n = mArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    const list<ImpCell>& imp_list = mArray[i];
    c += imp_list.size();
  }
  return c;
}

// @brief 内容を出力する．
// @param[in] s 出力先のストリーム
void
ImpInfo::print(ostream& s) const
{
  ymuint n = mArray.size();
  for (ymuint i = 0; i < n; ++ i) {
    const list<ImpCell>& imp_list = mArray[i];
    if ( imp_list.empty() ) continue;
    ymuint src_id = i / 2;
    ymuint src_val = i % 2;
    cout << "Node#" << src_id << ": " << src_val << endl;
    for (list<ImpCell>::const_iterator p = imp_list.begin();
	 p != imp_list.end(); ++ p) {
      const ImpCell& imp = *p;
      ymuint dst_id = imp.dst_id();
      ymuint dst_val = imp.dst_val();
      cout << "   => Node#" << dst_id << ": " << dst_val << endl;
    }
    cout << endl;
  }
  cout << "Total " << size() << " implications" << endl;
}

// @brief サイズを設定する．
// @param[in] max_id ID番号の最大値
void
ImpInfo::set_size(ymuint max_id)
{
  mArray.clear();
  mArray.resize(max_id * 2);
}

// @brief 含意情報を追加する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
// @param[in] dst_id 含意先のノード番号
// @param[in] dst_val 含意先の値 ( 0 or 1 )
void
ImpInfo::put(ymuint src_id,
	     ymuint src_val,
	     ymuint dst_id,
	     ymuint dst_val)
{
  mArray[src_id * 2 + src_val].push_back(ImpCell(dst_id, dst_val));
  ++ mSize;
}

// @brief 含意情報を追加する．
// @param[in] src_id 含意元のノード番号
// @param[in] src_val 含意元の値 ( 0 or 1 )
// @param[in] imp_list 含意リスト
void
ImpInfo::put(ymuint src_id,
	     ymuint src_val,
	     const vector<ImpCell>& imp_list)
{
  list<ImpCell>& dst_list = mArray[src_id * 2 + src_val];
  for (vector<ImpCell>::const_iterator p = imp_list.begin();
       p != imp_list.end(); ++ p) {
    ymuint dst_id = p->dst_id();
    ymuint dst_val = p->dst_val();
    if ( dst_id == src_id ) continue;
    dst_list.push_back(ImpCell(dst_id, dst_val));
  }
  mSize += imp_list.size();
}

END_NAMESPACE_YM_NETWORKS
