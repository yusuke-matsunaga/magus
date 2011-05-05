#ifndef LIBYM_MISLIB_MISLIBPARSERIMPL_H
#define LIBYM_MISLIB_MISLIBPARSERIMPL_H

/// @file libym_mislib/MislibParserImpl.h
/// @brief MislibParserImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MislibParser.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mislib/mislib_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/Alloc.h"
#include "ym_utils/ShString.h"

#include "MislibNodeImpl.h"
#include "MislibLex.h"


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
  /// @return パース木の根のノードを返す．
  /// @note 読み込みが失敗したら NULL を返す．
  const MislibNode*
  read(const string& filename);


public:
  //////////////////////////////////////////////////////////////////////
  // mislib_grammer.yy で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 今までに生成したすべてのオブジェクトを解放する．
  void
  clear();

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

  /// @brief GATE ノードを生成する．(通常版)
  void
  new_gate1(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin_list);

  /// @brief GATE ノードを生成する．(ワイルドカードの入力ピン)
  void
  new_gate2(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr,
	    const MislibNode* ipin);

  /// @brief GATE ノードを生成する．(入力ピンなし:定数ノード)
  void
  new_gate3(const FileRegion& loc,
	    const MislibNode* name,
	    const MislibNode* area,
	    const MislibNode* oname,
	    const MislibNode* expr);

  /// @brief 字句解析を行う．
  /// @param[out] lval トークンの値を格納する変数
  /// @param[out] lloc トークンの位置を格納する変数
  /// @return トークンの型を返す．
  int
  scan(MislibNodeImpl*& lval,
       FileRegion& lloc);

  /// @brief エラーメッセージを出力する．
  /// @note 副作用で mError が true にセットされる．
  void
  error(const FileRegion& loc,
	const char* msg);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ使われるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列ノードを生成する．
  MislibNodeImpl*
  new_str(const FileRegion& loc,
	  ShString str);

  /// @brief 数値ノードを生成する．
  MislibNodeImpl*
  new_num(const FileRegion& loc,
	  double num);

  /// @brief NONINV ノードを生成する．
  MislibNodeImpl*
  new_noninv(const FileRegion& loc);

  /// @brief INV ノードを生成する．
  MislibNodeImpl*
  new_inv(const FileRegion& loc);

  /// @brief UNKNOWN ノードを生成する．
  MislibNodeImpl*
  new_unknown(const FileRegion& loc);

  /// @brief 定数0ノードを生成する．
  MislibNodeImpl*
  new_const0(const FileRegion& loc);

  /// @brief 定数1ノードを生成する．
  MislibNodeImpl*
  new_const1(const FileRegion& loc);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // const MislibNode のメモリ確保用アロケータ
  SimpleAlloc mAlloc;

  // 字句解析器
  MislibLex mLex;

  // ゲートのリスト
  MislibNodeImpl* mGateList;

  // 読み込み時のエラーの有無を示すフラグ
  bool mError;

};

END_NAMESPACE_YM_MISLIB

#endif // LIBYM_MISLIB_MISLIBPARSERIMPL_H
