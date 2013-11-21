#ifndef BDDNODE_H
#define BDDNODE_H

/// @file BddNode.h
/// @brief BddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/bdd_nsdef.h"
#include "BddEdge.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDDのノードを表す構造体
//////////////////////////////////////////////////////////////////////
class BddNode
{
  friend class BddMgrImpl;
  friend class BddMgrClassic;
  friend class BddMgrModern;
  friend class BmmVar;

private:

  /// @brief コンストラクタ
  /// @note BddMgrImpl以外はこのオブジェクトを生成できない．
  BddNode();


public:

  /// @brief 中間ノードの場合に0枝を得る．
  BddEdge
  edge0() const;

  /// @brief 中間ノードの場合に1枝を得る．
  BddEdge
  edge1() const;

  /// @brief 中間ノードの場合に0枝を得る．
  BddEdge
  edge0(tPol p) const;

  /// @brief 中間ノードの場合に1枝を得る．
  BddEdge
  edge1(tPol p) const;

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief p-mark が付いていたらtrueを返す
  bool
  pmark() const;

  /// @brief n-mark が付いていたらtrueを返す
  bool
  nmark() const;

  /// @brief p-mark を付ける
  /// @note val が非0でセット,0でリセット
  void
  pmark(int val);

  /// @brief n-mark を付ける
  /// @note val が非0でセット,0でリセット
  void
  nmark(int val);

  /// @brief 両方のマークを消す
  void
  rst_mark();

  /// @brief リンク数を得る．
  ymuint
  refcount() const;

  /// @brief 参照されていない時にtrueを返す
  bool
  noref() const;


private:

  /// @brief リンク数が最大値を越えている時に0，それ以外には1を返す．
  int
  linkdelta() const;

  /// @brief リンク数を増やす(オーバーフロー時は何もしない)
  /// @return 結果のリンク数を返す．
  ymuint
  linkinc();

  /// @brief リンク数を減らす(オーバーフロー時は何もしない)
  /// @return 結果のリンク数を返す．
  ymuint
  linkdec();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0枝
  BddEdge mEdge0;

  // 1枝
  BddEdge mEdge1;

  // レベル
  ymuint32 mLevel;

  // 参照回数＋α(上の定数を参照)
  ymuint32 mRefMark;

  // 節点テーブル用のリンクポインタ
  BddNode* mLink;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  // mRefMarkにパックされる情報
  // 最上位ビットから: positive-mark
  //                   negative-mark
  //                   リンクオーバーフロービット
  //                   残りビットは全てリンク数を表す．
  //////////////////////////////////////////////////////////////////////
  static
  const int kPBit = sizeof(ymuint32) * 8 - 1;
  static
  const int kNBit = sizeof(ymuint32) * 8 - 2;
  static
  const int kLMaxBit = sizeof(ymuint32) * 8 - 3;
  static
  const ymuint32 kPMask = 1UL << kPBit;
  static
  const ymuint32 kNMask = 1UL << kNBit;
  static
  const ymuint32 kLMax = 1UL << kLMaxBit;
  static
  const ymuint32 kLMax_1 = kLMax - 1;
  static
  const ymuint32 kLMask = kLMax | kLMax_1;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 中間ノードの場合に左右の子供への枝を得る．
inline
BddEdge
BddNode::edge0() const
{
  return mEdge0;
}
inline
BddEdge
BddNode::edge1() const
{
  return mEdge1;
}
inline
BddEdge
BddNode::edge0(tPol p) const
{
  return BddEdge(mEdge0, p);
}
inline
BddEdge
BddNode::edge1(tPol p) const
{
  return BddEdge(mEdge1, p);
}

// レベルを得る
inline
ymuint
BddNode::level() const
{
  return mLevel;
}

// p-mark が付いていたらtrueを返す
inline
bool
BddNode::pmark() const
{
  return ((mRefMark >> kPBit) & 1UL);
}

// n-mark が付いていたらtrueを返す
inline
bool
BddNode::nmark() const
{
  return ((mRefMark >> kNBit) & 1UL);
}

// p-mark を付ける
inline
void
BddNode::pmark(int val)
{
  if ( val ) {
    mRefMark |= kPMask;
  }
  else {
    mRefMark &= ~kPMask;
  }
}

// n-mark を付ける
inline
void
BddNode::nmark(int val)
{
  if ( val ) {
    mRefMark |= kNMask;
  }
  else {
    mRefMark &= ~kNMask;
  }
}

// 両方のマークを消す
inline
void
BddNode::rst_mark()
{
  mRefMark &= ~(kPMask | kNMask);
}

// リンク数を得る．
inline
ymuint
BddNode::refcount() const
{
  return static_cast<ymuint>(mRefMark & kLMask);
}

// 参照されていない時にtrueを返す
inline
bool
BddNode::noref() const
{
  return (mRefMark & kLMax_1) == 0UL;
}

// リンク数が最大値を越えている時に0，それ以外には1を返す．
inline
int
BddNode::linkdelta() const
{
  return static_cast<int>(((~mRefMark) >> kLMaxBit) & 1);
}

// リンク数を増やす(オーバーフロー時は何もしない)
inline
ymuint
BddNode::linkinc()
{
  int d = linkdelta();
  return (mRefMark += d) & kLMask;
}

// リンク数を減らす(オーバーフロー時は何もしない)
inline
ymuint
BddNode::linkdec()
{
  int d = linkdelta();
  return (mRefMark -= d) & kLMask;
}

// コンストラクタ
// BddMgrClassic以外はこのオブジェクトを生成できない．
inline
BddNode::BddNode()
{
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH
// BddNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::BddNode*>
{
  ymuint
  operator()(nsYm::nsBdd::BddNode* node) const
  {
    return reinterpret_cast<ympuint>(node)/sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // BMCNODE_H
