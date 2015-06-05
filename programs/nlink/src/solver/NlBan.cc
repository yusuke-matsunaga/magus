
/// @file NlBan.cc
/// @brief NlBan の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "NlBan.h"
#include "NlGraph.h"
#include "NlNode.h"


#define DEBUG_ROUTE 0

BEGIN_NAMESPACE_YM_NLINK

string
dir_str(ymuint dir_bits)
{
  ostringstream buf;
  if ( dir_bits & NlBan::kU__Dir ) {
    buf << " U";
  }
  if ( dir_bits & NlBan::kUR_Dir ) {
    buf << " UR";
  }
  if ( dir_bits & NlBan::kR__Dir ) {
    buf << " R";
  }
  if ( dir_bits & NlBan::kDR_Dir ) {
    buf << " DR";
  }
  if ( dir_bits & NlBan::kD__Dir ) {
    buf << " D";
  }
  if ( dir_bits & NlBan::kDL_Dir ) {
    buf << " DL";
  }
  if ( dir_bits & NlBan::kL__Dir ) {
    buf << " L";
  }
  if ( dir_bits & NlBan::kUL_Dir ) {
    buf << " UL";
  }
  return buf.str();
}

//////////////////////////////////////////////////////////////////////
// クラス NlBan
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] graph 問題を表すグラフ
NlBan::NlBan(const NlGraph& graph) :
  mGraph(graph)
{
  mWidth = graph.width();
  mHeight = graph.height();
  mNum = graph.num();

  mGridArray.resize((mWidth + 2) * (mHeight * 2));
  for (ymuint i = 0; i < mGridArray.size(); ++ i) {
    // 0ビット目は外周フラグ
    mGridArray[i] = 1;
  }

  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      grid(x, y) = 0;
    }
  }

  for (ymuint i = 0; i < mNum; ++ i) {
    // 1ビット目は端子フラグ
    const NlNode* node1 = graph.start_node(i);
    {
      ymuint x = node1->x();
      ymuint y = node1->y();
      grid(x, y) = ((i + 1) << 2) | 2;
    }
    const NlNode* node2 = graph.end_node(i);
    {
      ymuint x = node2->x();
      ymuint y = node2->y();
      grid(x, y) = ((i + 1) << 2) | 2;
    }
  }
}

// @brief デストラクタ
NlBan::~NlBan()
{
}

// @brief 外周部の線分を確定させる．
void
NlBan::phase1(vector<pair<const NlNode*, ymuint> >& fixed_list)
{
  // 最初の外周を求める．
  vector<const NlNode*> fringe_list;
  for (ymuint x = 0; x < mWidth; ++ x) {
    const NlNode* node = mGraph.node(x, 0);
    fringe_list.push_back(node);
  }
  for (ymuint y = 1; y < mHeight - 1; ++ y) {
    const NlNode* node = mGraph.node(mWidth - 1, y);
    fringe_list.push_back(node);
  }
  for (ymuint x = mWidth; x > 0; -- x) {
    const NlNode* node = mGraph.node(x - 1, mHeight - 1);
    fringe_list.push_back(node);
  }
  for (ymuint y = mHeight - 2; y > 0; -- y) {
    const NlNode* node = mGraph.node(0, y);
    fringe_list.push_back(node);
  }

  for ( ; ; ) {
#if DEBUG_ROUTE
    cout << "fringe list =";
    for (ymuint i = 0; i < fringe_list.size(); ++ i) {
      const NlNode* node = fringe_list[i];
      cout << " " << node->str();
    }
    cout << endl;
#endif

    // 外周上で同じ番号の端子が連続しているか調べる．
    vector<ymuint> termpos_list;
    for (ymuint i = 0; i < fringe_list.size(); ++ i) {
      const NlNode* node = fringe_list[i];
      ymuint tid = node->terminal_id();
      if ( tid > 0 ) {
	termpos_list.push_back(i);
      }
    }

    if ( termpos_list.size() <= 2 ) {
      break;
    }

#if DEBUG_ROUTE
    cout << "terminal list =";
    for (ymuint i = 0; i < termpos_list.size(); ++ i) {
      ymuint pos = termpos_list[i];
      const NlNode* node = fringe_list[pos];
      cout << " " << node->terminal_id();
    }
    cout << endl;
#endif

    // 連続していたら外周に沿って線を確定させる．
    ymuint last_pos = fixed_list.size();
    ymuint pos1 = termpos_list[0];
    const NlNode* node1 = fringe_list[pos1];
    ymuint tid1 = node1->terminal_id();
    for (ymuint i = 1; i < termpos_list.size(); ++ i) {
      ymuint pos2 = termpos_list[i];
      const NlNode* node2 = fringe_list[pos2];
      ymuint tid2 = node2->terminal_id();
      if ( tid1 == tid2 ) {
	fix_route(fringe_list, pos1, pos2, tid1, fixed_list);
      }
      pos1 = pos2;
      node1 = node2;
      tid1 = tid2;
    }
    ymuint pos2 = termpos_list[0];
    const NlNode* node2 = fringe_list[pos2];
    ymuint tid2 = node2->terminal_id();
    if ( tid1 == tid2 ) {
      fix_route(fringe_list, pos1, fringe_list.size() - 1, tid1, fixed_list);
      fix_route(fringe_list, 0, pos2, tid1, fixed_list);
    }

    if ( last_pos == fixed_list.size() ) {
      // 変化がなくなったら終わる．
      break;
    }

    // 新しい外周を求める．

    // 開始点を探す．
    ymuint x0;
    ymuint y0;
    bool found = false;
    for (x0 = 0; x0 < mWidth; ++ x0) {
      for (y0 = 0; y0 < mHeight; ++ y0) {
	if ( !is_frame(x0, y0) ) {
	  found = true;
	  break;
	}
      }
      if ( found ) {
	break;
      }
    }
    if ( !found ) {
      break;
    }

    ymuint dir_list[] = { kU__Dir, kR__Dir, kD__Dir, kL__Dir };

    // (x0, y0) は少なくとも2つは枠に隣接しているはず．
    // 具体的には上と左
    fringe_list.clear();
    ymuint x = x0;
    ymuint y = y0;
    ymuint start_pos = 0;
    for ( ; ; ) {
      fringe_list.push_back(mGraph.node(x, y));
      ymuint fcands = frame_dir(x, y);
#if DEBUG_ROUTE
      cout << " (" << x << ", " << y << "): fcands = " << dir_str(fcands) << endl;
#endif
      bool found = false;
      for (ymuint pos = start_pos; ; ) {
	ymuint dir = dir_list[pos];
	if ( find_neighbor(x, y, fcands, dir) ) {
	  ymuint alt_pos = pos ^ 2;
	  start_pos = (alt_pos + 1) % 4;
	  found = true;
#if DEBUG_ROUTE
	  cout << " dir       = " << dir_str(dir) << endl;
	  cout << " start_pos = " << start_pos << endl;
#endif
	  break;
	}
	pos = (pos + 1) % 4;
      }
      if ( !found ) {
	break;
      }
      if ( x == x0 && y == y0 ) {
	// 一周した．
	break;
      }
    }
  }
}

