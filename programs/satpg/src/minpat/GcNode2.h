#ifndef GCNODE2_H
#define GCNODE2_H

/// @file GcNode2.h
/// @brief GcNode2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "NodeValList.h"
#include "TpgCnf1.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class GcNode2 GcNode2.h "GcNode2.h"
/// @brief 彩色問題を表すグラフのノード
//////////////////////////////////////////////////////////////////////
class GcNode2
{
public:

  /// @brief コンストラクタ
  GcNode2();

  /// @brief デストラクタ
  ~GcNode2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] id 番号
  /// @param[in] fault 故障
  /// @param[in] vectlen ベクタ長
  void
  init(ymuint id,
       TpgFault* fault,
       ymuint vectlen);

  /// @brief ID番号を得る．
  ymuint
  id() const;

  /// @brief 故障を返す．
  TpgFault*
  fault() const;

  /// @brief saturation degree を返す．
  ymuint
  sat_degree() const;

  /// @brief 色番号を設定する．
  void
  set_color(ymuint color);

  /// @brief 色番号を得る．
  /// @note 0 は未彩色を表す．
  ymuint
  color() const;

  /// @brief color のノードが隣接しているときに true を返す．
  bool
  check_adj_color(ymuint color) const;

  /// @brief color を隣接色に加える．
  void
  add_adj_color(ymuint color);

  /// @brief ヒープ上の位置(+1)を返す．
  ///
  /// ヒープになければ 0 を返す．
  ymuint
  heap_location() const;

  /// @brief ヒープ上の位置を設定する．
  void
  set_heap_location(ymuint pos);

  /// @brief 十分割当リストをセットする．
  void
  set_suf_list(ymuint color,
	       const NodeValList& suf_list);

  /// @brief 十分割当リストを得る．
  const NodeValList&
  suf_list(ymuint color);

  /// @brief CNFを削除する．
  void
  clear_cnf();

  /// @brief 両立性のチェックの要求を記録する．
  void
  reg_check(ymuint col_id);

  /// @brief ペンディングしているチェック要求の数を返す．
  ymuint
  pending_num();

  /// @brief ペンディングしているチェック要求を1つ取り出す．
  ymuint
  get_check();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // ヒープ中の位置(+1)
  ymuint mHeapIdx;

  // 故障
  TpgFault* mFault;

  // 隣接するノードの色の集合を表すビットベクタ
  ymuint64* mColorSet;

  // SAT degree
  ymuint mSatDegree;

  // 色番号
  // 0 が未彩色
  ymuint mColor;

  // 十分割り当てリストの配列
  vector<NodeValList> mSufListArray;

  // チェック用のCNF式
  TpgCnf1* mTpgCnf;

  // チェックをペンディングしているグループを表すフラグ配列
  vector<bool> mPendingArray;

  // ペンディングしているグループ数
  ymuint mPendingNum;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID番号を得る．
inline
ymuint
GcNode2::id() const
{
  return mId;
}

// @brief 故障を返す．
inline
TpgFault*
GcNode2::fault() const
{
  return mFault;
}

// @brief saturation degree を返す．
inline
ymuint
GcNode2::sat_degree() const
{
  return mSatDegree;
}

// @brief 色番号を設定する．
inline
void
GcNode2::set_color(ymuint color)
{
  mColor = color;
}

// @brief 色番号を得る．
// @note 0 は未彩色を表す．
inline
ymuint
GcNode2::color() const
{
  return mColor;
}

// @brief color のノードが隣接しているときに true を返す．
inline
bool
GcNode2::check_adj_color(ymuint color) const
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  return ((mColorSet[blk] >> sft) & 1UL) == 1UL;
}

// @brief color を隣接色に加える．
inline
void
GcNode2::add_adj_color(ymuint color)
{
  ymuint blk = color / 64;
  ymuint sft = color % 64;
  mColorSet[blk] |= (1UL << sft);
  ++ mSatDegree;
}

// @brief ヒープ上の位置(+1)を返す．
//
// ヒープになければ 0 を返す．
inline
ymuint
GcNode2::heap_location() const
{
  return mHeapIdx;
}

// @brief ヒープ上の位置を設定する．
inline
void
GcNode2::set_heap_location(ymuint pos)
{
  mHeapIdx = pos;
}

// @brief 十分割当リストをセットする．
inline
void
GcNode2::set_suf_list(ymuint color,
		      const NodeValList& suf_list)
{
  while ( mSufListArray.size() <= color ) {
    mSufListArray.push_back(NodeValList());
  }
  mSufListArray[color] = suf_list;
}

// @brief 十分割当リストを得る．
inline
const NodeValList&
GcNode2::suf_list(ymuint color)
{
  return mSufListArray[color];
}

// @brief CNFを削除する．
inline
void
GcNode2::clear_cnf()
{
  delete mTpgCnf;
}

// @brief 両立性のチェックの要求を記録する．
inline
void
GcNode2::reg_check(ymuint col_id)
{
  while ( mPendingArray.size() <= col_id ) {
    mPendingArray.push_back(false);
  }
  if ( !mPendingArray[col_id] ) {
    mPendingArray[col_id] = true;
    ++ mPendingNum;
  }
}

// @brief ペンディングしているチェック要求の数を返す．
inline
ymuint
GcNode2::pending_num()
{
  return mPendingNum;
}

// @brief ペンディングしているチェック要求を1つ取り出す．
inline
ymuint
GcNode2::get_check()
{
  for (ymuint i = 0; i < mPendingArray.size(); ++ i) {
    if ( mPendingArray[i] ) {
      mPendingArray[i] = false;
      -- mPendingNum;
      return i;
    }
  }
  ASSERT_NOT_REACHED;
  return 0;
}

END_NAMESPACE_YM_SATPG

#endif // GCNODE2_H
