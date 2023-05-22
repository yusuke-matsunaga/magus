#ifndef SMDNODE_H
#define SMDNODE_H

/// @file SmdNode.h
/// @brief SmdNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "sbj_nsdef.h"


BEGIN_NAMESPACE_SBJ

//////////////////////////////////////////////////////////////////////
/// @class SmdEdge SmdNode.h "SmdNode.h"
/// @brief SmdNode をつなぐ枝を表すクラス
//////////////////////////////////////////////////////////////////////
class SmdEdge
{
  friend class SmdNode;

public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報を取得す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力側のノードを得る．
  SmdNode*
  from()
  {
    return mFrom;
  }

  /// @brief 出力側のノードを得る．
  SmdNode*
  to()
  {
    return mTo;
  }

  /// @brief 出力側のノードのファンイン番号 ( 0 or 1 ) を得る．
  SizeType
  pos()
  {
    return static_cast<SizeType>(mFlags[0]);
  }


public:
  //////////////////////////////////////////////////////////////////////
  // get_min_depth() で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief flow が流れている時 true を返す．
  bool
  flow() const
  {
    return mFlags[1];
  }

  /// @brief flow フラグをセットする．
  void
  set_flow()
  {
    mFlags[1] = true;
  }

  /// @brief flow フラグをクリアする．
  void
  clear_flow()
  {
    mFlags[1] = false;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力側のノード
  SmdNode* mFrom;

  // 出力側のノード
  SmdNode* mTo;

  // ファンイン番号＋ flow フラグ
  bitset<2> mFlags;

};


//////////////////////////////////////////////////////////////////////
/// @class SmdNode SmdNode.h "SmdNode.h"
/// @brief SbjMinDepth 用の作業領域
//////////////////////////////////////////////////////////////////////
class SmdNode
{
public:

  /// @brief コンストラクタ
  SmdNode() = default;

  /// @brief デストラクタ
  ~SmdNode() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 構造に関する情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号の取得
  SizeType
  id() const
  {
    return (mId >> 1);
  }

  /// @brief 入力ノードの時に true を返す．
  bool
  is_input() const
  {
    return !is_logic();
  }

  /// @brief 論理ノードの時に true を返す．
  bool
  is_logic() const
  {
    return static_cast<bool>(mId & 1U);
  }

  /// @brief 1つ目のファンインを得る．
  SmdNode*
  fanin0()
  {
    return mFanin0.from();
  }

  /// @brief 2つ目のファンインを得る．
  SmdNode*
  fanin1()
  {
    return mFanin1.from();
  }

  /// @brief 1つ目のファンインの枝を得る．
  SmdEdge*
  fanin0_edge()
  {
    return &mFanin0;
  }

  /// @brief 2つ目のファンインの枝を得る．
  SmdEdge*
  fanin1_edge()
  {
    return &mFanin1;
  }

  /// @brief ファンアウト数を得る．
  SizeType
  fanout_num()
  {
    return mFanoutArray.size();
  }

  /// @brief pos 番目のファンアウトの枝を取り出す．
  SmdEdge*
  fanout_edge(
    SizeType pos
  )
  {
    ASSERT_COND( pos >=0 && pos < fanout_num() );

    return mFanoutArray[pos];
  }

  /// @brief 深さを得る．
  SizeType
  depth() const
  {
    return mDepth;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 内容を設定する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を設定する．
  void
  set_id(
    SizeType id,
    bool input = false
  )
  {
    mId = (id << 1) | static_cast<int>(input);
  }

  /// @brief 1つ目のファンインを設定する．
  void
  set_fanin0(
    SmdNode* from
  )
  {
    mFanin0.mFrom = from;
    mFanin0.mTo = this;
    mFanin0.mFlags = 0U;
  }

  /// @brief 2つ目のファンインを設定する．
  void
  set_fanin1(
    SmdNode* from
  )
  {
    mFanin1.mFrom = from;
    mFanin1.mTo = this;
    mFanin1.mFlags = 1U;
  }

  /// @brief ファンアウト配列を設定する．
  void
  set_fanout_array(
    const vector<SmdEdge*>& foedge_list
  )
  {
    mFanoutArray = foedge_list;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // get_min_depth() で用いる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief range マークを調べる．
  /// @note 副作用として range マークを付ける．
  bool
  check_rmark()
  {
    auto old_mark = mMark;
    mMark |= kRangeMask;
    return static_cast<bool>((old_mark >> kRangeShift) & 1U);
  }

  /// @brief range マークを返す．
  bool
  rmark()
  {
    return static_cast<bool>((mMark >> kRangeShift) & 1U);
  }

  /// @brief target マークを返す．
  bool
  tmark()
  {
    return static_cast<bool>((mMark >> kTargetShift) & 1U);
  }

  /// @brief target マークを付ける．
  void
  set_tmark()
  {
    mMark |= kTargetMask;
  }

  /// @brief flow 用のマークを返す．
  bool
  fmark()
  {
    return static_cast<bool>((mMark >> kFlowShift) & 1U);
  }

  /// @brief flow 用のマークを付ける．
  void
  set_fmark()
  {
    mMark |= kFlowMask;
  }

  /// @brief range/target/flow マークを消す．
  void
  clear_rtfmark()
  {
    mMark &= ~(kRangeMask | kTargetMask | kFlowMask);
  }

  /// @brief 入力側の visit フラグを調べる．
  /// @note 副作用として v1 マークを付ける．
  bool
  check_vmark1()
  {
    auto old_mark = mMark;
    mMark |= kV1Mask;
    return static_cast<bool>((old_mark >> kV1Shift) & 1U);
  }

  /// @brief 出力側の visit フラグを調べる．
  /// @note 副作用として v2 マークを付ける．
  bool
  check_vmark2()
  {
    auto old_mark = mMark;
    mMark |= kV2Mask;
    return static_cast<bool>((old_mark >> kV2Shift) & 1U);
  }

  /// @brief visit フラグを消す．
  void
  clear_vmark()
  {
    mMark &= ~(kV1Mask | kV2Mask);
  }

  /// @brief 深さを設定する．
  void
  set_depth(
    SizeType depth
  )
  {
    mDepth = depth;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号 + 入力/論理フラグ
  SizeType mId;

  // 1つ目のファンインの枝
  SmdEdge mFanin0;

  // 2つ目のファンインの枝
  SmdEdge mFanin1;

  // ファンアウトの枝のポインタの配列
  vector<SmdEdge*> mFanoutArray;

  // get_min_depth() 用の作業領域
  SizeType mMark;

  // 深さ
  SizeType mDepth;


private:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  //////////////////////////////////////////////////////////////////////

  // mMarkArray の演算で用いる定数
  static
  const int kRangeShift = 0;
  static
  const int kTargetShift = 1;
  static
  const int kFlowShift = 2;
  static
  const int kPoShift = 3;
  static
  const int kV1Shift = 4;
  static
  const int kV2Shift = 5;

  static
  const SizeType kRangeMask = 1U << kRangeShift;
  static
  const SizeType kTargetMask = 1U << kTargetShift;
  static
  const SizeType kFlowMask = 1U << kFlowShift;
  static
  const SizeType kPoMask = 1U << kPoShift;
  static
  const SizeType kV1Mask = 1U << kV1Shift;
  static
  const SizeType kV2Mask = 1U << kV2Shift;

};

END_NAMESPACE_SBJ

#endif // SMDNODE_H
