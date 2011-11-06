#ifndef ZDDNODE_H
#define ZDDNODE_H

/// @file ZddNode.h
/// @brief ZddNode のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_logic/Zdd.h"


BEGIN_NAMESPACE_YM_ZDD

//////////////////////////////////////////////////////////////////////
/// @class ZddVar ZddNode.h "ZddNode.h"
/// @brief 変数の情報を格納しておくクラス
//////////////////////////////////////////////////////////////////////
class ZddVar
{
  friend class ZddMgrImpl;
public:

  // 変数番号を得る．
  tVarId
  varid() const
  {
    return mId;
  }

  // レベルを得る．
  tLevel
  level() const
  {
    return tLevel(mId);
  }


private:

  // コンストラクタ
  ZddVar(tVarId id) :
    mId(id),
    mMark(0),
    mLink(NULL)
  {
  }

  // デストラクタ
  ~ZddVar()
  {
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号
  // レベルも同一
  tVarId mId;

  // 作業用のマーク
  int mMark;

  // compose用にZDDの枝を入れておくメンバ
  ZddEdge mCompEdge;

  // 変数リスト中の次の要素を指すポインタ
  ZddVar* mNext;

  // ハッシュ表中の次の要素を指すポインタ
  ZddVar* mLink;

};


//////////////////////////////////////////////////////////////////////
// ZDDのノードを表す構造体
//////////////////////////////////////////////////////////////////////
class ZddNode
{
  friend class ZddMgrImpl;

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

public:

  /// @brief 中間ノードの場合に0枝を得る．
  ZddEdge
  edge0() const;

  /// @brief 中間ノードの場合に1枝を得る．
  ZddEdge
  edge1() const;

  /// @brief 変数を得る．
  ZddVar*
  var() const;

  /// @brief 変数インデックス値を得る
  tVarId
  varid() const;

  /// @brief レベルを得る．
  tLevel
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
  /// ZddMgrImpl以外はこのオブジェクトを生成できない．
  ZddNode();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 0枝
  ZddEdge mEdge0;

  // 1枝
  ZddEdge mEdge1;

  // 変数へのポインタ
  ZddVar* mVar;

  // 参照回数＋α(上の定数を参照)
  ymuint32 mRefMark;

  // 節点テーブル用のリンクポインタ
  ZddNode* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 中間ノードの場合に左右の子供への枝を得る．
inline
ZddEdge
ZddNode::edge0() const
{
  return mEdge0;
}

inline
ZddEdge
ZddNode::edge1() const
{
  return mEdge1;
}

// 変数を得る．
inline
ZddVar*
ZddNode::var() const
{
  return mVar;
}

// 変数インデックス値を得る
inline
tVarId
ZddNode::varid() const
{
  return mVar->varid();
}

// レベルを得る
inline
tLevel
ZddNode::level() const
{
  return mVar->level();
}

// mark が付いていたらtrueを返す
inline
bool
ZddNode::mark() const
{
  return ((mRefMark >> kMBit) & 1UL);
}

// mark を付ける
inline
void
ZddNode::pmark(int val)
{
  if ( val ) {
    mRefMark |= kPMask;
  }
  else {
    mRefMark &= ~kPMask;
  }
}

// 両方のマークを消す
inline
void
ZddNode::rst_mark()
{
  mRefMark &= ~kMMask;
}

// リンク数を得る．
inline
ymuint
ZddNode::refcount() const
{
  return static_cast<ymuint>(mRefMark & kLMask);
}

// 参照されていない時にtrueを返す
inline
bool
ZddNode::noref() const
{
  return (mRefMark & kLMax_1) == 0UL;
}

// リンク数が最大値を越えている時に0，それ以外には1を返す．
inline
int
ZddNode::linkdelta() const
{
  return static_cast<int>(((~mRefMark) >> kLMaxBit) & 1);
}

// リンク数を増やす(オーバーフロー時は何もしない)
inline
ymuint
ZddNode::linkinc()
{
  int d = linkdelta();
  return (mRefMark += d) & kLMask;
}

// リンク数を減らす(オーバーフロー時は何もしない)
inline
ymuint
ZddNode::linkdec()
{
  int d = linkdelta();
  return (mRefMark -= d) & kLMask;
}

// コンストラクタ
// ZddMgrImpl以外はこのオブジェクトを生成できない．
inline
ZddNode::ZddNode()
{
}

END_NAMESPACE_YM_ZDD

BEGIN_NAMESPACE_HASH
// ZddNode へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsZdd::ZddNode*>
{
  ymuint
  operator()(nsYm::nsZdd::ZddNode* node) const
  {
    return reinterpret_cast<ymuint>(node)/sizeof(void*);
  }
};
END_NAMESPACE_HASH

#endif // ZDDNODE_H
