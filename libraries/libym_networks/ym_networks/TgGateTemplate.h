#ifndef YM_NETWORKS_TGGATETEMPLATE_H
#define YM_NETWORKS_TGGATETEMPLATE_H

/// @file ym_networks/TgGateTemplate.h
/// @brief 論理ゲートの種類を定義したファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TgGateTemplate.h 1920 2008-12-20 15:52:42Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_networks/tgnet.h"


BEGIN_NAMESPACE_YM_NETWORKS

//////////////////////////////////////////////////////////////////////
/// @class TgGateTemplate TgGateTemplate.h <ym_networks/TgGateTemplate.h>
/// @brief 論理ゲートのテンプレートを表すクラス
/// @note 扱える入力数は最大で 4096
/// @note 保持できるユーザー定義論理関数の種類は約 1M 個
//////////////////////////////////////////////////////////////////////
class TgGateTemplate
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 論理関数のID
  /// @param[in] ni 入力数
  TgGateTemplate(tTgGateType type = kTgUndef,
		 size_t ni = 0);


public:

  /// @brief 論理関数の型を得る．
  tTgGateType
  type() const;

  /// @brief 入力数を得る．
  size_t
  ni() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // データ
  ymuint32 mData;

};

/// @relates TgGateTemplate
/// @brief TgGateTemplate の内容を出力する．
ostream&
operator<<(ostream& s,
	   TgGateTemplate gt_id);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 論理関数のID
// @param[in] ni 入力数
inline
TgGateTemplate::TgGateTemplate(tTgGateType type,
			       size_t ni)
{
  mData = (static_cast<ymuint32>(type) << 12) | static_cast<ymuint32>(ni);
}

// @brief 論理関数の型を得る．
inline
tTgGateType
TgGateTemplate::type() const
{
  return static_cast<tTgGateType>(mData >> 12);
}

// @brief 入力数を得る．
inline
size_t
TgGateTemplate::ni() const
{
  return static_cast<size_t>(mData & 0xFFF);
}

END_NAMESPACE_YM_NETWORKS

#endif // YM_NETWORKS_TGGATETEMPLATE_H
