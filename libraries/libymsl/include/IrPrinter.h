#ifndef IRPRINTER_H
#define IRPRINTER_H

/// @file IrPrinter.h
/// @brief IrPrinter のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrPrinter IrPrinter.h "IrPrinter.h"
/// @brief IrNode の内容を出力するクラス
//////////////////////////////////////////////////////////////////////
class IrPrinter
{
public:

  /// @brief コンストラクタ
  /// @param[in] s 出力先のストリーム
  IrPrinter(ostream& s);

  /// @brief デストラクタ
  ~IrPrinter();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コードの内容を出力する．
  void
  print_code(const vector<IrNode*>& node_list);

  /// @brief ノードの内容を出力する．
  void
  print_node(IrNode* node);

  /// @brief ハンドルの内容を出力する．
  void
  print_handle(IrHandle* handle);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードに番号をつける．
  /// @param[in] node ノード
  /// @param[in] node_list ノードリスト
  void
  dfs_node(IrNode* node,
	   vector<IrNode*>& node_list);

  /// @brief ハンドルが指しているノードに番号をつける．
  /// @param[in] handle ハンドル
  /// @param[in] node_list ノードリスト
  void
  dfs_handle(IrHandle* handle,
	     vector<IrNode*>& node_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mS;

};

END_NAMESPACE_YM_YMSL

#endif // IRPRINTER_H
