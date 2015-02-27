
/// @file VsmGen.cc
/// @brief VsmGen の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "VsmGen.h"
#include "IrToplevel.h"
#include "IrFuncBlock.h"
#include "IrHandle.h"
#include "IrNode.h"
#include "VsmNativeFunc.h"
#include "VsmNativeModule.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス VsmGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VsmGen::VsmGen()
{
}

// @brief デストラクタ
VsmGen::~VsmGen()
{
}

// @brief コード生成を行う．
// @param[in] topleve トップレベルのブロック
// @param[in] name 名前
VsmModule*
VsmGen::code_gen(const IrToplevel* toplevel,
		 ShString name)
{
  VsmCodeList::Builder toplevel_builder;

  // トップレベルのコードを作る．
  gen_block(toplevel, toplevel_builder);

  VsmModule::Builder module_builder(name);

  // import しているモジュール
  const vector<VsmModule*>& module_list = toplevel->imported_module_list();
  for (vector<VsmModule*>::const_iterator p = module_list.begin();
       p != module_list.end(); ++ p) {
    VsmModule* sub_module = *p;
    module_builder.add_imported_module(sub_module);
  }

  // 関数のコードを作る．
  const vector<IrFuncBlock*>& func_list = toplevel->func_list();
  ymuint nf = func_list.size();
  for (ymuint i = 0; i < nf; ++ i) {
    VsmCodeList::Builder code_builder;
    IrFuncBlock* func_block = func_list[i];
    gen_block(func_block, code_builder);
    VsmFunction* func = new VsmNativeFunc(func_block->func_handle()->name(), code_builder);
    module_builder.add_exported_function(func);
  }

  VsmModule* module = new VsmNativeModule(module_builder, toplevel_builder);
  return module;
}

// @brief コードブロックに対するコード生成を行う．
// @param[in] code_block コードブロック
// @param[in] builder CodeList ビルダー
void
VsmGen::gen_block(const IrCodeBlock* code_block,
		  VsmCodeList::Builder& builder)
{
  const vector<IrNode*>& node_list = code_block->node_list();
  for (vector<IrNode*>::const_iterator p = node_list.begin();
       p != node_list.end(); ++ p) {
    IrNode* node = *p;
    switch ( node->node_type() ) {
    case IrNode::kUniOp:
    case IrNode::kBinOp:
    case IrNode::kTriOp:
    case IrNode::kLoad:
    case IrNode::kStore:
    case IrNode::kInplaceUniOp:
    case IrNode::kInplaceBinOp:
    case IrNode::kFuncCall:
    case IrNode::kReturn:
    case IrNode::kJump:
    case IrNode::kBranchTrue:
    case IrNode::kBranchFalse:
    case IrNode::kLabel:
    case IrNode::kHalt:
      break;
    }
  }
}

END_NAMESPACE_YM_YMSL
