#ifndef YM_NETWORKS_MVNINPUTPIN_H
#define YM_NETWORKS_MVNINPUTPIN_H

/// @file ym_networks/MvnInputPin.h
/// @brief MvnInputPin のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/mvn.h"


BEGIN_NAMESPACE_YM_NETWORKS_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInputPin MvPin.h "ym_networks/MvPin.h"
/// @brief ノードの入力ピンを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnInputPin :
  public DlElem
{
  friend class MvnMgr;
  friend class MvnNodeBase;

private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MvnInputPin();

  /// @brief デストラクタ
  ~MvnInputPin();


public:

  /// @breif 属しているノードを得る．
  MvnNode*
  node() const;

  /// @brief ビット幅を得る．
  ymuint
  bit_width() const;

  /// @brief ノードの何番目のピンかを返す．
  /// @note 入力ピンと出力ピンは区別される．
  ymuint
  pos() const;

  /// @brief 接続している出力ピンを得る．
  MvnOutputPin*
  src_pin() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 設定用の関数
  void
  init(MvnNode* node,
       ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のノード
  MvnNode* mNode;

  // 位置
  ymuint32 mPos;

  // ビット幅
  ymuint32 mBitWidth;

  // 接続している出力ピン
  MvnOutputPin* mSrcPin;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @breif 属しているノードを得る．
inline
MvnNode*
MvnInputPin::node() const
{
  return mNode;
}

// @brief ビット幅を得る．
inline
ymuint
MvnInputPin::bit_width() const
{
  return mBitWidth;
}

// @brief ノードの何番目のピンかを返す．
// @note 入力ピンと出力ピンは区別される．
inline
ymuint
MvnInputPin::pos() const
{
  return mPos;
}

// @brief 接続している出力ピンを得る．
inline
MvnOutputPin*
MvnInputPin::src_pin() const
{
  return mSrcPin;
}

// @brief 設定用の関数
inline
void
MvnInputPin::init(MvnNode* node,
		  ymuint pos)
{
  mNode = node;
  mPos = pos;
  mBitWidth = 1;
  mSrcPin = NULL;
}

END_NAMESPACE_YM_NETWORKS_MVN

#endif // YM_NETWORKS_MVNINPUTPIN_H
