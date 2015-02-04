#ifndef IRLOAD_H
#define IRLOAD_H

/// @file IrLoad.h
/// @brief IrLoad のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrLoad IrLoad.h "IrLoad.h"
/// @brief ロード命令を表すノード
//////////////////////////////////////////////////////////////////////
class IrLoad :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] base ベースアドレス
  /// @param[in] offset オフセット
  IrLoad(const Type* type,
	 IrNode* base,
	 IrNode* offset);

  /// @brief デストラクタ
  virtual
  ~IrLoad();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ベースアドレスを返す．
  virtual
  IrNode*
  base() const;

  /// @brief オフセットを返す．
  virtual
  IrNode*
  offset() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ベースアドレス
  IrNode* mBase;

  // オフセット
  IrNode* mOffset;

};

END_NAMESPACE_YM_YMSL

#endif // IRLOAD_H
