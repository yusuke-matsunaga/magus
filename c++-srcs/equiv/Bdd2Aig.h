#ifndef BDD2AIG_H
#define BDD2AIG_H

/// @file Bdd2Aig.h
/// @brief Bdd2Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"
#include "ym/Bdd.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class Bdd2Aig Bdd2Aig.h "Bdd2Aig.h"
/// @brief BDD を表す AIG を作るクラス
//////////////////////////////////////////////////////////////////////
class Bdd2Aig
{
public:

  /// @brief コンストラクタ
  Bdd2Aig(
    FraigMgr& mgr ///< [in] FraigMgr
  ) : mMgr{mgr}
  {
  }

  /// @brief デストラクタ
  ~Bdd2Aig() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief BDD を AIG に変換する．
  FraigHandle
  conv(
    const Bdd& bdd,                   ///< [in] BDD
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 枝をAIGに変換する．
  FraigHandle
  edge2aig(
    SizeType edge ///< [in] 枝
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // ノード番号をキーにしてAIGを格納する配列
  vector<FraigHandle> mAigMap;

};

END_NAMESPACE_FRAIG

#endif // BDD2AIG_H
