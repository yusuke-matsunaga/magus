#ifndef CUT_H
#define CUT_H

/// @file Cut.h
/// @brief Cut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/bdn.h"
#include "ym_logic/TvFunc.h"
#include "ym_logic/Aig.h"


BEGIN_NAMESPACE_YM_NETWORKS

class CutMgr;

//////////////////////////////////////////////////////////////////////
/// @class Cut Cut.h "Cut.h"
/// @brief カットとそれに関連する情報を表すクラス
//////////////////////////////////////////////////////////////////////
class Cut
{
  friend class CutHash;

public:

  /// @brief コンストラクタ
  /// @param[in] f 関数
  /// @param[in] aig AIG
  Cut(const TvFunc& f,
      Aig aig);

  /// @brief デストラクタ
  ~Cut();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数を返す．
  ymuint
  input_num() const;

  /// @brief 論理関数を返す．
  TvFunc
  function() const;

  /// @brief 論理関数を表す AIG を返す．
  Aig
  aig() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 関数
  TvFunc mFunc;

  // AIG
  Aig mAig;

  // 次の要素を指すリンク
  Cut* mLink;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] f 関数
// @param[in] aig AIG
inline
Cut::Cut(const TvFunc& f,
	 Aig aig) :
  mFunc(f),
  mAig(aig)
{
}

// @brief デストラクタ
inline
Cut::~Cut()
{
}

// @brief 入力数を返す．
inline
ymuint
Cut::input_num() const
{
  return mFunc.input_num();
}

// @brief 論理関数を返す．
inline
TvFunc
Cut::function() const
{
  return mFunc;
}

// @brief 論理関数を表す AIG を返す．
inline
Aig
Cut::aig() const
{
  return mAig;
}

END_NAMESPACE_YM_NETWORKS

#endif // CUT_H
