#ifndef RTPGOP_H
#define RTPGOP_H

/// @file RtpgOp.h
/// @brief RtpgOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgOp RtpgOp.h "RtpgOp.h"
/// @brief rtpg 用の DetectOp
//////////////////////////////////////////////////////////////////////
class RtpgOp :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  RtpgOp();

  /// @brief デストラクタ
  virtual
  ~RtpgOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  virtual
  void
  operator()(TpgFault* f,
	     TestVector* tv);

  /// @brief テストベクタを設定する．
  void
  set_tv(const vector<TestVector*>& tv_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////


};

END_NAMESPACE_YM_SATPG

#endif // RTPGOP_H
