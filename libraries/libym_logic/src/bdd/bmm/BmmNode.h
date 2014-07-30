#ifndef BMMNODE_H
#define BMMNODE_H

/// @file BmmNode.h
/// @brief BmmNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Bdd.h"

#include "BmmVar.h"


BEGIN_NAMESPACE_YM_BDD

//////////////////////////////////////////////////////////////////////
// BDDのノードを表す構造体
//////////////////////////////////////////////////////////////////////
class BmmNode
{
  friend class BddMgrModern;
  friend class BmmVar;
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
  const int kPBit = sizeof(int) * 8 - 1;
  static
  const int kNBit = sizeof(int) * 8 - 2;
  static
  const int kLMaxBit = sizeof(int) * 8 - 3;
  static
  const ymuint32 kPMask = 1L << kPBit;
  static
  const ymuint32 kNMask = 1L << kNBit;
  static
  const ymuint32 kLMax = 1L << kLMaxBit;
  static
  const ymuint32 kLMax_1 = kLMax - 1;
  static
  const ymuint32 kLMask = kLMax | kLMax_1;


public:

  // 中間ノードの場合に0枝/1枝を得る．
  BddEdge
  edge0() const;

  BddEdge
  edge1() const;

  BddEdge
  edge0(tPol p) const;

  BddEdge
  edge1(tPol p) const;

  // 変数を得る．
  BmmVar*
  var() const;

  // 変数インデックス値を得る
  VarId
  varid() const;

  // レベルを得る．
  ymuint
  level() const;

  // p-mark が付いていたらtrueを返す
  bool
  pmark() const;

  // n-mark が付いていたらtrueを返す
  bool
  nmark() const;

  // p-mark を付ける
  // val が非0でセット,0でリセット
  void
  pmark(int val);

  // n-mark を付ける
  // val が非0でセット,0でリセット
  void
  nmark(int val);

  // 両方のマークを消す
  void
  rst_mark();

  // リンク数を得る．
  ymuint
  refcount() const;

  // 参照されていない時にtrueを返す
  bool
  noref() const;


private:

  // リンク数が最大値を越えている時に0，それ以外には1を返す．
  int
  linkdelta() const;

  // リンク数を増やす(オーバーフロー時は何もしない)
  ymuint
  linkinc();

  // リンク数を減らす(オーバーフロー時は何もしない)
  ymuint
  linkdec();

  // コンストラクタ
  // BddMgr以外はこのオブジェクトを生成できない．
  BmmNode();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0枝
  BddEdge mEdge0;

  // 1枝
  BddEdge mEdge1;

  // 変数へのポインタ
  BmmVar* mVar;

  // 参照回数＋α(上の定数を参照)
  ymuint32 mRefMark;

  // 節点テーブル用のリンクポインタ
  BmmNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 中間ノードの場合に左右の子供への枝を得る．
inline
BddEdge
BmmNode::edge0() const
{
  return mEdge0;
}
inline
BddEdge
BmmNode::edge1() const
{
  return mEdge1;
}
inline
BddEdge
BmmNode::edge0(tPol p) const
{
  return BddEdge(mEdge0, p);
}
inline
BddEdge
BmmNode::edge1(tPol p) const
{
  return BddEdge(mEdge1, p);
}

// 変数を得る．
inline
BmmVar*
BmmNode::var() const
{
  return mVar;
}

// 変数インデックス値を得る
inline
VarId
BmmNode::varid() const
{
  return mVar->varid();
}

// レベルを得る
inline
ymuint
BmmNode::level() const
{
  return mVar->level();
}

// p-mark が付いていたらtrueを返す
inline
bool
BmmNode::pmark() const
{
  return ((mRefMark >> kPBit) & 1UL);
}

// n-mark が付いていたらtrueを返す
inline
bool
BmmNode::nmark() const
{
  return ((mRefMark >> kNBit) & 1UL);
}

// p-mark を付ける
inline
void
BmmNode::pmark(int val)
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
BmmNode::nmark(int val)
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
BmmNode::rst_mark()
{
  mRefMark &= ~(kPMask | kNMask);
}

// リンク数を得る．
inline
ymuint
BmmNode::refcount() const
{
  return static_cast<ymuint>(mRefMark & kLMask);
}

// 参照されていない時にtrueを返す
inline
bool
BmmNode::noref() const
{
  return (mRefMark & kLMax_1) == 0UL;
}

// リンク数が最大値を越えている時に0，それ以外には1を返す．
inline
int
BmmNode::linkdelta() const
{
  return static_cast<int>(((~mRefMark) >> kLMaxBit) & 1);
}

// リンク数を増やす(オーバーフロー時は何もしない)
inline
ymuint
BmmNode::linkinc()
{
  int d = linkdelta();
  return static_cast<ymuint>((mRefMark += d) & kLMask);
}

// リンク数を減らす(オーバーフロー時は何もしない)
inline
ymuint
BmmNode::linkdec()
{
  int d = linkdelta();
  return static_cast<ymuint>((mRefMark -= d) & kLMask);
}

// コンストラクタ
// BddMgr以外はこのオブジェクトを生成できない．
inline
BmmNode::BmmNode()
{
}

END_NAMESPACE_YM_BDD

BEGIN_NAMESPACE_HASH
// BmmNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsBdd::BmmNode*>
{
  ymuint
  operator()(nsYm::nsBdd::BmmNode* node) const
  {
    return reinterpret_cast<ympuint>(node)/sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // BMMNODE_H
