#ifndef XORSPLITTER_H
#define XORSPLITTER_H

/// @file XorSplitter.h
/// @brief XorSplitter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "Splitter.h"


BEGIN_NAMESPACE_YM_IGF

//////////////////////////////////////////////////////////////////////
/// @class XorSplitter XorSplitter.h "XorSplitter.h"
/// @brief 変数の XOR による Splitter
//////////////////////////////////////////////////////////////////////
class XorSplitter :
  public Splitter
{
public:

  /// @brief コンストラクタ
  /// @param[in] varid_list 変数番号のリスト
  XorSplitter(const vector<ymuint32>& varid_list);

  /// @brief デストラクタ
  virtual
  ~XorSplitter();


public:
  //////////////////////////////////////////////////////////////////////
  // Splitter の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 分類数を返す．
  virtual
  ymuint
  split_num() const;

  /// @brief 変数番号のリストを返す．
  const vector<ymuint32>&
  varid_list() const;

  /// @brief ベクタを分類する．
  /// @param[in] vect 対象のベクタ
  virtual
  ymuint
  operator()(const RegVect* vect) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数番号のリスト
  vector<ymuint32> mVarIdList;

};

END_NAMESPACE_YM_IGF

#endif // XORSPLITTER_H
