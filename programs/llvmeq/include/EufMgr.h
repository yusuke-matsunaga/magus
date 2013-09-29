#ifndef EUFMGR_H
#define EUFMGR_H

/// @file EufMgr.h
/// @brief EufMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"


BEGIN_NAMESPACE_YM_LLVMEQ

class EufVarMgr;
class EufFuncMgr;
class EufBinMgr;

//////////////////////////////////////////////////////////////////////
/// @class EufMgr EufMgr.h "EufMgr.h"
/// @brief EUF 式を生成するための管理クラス
//////////////////////////////////////////////////////////////////////
class EufMgr
{
public:

  /// @brief コンストラクタ
  EufMgr();

  /// @brief デストラクタ
  ~EufMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 変数を生成する．
  /// @param[in] name 変数名
  EufNode*
  new_variable(const string& name);

  /// @brief 関数を生成する．
  /// @param[in] name 関数名
  /// @param[in] arg_list 引数のリスト
  /// @param[in] sym_flag 対称関数の時 true にするフラグ
  /// @note sym_flag が true の時は arg_list の順番を正規化する．
  EufNode*
  new_function(const string& name,
	       const vector<EufNode*>& arg_list,
	       bool sym_flag = false);

  /// @brief 等価演算子を生成する．
  /// @param[in] left 左辺式
  /// @param[in] right 右辺式
  /// @note 場合によっては左辺式と右辺式は交換される．
  EufNode*
  new_equality(EufNode* left,
	       EufNode* right);

  /// @brief 連言(conjunction)を生成する．
  /// @param[in] left 左辺式
  /// @param[in] right 右辺式
  EufNode*
  new_conjunction(EufNode* left,
		  EufNode* right);

  /// @brief 選言(disjunction)を生成する．
  /// @param[in] left 左辺式
  /// @param[in] right 右辺式
  EufNode*
  new_disjunction(EufNode* left,
		  EufNode* right);

  /// @brief 否定(negation)を生成する．
  /// @param[in] operand 式
  EufNode*
  new_negation(EufNode* operand);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数用のハッシュ
  EufVarMgr* mVarMgr;

  // 関数用のハッシュ
  EufFuncMgr* mFuncMgr;

  // 二項演算子用のハッシュ
  EufBinMgr* mBinMgr;

  // 次のID番号
  ymuint32 mLastId;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFMGR_H
