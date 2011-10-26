#ifndef VLWINST_H
#define VLWINST_H

/// @file VlwInst.h
/// @brief VlwInst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "VlwBase.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class VlwInst VlwInst.h "VlwInst.h"
/// @brief インスタンス記述のヘッダを出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwInst :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] def_name インスタンスの定義名
  VlwInst(VlWriter& writer,
	  const char* def_name);

  /// @brief コンストラクタ
  /// @param[in] writer 出力用オブジェクト
  /// @param[in] def_name インスタンスの定義名
  VlwInst(VlWriter& writer,
	  const string& def_name);

  /// @brief デストラクタ
  ~VlwInst();


public:

  /// @brief 要素間のデリミタを出力する．
  void
  put_delimiter();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // まだ要素を出力していないときに true となるフラグ
  bool mFirst;

};


//////////////////////////////////////////////////////////////////////
/// @class VlwInstElem VlwInst.h "VlwInst.h"
/// @brief インスタンスの要素を出力するためのクラス
//////////////////////////////////////////////////////////////////////
class VlwInstElem :
  public VlwBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] inst 親の VlwInst
  /// @param[in] inst_name インスタンス名
  VlwInstElem(VlwInst& inst,
	      const char* inst_name);

  /// @brief コンストラクタ
  /// @param[in] inst 親の VlwInst
  /// @param[in] inst_name インスタンス名
  VlwInstElem(VlwInst& inst,
	      const string& inst_name);

  /// @brief デストラクタ
  ~VlwInstElem();


public:

  /// @brief ピン割り当てを出力する．
  /// @param[in] pin_name ピン名
  /// @param[in] net_name ピンに接続するネット名
  void
  put_pinassign(const char* pin_name,
		const char* net_name);

  /// @brief ピン割り当てを出力する．
  /// @param[in] pin_name ピン名
  /// @param[in] net_name ピンに接続するネット名
  void
  put_pinassign(const char* pin_name,
		const string& net_name);

  /// @brief ピン割り当てを出力する．
  /// @param[in] pin_name ピン名
  /// @param[in] net_name ピンに接続するネット名
  void
  put_pinassign(const string& pin_name,
		const char* net_name);

  /// @brief ピン割り当てを出力する．
  /// @param[in] pin_name ピン名
  /// @param[in] net_name ピンに接続するネット名
  void
  put_pinassign(const string& pin_name,
		const string& net_name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // まだ要素を出力していない時 true となるフラグ
  bool mFirst;

};

END_NAMESPACE_YM_NETWORKS

#endif // VLWINST_H
