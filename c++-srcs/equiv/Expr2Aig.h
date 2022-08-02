#ifndef EXPR2AIG_H
#define EXPR2AIG_H

/// @file Expr2Aig.h
/// @brief Expr2Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class Expr2Aig Expr2Aig.h "Expr2Aig.h"
/// @brief Expr から Aig を作るクラス
//////////////////////////////////////////////////////////////////////
class Expr2Aig
{
public:

  /// @brief コンストラクタ
  Expr2Aig(
    FraigMgr& mgr,                    ///< [in] FraigMgr
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  ) : mMgr{mgr},
      mInputs{inputs}
  {
  }

  /// @brief デストラクタ
  ~Expr2Aig() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Expr を AIG に変換する．
  FraigHandle
  conv(
    const Expr& expr ///< [in] 論理式
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // 入力のハンドルのリスト
  const vector<FraigHandle>& mInputs;

};

END_NAMESPACE_FRAIG

#endif // EXPR2AIG_H
