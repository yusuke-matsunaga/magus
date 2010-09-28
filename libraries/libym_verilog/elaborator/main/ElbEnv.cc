
/// @file libym_verilog/elaborator/ElbEnv.cc
/// @brief ElbEnv の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ElbEnv.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbEnv.h"


BEGIN_NAMESPACE_YM_VERILOG

BEGIN_NONAMESPACE

const int SFT_CONSTANT = 0;
const int SFT_FUNCTION = 1;
const int SFT_CF = 2;
const int SFT_TASK = 3;
const int SFT_ARG = 4;
const int SFT_EVENT = 5;
const int SFT_NET = 6;
const int SFT_VAR = 7;
const int SFT_PCA = 8;
const int SFT_FORCE = 9;

const ymuint32 MASK_CONSTANT = 1U << SFT_CONSTANT;
const ymuint32 MASK_FUNCTION = 1U << SFT_FUNCTION;
const ymuint32 MASK_CF       = 1U << SFT_CF;
const ymuint32 MASK_TASK     = 1U << SFT_TASK;
const ymuint32 MASK_ARG      = 1U << SFT_ARG;
const ymuint32 MASK_EVENT    = 1U << SFT_EVENT;
const ymuint32 MASK_NET      = 1U << SFT_NET;
const ymuint32 MASK_VAR      = 1U << SFT_VAR;
const ymuint32 MASK_PCA      = 1U << SFT_PCA;
const ymuint32 MASK_FORCE    = 1U << SFT_FORCE;

END_NONAMESPACE


// @brief コンストラクタ
ElbEnv::ElbEnv() :
  mFlags(0U),
  mTf(NULL)
{
}

// @brief コピーコンストラクタ
ElbEnv::ElbEnv(const ElbEnv& src) :
  mFlags(src.mFlags),
  mTf(src.mTf)
{
}

// @brief 代入演算子
const ElbEnv&
ElbEnv::operator=(const ElbEnv& src)
{
  mFlags = src.mFlags;
  mTf = src.mTf;
  return *this;
}

// @brief デストラクタ
ElbEnv::~ElbEnv()
{
}

// @brief 定数式を要求する．
void
ElbEnv::set_constant()
{
  mFlags |= MASK_CONSTANT;
}

// @brief 親の function を設定する．
// @param[in] function 設定する function
// @param[in] cf constant function の時 true にするフラグ
void
ElbEnv::set_function(const VlNamedObj* function,
		     bool cf)
{
  if ( cf ) {
    mFlags |= MASK_CF;
  }
  mFlags |= MASK_FUNCTION;
  mTf = function;
}

// @brief 親の task を設定する．
void
ElbEnv::set_task(const VlNamedObj* task)
{
  mFlags |= MASK_TASK;
  mTf = task;
}

// @brief system task/system function の引数の印をつける．
void
ElbEnv::set_system_tf_arg()
{
  mFlags |= MASK_ARG;
}

// @brief イベント式の印をつける．
void
ElbEnv::set_event_expr()
{
  mFlags |= MASK_EVENT;
}

// @brief net 型の左辺式の印をつける．
void
ElbEnv::set_net_lhs()
{
  mFlags |= MASK_NET;
}

// @brief reg/var 型の左辺式の印をつける．
void
ElbEnv::set_var_lhs()
{
  mFlags |= MASK_VAR;
}

// @brief pca 代入文の左辺式の印をつける．
void
ElbEnv::set_pca_lhs()
{
  mFlags |= MASK_PCA;
}

// @brief force 代入文の左辺式の印をつける．
void
ElbEnv::set_force_lhs()
{
  mFlags |= MASK_FORCE;
}

// @brief 定数式が要求されている時に true を返す．
bool
ElbEnv::is_constant() const
{
  return static_cast<bool>((mFlags >> SFT_CONSTANT) & 1U);
}

// @brief function 内の生成の時に親の function を返す．
const VlNamedObj*
ElbEnv::function() const
{
  if ( inside_function() ) {
    return mTf;
  }
  return NULL;
}

// @brief function 内の生成時に true を返す．
bool
ElbEnv::inside_function() const
{
  return static_cast<bool>((mFlags >> SFT_FUNCTION) & 1U);
}

// @brief constant function 内の生成の時に true を返す．
bool
ElbEnv::inside_constant_function() const
{
  return static_cast<bool>((mFlags >> SFT_CF) & 1U);
}

// @brief task 内の生成の時に親の function を返す．
const VlNamedObj*
ElbEnv::task() const
{
  if ( inside_task() ) {
    return mTf;
  }
  return NULL;
}

// @brief task 内の生成時に true を返す．
bool
ElbEnv::inside_task() const
{
  return static_cast<bool>((mFlags >> SFT_TASK) & 1U);
}

// @brief system task/system function の引数の時に true を返す．
bool
ElbEnv::is_system_tf_arg() const
{
  return static_cast<bool>((mFlags >> SFT_ARG) & 1U);
}

// @brief イベント式の時に true を返す．
bool
ElbEnv::is_event_expr() const
{
  return static_cast<bool>((mFlags >> SFT_EVENT) & 1U);
}

// @brief net 型の左辺式の時に true を返す．
bool
ElbEnv::is_net_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_NET) & 1U);
}

// @brief reg/var 型の左辺式の時に true を返す．
bool
ElbEnv::is_var_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_VAR) & 1U);
}

// @brief pca 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_pca_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_PCA) & 1U);
}

// @brief force 代入文の左辺式の時に true を返す．
bool
ElbEnv::is_force_lhs() const
{
  return static_cast<bool>((mFlags >> SFT_FORCE) & 1U);
}

END_NAMESPACE_YM_VERILOG
