#ifndef MULTISPLITTER_H
#define MULTISPLITTER_H

/// @file MultiSplitter.h
/// @brief MultiSplitter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Splitter.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class MutlSplitter MultiSplitter.h "MultiSplitter.h"
/// @brief 複数の Splitter を組み合わせた Splitter クラス
//////////////////////////////////////////////////////////////////////
class MultiSplitter :
  public Splitter
{
public:

  /// @brief コンストラクタ
  /// @param[in] splitter_list 元となる Splitter のリスト
  MultiSplitter(const vector<Splitter*>& splitter_list);

  /// @brief デストラクタ
  virtual
  ~MultiSplitter();


public:
  //////////////////////////////////////////////////////////////////////
  // Splitter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 分類数を返す．
  virtual
  ymuint
  split_num() const;

  /// @brief ベクタを分類する．
  /// @param[in] vect 対象のベクタ
  virtual
  ymuint
  operator()(const RegVect* vect) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Splitter のリスト
  vector<Splitter*> mSplitterList;

};

END_NAMESPACE_YM_IGF

#endif // MULTISPLITTER_H
