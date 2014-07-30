#ifndef YMUTILS_RANDPERMGEN_H
#define YMUTILS_RANDPERMGEN_H

/// @file YmUtils/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class RandGen;

//////////////////////////////////////////////////////////////////////
/// @class RandPermGen RandGen.h "YmUtils/RandGen.h"
/// @brief ランダムな順列を作るクラス
//////////////////////////////////////////////////////////////////////
class RandPermGen
{
public:

  /// @brief コンストラクタ
  /// @param[in] n 要素数
  RandPermGen(ymuint n);

  /// @brief デストラクタ
  ~RandPermGen();


public:

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief ランダムな順列を生成する．
  /// @param[in] randgen 乱数発生器
  void
  generate(RandGen& randgen);

  /// @brief 順列の要素を取り出す．
  /// @param[in] pos 要素の位置番号 ( 0 <= pos < num() )
  ymuint32
  elem(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint32 mNum;

  // 現在の順列
  ymuint32* mArray;

};

END_NAMESPACE_YM

#endif // YMUTILS_RANDPERMGEN_H
