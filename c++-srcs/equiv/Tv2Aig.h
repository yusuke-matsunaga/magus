#ifndef TV2AIG_H
#define TV2AIG_H

/// @file Tv2Aig.h
/// @brief Tv2Aig のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "FraigMgr.h"
#include "ym/TvFunc.h"


BEGIN_NAMESPACE_FRAIG

//////////////////////////////////////////////////////////////////////
/// @class Tv2Aig Tv2Aig.h "Tv2Aig.h"
/// @brief TvFunc を表す AIG を作るクラス
//////////////////////////////////////////////////////////////////////
class Tv2Aig
{
public:

  /// @brief コンストラクタ
  Tv2Aig(
    FraigMgr& mgr,                    ///< [in] FraigMgr
    const vector<FraigHandle>& inputs ///< [in] 入力のハンドルのリスト
  ) : mMgr{mgr},
      mInputs{inputs}
  {
  }

  /// @brief デストラクタ
  ~Tv2Aig() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief TvFunc を AIG に変換する．
  FraigHandle
  conv(
    const TvFunc& func, ///< [in] 関数
    SizeType pos        ///< [in] 次の入力位置
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FraigMgr
  FraigMgr& mMgr;

  // 入力のハンドルのリスト
  const vector<FraigHandle>& mInputs;

  // TvFuncをキーにしてAIGを格納する辞書
  unordered_map<TvFunc, FraigHandle> mAigMap;

};

END_NAMESPACE_FRAIG

#endif // TV2AIG_H
