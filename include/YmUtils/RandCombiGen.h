#ifndef YMUTILS_RANDCOMBIGEN_H
#define YMUTILS_RANDCOMBIGEN_H

/// @file YmUtils/RandCombiGen.h
/// @brief RandCombiGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class RandGen;

//////////////////////////////////////////////////////////////////////
/// @class RandCombiGen RandGen.h "YmUtils/RandGen.h"
/// @brief ランダムな組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class RandCombiGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 全要素数
  /// @param[in] k 組み合わせの要素数
  RandCombiGen(ymuint n,
	       ymuint k);

  /// @brief デストラクタ
  ~RandCombiGen();


public:

  /// @brief 全要素数を返す．
  ymuint
  num() const;

  /// @brief 組み合わせの要素数を返す．
  ymuint
  combi_num() const;

  /// @brief ランダムな組み合わせを生成する．
  /// @param[in] randgen 乱数発生器
  void
  generate(RandGen& randgen);

  /// @brief 組み合わせの要素を取り出す．
  /// @param[in] pos 要素の位置番号 ( 0 <= pos < combi_num() )
  ymuint32
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // 組み合わせの要素数
  ymuint32 mCombiNum;

  // 現在の順列
  ymuint32* mArray;

};

END_NAMESPACE_YM

#endif // YMUTILS_RANDCOMBIGEN_H
