#ifndef LIBYM_MVN_MVNINST_H
#define LIBYM_MVN_MVNINST_H

/// @file libym_mvn/MvnInst.h
/// @brief MvnInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnInst MvnInst.h "MvnInst.h"
/// @brief モジュールインスタンスを表すノード
//////////////////////////////////////////////////////////////////////
class MvnInst :
  public MvnNode
{
  friend class MvnMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 下位モジュールを得る．
  /// @note type() が kInst の時のみ意味を持つ．
  virtual
  const MvnModule*
  module() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] submodule 子のモジュール
  MvnInst(MvnModule* module,
	  MvnModule* submodule);

  /// @brief デストラクタ
  ~MvnInst();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 子のモジュール
  MvnModule* mSubmodule;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNINST_H
