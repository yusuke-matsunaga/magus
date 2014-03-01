
/// @file src/PtMgr.cc
/// @brief PtMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "Var.h"

#include "PtIdNode.h"
#include "PtConstNode.h"
#include "PtNegOp.h"
#include "PtUminusOp.h"
#include "PtAddOp.h"
#include "PtSubOp.h"
#include "PtMulOp.h"
#include "PtDivOp.h"
#include "PtModOp.h"
#include "PtSllOp.h"
#include "PtSrlOp.h"
#include "PtAndOp.h"
#include "PtOrOp.h"
#include "PtXorOp.h"


BEGIN_NAMESPACE_YM_BB


//////////////////////////////////////////////////////////////////////
// クラス PtMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtMgr::PtMgr() :
  mAlloc(4096)
{
  mLastId = 0;
}

// @brief デストラクタ
PtMgr::~PtMgr()
{
}

// @brief 初期化する．
void
PtMgr::init()
{
  mIdMap.clear();
  mVarList.clear();
  mConstrList.clear();
  mAlloc.destroy();
  mLastId = 0;
}

// @brief 変数のリストを得る．
const vector<Var*>&
PtMgr::var_list() const
{
  return mVarList;
}

// @brief 代入文のリストを得る．
const vector<Assign*>&
PtMgr::assign_list() const
{
  return mAssignList;
}

// @brief 制約式のリストを得る．
const vector<Constr*>&
PtMgr::constr_list() const
{
  return mConstrList;
}

// @brief ID ノードを作る．
PtNode*
PtMgr::new_id(const FileRegion& file_region,
	      ymuint id)
{
  void* p = mAlloc.get_memory(sizeof(PtIdNode));
  PtNode* node = new (p) PtIdNode(file_region, id);
  return node;
}

// @brief 定数ノードを作る．
PtNode*
PtMgr::new_const(const FileRegion& file_region,
		 ymint32 value)
{
  void* p = mAlloc.get_memory(sizeof(PtConstNode));
  PtNode* node = new (p) PtConstNode(file_region, value);
  return node;
}

// @brief neg ノードを作る．
PtNode*
PtMgr::new_neg(const FileRegion& file_region,
	       PtNode* opr1)
{
  void* p = mAlloc.get_memory(sizeof(PtNegOp));
  PtNode* node = new (p) PtNegOp(file_region, opr1);
  return node;
}

// @brief uminus ノードを作る．
PtNode*
PtMgr::new_uminus(const FileRegion& file_region,
		  PtNode* opr1)
{
  void* p = mAlloc.get_memory(sizeof(PtUminusOp));
  PtNode* node = new (p) PtUminusOp(file_region, opr1);
  return node;
}

// @brief add ノードを作る．
PtNode*
PtMgr::new_add(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtAddOp));
  PtNode* node = new (p) PtAddOp(file_region, opr1, opr2);
  return node;
}

// @brief sub ノードを作る．
PtNode*
PtMgr::new_sub(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtSubOp));
  PtNode* node = new (p) PtSubOp(file_region, opr1, opr2);
  return node;
}

// @brief mul ノードを作る．
PtNode*
PtMgr::new_mul(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtMulOp));
  PtNode* node = new (p) PtMulOp(file_region, opr1, opr2);
  return node;
}

// @brief div ノードを作る．
PtNode*
PtMgr::new_div(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtDivOp));
  PtNode* node = new (p) PtDivOp(file_region, opr1, opr2);
  return node;
}

// @brief mod ノードを作る．
PtNode*
PtMgr::new_mod(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtModOp));
  PtNode* node = new (p) PtModOp(file_region, opr1, opr2);
  return node;
}

// @brief and ノードを作る．
PtNode*
PtMgr::new_and(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtAndOp));
  PtNode* node = new (p) PtAndOp(file_region, opr1, opr2);
  return node;
}

// @brief or ノードを作る．
PtNode*
PtMgr::new_or(const FileRegion& file_region,
	      PtNode* opr1,
	      PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtOrOp));
  PtNode* node = new (p) PtOrOp(file_region, opr1, opr2);
  return node;
}

// @brief xor ノードを作る．
PtNode*
PtMgr::new_xor(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtXorOp));
  PtNode* node = new (p) PtXorOp(file_region, opr1, opr2);
  return node;
}

// @brief sll ノードを作る．
PtNode*
PtMgr::new_sll(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtSllOp));
  PtNode* node = new (p) PtSllOp(file_region, opr1, opr2);
  return node;
}

// @brief srl ノードを作る．
PtNode*
PtMgr::new_srl(const FileRegion& file_region,
	       PtNode* opr1,
	       PtNode* opr2)
{
  void* p = mAlloc.get_memory(sizeof(PtSrlOp));
  PtNode* node = new (p) PtSrlOp(file_region, opr1, opr2);
  return node;
}

// @brief 新しい変数を追加する．
void
PtMgr::new_var(const FileRegion& file_region,
	       ymuint id)
{
  void* p = mAlloc.get_memory(sizeof(Var));
  Var* var = new (p) Var(file_region, id, 0, 0, 0);
  mVarList.push_back(var);
}

// @brief 新しい変数を追加する．
void
PtMgr::new_var(const FileRegion& file_region,
	       ymuint id,
	       int start,
	       int end)
{
  void* p = mAlloc.get_memory(sizeof(Var));
  Var* var = new (p) Var(file_region, id, start, end, 1);
  mVarList.push_back(var);
}

// @brief 新しい変数を追加する．
void
PtMgr::new_var(const FileRegion& file_region,
	       ymuint id,
	       int start,
	       int end,
	       int delta)
{
  void* p = mAlloc.get_memory(sizeof(Var));
  Var* var = new (p) Var(file_region, id, start, end, delta);
  mVarList.push_back(var);
}

// @brief 名前を ID 番号に変換する．
ymuint
PtMgr::str2id(const char* name)
{
  string str(name);
  ymuint id = 0;
  unordered_map<string, ymuint32>::iterator p = mIdMap.find(str);
  if ( p == mIdMap.end() ) {
    id = mLastId;
    ++ mLastId;
    mIdMap.insert(make_pair(str, id));
  }
  else {
    id = p->second;
  }
  return id;
}


//////////////////////////////////////////////////////////////////////
// クラス Var
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Var::Var(const FileRegion& file_region,
	 ymuint id,
	 int start,
	 int end,
	 int delta) :
  mFileRegion(file_region),
  mId(id),
  mStart(start),
  mEnd(end),
  mDelta(delta)
{
}

// @brief デストラクタ
Var::~Var()
{
}

END_NAMESPACE_YM_BB
