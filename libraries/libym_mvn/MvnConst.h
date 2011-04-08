#ifndef LIBYM_MVN_MVNCONST_H
#define LIBYM_MVN_MVNCONST_H

/// @file libym_mvn/MvnConst.h
/// @brief MvnConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"
#include "ym_mvn/MvnPin.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnConst MvnConst.h "MvnConst.h"
/// @brief 入力ノードを表すクラス
//////////////////////////////////////////////////////////////////////
class MvnConst :
  public MvnNode
{
  friend class MvnMgr;

public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;

  /// @brief 定数値を得る．
  /// @param[out] val 値を格納するベクタ
  /// @note type() が kConst の時のみ意味を持つ．
  virtual
  void
  const_value(vector<ymuint32>& val) const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] bit_width ビット幅
  /// @param[in] val 値
  MvnConst(MvnModule* module,
	   ymuint bit_width,
	   const vector<ymuint32>& val);

  /// @brief デストラクタ
  ~MvnConst();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  vector<ymuint32> mVal;

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNCONST_H