// @brief 隣の外周ノードを見つける．
bool
NlBan::find_neighbor(ymuint& x,
		     ymuint& y,
		     ymuint fcands,
		     ymuint dir)
{
  if ( (fcands & dir) == 0 ) {
    ymuint x1 = x;
    ymuint y1 = y;
    switch ( dir ) {
    case kU__Dir: -- y1; break;
    case kD__Dir: ++ y1; break;
    case kR__Dir: ++ x1; break;
    case kL__Dir: -- x1; break;
    }
    if ( frame_dir(x1, y1) != 0 ) {
      x = x1;
      y = y1;
      return true;
    }
  }
  return false;
}

// @brief 外周に沿った経路を確定させる．
void
NlBan::fix_route(const vector<const NlNode*>& fringe_list,
		 ymuint pos1,
		 ymuint pos2,
		 ymuint terminal_id,
		 vector<pair<const NlNode*, ymuint> >& fixed_list)
{
  ASSERT_COND( pos1 <= pos2 );

  for (ymuint i = pos1; i <= pos2; ++ i) {
    const NlNode* node = fringe_list[i];
    fixed_list.push_back(make_pair(node, terminal_id));
    grid(node->x(), node->y()) = (terminal_id << 2) | 1;
#if DEBUG_ROUTE
    cout << "  fix: " << node->str() << " " << terminal_id << endl;
#endif
  }
}

// @brief 外周のとき true を返す．
bool
NlBan::is_frame(ymuint x,
		ymuint y) const
{
  return static_cast<bool>(mGridArray[index(x, y)] & 1U);
}

// @brief 隣接している外周の方向を求める．
ymuint
NlBan::frame_dir(ymuint x,
		 ymuint y) const
{
  ASSERT_COND( !is_frame(x, y) );

  ymuint dx = mHeight + 2;
  ymuint dy = 1;
  ymuint index0 = index(x, y);
  ymuint ans = 0;
  if ( mGridArray[index0 - dy] & 1U ) {
    ans |= kU__Dir;
  }
  if ( mGridArray[index0 + dx - dy] & 1U ) {
    ans |= kUR_Dir;
  }
  if ( mGridArray[index0 + dx] & 1U ) {
    ans |= kR__Dir;
  }
  if ( mGridArray[index0 + dx + dy] & 1U ) {
    ans |= kDR_Dir;
  }
  if ( mGridArray[index0 + dy] & 1U ) {
    ans |= kD__Dir;
  }
  if ( mGridArray[index0 - dx + dy] & 1U ) {
    ans |= kDL_Dir;
  }
  if ( mGridArray[index0 - dx] & 1U ) {
    ans |= kL__Dir;
  }
  if ( mGridArray[index0 - dx - dy] & 1U ) {
    ans |= kUL_Dir;
  }
  return ans;
}

ymuint&
NlBan::grid(ymuint x,
	    ymuint y)
{
  return mGridArray[index(x, y)];
}

// @brief 格子のインデックスを計算する．
ymuint
NlBan::index(ymuint x,
	     ymuint y) const
{
  return (x + 1) * (mHeight + 2) + (y + 1);
}

END_NAMESPACE_YM_NLINK
