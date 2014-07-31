#ifndef VLWMODULEHEADER_H
#define VLWMODULEHEADER_H

/// @file VlwModuleHeader.h
/// @brief VlwModuleHeader の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlwModuleHeader VlwModuleHeader.h "VlwModuleHeader.h"
/// @brief モジュールのヘッダを出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwModuleHeader :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  VlwModuleHeader(VlWriter& writer);

  /// @brief デストラクタ
  ~VlwModuleHeader();

};

#if 0
//////////////////////////////////////////////////////////////////////
/// @class VlwPort
/// @brief ポートを出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwPort
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  VlwPort();

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] elem_list ポート要素のリスト
  VlwPort(const vector<VlwPortElem>& elem_list);

  /// @brief デストラクタ
  ~VlwPort();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ポート要素のリスト
  vector<VlwPortElem> mElemList;

};
#endif
END_NAMESPACE_YM_NETWORKS

#endif // VLWMODULEHEADER_H
