#ifndef YM_MVN_MVNNET_H
#define YM_MVN_MVNNET_H

/// @file ym_mvn/MvnNet.h
/// @brief MvnNet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/mvn_nsdef.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvNet MvNet.h "ym_mvn/MvNet.h"
/// @brief ネットを表すクラス
//////////////////////////////////////////////////////////////////////
class MvNet :
  public DlElem
{
  friend class MvMgr;

public:

  /// @brief ID 番号を得る．
  ymuint
  id() const;

  /// @brief 入力元のピンを得る．
  const MvOutputPin*
  src_pin() const;

  /// @brief 出力先のピンを得る．
  const MvInputPin*
  dst_pin() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] src 入力元のピン
  /// @param[in] dst 出力先のピン
  MvNet(MvOutputPin* src,
	MvInputPin* dst);

  /// @brief デストラクタ
  ~MvNet();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID 番号
  ymuint32 mId;
  
  // 入力元のピン
  MvOutputPin* mSrc;

  // 出力先のピン
  MvInputPin* mDst;
  
};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ID 番号を得る．
inline
ymuint
MvNet::id() const
{
  return mId;
}

// @brief 入力元のピンを得る．
inline
const MvOutputPin*
MvNet::src_pin() const
{
  return mSrc;
}

// @brief 出力先のピンを得る．
inline
const MvInputPin*
MvNet::dst_pin() const
{
  return mDst;
}

END_NAMESPACE_YM_MVN

#endif // YM_MVN_MVNNET_H
