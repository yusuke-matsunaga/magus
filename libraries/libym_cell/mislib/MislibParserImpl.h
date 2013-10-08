#ifndef LIBYM_MISLIB_MISLIBPARSERIMPL_H
#define LIBYM_MISLIB_MISLIBPARSERIMPL_H

/// @file libym_mislib/MislibParserImpl.h
/// @brief MislibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "mislib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/ShString.h"

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

  /// @brief mislib ファイルを読み込む．
  /// @param[in] filename ファイル名
  /// @param[in] mgr MislibNode を管理するオブジェクト
  /// @return パース木の根のノードを返す．
  /// @note 読み込みが失敗したら false を返す．
  bool
  read_file(const string& filename,
	    MislibMgrImpl* mgr);


public:
  //////////////////////////////////////////////////////////////////////
  // MislibNode を生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief GATE ノードを生成する．(通常版)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate1(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin_list);

  /// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate2(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin);

  /// @brief GATE ノードを生成する．(入力ピンなし:定数ノード)
  /// @note 結果はゲートのリストに追加される．
  void
  new_gate3(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr);

  /// @brief PIN ノードを生成する．
  MislibNodeImpl*
  new_pin(const FileRegion& loc,
	  const MislibNode* name,
	  const MislibNode* phase,
	  const MislibNode* input_load,
	  const MislibNode* max_load,
	  const MislibNode* rise_block_delay,
	  const MislibNode* rise_fanout_delay,
	  const MislibNode* fall_block_delay,
	  const MislibNode* fall_fanout_delay);

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


public:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
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
  MislibScanner mScanner;

  // MislibNode を管理するオブジェクト
  MislibMgrImpl* mMislibMgr;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief GATE ノードを生成する．(通常版)
// @note 結果はゲートのリストに追加される．
inline
void
MislibParserImpl::new_gate1(const FileRegion& loc,
			    const MislibNode* name,
			    const MislibNode* area,
			    const MislibNode* oname,
			    const MislibNode* expr,
			    const MislibNode* ipin_list)
{
  return mMislibMgr->new_gate1(loc, name, area, oname, expr, ipin_list);
}

// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
// @note 結果はゲートのリストに追加される．
inline
void
MislibParserImpl::new_gate2(const FileRegion& loc,
			    const MislibNode* name,
			    const MislibNode* area,
			    const MislibNode* oname,
			    const MislibNode* expr,
			    const MislibNode* ipin)
{
  return mMislibMgr->new_gate2(loc, name, area, oname, expr, ipin);
}

// @brief GATE ノードを生成する．(入力ピンなし:定数ノード)
// @note 結果はゲートのリストに追加される．
inline
void
MislibParserImpl::new_gate3(const FileRegion& loc,
			    const MislibNode* name,
			    const MislibNode* area,
			    const MislibNode* oname,
			    const MislibNode* expr)
{
  return mMislibMgr->new_gate3(loc, name, area, oname, expr);
}

// @brief PIN ノードを生成する．
inline
MislibNodeImpl*
MislibParserImpl::new_pin(const FileRegion& loc,
			  const MislibNode* name,
			  const MislibNode* phase,
			  const MislibNode* input_load,
			  const MislibNode* max_load,
			  const MislibNode* rise_block_delay,
			  const MislibNode* rise_fanout_delay,
			  const MislibNode* fall_block_delay,
			  const MislibNode* fall_fanout_delay)
{
  return mMislibMgr->new_pin(loc, name, phase, input_load, max_load,
			     rise_block_delay, rise_fanout_delay,
			     fall_block_delay, fall_fanout_delay);
}

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

#endif // LIBYM_MISLIB_MISLIBPARSERIMPL_H
