#ifndef MAGUS_LUTMAP_ENUMCUT_H
#define MAGUS_LUTMAP_ENUMCUT_H

/// @file lutmap/EnumCut.h
/// @brief EnumCut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "EnumCutOp.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_LUTMAP

//////////////////////////////////////////////////////////////////////
/// @class EnumCut EnumCut.h "EnumCut.h"
/// @brief cut 列挙用のクラス
//////////////////////////////////////////////////////////////////////
class EnumCut
{
  class NodeTemp;

public:

  /// @brief コンストラクタ
  EnumCut();

  /// @brief デストラクタ
  virtual
  ~EnumCut();

  /// @brief 入力数が limit 以下のクラスタを列挙する．
  /// @return 全 cut 数を返す．
  SizeType
  operator()(
    const SbjGraph& sbjgraph, ///< [in] 対象のサブジェクトグラフ
    SizeType limit,           ///< [in] 入力数の制限
    EnumCutOp* op             ///< [in] カットが列挙される時に呼ばれるクラス
  );


private:
  //////////////////////////////////////////////////////////////////////
  // プライベートメンバ関数
  //////////////////////////////////////////////////////////////////////

  // cut の列挙を行なうための再帰関数
  bool
  enum_recur();

  // node のカットになったノードに c1mark を付け，mMarkedNodes に入れる．
  void
  mark_cnode(
    const SbjNode* node
  );

  // node の TFI に c1mark を付ける．
  void
  mark_cnode2(
    const SbjNode* node
  );

  // node のカットになったノードに c1mark を付け，marked_nodes に入れる．
  void
  mark_cnode3(
    const SbjNode* node
  );

#if 0
  // root_depth よりも小さな depth を持つノードを frontier stack に積む．
  void
  get_frontier(
    const SbjNode* node,
    SizeType root_depth
  );

  // get_frontier で付けた印を消す．
  void
  clear_frontier(
    const SbjNode* node
  );
#endif

  // cmark の付いているノードを cnode_list に入れて
  // cmark を消す．
  void
  set_cut_node_list_recur(
    const SbjNode* node,
    vector<const SbjNode*>& cnode_list
  );

  // frontier stack にノードをプッシュする
  void
  push_node(
    const SbjNode* node
  );

  // frontier stack からノードをポップする
  const SbjNode*
  pop_node();

  // frontier stack が空のとき true を返す．
  bool
  frontier_is_empty() const;

  const NodeTemp&
  node_temp(
    const SbjNode* node
  ) const
  {
    return mNodeTemp[node->id()];
  }

  NodeTemp&
  node_temp(
    const SbjNode* node
  )
  {
    return mNodeTemp[node->id()];
  }

  int
  state(
    const SbjNode* node
  )
  {
    return node_temp(node).state();
  }

  void
  set_state1(
    const SbjNode* node
  )
  {
    node_temp(node).set_state1();
  }

  void
  set_state2(
    const SbjNode* node
  )
  {
    node_temp(node).set_state2();
  }

  void
  clear_state(
    const SbjNode* node
  )
  {
    node_temp(node).clear_state();
  }

  bool
  cmark(
    const SbjNode* node
  )
  {
    return node_temp(node).cmark();
  }

  void
  set_cmark(
    const SbjNode* node
  )
  {
    node_temp(node).set_cmark();
  }

  void
  clear_cmark(
    const SbjNode* node
  )
  {
    node_temp(node).clear_cmark();
  }

  bool
  temp1mark(
    const SbjNode* node
  )
  {
    return node_temp(node).temp1mark();
  }

  void
  set_temp1mark(
    const SbjNode* node
  )
  {
    node_temp(node).set_temp1mark();
  }

  bool
  temp2mark(
    const SbjNode* node
  )
  {
    return node_temp(node).temp2mark();
  }

  void
  set_temp2mark(
    const SbjNode* node
  )
  {
    node_temp(node).set_temp2mark();
  }

  void
  clear_tempmark(
    const SbjNode* node
  )
  {
    node_temp(node).clear_tempmark();
  }

  bool
  edge_mark(
    const SbjNode* node,
    SizeType pos
  )
  {
    return node_temp(node).edge_mark(pos);
  }

  void
  set_edge_mark(
    const SbjNode* node,
    SizeType pos
  )
  {
    node_temp(node).set_edge_mark(pos);
  }

  void
  clear_edge_mark(
    const SbjNode* node,
    SizeType pos
  )
  {
    node_temp(node).clear_edge_mark(pos);
  }

  vector<const SbjNode*>&
  cnode_list(
    const SbjNode* node
  )
  {
    return node_temp(node).cnode_list();
  }

