#ifndef CNFDDNODE_H
#define CNFDDNODE_H

/// @file CNFddNode.h
/// @brief CNFddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/CNFdd.h"
#include "CNFddEdge.h"


BEGIN_NAMESPACE_YM_CNFDD

//////////////////////////////////////////////////////////////////////
// CNFDDのノードを表す構造体
//////////////////////////////////////////////////////////////////////
class CNFddNode
{
  friend class CNFddMgrImpl;

public:

  /// @brief 中間ノードの場合に0枝を得る．
  CNFddEdge
  edge_0() const;

  /// @brief 中間ノードの場合にp枝を得る．
  CNFddEdge
  edge_p() const;

  /// @brief 中間ノードの場合にn枝を得る．
  CNFddEdge
  edge_n() const;


public:

  /// @brief レベルを得る．
  ymuint
  level() const;

  /// @brief mark が付いていたらtrueを返す
  bool
  mark() const;

  /// @brief mark を付ける
  /// @param[in] val マークの値(非0でセット,0でリセット)
  void
  mark(int val);

  /// @brief マークを消す
  void
  rst_mark();

  /// @brief リンク数を得る．
  ymuint
  refcount() const;

  // 参照されていない時にtrueを返す
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

  // リンク数を減らす(オーバーフロー時は何もしない)
  /// @return 結果のリンク数を返す．
  ymuint
  linkdec();

  /// @brief コンストラクタ
  /// CNFddMgrImpl以外はこのオブジェクトを生成できない．
  CNFddNode();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0枝
  CNFddEdge mEdge0;

  // p枝
  CNFddEdge mEdgeP;

  // n枝
  CNFddEdge mEdgeN;

  // レベル
  ymuint32 mLevel;

  // 参照回数＋α(下の定数を参照)
  ymuint32 mRefMark;

  // 節点テーブル用のリンクポインタ
  CNFddNode* mLink;


public:
  //////////////////////////////////////////////////////////////////////
  // 定数の定義
  // mRefMarkにパックされる情報
  // 最上位ビットから: tmporary-mark
  //                   リンクオーバーフロービット
  //                   残りビットは全てリンク数を表す．
  //////////////////////////////////////////////////////////////////////
  static
  const int kMBit = sizeof(ymuint32) * 8 - 1;
  static
  const int kLMaxBit = sizeof(ymuint32) * 8 - 2;
  static
  const ymuint32 kMMask = 1UL << kMBit;
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
CNFddEdge
CNFddNode::edge_0() const
{
  return mEdge0;
}

inline
CNFddEdge
CNFddNode::edge_p() const
{
  return mEdgeP;
}

inline
CNFddEdge
CNFddNode::edge_n() const
{
  return mEdgeN;
}

// レベルを得る
inline
ymuint
CNFddNode::level() const
{
  return mLevel;
}

// mark が付いていたらtrueを返す
inline
bool
CNFddNode::mark() const
{
  return ((mRefMark >> kMBit) & 1UL);
}

// mark を付ける
inline
void
CNFddNode::mark(int val)
{
  if ( val ) {
    mRefMark |= kMMask;
  }
  else {
    mRefMark &= ~kMMask;
  }
}

// 両方のマークを消す
inline
void
CNFddNode::rst_mark()
{
  mRefMark &= ~kMMask;
}

// リンク数を得る．
inline
ymuint
CNFddNode::refcount() const
{
  return static_cast<ymuint>(mRefMark & kLMask);
}

// 参照されていない時にtrueを返す
inline
bool
CNFddNode::noref() const
{
  return (mRefMark & kLMax_1) == 0UL;
}

// リンク数が最大値を越えている時に0，それ以外には1を返す．
inline
int
CNFddNode::linkdelta() const
{
  return static_cast<int>(((~mRefMark) >> kLMaxBit) & 1);
}

// リンク数を増やす(オーバーフロー時は何もしない)
inline
ymuint
CNFddNode::linkinc()
{
  int d = linkdelta();
  return (mRefMark += d) & kLMask;
}

// リンク数を減らす(オーバーフロー時は何もしない)
inline
ymuint
CNFddNode::linkdec()
{
  int d = linkdelta();
  return (mRefMark -= d) & kLMask;
}

// コンストラクタ
// CNFddMgrImpl以外はこのオブジェクトを生成できない．
inline
CNFddNode::CNFddNode()
{
}

END_NAMESPACE_YM_CNFDD

BEGIN_NAMESPACE_HASH
// CNFddNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsCNFdd::CNFddNode*>
{
  ymuint
  operator()(nsYm::nsCNFdd::CNFddNode* node) const
  {
    return reinterpret_cast<ympuint>(node) / sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // CNFDDNODE_H
