#ifndef MAGUS_LUTMAP_CRNODE_H
#define MAGUS_LUTMAP_CRNODE_H

/// @file libym_techmap/lutmap/CrNode.h
/// @brief CrNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class CrNode CrNode.h "CrNode.h"
/// @brief CutResub で用いる作業領域のクラス
//////////////////////////////////////////////////////////////////////
class CrNode
{
  friend class CrHeap;

public:

  /// @brief コンストラクタ
  CrNode() = default;

  /// @brief デストラクタ
  ~CrNode() = default;


public:

  /// @brief 対応する SbjNode を設定する．
  void
  set_sbjnode(
    const SbjNode* node ///< [in] ノード
  );

  /// @brief 対応する SbjNode を返す．
  const SbjNode*
  sbjnode() const
  {
    return mNode;
  }

  /// @brief 外部入力ノードのとき true を返す．
  bool
  is_input() const
  {
    return mFlags.test(INPUT_BIT);
  }

  /// @brief 外部出力ノードにファンアウトしているとき true を返す．
  bool
  is_output() const
  {
    return mFlags.test(OUTPUT_BIT);
  }

  /// @brief 削除済みのマークをつける．
  void
  set_deleted()
  {
    mFlags.set(DELETE_BIT);
  }

  /// @brief 削除済みのとき true を返す．
  bool
  deleted() const
  {
    return mFlags.test(DELETE_BIT);
  }

  /// @brief GQ マークをつける．
  void
  set_GQ()
  {
    mFlags.set(GQ_BIT);
  }

  /// @brief GQ マークを消す．
  void
  clear_GQ()
  {
    mFlags.reset(GQ_BIT);
  }

  /// @brief GQ に入っているとき true を返す．
  bool
  in_GQ() const
  {
    return mFlags.test(GQ_BIT);
  }

  /// @brief LQ マークをつける．
  void
  set_LQ()
  {
    mFlags.set(LQ_BIT);
  }

  /// @brief LQ マークを消す．
  void
  clear_LQ()
  {
    mFlags.reset(LQ_BIT);
  }

  /// @brief LQ に入っているとき true を返す．
  bool
  in_LQ() const
  {
    return mFlags.test(LQ_BIT);
  }

  /// @brief RQ マークをつける．
  void
  set_RQ()
  {
    mFlags.set(RQ_BIT);
  }

  /// @brief RQ マークを消す．
  void
  clear_RQ()
  {
    mFlags.reset(RQ_BIT);
  }

  /// @brief RQ に入っているとき true を返す．
  bool
  in_RQ() const
  {
    return mFlags.test(RQ_BIT);
  }

  /// @brief カットをセットする．
  void
  set_cut(
    const Cut* cut ///< [in] カット
  )
  {
    mCurCut = cut;
  }

  /// @brief 現在のカットを返す．
  const Cut*
  cut() const
  {
    return mCurCut;
  }

  /// @brief ファンアウトリストに追加する．
  ///
  /// 重複していたら追加せず false を返す．
  /// 通常は追加して true を返す．
  bool
  add_fanout(
    CrNode* node ///< [in] 追加するノード
  );

  /// @brief ファンアウトリストから削除する．
  void
  delete_fanout(
    CrNode* node ///< [in] 削除するノード
  );

  /// @brief ファンアウトリストを得る．
  const vector<CrNode*>&
  fanout_list()
  {
    return mFanoutList;
  }

  /// @brief ゲインをセットする．
  void
  set_gain(
    SizeType gain ///< [in] 値
  )
  {
    mCurGain = gain;
  }

  /// @brief ゲインを得る．
  SizeType
  gain() const
  {
    return mCurGain;
  }

  /// @brief レベルをセットする．
  void
  set_level(
    SizeType level ///< [in] 値
  )
  {
    mLevel = level;
  }

  /// @brief レベルを返す．
  SizeType
  level() const
  {
    return mLevel;
  }

  /// @brief 要求レベルをセットする．
  void
  set_req_level(
    SizeType val ///< [in] 値
  )
  {
    mReqLevel = val;
  }

  /// @brief 要求レベルを返す．
  SizeType
  req_level() const
  {
    return mReqLevel;
  }

  /// @brief lock マークをつける．
  void
  lock()
  {
    mFlags.set(LOCK_BIT);
  }

  /// @brief lock マークを消す．
  void
  unlock()
  {
    mFlags.reset(LOCK_BIT);
  }

  /// @brief lock マークを返す．
  bool
  is_locked() const
  {
    return mFlags.test(LOCK_BIT);
  }

  /// @brief old マークをつける．
  void
  set_oldmark()
  {
    mFlags.set(OLD_BIT);
  }

  /// @brief old マークを消す．
  void
  clear_oldmark()
  {
    mFlags.reset(OLD_BIT);
  }

  /// @brief old マークを返す．
  bool
  oldmark()
  {
    return mFlags.test(OLD_BIT);
  }

  /// @brief new マークをつける．
  void
  set_newmark()
  {
    mFlags.set(NEW_BIT);
  }

  /// @brief new マークを消す．
  void
  clear_newmark()
  {
    mFlags.reset(NEW_BIT);
  }

  /// @brief new マークを返す．
  bool
  newmark()
  {
    return mFlags.test(NEW_BIT);
  }

  /// @brief ヒープ上の位置を返す．
  SizeType
  heap_index() const
  {
    return mHeapIndex;
  }

  /// @brief ヒープに入っていたら true を返す．
  bool
  in_heap() const
  {
    return mHeapIndex != static_cast<SizeType>(-1);
  }

  /// @brief ヒープ上の位置をセットする．
  void
  set_heap_index(
    SizeType pos ///< [in] 設定する位置
  )
  {
    mHeapIndex = pos;
  }


public:

  // 代役候補のカットのリスト
  vector<const Cut*> mAltCutList;

  SizeType mTmpLevel;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる定数
  //////////////////////////////////////////////////////////////////////

  static
  const int INPUT_BIT = 0;
  static
  const int OUTPUT_BIT = 1;
  static
  const int DELETE_BIT = 2;
  static
  const int GQ_BIT = 3;
  static
  const int LQ_BIT = 4;
  static
  const int RQ_BIT = 5;
  static
  const int LOCK_BIT = 6;
  static
  const int OLD_BIT = 7;
  static
  const int NEW_BIT = 8;
  static
  const int BIT_NUM = 9;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 根のノード
  const SbjNode* mNode{nullptr};

  // このノードを根とする最適カット
  const Cut* mCurCut{nullptr};

  // ファンアウト先のカットの根のノードのリスト
  vector<CrNode*> mFanoutList;

  // 種々のフラグ
  bitset<BIT_NUM> mFlags{0U};

  // 現在のゲイン
  SizeType mCurGain{0};

  // ヒープ上のインデックス
  // ヒープに入っていない時は -1
  int mHeapIndex{-1};

  // CrNode のネットワーク中の段数
  SizeType mLevel{0};

  // CrNode のネットワーク中の要求段数
  SizeType mReqLevel{0};

};


/// @relates CrNode
/// @brief CrHeap のための比較関数
bool
compare(
  CrNode* node1, ///< [in] ノード1
  CrNode* node2  ///< [in] ノード2
);

END_NAMESPACE_LUTMAP

#endif // MAGUS_LUTMAP_CRNODE_H
