#ifndef EUFBINMGR_H
#define EUFBINMGR_H

/// @file EufBinMgr.h
/// @brief EufBinMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "llvmeq_nsdef.h"
#include "YmUtils/UnitAlloc.h"
#include "YmLogic/VarId.h"


BEGIN_NAMESPACE_YM_LLVMEQ

class EufBin;

//////////////////////////////////////////////////////////////////////
/// @class EufBinMgr EufBinMgr.h "EufBinMgr.h"
/// @brief EufBin を管理するクラス
//////////////////////////////////////////////////////////////////////
class EufBinMgr
{
public:

  /// @brief コンストラクタ
  EufBinMgr();

  /// @brief デストラクタ
  ~EufBinMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 等価式を探す．
  /// @param[in] left, right 左辺式，右辺式
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find_equality(EufNode* left,
		EufNode* right) const;

  /// @brief 連言(conjunction)を探す．
  /// @param[in] left, right 左辺式，右辺式
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find_conjunction(EufNode* left,
		   EufNode* right) const;

  /// @brief 選言(disjunction)を探す．
  /// @param[in] left, right 左辺式，右辺式
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find_disjunction(EufNode* left,
		   EufNode* right) const;

  /// @brief 否定(negation)を探す．
  /// @param[in] left オペランド
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find_negation(EufNode* left) const;

  /// @brief 等価式を生成する．
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺式，右辺式
  EufNode*
  new_equality(ymuint id,
	       VarId vid,
	       EufNode* left,
	       EufNode* right);

  /// @brief 連言(conjunction)を生成する．
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺式，右辺式
  EufNode*
  new_conjunction(ymuint id,
		  VarId vid,
		  EufNode* left,
		  EufNode* right);

  /// @brief 選言(disjunction)を生成する．
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left, right 左辺式，右辺式
  EufNode*
  new_disjunction(ymuint id,
		  VarId vid,
		  EufNode* left,
		  EufNode* right);

  /// @brief 否定(negation)を生成する．
  /// @param[in] id ID番号
  /// @param[in] vid SatSolver 用の変数番号
  /// @param[in] left オペランド
  EufNode*
  new_negation(ymuint id,
	       VarId vid,
	       EufNode* left);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードを探す．
  /// @param[in] type 型
  /// @param[in] left, right 左辺式，右辺式
  /// @note 見つからなければ NULL を返す．
  EufNode*
  find(ymuint type,
       EufNode* left,
       EufNode* right) const;

  /// @brief ノードを追加する．
  void
  put(EufBin* node);

  /// @brief ハッシュテーブルを拡大する．
  void
  expand(ymuint req_size);

  /// @brief ハッシュ値を計算する．
  /// @param[in] type 型
  /// @param[in] left, right 左辺式，右辺式
  ymuint
  hash(ymuint type,
       EufNode* left,
       EufNode* right) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // EufEq 用のアロケータ
  UnitAlloc mAlloc;

  // ハッシュ中の要素数
  ymuint32 mNum;

  // ハッシュテーブルのサイズ
  ymuint32 mTableSize;

  // ハッシュテーブル
  EufBin** mTable;

  // ハッシュを拡張する目安
  ymuint32 mNextLimit;

};

END_NAMESPACE_YM_LLVMEQ

#endif // EUFBINMGR_H
