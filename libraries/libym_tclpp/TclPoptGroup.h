#ifndef LIBYM_TCLPP_TCLPOPTGROUP_H
#define LIBYM_TCLPP_TCLPOPTGROUP_H

/// @file TclPoptGroup.h
/// @brief TclPoptGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_tclpp/tclpp_nsdef.h"


BEGIN_NAMESPACE_YM_TCLPP

class TclPopt;

//////////////////////////////////////////////////////////////////////
/// @class TclPoptGroup TclPoptGroup.h "TclPoptGroup.h"
/// @brief 排他的な TclPopt のグループを表すクラス
//////////////////////////////////////////////////////////////////////
class TclPoptGroup
{
  friend class TclCmd;
private:

  /// @brief コンストラクタ
  TclPoptGroup();

  /// @brief デストラクタ
  ~TclPoptGroup();


public:

  /// @brief 要素を追加する．
  void
  add(TclPopt* popt);

  /// @brief 複数のオプションが指定されていたら true を返す．
  bool
  check() const;

  /// @brief 要素のリストを返す．
  const list<TclPopt*>&
  popt_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素のリスト
  list<TclPopt*> mPoptList;

};

END_NAMESPACE_YM_TCLPP

#endif // LIBYM_TCLPP_TCLPOPTGROUP_H
