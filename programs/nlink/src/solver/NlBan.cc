
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

  // 0ビット目は枠フラグ
  for (ymuint i = 0; i < mGridArray.size(); ++ i) {
    mGridArray[i] = 1;
  }

  // 枠以外を 0 で初期化する．
  for (ymuint x = 0; x < mWidth; ++ x) {
    for (ymuint y = 0; y < mHeight; ++ y) {
      grid(x, y) = 0;
    }
  }

  // 端子の印(1ビット目)をつける．
  for (ymuint i = 0; i < mNum; ++ i) {
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
NlBan::phase1()
{
  ymuint dir_list[] = { kU__Dir, kR__Dir, kD__Dir, kL__Dir };

  for ( ; ; ) {
    // 外周を求める．
    mFringeList.clear();
    mFringePos.clear();
    mFringePos.resize(mGraph.max_node_id(), 0);
    mFringeMark.clear();
    mFringeMark.resize(mGraph.max_node_id(), false);

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

    // (x0, y0) から始まる外周を求める．
    // 具体的には上と左
    ymuint x = x0;
    ymuint y = y0;
    ymuint start_pos = 0;
    for ( ; ; ) {
      const NlNode* node = mGraph.node(x, y);
      mFringePos[node->id()] = mFringeList.size();
      mFringeMark[node->id()] = true;
      mFringeList.push_back(node);

      ymuint fbits = frame_dir(x, y);
#if DEBUG_ROUTE
      cout << " (" << x << ", " << y << "): fbits = " << dir_str(fbits) << endl;
#endif
      bool found = false;
      for (ymuint pos = start_pos; ; ) {
	ymuint dir = dir_list[pos];
	if ( find_neighbor(x, y, fbits, dir) ) {
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

#if DEBUG_ROUTE
    cout << "fringe list =";
    for (ymuint i = 0; i < mFringeList.size(); ++ i) {
      const NlNode* node = mFringeList[i];
      cout << " " << node->str();
    }
    cout << endl;
#endif

    // 外周上で同じ番号の端子が連続しているか調べる．
    mTermPosList.clear();
    for (ymuint i = 0; i < mFringeList.size(); ++ i) {
      const NlNode* node = mFringeList[i];
      ymuint tid = label(node->x(), node->y());
      if ( tid > 0 ) {
	mTermPosList.push_back(i);
      }
    }

    if ( mTermPosList.size() <= 2 ) {
      break;
    }

#if DEBUG_ROUTE
    cout << "terminal list =";
    for (ymuint i = 0; i < mTermPosList.size(); ++ i) {
      ymuint pos = mTermPosList[i];
      const NlNode* node = mFringeList[pos];
      cout << " " << label(node->x(), node->y());
    }
    cout << endl;
#endif

    // 連続していたら外周に沿って線を確定させる．
    bool changed = false;
    ymuint pos1 = mTermPosList[0];
    const NlNode* node1 = mFringeList[pos1];
    ymuint tid1 = label(node1->x(), node1->y());
    for (ymuint i = 1; i < mTermPosList.size(); ++ i) {
      ymuint pos2 = mTermPosList[i];
      const NlNode* node2 = mFringeList[pos2];
      ymuint tid2 = label(node2->x(), node2->y());
      if ( tid1 == tid2 ) {
	fix_route(mFringeList, pos1, pos2, tid1);
	changed = true;
      }
      pos1 = pos2;
      node1 = node2;
      tid1 = tid2;
    }
    ymuint pos2 = mTermPosList[0];
    const NlNode* node2 = mFringeList[pos2];
    ymuint tid2 = label(node2->x(), node2->y());
    if ( tid1 == tid2 ) {
      fix_route(mFringeList, pos1, pos2, tid1);
      changed = true;
    }

    if ( !changed ) {
      // 変化がなくなったら終わる．
      break;
    }
  }
}

// @brief トポロジから経路を確定させる．
bool
NlBan::phase2()
{
  // 外周上に2つの端子がある線分を探す
  bool found = false;
  for (ymuint i1 = 0; i1 < mTermPosList.size(); ++ i1) {
    ymuint pos1 = mTermPosList[i1];
    const NlNode* node1 = mFringeList[pos1];
    ymuint tid1 = label(node1->x(), node1->y());
    for (ymuint i2 = i1 + 1; i2 < mTermPosList.size(); ++ i2) {
      ymuint pos2 = mTermPosList[i2];
      const NlNode* node2 = mFringeList[pos2];
      ymuint tid2 = label(node2->x(), node2->y());
      if ( tid1 != tid2 ) {
	continue;
      }
      if ( i1 + 1 == i2 ) {
	continue;
      }

      // i1 と i2 の間の端子を調べる．
      for (ymuint i = i1 + 1; i < i2; ++ i) {
	ymuint pos = mTermPosList[i];
	const NlNode* node = mFringeList[pos];
	ymuint tid = label(node->x(), node->y());
	ASSERT_COND( tid > 0 );
	// tid の反対側の端子を求める．
	const NlNode* s_node = mGraph.start_node(tid - 1);
	const NlNode* e_node = mGraph.end_node(tid - 1);
	const NlNode* alt_node = (s_node == node) ? e_node : s_node;
	if ( mFringeMark[alt_node->id()] ) {
	  // alt_node も外周上なら無視する．
	  //ymuint pos = mFringePos[alt_node->id()];
	  //ASSERT_COND( pos > i1 && pos < i2 );
	  continue;
	}
	// alt_node が外周ノードに隣接していたら
	// そのノードは tid1 の経路になる．
	ymuint x0 = alt_node->x();
	ymuint y0 = alt_node->y();
	if ( x0 > 0 ) {
	  found |= check_bottleneck(x0 - 1, y0, tid1, pos1, pos2);
	}
	if ( x0 < mWidth - 1) {
	  found |= check_bottleneck(x0 + 1, y0, tid1, pos1, pos2);
	}
	if ( y0 > 0 ) {
	  found |= check_bottleneck(x0, y0 - 1, tid1, pos1, pos2);
	}
	if ( y0 < mHeight - 1 ) {
	  found |= check_bottleneck(x0, y0 + 1, tid1, pos1, pos2);
	}
      }

      // i2 と i1 の間の端子を調べる．
      for (ymuint i = (i2 + 1) % mTermPosList.size();
	   i != i1; i = (i + 1) % mTermPosList.size()) {
	ymuint pos = mTermPosList[i];
	const NlNode* node = mFringeList[pos];
	ymuint tid = label(node->x(), node->y());
	ASSERT_COND( tid > 0 );
	// tid の反対側の端子を求める．
	const NlNode* s_node = mGraph.start_node(tid - 1);
	const NlNode* e_node = mGraph.end_node(tid - 1);
	const NlNode* alt_node = (s_node == node) ? e_node : s_node;
	if ( mFringeMark[alt_node->id()] ) {
	  // alt_node も外周上なら無視する．
	  //ymuint pos = mFringePos[alt_node->id()];
	  //ASSERT_COND( pos > i1 && pos < i2 );
	}
	else {
	  // alt_node が外周ノードに隣接していたら
	  // そのノードは tid1 の経路になる．
	  ymuint x0 = alt_node->x();
	  ymuint y0 = alt_node->y();
	  if ( x0 > 0 ) {
	    found |= check_bottleneck(x0 - 1, y0, tid1, pos2, pos1);
	  }
	  if ( x0 < mWidth - 1) {
	    found |= check_bottleneck(x0 + 1, y0, tid1, pos2, pos1);
	  }
	  if ( y0 > 0 ) {
	    found |= check_bottleneck(x0, y0 - 1, tid1, pos2, pos1);
	  }
	  if ( y0 < mHeight - 1 ) {
	    found |= check_bottleneck(x0, y0 + 1, tid1, pos2, pos1);
	  }
	}
      }
    }
  }

  return found;
}

// @brief トポロジから経路を確定させる．
bool
NlBan::phase3()
{
  bool changed = false;
  for (ymuint node_id = 0; node_id < mGraph.max_node_id(); ++ node_id) {
    const NlNode* node = mGraph.node(node_id);
    ymuint x = node->x();
    ymuint y = node->y();
    ymuint tid = label(x, y);
    if ( tid == 0 ) {
      continue;
    }
    vector<const NlNode*> avail_list;
    avail_list.reserve(4);
    ymuint n = 0;
    if ( x > 0 ) {
      ymuint x1 = x - 1;
      ymuint y1 = y;
      ymuint tid1 = label(x1, y1);
      if ( tid1 == 0 ) {
	const NlNode* node1 = mGraph.node(x1, y1);
	avail_list.push_back(node1);
      }
      else if ( tid1 == tid ) {
	++ n;
      }
    }
    if ( x < mWidth - 1 ) {
      ymuint x1 = x + 1;
      ymuint y1 = y;
      ymuint tid1 = label(x1, y1);
      if ( tid1 == 0 ) {
	const NlNode* node1 = mGraph.node(x1, y1);
	avail_list.push_back(node1);
      }
      else if ( tid1 == tid ) {
	++ n;
      }
    }
    if ( y > 0 ) {
      ymuint x1 = x;
      ymuint y1 = y - 1;
      ymuint tid1 = label(x1, y1);
      if ( tid1 == 0 ) {
	const NlNode* node1 = mGraph.node(x1, y1);
	avail_list.push_back(node1);
      }
      else if ( tid1 == tid ) {
	++ n;
      }
    }
    if ( y < mHeight - 1 ) {
      ymuint x1 = x;
      ymuint y1 = y + 1;
      ymuint tid1 = label(x1, y1);
      if ( tid1 == 0 ) {
	const NlNode* node1 = mGraph.node(x1, y1);
	avail_list.push_back(node1);
      }
      else if ( tid1 == tid ) {
	++ n;
      }
    }
    bool terminal = is_terminal(x, y);
    if ( terminal ) {
      if ( n == 0 && avail_list.size() == 1 ) {
	const NlNode* node1 = avail_list[0];
	set_label(node1->x(), node1->y(), tid, false);
	changed = true;
      }
    }
    else {
      if ( n == 0 ) {
	if ( avail_list.size() == 2 ) {
	  const NlNode* node1 = avail_list[0];
	  set_label(node1->x(), node1->y(), tid, false);
	  const NlNode* node2 = avail_list[1];
	  set_label(node2->x(), node2->y(), tid, false);
	  changed = true;
	}
      }
      else if ( n == 1 ) {
	if ( avail_list.size() == 1 ) {
	  const NlNode* node1 = avail_list[0];
	  set_label(node1->x(), node1->y(), tid, false);
	  changed = true;
	}
      }
    }
  }

  if ( changed ) {
    // frame ビットの更新を行う．
    for (ymuint node_id = 0; node_id < mGraph.max_node_id(); ++ node_id) {
      const NlNode* node = mGraph.node(node_id);
      ymuint x = node->x();
      ymuint y = node->y();
      ymuint tid = label(x, y);
      if ( tid == 0 ) {
	continue;
      }

      ymuint n = 0;
      if ( x > 0 ) {
	ymuint x1 = x - 1;
	ymuint y1 = y;
	ymuint tid1 = label(x1, y1);
	if ( tid1 == tid ) {
	  ++ n;
	}
      }
      if ( x < mWidth - 1 ) {
	ymuint x1 = x + 1;
	ymuint y1 = y;
	ymuint tid1 = label(x1, y1);
	if ( tid1 == tid ) {
	  ++ n;
	}
      }
      if ( y > 0 ) {
	ymuint x1 = x;
	ymuint y1 = y - 1;
	ymuint tid1 = label(x1, y1);
	if ( tid1 == tid ) {
	  ++ n;
	}
      }
      if ( y < mHeight - 1 ) {
	ymuint x1 = x;
	ymuint y1 = y + 1;
	ymuint tid1 = label(x1, y1);
	if ( tid1 == tid ) {
	  ++ n;
	}
      }
      if ( is_terminal(x, y) ) {
	if ( n == 1 ) {
	  set_frame(x, y);
	}
      }
      else {
	if ( n == 2 ) {
	  set_frame(x, y);
	}
	else {
	  clear_frame(x, y);
	}
      }
    }
  }

  return changed;
}

// @brief 外周に沿った経路を確定させる．
void
NlBan::fix_route(const vector<const NlNode*>& fringe_list,
		 ymuint pos1,
		 ymuint pos2,
		 ymuint terminal_id)
{
  for (ymuint i = pos1; ; i = (i + 1) % fringe_list.size()) {
    const NlNode* node = fringe_list[i];
    ymuint x = node->x();
    ymuint y = node->y();
    set_label(x, y, terminal_id, true);

#if DEBUG_ROUTE
    cout << "  fix: " << node->str() << " " << terminal_id << endl;
#endif
    if ( i == pos2 ) {
      break;
    }
  }
}

// @brief 隣の外周ノードを見つける．
// @param[in] x, y 座標
// @param[in] frame_bits 枠のある方向のビット
// @param[in] dir 方向ビット
bool
NlBan::find_neighbor(ymuint& x,
		     ymuint& y,
		     ymuint frame_bits,
		     ymuint dir)
{
  if ( (frame_bits & dir) == 0 ) {
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

// @brief 隣接している枠の方向を求める．
// @param[in] x, y 座標
// @return 枠のある方向のビット(のOR)を返す．
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

// @brief phase2 の下請け関数
bool
NlBan::check_bottleneck(ymuint x,
			ymuint y,
			ymuint tid,
			ymuint pos1,
			ymuint pos2)
{
  const NlNode* node = mGraph.node(x, y);
  if ( !mFringeMark[node->id()] ) {
    return false;
  }
  if ( label(x, y) > 0 ) {
    return false;
  }
  ymuint pos = mFringePos[node->id()];
  if ( pos1 < pos2 ) {
    // pos1 |----------| pos2
    if ( pos > pos1 && pos < pos2 ) {
      return false;
    }
  }
  else {
    // -----| pos2    pos1 |-------
    if ( pos < pos2 || pos > pos1 ) {
      return false;
    }
  }
  if ( mFringeMark[node->id()] ) {
    set_label(x, y, tid, false);

#if DEBUG_ROUTE
    cout << " " << node->str() << " : " << tid << endl;
#endif
    return true;
  }
  return false;
}

// @brief 枠のとき true を返す．
// @param[in] x, y 座標
bool
NlBan::is_frame(ymuint x,
		ymuint y) const
{
  return static_cast<bool>(mGridArray[index(x, y)] & 1U);
}

// @brief 端子のとき true を返す．
// @param[in] x, y 座標
// @return (x, y) が端子の時 true を返す．
bool
NlBan::is_terminal(ymuint x,
		   ymuint y) const
{
  return static_cast<bool>((mGridArray[index(x, y)] >> 1) & 1U);
}

// @brief ラベルを得る．
// @param[in] x, y 座標
ymuint
NlBan::label(ymuint x,
	     ymuint y) const
{
  return mGridArray[index(x, y)] >> 2;
}

// @brief ラベルをつける．
// @param[in] x, y 座標
// @param[in] label ラベル
// @param[in] frame 枠フラグ
void
NlBan::set_label(ymuint x,
		 ymuint y,
		 ymuint label,
		 bool frame)
{
  ymuint fbit = frame ? 1U : 0U;
  mGridArray[index(x, y)] |= (label << 2) | fbit;
}

// @brief 枠の印をつける．
// @param[in] x, y 座標
void
NlBan::set_frame(ymuint x,
		 ymuint y)
{
  mGridArray[index(x, y)] |= 1U;
}

// @brief 枠の印を消す．
// @param[in] x, y 座標
void
NlBan::clear_frame(ymuint x,
		   ymuint y)
{
  mGridArray[index(x, y)] &= ~1U;
}

ymuint&
NlBan::grid(ymuint x,
	    ymuint y)
{
  return mGridArray[index(x, y)];
}

// @brief 格子のインデックスを計算する．
// @param[in] x, y 座標
ymuint
NlBan::index(ymuint x,
	     ymuint y) const
{
  return (x + 1) * (mHeight + 2) + (y + 1);
}

// @brief 内容を出力する．
void
NlBan::dump(ostream& s) const
{
  for (ymuint y = 0; y < mHeight; ++ y) {
    for (ymuint x = 0; x < mWidth; ++ x) {
      ymuint tid = label(x, y);
      bool f = is_frame(x, y);
      bool t = is_terminal(x, y);
      s << " " << setw(2) << tid;
      if ( t ) {
	s << "T";
      }
      else {
	s << " ";
      }
      if ( f ) {
	s << "F";
      }
      else {
	s << " ";
      }
    }
    s << endl;
  }
  s << endl;
}

END_NAMESPACE_YM_NLINK
