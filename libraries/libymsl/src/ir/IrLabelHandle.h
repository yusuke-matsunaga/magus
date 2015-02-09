#ifndef IRLABELHANEL_H
#define IRLABELHANEL_H

/// @file IrLabelHandle.h
/// @brief IrLabelHandle のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "IrHandle.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLabelHandle IrLabelHandle.h "IrLabelHandle.h"
/// @brief Label を保持する IrHandle
//////////////////////////////////////////////////////////////////////
class IrLabelHandle :
  public IrHandle
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] label ラベル
  IrLabelHandle(ShString name,
		IrNode* label);

  /// @brief デストラクタ
  virtual
  ~IrLabelHandle();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を返す．
  virtual
  ShString
  name() const;

  /// @brief 種類を返す．
  virtual
  HandleType
  handle_type() const;

  /// @brief ラベルを返す．
  ///
  /// 他の要素の場合には NULL を返す．
  virtual
  IrNode*
  label() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // ラベル
  IrNode* mLabel;

};

END_NAMESPACE_YM_YMSL

#endif // IRLABELHANEL_H
