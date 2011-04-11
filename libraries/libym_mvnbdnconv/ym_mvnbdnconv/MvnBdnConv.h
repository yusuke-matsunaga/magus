#ifndef YM_MVNBDNCONV_MVNBDNCONV_H
#define YM_MVNBDNCONV_MVNBDNCONV_H

/// @file ym_mvnbdnconv/MvnBdnConv.h
/// @brief MvnBdnConv のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvnbdnconv/mvnbdnconv_nsdef.h"
#include "ym_mvn/mvn_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"


BEGIN_NAMESPACE_YM_MVNBDNCONV

class MvnConv;

//////////////////////////////////////////////////////////////////////
/// @class MvnBdnConv MvnBdnConv.h "MvnBdnConv.h"
/// @brief Mvn から BdnMgr に変換するクラス
//////////////////////////////////////////////////////////////////////
class MvnBdnConv
{
public:

  /// @brief コンストラクタ
  MvnBdnConv();

  /// @brief デストラクタ
  ~MvnBdnConv();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief MvnMgr の内容を BdnMgr に変換する．
  /// @param[in] mvmgr 対象の MvNetwork
  /// @param[out] bdn 変換先の BdnMgr
  /// @param[out] mvnode_map 対応関係を格納するオブジェクト
  void
  operator()(const MvnMgr& mvmgr,
	     BdnMgr& bdn,
	     MvnBdnMap& mvnode_map);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // MvNode の変換関数のリスト
  list<MvnConv*> mConvList;

};

END_NAMESPACE_YM_MVNBDNCONV

#endif // YM_MVNBDNCONV_MVNBDNCONV_H
