#ifndef LIBYM_MVN_MVNUDP_H
#define LIBYM_MVN_MVNUDP_H

/// @file libym_mvn/MvnUdp.h
/// @brief MvnCombUdp, MvnSeqUdp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvnNode.h"


BEGIN_NAMESPACE_YM_MVN

//////////////////////////////////////////////////////////////////////
/// @class MvnCombUdp MvnUdp.h "MvnUdp.h"
/// @brief 組み合わせ回路の UDP を表すクラス
//////////////////////////////////////////////////////////////////////
class MvnCombUdp :
  public MvnNode
{
  friend class MvnMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力数
  MvnCombUdp(MvnModule* module,
	     ymuint ni);

  /// @brief デストラクタ
  ~MvnCombUdp();

};


//////////////////////////////////////////////////////////////////////
/// @class MvnSeqUdp MvnUdp.h "MvnUdp.h"
/// @brief 組み合わせ回路の UDP を表すクラス
//////////////////////////////////////////////////////////////////////
class MvnSeqUdp :
  public MvnNode
{
  friend class MvnMgr;
public:

  /// @brief ノードの種類を得る．
  virtual
  tType
  type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ / デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  /// @param[in] module 親のモジュール
  /// @param[in] ni 入力数
  MvnSeqUdp(MvnModule* module,
	    ymuint ni);

  /// @brief デストラクタ
  ~MvnSeqUdp();

};

END_NAMESPACE_YM_MVN

#endif // LIBYM_MVN_MVNUDP_H
