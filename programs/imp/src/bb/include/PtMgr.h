#ifndef INCLUDE_PTMGR_H
#define INCLUDE_PTMGR_H

/// @file include/PtMgr.h
/// @brief PtMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "bb_nsdef.h"
#include "YmUtils/FileRegion.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_BB

class PtNode;
class Var;
class Assign;
class Constr;

//////////////////////////////////////////////////////////////////////
/// @class PtMgr PtMgr.h "PtMgr.h"
/// @brief パース木のノードを管理するクラス
//////////////////////////////////////////////////////////////////////
class PtMgr
{
public:

  /// @brief コンストラクタ
  PtMgr();

  /// @brief デストラクタ
  ~PtMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部に対するインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 変数のリストを得る．
  const vector<Var*>&
  var_list() const;

  /// @brief 代入文のリストを得る．
  const vector<Assign*>&
  assign_list() const;

  /// @brief 制約式のリストを得る．
  const vector<Constr*>&
  constr_list() const;


public:
  //////////////////////////////////////////////////////////////////////
  // Parser 用のインターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID ノードを作る．
  PtNode*
  new_id(const FileRegion& file_region,
	 ymuint id);

  /// @brief 定数ノードを作る．
  PtNode*
  new_const(const FileRegion& file_region,
	    ymint32 value);

  /// @brief neg ノードを作る．
  PtNode*
  new_neg(const FileRegion& file_region,
	  PtNode* opr1);

  /// @brief uminus ノードを作る．
  PtNode*
  new_uminus(const FileRegion& file_region,
	     PtNode* opr1);

  /// @brief add ノードを作る．
  PtNode*
  new_add(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief sub ノードを作る．
  PtNode*
  new_sub(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief mul ノードを作る．
  PtNode*
  new_mul(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief div ノードを作る．
  PtNode*
  new_div(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief mod ノードを作る．
  PtNode*
  new_mod(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief and ノードを作る．
  PtNode*
  new_and(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief or ノードを作る．
  PtNode*
  new_or(const FileRegion& file_region,
	 PtNode* opr1,
	 PtNode* opr2);

  /// @brief xor ノードを作る．
  PtNode*
  new_xor(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief sll ノードを作る．
  PtNode*
  new_sll(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief srl ノードを作る．
  PtNode*
  new_srl(const FileRegion& file_region,
	  PtNode* opr1,
	  PtNode* opr2);

  /// @brief 新しい代入文を追加する．
  void
  new_assign(const FileRegion& file_region,
	     ymuint lhs_id,
	     PtNode* rhs);

  /// @brief 新しい等式を追加する．
  void
  new_eq(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_lt(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_le(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_gt(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい不等式を追加する．
  void
  new_ge(const FileRegion& file_region,
	 PtNode* lhs,
	 PtNode* rhs);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id,
	  int start,
	  int end);

  /// @brief 新しい変数を追加する．
  void
  new_var(const FileRegion& file_region,
	  ymuint id,
	  int start,
	  int end,
	  int delta);

  /// @brief 名前を ID 番号に変換する．
  ymuint
  str2id(const char* name);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリ確保用のオブジェクト
  SimpleAlloc mAlloc;

  // 名前と ID 番号の辞書
  unordered_map<string, ymuint32> mIdMap;

  // 次に割り当てる ID 番号
  ymuint32 mLastId;

  // 変数のリスト
  vector<Var*> mVarList;

  // 代入文のリスト
  vector<Assign*> mAssignList;

  // 制約式のリスト
  vector<Constr*> mConstrList;

};

END_NAMESPACE_YM_BB

#endif // INCLUDE_PTMGR_H
