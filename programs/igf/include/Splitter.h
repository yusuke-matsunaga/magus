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
  ~Splitter();


public:

  /// @brief ベクタを分割する．
  virtual
  void
  operator()(const vector<RegVect*>& src_list,
	     vector<RegVect*>& dst0_list,
	     vector<RegVect*>& dst1_list);

};

END_NAMESPACE_YM_IGF

#endif // SPLITTER_H