  const vector<const SbjNode*>&
  cnode_list(
    const SbjNode* node
  ) const
  {
    return node_temp(node).cnode_list();
  }


private:

  // 各ノードごとの作業領域
  class NodeTemp
  {
  public:

    /// @brief コンストラクタ
    NodeTemp() = default;

    /// @brief 状態を取り出す．
    /// - 0: 初期状態
    /// - 1: cut の内部に含まれている
    /// - 2: cut の入力になっている
    int
    state() const
    {
      return mMarks & 3U;
    }

    /// @brief 内部に含まれた．
    void
    set_state1()
    {
      mMarks &= ~3U;
      mMarks |= 1U;
    }

    /// @brief 入力となった．
    void
    set_state2()
    {
      mMarks &= ~3U;
      mMarks |= 2U;
    }

    /// @brief 初期状態に戻す．
    void
    clear_state()
    {
      mMarks &= ~3U;
    }

    /// @brief クラスタノードの印を返す．
    bool
    cmark() const
    {
      return static_cast<bool>((mMarks >> 2) & 1U);
    }

    /// @brief クラスタノードの印をつける．
    void
    set_cmark()
    {
      mMarks |= 4U;
    }

    /// @brief クラスタノードの印を消す．
    void
    clear_cmark()
    {
      mMarks &= ~4U;
    }

    /// @brief temporary 用のマーク1を取り出す．
    bool
    temp1mark() const
    {
      return static_cast<bool>((mMarks >> 3) & 1U);
    }

    /// @brief temporary 用のマーク1をつける．
    void
    set_temp1mark()
    {
      mMarks |= 8U;
    }

    /// @brief temporary 用のマーク2を取り出す．
    bool
    temp2mark() const
    {
      return static_cast<bool>((mMarks >> 4) & 1U);
    }

    /// @brief temporary 用のマーク2をつける．
    void
    set_temp2mark()
    {
      mMarks |= 16U;
    }

    /// @brief temporary 用のマーク1/2を消す．
    void
    clear_tempmark()
    {
      mMarks &= ~(8U | 16U);
    }

    /// @brief pos 番めの枝のマークを取り出す．
    /// @note pos < 20
    bool
    edge_mark(
      SizeType pos
    ) const
    {
      return static_cast<bool>((mMarks >> (pos + 5)) & 1U);
    }

    /// @brief pos 番めの枝のマークを付ける．
    void
    set_edge_mark(
      SizeType pos
    )
    {
      mMarks |= (32U << pos);
    }

    /// @brief pos 番めの枝のマークを消す．
    void
    clear_edge_mark(
      SizeType pos
    )
    {
      mMarks &= ~(32U << pos);
    }

    /// @brief クラスタノードのリストを返す．
    vector<const SbjNode*>&
    cnode_list()
    {
      return mCnodeList;
    }

    /// @brief クラスタノードのリストを返す．
    const vector<const SbjNode*>&
    cnode_list() const
    {
      return mCnodeList;
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // このノードを根とする全カットのフットプリント
    vector<const SbjNode*> mCnodeList;

    // 種々のマーク
    std::uint32_t mMarks{0U};

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 入力数の最大値
  SizeType mLimit;

  // 全ノード数
  SizeType mNall;

  // 現在処理中のノード番号
  SizeType mCurPos;

  // 全 cut 数
  SizeType mNcAll;

  // 現在処理中のノードの cut 数
  SizeType mNcCur;

  // 根のノード
  const SbjNode* mRoot;

  // mFrontierStack のサイズ
  SizeType mFsSize;

  // 最前線ノードのスタック
  const SbjNode** mFrontierStack;

  // mFrontierStack の先頭を指すポインタ
  const SbjNode** mFsPos;

  // 確定した境界ノードを入れるベクタ
  const SbjNode** mInputs;

  // mInputs の次の書き込み位置
  SizeType mInputPos;

  // 内部ノードのスタック
  SizeType* mInodeStack;

  // 内部ノードスタックのスタックポインタ
  SizeType* mIsPos;

  // 各ノードごとの作業領域
  vector<NodeTemp> mNodeTemp;

  // マークの付いたノードを入れておく配列
  vector<const SbjNode*> mMarkedNodes;

  // mMarkedNodes の末尾
  SizeType mMarkedNodesLast;

  // カットが列挙されたときに呼ばれるクラス
  EnumCutOp* mOp;

};

END_NAMESPACE_LUTMAP

#endif // LIBYM_TECHMAP_LUTMAP_ENUMCUT_H
