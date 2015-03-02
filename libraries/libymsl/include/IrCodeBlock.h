#ifndef IRCODEBLOCK_H
#define IRCODEBLOCK_H

/// @file IrCodeBlock.h
/// @brief IrCodeBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrCodeBlock IrCodeBlock.h "IrCodeBlock.h"
/// @brief プログラムの中間表現
///
/// 派生クラスに IrToplevel と IrFunction がある．
//////////////////////////////////////////////////////////////////////
class IrCodeBlock
{
public:

  /// @brief コンストラクタ
  IrCodeBlock();

  /// @brief デストラクタ
  virtual
  ~IrCodeBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を追加する．
  /// @param[in] var 変数のハンドル
  virtual
  void
  add_var(IrHandle* var);

  /// @brief ノードを追加する．
  void
  add_node(IrNode* node);

  /// @brief 変数のリストを得る．
  const vector<IrHandle*>&
  var_list() const;

  /// @brief ノードのリストを得る．
  const vector<IrNode*>&
  node_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のリスト
  vector<IrHandle*> mVarList;

  // 本体のノードリスト
  vector<IrNode*> mNodeList;

};

END_NAMESPACE_YM_YMSL

#endif // IRCODEBLOCK_H
