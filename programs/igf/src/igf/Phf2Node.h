#ifndef PHF2NODE_H
#define PFH2NODE_H

/// @file Phf2Node.h
/// @brief Phf2Node のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

class Phf2Edge;

//////////////////////////////////////////////////////////////////////
/// @class Phf2Node Phf2Node.h "Phf2Node.h"
/// @brief PHF 用のノード
//////////////////////////////////////////////////////////////////////
class Phf2Node
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] pat パタン
  Phf2Node(ymuint id,
	   ymuint32 pat);

  /// @brief デストラクタ
  ~Phf2Node();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief パタンを返す．
  ymuint32
  pat() const;

  /// @brief 接続している枝の数を返す．
  ymuint
  edge_num() const;

  /// @brief 接続している枝を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
  Phf2Edge*
  edge(ymuint pos) const;

  /// @brief 値を得る．
  ymuint
  val() const;

  /// @brief 値が設定されていたら true を返す．
  bool
  is_assigned() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝を追加する．
  void
  add_edge(Phf2Edge* edge);

  /// @brief 値を設定する．
  void
  set_val(ymuint32 val);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード ID
  ymuint32 mId;

  // パタン
  ymuint32 mPat;

  // 接続している枝のリスト
  vector<Phf2Edge*> mEdgeList;

  // このノードの値
  ymuint32 mVal;

  bool mAssigned;
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id ID番号
// @param[in] pat パタン
inline
Phf2Node::Phf2Node(ymuint id,
		   ymuint32 pat)
{
  mId = id;
  mPat = pat;
  mVal = 0;
  mAssigned = false;
}

// @brief デストラクタ
inline
Phf2Node::~Phf2Node()
{
}

// @brief ID番号を返す．
inline
ymuint
Phf2Node::id() const
{
  return mId;
}

// @brief パタンを返す．
inline
ymuint32
Phf2Node::pat() const
{
  return mPat;
}

// @brief 接続している枝の数を返す．
inline
ymuint
Phf2Node::edge_num() const
{
  return mEdgeList.size();
}

// @brief 接続している枝を返す．
// @param[in] pos 位置番号 ( 0 <= pos < edge_num() )
inline
Phf2Edge*
Phf2Node::edge(ymuint pos) const
{
  assert_cond( pos < mEdgeList.size(), __FILE__, __LINE__);
  return mEdgeList[pos];
}

// @brief 値を得る．
inline
ymuint
Phf2Node::val() const
{
  return mVal;
}

// @brief 値が設定されていたら true を返す．
inline
bool
Phf2Node::is_assigned() const
{
  return mAssigned;
}

// @brief 枝を追加する．
inline
void
Phf2Node::add_edge(Phf2Edge* edge)
{
  mEdgeList.push_back(edge);
}

// @brief 値を設定する．
inline
void
Phf2Node::set_val(ymuint32 val)
{
  assert_cond( mAssigned == false, __FILE__, __LINE__);
  mVal = val;
  mAssigned = true;
}

END_NAMESPACE_YM_IGF

#endif // PHF2NODE_H
