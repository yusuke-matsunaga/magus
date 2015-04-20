#ifndef MVALCNF_H
#define MVALCNF_H

/// @file MvalCnf.h
/// @brief MvalCnf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "GvalCnf.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MvalCnf MvalCnf.h "MvalCnf.h"
/// @brief 複数の故障検出用CNFを作るクラス
//////////////////////////////////////////////////////////////////////
class MvalCnf
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_node_id ノード番号の最大値
  MvalCnf(ymuint max_node_id);

  /// @brief デストラクタ
  ~MvalCnf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @param[in] max_node_id ノード番号の最大値
  void
  init(ymuint max_node_id);

  /// @brief ノード番号の最大値を返す．
  ymuint
  max_node_id() const;

  /// @brief 正常回路のCNFを生成するクラスを返す．
  GvalCnf&
  gval_cnf();

  /// @brief 正常回路のCNFを生成するクラスを返す．
  const GvalCnf&
  gval_cnf() const;

  /// @brief 正常回路の変数マップを得る．
  const VidMap&
  gvar_map() const;

  /// @brief 故障回路の変数マップを得る．
  const VidMap&
  fvar_map() const;

  /// @brief 伝搬条件の変数マップを得る．
  const VidMap&
  dvar_map() const;

  /// @brief 正常値の変数を得る．
  /// @param[in] node ノード
  VarId
  gvar(const TpgNode* node) const;

  /// @brief 故障値の変数を得る．
  /// @param[in] node ノード
  VarId
  fvar(const TpgNode* node) const;

  /// @brief 伝搬値の変数を得る．
  /// @param[in] node ノード
  VarId
  dvar(const TpgNode* node) const;

  /// @brief 故障に対応した変数を得る．
  /// @param[in] pos 位置
  VarId
  fault_var(ymuint pos) const;

  /// @brief ノードに検出対象の故障があるか調べる．
  bool
  has_faults(const TpgNode* node) const;

  /// @brief 出力の故障に対応した変数を得る．
  /// @param[in] node ノード
  /// @param[in] fval 故障値 (0/1)
  VarId
  ofvar(const TpgNode* node,
	int fval) const;

  /// @brief 入力の故障に対応した変数を得る．
  /// @param[in] node ノード
  /// @param[in] pos 入力番号
  /// @param[in] fval 故障値 (0/1)
  VarId
  ifvar(const TpgNode* node,
	ymuint pos,
	int fval) const;

  /// @brief ノードに正常値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] gvar 正常値の変数番号
  void
  set_gvar(const TpgNode* node,
	   VarId gvar);

  /// @brief ノードに故障値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] fvar 故障値の変数番号
  void
  set_fvar(const TpgNode* node,
	   VarId fvar);

  /// @brief ノードに伝搬値用の変数番号を割り当てる．
  /// @param[in] node ノード
  /// @param[in] dvar 伝搬値の変数番号
  void
  set_dvar(const TpgNode* node,
	   VarId dvar);

  /// @brief 故障数をセットする．
  /// @param[in] num 故障数
  void
  set_fault_num(ymuint num);

  /// @brief 故障に対応した変数をセットする．
  void
  set_fault_var(ymuint pos,
		VarId var);

  /// @brief 出力の故障検出用の変数を割り当てる．
  /// @param[in] node ノード
  /// @param[in] fval 故障値 (0/1)
  /// @param[in] fdvar 変数番号
  void
  set_ofvar(const TpgNode* node,
	    int fval,
	    VarId fdvar);

  /// @brief 入力の故障検出用の変数を割り当てる．
  /// @param[in] node ノード
  /// @param[in] pos 入力番号
  /// @param[in] fval 故障値 (0/1)
  /// @param[in] fdvar 変数番号
  void
  set_ifvar(const TpgNode* node,
	    ymuint pos,
	    int fval,
	    VarId fdvar);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの故障検出用変数マップ
  struct FdMap
  {
    // コンストラクタ
    FdMap(ymuint ni) :
      mNi(ni),
      mIvar(ni * 2)
    {
    }

    // 入力数
    ymuint mNi;

    // 出力の故障に対応する変数番号
    VarId mOvar[2];

    // 入力の故障に対応する変数番号
    // サイズは mNi * 2
    vector<VarId> mIvar;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FdMap を初期化する．
  /// @param[in] node ノード
  FdMap*
  setup_fdmap(const TpgNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノード番号の最大値
  ymuint mMaxId;

  // 正常回路のCNFを作るクラス
  GvalCnf mGvalCnf;

  // 故障値の変数マップ
  GenVidMap mFvarMap;

  // 故障伝搬値の変数マップ
  GenVidMap mDvarMap;

  // 故障検出用の変数マップ配列
  // サイズは mMaxId
  vector<FdMap*> mFdMapArray;

  // 故障に対応した変数の配列
  vector<VarId> mFaultVarArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief ノード番号の最大値を返す．
inline
ymuint
MvalCnf::max_node_id() const
{
  return mMaxId;
}

// @brief 正常回路のCNFを生成するクラスを返す．
inline
GvalCnf&
MvalCnf::gval_cnf()
{
  return mGvalCnf;
}

// @brief 正常回路のCNFを生成するクラスを返す．
inline
const GvalCnf&
MvalCnf::gval_cnf() const
{
  return mGvalCnf;
}

// @brief 正常回路の変数マップを得る．
inline
const VidMap&
MvalCnf::gvar_map() const
{
  return mGvalCnf.var_map();
}

// @brief 故障回路の変数マップを得る．
inline
const VidMap&
MvalCnf::fvar_map() const
{
  return mFvarMap;
}

// @brief 伝搬条件の変数マップを得る．
inline
const VidMap&
MvalCnf::dvar_map() const
{
  return mDvarMap;
}

// @brief 正常値の変数を得る．
// @param[in] node ノード
inline
VarId
MvalCnf::gvar(const TpgNode* node) const
{
  return mGvalCnf.var(node);
}

// @brief 故障値の変数を得る．
// @param[in] node ノード
inline
VarId
MvalCnf::fvar(const TpgNode* node) const
{
  return mFvarMap(node);
}

// @brief 伝搬値の変数を得る．
// @param[in] node ノード
inline
VarId
MvalCnf::dvar(const TpgNode* node) const
{
  return mDvarMap(node);
}

// @brief 故障に対応した変数を得る．
// @param[in] pos 位置
inline
VarId
MvalCnf::fault_var(ymuint pos) const
{
  ASSERT_COND( pos < mFaultVarArray.size() );
  return mFaultVarArray[pos];
}

// @brief ノードに正常値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] gvar 正常値の変数番号
inline
void
MvalCnf::set_gvar(const TpgNode* node,
		  VarId gvar)
{
  mGvalCnf.set_var(node, gvar);
}

// @brief ノードに故障値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] fvar 故障値の変数番号
inline
void
MvalCnf::set_fvar(const TpgNode* node,
		  VarId fvar)
{
  mFvarMap.set_vid(node, fvar);
}

// @brief ノードに伝搬値用の変数番号を割り当てる．
// @param[in] node ノード
// @param[in] dvar 伝搬値の変数番号
inline
void
MvalCnf::set_dvar(const TpgNode* node,
		  VarId dvar)
{
  mDvarMap.set_vid(node, dvar);
}

// @brief 故障数をセットする．
// @param[in] num 故障数
inline
void
MvalCnf::set_fault_num(ymuint num)
{
  mFaultVarArray.clear();
  mFaultVarArray.resize(num);
}

// @brief 故障に対応した変数をセットする．
inline
void
MvalCnf::set_fault_var(ymuint pos,
		       VarId var)
{
  ASSERT_COND( pos < mFaultVarArray.size() );
  mFaultVarArray[pos] = var;
}

END_NAMESPACE_YM_SATPG

#endif // MVALCNF_H
