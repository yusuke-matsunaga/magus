
/// @file FrontierInfo.cc
/// @brief FrontierInfo の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "FrontierInfo.h"
#include "YmUtils/HashMap.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
// クラス FrontierInfo
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FrontierInfo::FrontierInfo()
{
}

// @brief コンストラクタ
// @param[in] src コピー元のソース
FrontierInfo::FrontierInfo(const FrontierInfo& src) :
  mNodeList(src.mNodeList)
{
}

// @brief デストラクタ
FrontierInfo::~FrontierInfo()
{
}

// @brief ノード数を返す．
ymuint
FrontierInfo::node_num() const
{
  return mNodeList.size();
}

// @brief ノード番号を返す．
// @param[in] pos 位置番号 ( 0 <= pos < node_num() )
ymuint
FrontierInfo::node_id(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mId;
}

// @brief 次数を返す．
// @param[in] pos ノードの位置番号
//
// pos はこのフロンティア内の相対的な位置
// ノード番号ではない．
ymuint
FrontierInfo::deg(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mDeg;
}

// @brief 次数を1つ増やす．
// @param[in] pos ノードの位置番号
//
// pos はこのフロンティア内の相対的な位置
// ノード番号ではない．
void
FrontierInfo::inc_deg(ymuint pos)
{
  ASSERT_COND( pos < node_num() );
  ++ mNodeList[pos].mDeg;
}

// @brief 連結成分番号を返す．
// @param[in] pos ノードの位置番号
//
// pos はこのフロンティア内の相対的な位置
// ノード番号ではない．
// 端子につながっている場合は - 端子番号を
// を返す．
int
FrontierInfo::comp(ymuint pos) const
{
  ASSERT_COND( pos < node_num() );
  return mNodeList[pos].mComp;
}

// @brief 連結成分番号の書き換えを行う．
// @param[in] old_comp 旧番号
// @param[in] new_comp 新番号
void
FrontierInfo::replace_comp(int old_comp,
			   int new_comp)
{
  for (ymuint i = 0; i < node_num(); ++ i) {
    if ( mNodeList[i].mComp == old_comp ) {
      mNodeList[i].mComp = new_comp;
    }
  }
}

// @brief ノードを削除する．
// @param[in] pos ノードの位置番号
void
FrontierInfo::delete_node(ymuint pos)
{
  ASSERT_COND( pos < node_num() );
  ymuint wpos = 0;
  for (ymuint rpos = 0; rpos < node_num(); ++ rpos) {
    if ( pos != rpos ) {
      const Node& node = mNodeList[rpos];
      if ( wpos != rpos ) {
	mNodeList[wpos] = node;
      }
      ++ wpos;
    }
  }
  mNodeList.erase(mNodeList.begin() + wpos, mNodeList.end());
}

// @brief ノードを削除する．
// @param[in] del_list 削除するノード番号のリスト
void
FrontierInfo::delete_nodes(const vector<ymuint>& del_list)
{
  ymuint dpos = 0;
  ymuint rpos = 0;
  ymuint wpos = 0;
  while ( rpos < node_num() && dpos < del_list.size() ) {
    ymuint id = mNodeList[rpos].mId;
    ymuint did = del_list[dpos];
    if ( id < did ) {
      if ( wpos < rpos ) {
	mNodeList[wpos] = mNodeList[rpos];
      }
      ++ wpos;
      ++ rpos;
    }
    else if ( id == did ) {
      ++ rpos;
      ++ dpos;
      if ( dpos >= del_list.size() ) {
	break;
      }
    }
    else {
      ++ dpos;
      if ( dpos >= del_list.size() ) {
	break;
      }
    }
  }
  while ( rpos < node_num() ) {
    if ( wpos < rpos ) {
      mNodeList[wpos] = mNodeList[rpos];
    }
    ++ rpos;
    ++ wpos;
  }
  mNodeList.erase(mNodeList.begin() + wpos, mNodeList.end());
}

// @brief ノードを追加する．
// @param[in] node_id ノード番号
// @param[in] deg 次数
// @param[in] comp_id 連結成分番号
//
// deg は 1, comp は自分自身となる．
void
FrontierInfo::add_node(ymuint node_id,
		       ymuint deg,
		       int comp_id)
{
  mNodeList.push_back(Node(node_id, deg, comp_id));
}

// @brief シグネチャを求める．
string
FrontierInfo::signature() const
{
  HashMap<int, ymuint> sig_map;
  ostringstream buf;
  for (ymuint i = 0; i < node_num(); ++ i) {
    const Node& node = mNodeList[i];
    int comp = node.mComp;
    ymuint val;
    if ( comp < 0 ) {
      val = (-comp) * 2;
    }
    else {
      if ( !sig_map.find(comp, val) ) {
	val = i;
	sig_map.add(comp, val);
      }
      val = val * 2 + 1;
    }
    buf << node.mDeg << ":" << val << ":";
  }
  return buf.str();
}

// @brief 内容を出力する．
void
FrontierInfo::dump(ostream& s) const
{
  for (ymuint i = 0; i < node_num(); ++ i) {
    const Node& node = mNodeList[i];
    s << "#" << node.mId << ": " << node.mDeg << ", " << node.mComp << endl;
  }
}

END_NAMESPACE_YM_NLINK
