
/// @file libym_verilog/elaborator/elb_mgr/ElbFactory.cc
/// @brief ElbFactory の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ElbFactory.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
ElbFactory::ElbFactory()
{
}

// @brief デストラクタ
ElbFactory::~ElbFactory()
{
}

// @brief 宣言要素のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
// @param[in] has_delay 遅延値を持つとき true
ElbDeclHead*
ElbFactory::new_DeclHead(const VlNamedObj* parent,
			 const PtDeclHead* pt_head,
			 bool has_delay)
{
  return new_DeclHead(parent, pt_head, NULL, NULL, 0, 0, has_delay);
}

// @brief 宣言要素のヘッダを生成する．(IODecl 中の宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木のIO宣言ヘッダ
// @param[in] aux_type 補助的なデータ型
ElbDeclHead*
ElbFactory::new_DeclHead(const VlNamedObj* parent,
			 const PtIOHead* pt_head,
			 tVpiAuxType aux_type)
{
  return new_DeclHead(parent, pt_head, aux_type, NULL, NULL, 0, 0);
}

// @brief 宣言要素のヘッダを生成する．(function の暗黙宣言用)
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の関数定義
ElbDeclHead*
ElbFactory::new_DeclHead(const VlNamedObj* parent,
			 const PtItem* pt_item)
{
  return new_DeclHead(parent, pt_item, NULL, NULL, 0, 0);
}

// @brief parameter 宣言のヘッダを生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head パース木の宣言ヘッダ
ElbParamHead*
ElbFactory::new_ParamHead(const VlNamedObj* parent,
			  const PtDeclHead* pt_head)
{
  return new_ParamHead(parent, pt_head);
}

// @brief function を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_item パース木の定義
ElbFunction*
ElbFactory::new_Function(const VlNamedObj* parent,
			 const PtItem* pt_item)
{
  return new_Function(parent, pt_item, NULL, NULL, 0, 0);
}

END_NAMESPACE_YM_VERILOG
