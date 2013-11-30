
/// @file src/pt/Constr.cc
/// @brief Constr とその継承クラスの実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtMgr.h"
#include "PtNode.h"
#include "Constr.h"
#include "Eq.h"
#include "Lt.h"
#include "Le.h"
#include "Gt.h"
#include "Ge.h"


BEGIN_NAMESPACE_YM_BB

// @brief 新しい等式を追加する．
void
PtMgr::new_eq(const FileRegion& file_region,
	      PtNode* lhs,
	      PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Eq));
  Eq* eq = new (p) Eq(file_region, lhs, rhs);
  mConstrList.push_back(eq);
}

// @brief 新しい不等式を追加する．
void
PtMgr::new_lt(const FileRegion& file_region,
	      PtNode* lhs,
	      PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Lt));
  Lt* lt = new (p) Lt(file_region, lhs, rhs);
  mConstrList.push_back(lt);
}

// @brief 新しい不等式を追加する．
void
PtMgr::new_le(const FileRegion& file_region,
	      PtNode* lhs,
	      PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Le));
  Le* le = new (p) Le(file_region, lhs, rhs);
  mConstrList.push_back(le);
}

// @brief 新しい不等式を追加する．
void
PtMgr::new_gt(const FileRegion& file_region,
	      PtNode* lhs,
	      PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Gt));
  Gt* gt = new (p) Gt(file_region, lhs, rhs);
  mConstrList.push_back(gt);
}

// @brief 新しい不等式を追加する．
void
PtMgr::new_ge(const FileRegion& file_region,
	      PtNode* lhs,
	      PtNode* rhs)
{
  void* p = mAlloc.get_memory(sizeof(Ge));
  Ge* ge = new (p) Ge(file_region, lhs, rhs);
  mConstrList.push_back(ge);
}


//////////////////////////////////////////////////////////////////////
// クラス Constr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Constr::Constr(const FileRegion& file_region,
	       PtNode* lhs,
	       PtNode* rhs) :
  mFileRegion(file_region),
  mLhs(lhs),
  mRhs(rhs)
{
}

// @brief デストラクタ
Constr::~Constr()
{
}

// @brief ファイル上の位置を得る．
FileRegion
Constr::file_region() const
{
  return mFileRegion;
}

// @brief 左辺式を得る．
PtNode*
Constr::lhs() const
{
  return mLhs;
}

// @brief 右辺式を得る．
PtNode*
Constr::rhs() const
{
  return mRhs;
}


//////////////////////////////////////////////////////////////////////
// クラス Eq
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Eq::Eq(const FileRegion& file_region,
       PtNode* lhs,
       PtNode* rhs) :
  Constr(file_region, lhs, rhs)
{
}

// @brief デストラクタ
Eq::~Eq()
{
}

// @brief 式の型を得る．
Constr::tType
Eq::type() const
{
  return kEq;
}

// @brief 対応した AIG を作る．
void
Eq::gen_aig(AigMgr& aigmgr,
	    const vector<Aig>& bvar_array,
	    ymuint bw,
	    vector<Aig>& tmp_list)
{
  vector<Aig> lnode_array(bw);
  lhs()->gen_aig(aigmgr, bvar_array, bw, lnode_array);
  vector<Aig> rnode_array(bw);
  rhs()->gen_aig(aigmgr, bvar_array, bw, rnode_array);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig tmp = aigmgr.make_xnor(lnode_array[i], rnode_array[i]);
    tmp_list.push_back(tmp);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Lt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Lt::Lt(const FileRegion& file_region,
       PtNode* lhs,
       PtNode* rhs) :
  Constr(file_region, lhs, rhs)
{
}

// @brief デストラクタ
Lt::~Lt()
{
}

// @brief 式の型を得る．
Constr::tType
Lt::type() const
{
  return kLt;
}

// @brief 対応した AIG を作る．
void
Lt::gen_aig(AigMgr& aigmgr,
	    const vector<Aig>& bvar_array,
	    ymuint bw,
	    vector<Aig>& tmp_list)
{
  vector<Aig> lnode_array(bw);
  lhs()->gen_aig(aigmgr, bvar_array, bw, lnode_array);
  vector<Aig> rnode_array(bw);
  rhs()->gen_aig(aigmgr, bvar_array, bw, rnode_array);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig tmp = aigmgr.make_xnor(lnode_array[i], rnode_array[i]);
    tmp_list.push_back(tmp);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Le
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Le::Le(const FileRegion& file_region,
       PtNode* lhs,
       PtNode* rhs) :
  Constr(file_region, lhs, rhs)
{
}

// @brief デストラクタ
Le::~Le()
{
}

// @brief 式の型を得る．
Constr::tType
Le::type() const
{
  return kLe;
}

// @brief 対応した AIG を作る．
void
Le::gen_aig(AigMgr& aigmgr,
	    const vector<Aig>& bvar_array,
	    ymuint bw,
	    vector<Aig>& tmp_list)
{
  vector<Aig> lnode_array(bw);
  lhs()->gen_aig(aigmgr, bvar_array, bw, lnode_array);
  vector<Aig> rnode_array(bw);
  rhs()->gen_aig(aigmgr, bvar_array, bw, rnode_array);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig tmp = aigmgr.make_xnor(lnode_array[i], rnode_array[i]);
    tmp_list.push_back(tmp);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Gt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Gt::Gt(const FileRegion& file_region,
       PtNode* lhs,
       PtNode* rhs) :
  Constr(file_region, lhs, rhs)
{
}

// @brief デストラクタ
Gt::~Gt()
{
}

// @brief 式の型を得る．
Constr::tType
Gt::type() const
{
  return kGt;
}

// @brief 対応した AIG を作る．
void
Gt::gen_aig(AigMgr& aigmgr,
	    const vector<Aig>& bvar_array,
	    ymuint bw,
	    vector<Aig>& tmp_list)
{
  vector<Aig> lnode_array(bw);
  lhs()->gen_aig(aigmgr, bvar_array, bw, lnode_array);
  vector<Aig> rnode_array(bw);
  rhs()->gen_aig(aigmgr, bvar_array, bw, rnode_array);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig tmp = aigmgr.make_xnor(lnode_array[i], rnode_array[i]);
    tmp_list.push_back(tmp);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス Ge
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] file_region ファイル上の位置
// @param[in] lhs 左辺式
// @param[in] rhs 右辺式
Ge::Ge(const FileRegion& file_region,
       PtNode* lhs,
       PtNode* rhs) :
  Constr(file_region, lhs, rhs)
{
}

// @brief デストラクタ
Ge::~Ge()
{
}

// @brief 式の型を得る．
Constr::tType
Ge::type() const
{
  return kGe;
}

// @brief 対応した AIG を作る．
void
Ge::gen_aig(AigMgr& aigmgr,
	    const vector<Aig>& bvar_array,
	    ymuint bw,
	    vector<Aig>& tmp_list)
{
  vector<Aig> lnode_array(bw);
  lhs()->gen_aig(aigmgr, bvar_array, bw, lnode_array);
  vector<Aig> rnode_array(bw);
  rhs()->gen_aig(aigmgr, bvar_array, bw, rnode_array);
  for (ymuint i = 0; i < bw; ++ i) {
    Aig tmp = aigmgr.make_xnor(lnode_array[i], rnode_array[i]);
    tmp_list.push_back(tmp);
  }
}

END_NAMESPACE_YM_BB
