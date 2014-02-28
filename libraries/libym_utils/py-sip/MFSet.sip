
/// @file MFSet.sip
/// @brief MFSet の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class MFSet
{
%TypeHeaderCode
#include "utils/MFSet.h"
using nsYm::MFSet;
%End

public:

  /// @brief コンストラクタ
  /// @param[in] n 確保したい要素の数．
  MFSet(unsigned int n);

  /// @brief 要素数を返す．
  unsigned int
  num() const;

  /// @brief 代表元の検索 (Find)
  /// @param[in] x 要素番号 ( 0 <= x < num() )
  /// @retval 要素 x の属する集合の代表元
  /// @retval kBadID 要素 x が存在していない場合
  unsigned int
  find(unsigned int x);

  /// @brief 2つの集合の併合 (Merge)
  /// @param[in] x, y 代表元 ( 0 <= x, y < num() )
  /// @retval 新たな代表元を返す．
  /// @retval kBadID x か y が存在していなかった
  /// @note 2つの代表元 x, y の表す集合を併合する．
  /// @note 実は x, y が代表元でない場合，
  /// 内部で find(x), find(y)を呼ぶので処理は行えるが，
  /// 代表元が分かっている場合にはそれを使ったほうが処理は速い．
  unsigned int
  merge(unsigned int x,
	unsigned int y);

};