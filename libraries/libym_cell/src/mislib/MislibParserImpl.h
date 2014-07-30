#ifndef MISLIBPARSERIMPL_H
#define MISLIBPARSERIMPL_H

/// @file MislibParserImpl.h
/// @brief MislibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/ShString.h"

#include "MislibMgrImpl.h"
#include "MislibNodeImpl.h"
#include "MislibScanner.h"


BEGIN_NAMESPACE_YM_MISLIB

//////////////////////////////////////////////////////////////////////
/// @class MislibParserImpl MislibParserImpl.h "MislibParserImpl.h"
/// @brief MislibParser の実装クラス
//////////////////////////////////////////////////////////////////////
class MislibParserImpl
{
public:

  /// @brief コンストラクタ
  MislibParserImpl();

  /// @brief デストラクタ
  ~MislibParserImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // メインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief mislib ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] mgr MislibNode を管理するオブジェクト
  /// @return パース木の根のノードを返す．
  /// @note 読み込みが失敗したら false を返す．
  bool
  read_file(const string& filename,
	    MislibMgrImpl* mgr);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ゲートを読み込む．
  /// @return ゲートを表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  MislibNode*
  read_gate();

  /// @brief 式を読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  MislibNode*
  read_expr();

  /// @brief LP を読み込んだ直後の式を読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  MislibNode*
  read_lp();

  /// @brief NOT を読み込んだ直後の式を読み込む．
  /// @return 式を表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  MislibNode*
  read_not();

  /// @brief ピンリスト記述を読み込む．
  /// @return ピンリストを表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  /// ピン名の代わりに * の場合があるので注意
  MislibNode*
  read_pin_list();

  /// @brief ピン記述を読み込む．
  /// @return ピンを表す AST のノードを返す．
  ///
  /// エラーが起きたら NULL を返す．
  MislibNode*
  read_pin();


private:
  //////////////////////////////////////////////////////////////////////
  // MislibNode を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief リストノードを生成する．
  MislibNodeImpl*
  new_list();

  /// @brief NOT ノードを生成する．
  MislibNodeImpl*
  new_not(const FileRegion& loc,
	  const MislibNode* child1);

  /// @brief AND ノードを生成する．
  MislibNodeImpl*
  new_and(const FileRegion& loc,
	  const MislibNode* child1,
	  const MislibNode* child2);

  /// @brief OR ノードを生成する．
  MislibNodeImpl*
  new_or(const FileRegion& loc,
	 const MislibNode* child1,
	 const MislibNode* child2);

  /// @brief XOR ノードを生成する．
  MislibNodeImpl*
  new_xor(const FileRegion& loc,
	  const MislibNode* child1,
	  const MislibNode* child2);

  /// @brief 定数0ノードを生成する．
  MislibNodeImpl*
  new_const0(const FileRegion& loc);

  /// @brief 定数1ノードを生成する．
  MislibNodeImpl*
  new_const1(const FileRegion& loc);

  /// @brief NONINV ノードを生成する．
  MislibNodeImpl*
  new_noninv(const FileRegion& loc);

  /// @brief INV ノードを生成する．
  MislibNodeImpl*
  new_inv(const FileRegion& loc);

  /// @brief UNKNOWN ノードを生成する．
  MislibNodeImpl*
  new_unknown(const FileRegion& loc);

  /// @brief 文字列ノードを生成する．
  MislibNodeImpl*
  new_str(const FileRegion& loc,
	  ShString str);

  /// @brief 数値ノードを生成する．
  MislibNodeImpl*
  new_num(const FileRegion& loc,
	  double num);


private:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc 場所を格納する変数
  /// @return トークンの型を返す．
  ///
  /// lval に値がセットされない場合もある．
  MislibToken
  scan(MislibNodeImpl*& lval,
       FileRegion& lloc);

  /// @brief エラーメッセージを出力する．
  void
  error(const FileRegion& loc,
	const char* msg);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 字句解析器
  MislibScanner* mScanner;

  // MislibNode を管理するオブジェクト
  MislibMgrImpl* mMislibMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief リストノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_list()
{
  return mMislibMgr->new_list();
}

// @brief NOT ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_not(const FileRegion& loc,
			  const MislibNode* child1)
{
  return mMislibMgr->new_not(loc, child1);
}

// @brief AND ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_and(const FileRegion& loc,
			  const MislibNode* child1,
			  const MislibNode* child2)
{
  return mMislibMgr->new_and(loc, child1, child2);
}

// @brief OR ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_or(const FileRegion& loc,
			 const MislibNode* child1,
			 const MislibNode* child2)
{
  return mMislibMgr->new_or(loc, child1, child2);
}

// @brief XOR ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_xor(const FileRegion& loc,
			  const MislibNode* child1,
			  const MislibNode* child2)
{
  return mMislibMgr->new_xor(loc, child1, child2);
}

// @brief 定数0ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_const0(const FileRegion& loc)
{
  return mMislibMgr->new_const0(loc);
}

// @brief 定数1ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_const1(const FileRegion& loc)
{
  return mMislibMgr->new_const1(loc);
}

// @brief NONINV ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_noninv(const FileRegion& loc)
{
  return mMislibMgr->new_noninv(loc);
}

// @brief INV ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_inv(const FileRegion& loc)
{
  return mMislibMgr->new_inv(loc);
}

// @brief UNKNOWN ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_unknown(const FileRegion& loc)
{
  return mMislibMgr->new_unknown(loc);
}

// @brief 文字列ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_str(const FileRegion& loc,
			  ShString str)
{
  return mMislibMgr->new_str(loc, str);
}

// @brief 数値ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_num(const FileRegion& loc,
			  double num)
{
  return mMislibMgr->new_num(loc, num);
}

END_NAMESPACE_YM_MISLIB

#endif // MISLIBPARSERIMPL_H
