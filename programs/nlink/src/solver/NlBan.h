#ifndef NLBAN_H
#define NLBAN_H

/// @file NlBan.h
/// @brief NlBan のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "nlink_nsdef.h"


BEGIN_NAMESPACE_YM_NLINK

//////////////////////////////////////////////////////////////////////
/// @class NlBan NlBan.h "NlBan.h"
/// @brief number link の盤面を表すクラス
//////////////////////////////////////////////////////////////////////
class NlBan
{
public:

  enum Dir {
    kNo_Dir = 0000U,
    kU__Dir = 0001U,
    kUR_Dir = 0002U,
    kR__Dir = 0004U,
    kDR_Dir = 0010U,
    kD__Dir = 0020U,
    kDL_Dir = 0040U,
    kL__Dir = 0100U,
    kUL_Dir = 0200U
  };

public:

  /// @brief コンストラクタ
  /// @param[in] graph 問題を表すグラフ
  NlBan(const NlGraph& graph);

  /// @brief デストラクタ
  ~NlBan();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 外周部の線分を確定させる．
  void
  phase1();

  /// @brief トポロジから経路を確定させる．
  bool
  phase2();

  /// @brief トポロジから経路を確定させる．
  bool
  phase3();

  /// @brief ラベルを得る．
  /// @param[in] x, y 座標
  ymuint
  label(ymuint x,
	ymuint y) const;

  /// @brief 内容を出力する．
  void
  dump(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 外周に沿った経路を確定させる．
  void
  fix_route(const vector<const NlNode*>& fringe_list,
	    ymuint pos1,
	    ymuint pos2,
	    ymuint terminal_id);

  /// @brief 隣の外周ノードを見つける．
  /// @param[in] x, y 座標
  /// @param[in] frame_bits 枠のある方向のビット
  /// @param[in] dir 方向ビット
  /// @return dir 方向に外周ノードがあれば true を返す．
  bool
  find_neighbor(ymuint& x,
		ymuint& y,
		ymuint frame_bits,
		ymuint dir);

  /// @brief 隣接している枠の方向を求める．
  /// @param[in] x, y 座標
  /// @return 枠のある方向のビット(のOR)を返す．
  ymuint
  frame_dir(ymuint x,
	    ymuint y) const;

  /// @brief phase2 の下請け関数
  bool
  check_bottleneck(ymuint x,
		   ymuint y,
		   ymuint tid,
		   ymuint pos1,
		   ymuint pos2);

  /// @brief 枠のとき true を返す．
  /// @param[in] x, y 座標
  /// @return (x, y) が枠の時 true を返す．
  bool
  is_frame(ymuint x,
	   ymuint y) const;

  /// @brief 端子のとき true を返す．
  /// @param[in] x, y 座標
  /// @return (x, y) が端子の時 true を返す．
  bool
  is_terminal(ymuint x,
	      ymuint y) const;

  /// @brief ラベルをつける．
  /// @param[in] x, y 座標
  /// @param[in] label ラベル
  /// @param[in] frame 枠フラグ
  void
  set_label(ymuint x,
	    ymuint y,
	    ymuint label,
	    bool frame);

  /// @brief 枠の印をつける．
  /// @param[in] x, y 座標
  void
  set_frame(ymuint x,
	    ymuint y);

  /// @brief 枠の印を消す．
  /// @param[in] x, y 座標
  void
  clear_frame(ymuint x,
	      ymuint y);

  /// @brief 格子を得る．
  /// @param[in] x, y 座標
  ymuint&
  grid(ymuint x,
       ymuint y);

  /// @brief 格子のインデックスを計算する．
  /// @param[in] x, y 座標
  ymuint
  index(ymuint x,
	ymuint y) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // グラフ
  const NlGraph& mGraph;

  // 幅
  ymuint mWidth;

  // 高さ
  ymuint mHeight;

  // 線分数
  ymuint mNum;

  // 格子の配列
  vector<ymuint> mGridArray;

  // phase1 終了後の外傷上のノードリスト
  vector<const NlNode*> mFringeList;

  // mFringeList の逆引きマップ
  vector<ymuint> mFringePos;

  // mFringeList に含まれるかどうかのマーク
  vector<bool> mFringeMark;

  // phase1 終了後の外周上の端子リスト
  vector<ymuint> mTermPosList;

};

END_NAMESPACE_YM_NLINK

#endif // NLBAN_H
