#ifndef SPLITTER_H
#define SPLITTER_H

/// @file Splitter.h
/// @brief Splitter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "igf_nsdef.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class Splitter Splitter.h "Splitter.h"
/// @brief ベクタを分割するクラス
//////////////////////////////////////////////////////////////////////
class Splitter
{
public:

  /// @brief デストラクタ
  virtual
  ~Splitter() { }


public:
  //////////////////////////////////////////////////////////////////////
  // Splitter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 分類数を返す．
  virtual
  ymuint
  split_num() const = 0;

  /// @brief ベクタを分類する．
  /// @param[in] vect 対象のベクタ
  virtual
  ymuint
  operator()(const RegVect* vect) const = 0;

};

END_NAMESPACE_YM_IGF

#endif // SPLITTER_H
