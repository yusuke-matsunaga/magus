
/// @file AstImport.cc
/// @brief AstImport の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstImport.h"
#include "AstModule.h"
#include "AstList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstImport
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module_list モジュールのリスト
// @param[in] loc ファイル位置
AstImport::AstImport(AstModuleList* module_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mModuleList(module_list->size())
{
  ymuint pos = 0;
  for (AstModuleList::Iterator p = module_list->begin();
       !p.is_end(); p.next()) {
    mModuleList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstImport::~AstImport()
{
}

// @brief 種類を返す．
StmtType
AstImport::stmt_type() const
{
  return kImport;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstImport::phase1(YmslScope* parent_scope,
		  YmslTypeMgr* type_mgr)
{
  // ここで import 処理を行う．
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstImport::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstImport::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstImport::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstImport::print(ostream& s,
		 ymuint indent) const
{
  print_indent(s, indent);
  s << "import ";
  ymuint n = mModuleList.size();
  const char* comma = "";
  for (ymuint i = 0; i < n; ++ i) {
    AstModule* module = mModuleList[i];
    s << comma << module->module_name()->str_val();
    if ( module->alias_name() != NULL ) {
      s << " as " << module->alias_name()->str_val();
    }
    comma = ", ";
  }
  s << ";" << endl;
}
#endif

END_NAMESPACE_YM_YMSL
