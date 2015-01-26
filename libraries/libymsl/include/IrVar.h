#ifndef IRVAR_H
#define IRVAR_H

/// @file IrVar.h
/// @brief IrVar のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrVar IrVar.h "IrVar.h"
/// @brief IR の変数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrVar
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] index インデックス
  IrVar(ShString name,
	const IrType* value_type,
	ymuint index);

  /// @brief デストラクタ
  ~IrVar();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数名を得る．
  ShString
  name() const;

  /// @brief 型を得る．
  const IrType*
  value_type() const;

  /// @brief 変数インデックスを得る．
  ymuint
  index() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // 型
  const IrType* mValueType;

  // インデックス
  ymuint mIndex;

};

END_NAMESPACE_YM_YMSL

#endif // IRVAR_H
