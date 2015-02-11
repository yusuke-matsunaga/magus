#ifndef IRJUMP_H
#define IRJUMP_H

/// @file IrJump.h
/// @brief IrJump のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrJump IrJump.h "IrJump.h"
/// @brief jump/branch 系の命令を表すクラス
//////////////////////////////////////////////////////////////////////
class IrJump :
  public IrNode
{
public:

  /// @brief コンストラクタ
  /// @param[in] irtype IR型
  /// @param[in] label ジャンプ先のラベル
  /// @param[in] cond 条件
  IrJump(IrType irtype,
	 IrNode* label,
	 IrNode* cond);

  /// @brief デストラクタ
  virtual
  ~IrJump();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 静的評価可能か調べる．
  ///
  /// 要するに定数式かどうかということ
  virtual
  bool
  is_static() const;

  /// @brief ジャンプ先のノードを得る．
  ///
  /// kOpJump, kOpBranchXXX のみ有効
  virtual
  IrNode*
  jump_addr() const;

  /// @brief 分岐条件
  virtual
  IrNode*
  branch_cond() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ジャンプ先のノード
  IrNode* mJumpAddr;

  // 条件
  IrNode* mCond;

};

END_NAMESPACE_YM_YMSL

#endif // IRJUMP_H
