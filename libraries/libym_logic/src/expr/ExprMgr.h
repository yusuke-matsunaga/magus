#ifndef EXPRMGR_H
#define EXPRMGR_H

/// @file ExprMgr.h
/// @brief ExprMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "YmLogic/Expr.h"
#include "expr_types.h"
#include "ExprNode.h"
#include "ExprNodePtr.h"

#include "YmUtils/FragAlloc.h"


BEGIN_NAMESPACE_YM_EXPR

//////////////////////////////////////////////////////////////////////
/// @class ExprMgr ExprMgr.h "ExprMgr.h"
/// @brief ExprNode の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class ExprMgr
{
  friend class ExprNode;

private:

  /// @brief コンストラクタ
  ExprMgr();

  /// @brief デストラクタ
  ~ExprMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 静的関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 唯一のインスタンスを返す．
  static
  ExprMgr&
  the_obj();

  /// @brief 確保したメモリを開放する．
  /// @note メモリリークチェックのための関数
  static
  void
  clear_memory();


public:
  //////////////////////////////////////////////////////////////////////
  /// @name ExprNode を作る基本演算
  /// @{

  /// @brief 恒偽関数を作る．
  ExprNodePtr
  make_zero();

  /// @brief 恒真関数を作る．
  ExprNodePtr
  make_one();

  /// @brief 肯定のリテラルを作る．
  /// @param[in] varid 変数番号
  ExprNodePtr
  make_posiliteral(VarId varid);

  /// @brief 否定のリテラルを作る．
  /// @param[in] varid 変数番号
  ExprNodePtr
  make_negaliteral(VarId varid);

  /// @brief AND ノードの生成
  /// @param[in] begin ノードスタック中の開始位置
  ///
  /// - 子供も AND ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  ExprNodePtr
  make_and(ymuint begin);

  /// @brief OR ノードの生成
  /// @param[in] begin ノードスタック中の開始位置
  ///
  /// - 子供も OR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には重複を取り除く
  ExprNodePtr
  make_or(ymuint begin);

  /// @brief XOR ノードの生成
  /// @param[in] begin ノードスタック中の開始位置
  ///
  /// - 子供も XOR ノードの場合にはマージする．
  /// - 子供が定数ノードの場合には値に応じた簡単化を行う．
  /// - 同一の子供ノードがあった場合には個数の偶奇に応じた処理を行う．
  ExprNodePtr
  make_xor(ymuint begin);

  /// @brief ノードスタックにノードを入れる．
  void
  nodestack_push(const ExprNode* node);

  /// @brief ノードスタックの先頭位置を返す．
  ymuint
  nodestack_top();

  /// @brief ノードスタックの先頭位置を戻す．
  /// @note この関数を明示的に使う必要があるののはノードを
  /// プッシュしておきながら make_XXX を呼ばないとき
  void
  nodestack_pop(ymuint oldtop);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:

  //////////////////////////////////////////////////////////////////////
  /// @name 式を元に新たな式を作り出す関数
  /// @{

  /// @brief 否定の形(双対形)を返す．
  ExprNodePtr
  complement(const ExprNode* node);

  /// @brief リテラルを論理式に置き換える．
  /// @param[in] varid 置き換え対象の変数番号
  /// @param[in] sub 置き換え先の論理式
  /// @note sub の中に varid 番目のリテラルが含まれている場合でも
  /// 正常に処理を行う．
  /// @note sub の中のリテラルは展開しない
  ExprNodePtr
  compose(const ExprNode* node,
	  VarId varid,
	  const ExprNodePtr& sub);

  /// @brief 一度に複数の置き換えを行う．
  /// @param[in] compmap 置き換え対象の変数番号と置き換え先の論理式
  /// を入れた連想配列
  ExprNodePtr
  compose(const ExprNode* node,
	  const VarExprMap& compmap);

  /// @brief 変数番号をマッピングし直す
  /// @param[in] varmap 置き換え元と置き換え先の変数番号を入れた連想配列
  ExprNodePtr
  remap_var(const ExprNode* node,
	    const VarVarMap& varmap);

  /// @brief 簡単化を行う．
  ExprNodePtr
  simplify(const ExprNode* node);

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報を取り出す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 使用されているメモリ量を返す．
  ymuint
  used_size();

  /// @brief 使用されているノード数を返す．
  ymuint
  node_num();

  /// @brief used_size() の最大値を返す．
  ymuint
  max_used_size();

  /// @brief nodenum() の最大値を返す．
  ymuint
  max_node_num();

  /// @brief 実際に確保したメモリ量を返す．
  ymuint
  allocated_size();

  /// @brief 実際に確保した回数を返す．
  ymuint
  allocated_count();

  /// @brief 内部状態を出力する．
  void
  print_stats(ostream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // make_and()/make_or() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる．
  // 同相で同じものがふくまれていたらなにもせず false を返す．
  // 逆相で同じものがふくまれていたらなにもせず true を返す．
  // 同じものがふくまれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node(const ExprNode* node);

  // make_xor() 用のサブルーティン
  // node が mTmpNodeList の中に含まれていないか調べる(XOR用)．
  // 同相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // false を返す．
  // 逆相で同じものが含まれていたら重複したものを mTmpNodeList から削除して
  // true を返す．
  // 同じものが含まれていなかったら node を mTmpNodeList に追加して
  // false を返す．
  bool
  check_node2(const ExprNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // id 番めまでのリテラルノードを作る．
  void
  make_literals(VarId id);

  // ExprNode を確保して内容を設定する．
  ExprNode*
  alloc_node(tType type);

  // ExprNode を削除する．
  void
  free_node(ExprNode* node);

  // ExprNode の入力数から必要なサイズを計算する．
  static
  ymuint
  calc_size(ymuint nc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード用のアロケーター
  FragAlloc mNodeAlloc;

  // 唯一の定数0ノード
  ExprNodePtr mConst0;

  // 唯一の定数1ノード
  ExprNodePtr mConst1;

  // リテラルを表すノードの配列
  vector<ExprNodePtr> mLiteralArray;

  // 作業領域として使われるノードの配列
  ExprNodeList mTmpNodeList;

  // 再帰関数のなかで作業領域として使われるノードの配列
  ExprNodeList mNodeStack;

  // 使用中のノード数
  ymuint32 mNodeNum;

  // 使用した最大のノード数
  ymuint32 mMaxNodeNum;

  // 絶対に開放されないノード数
  ymuint32 mStuckNodeNum;

};

END_NAMESPACE_YM_EXPR

#endif // EXPRMGR_H
