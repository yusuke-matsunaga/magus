#ifndef LIBYM_VERILOG_ELABORATOR_ELB_MGR_TAGDICTCELL_H
#define LIBYM_VERILOG_ELABORATOR_ELB_MGR_TAGDICTCELL_H

/// @file libym_verilog/elaborator/elb_mgr/TagDictCell.h
/// @brief TagDictCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"
#include "ym_utils/Alloc.h"
#include "ElbFwd.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class TagDictCell TagDictCell.h "TagDictCell.h"
/// @brief TagDict 用のセル
//////////////////////////////////////////////////////////////////////
class TagDictCell
{
  friend class TagDict;

private:

  /// @brief  宣言要素を追加する．
  virtual
  void
  add_decl(ElbDecl* obj);

  /// @brief  宣言要素の先頭を得る．
  virtual
  const ElbDecl*
  decl();

  /// @brief  宣言要素を追加する．
  virtual
  void
  add_declarray(ElbDeclArray* obj);

  /// @brief  宣言要素の先頭を得る．
  virtual
  const ElbDeclArray*
  declarray();

  /// @brief  パラメータを追加する．
  virtual
  void
  add_parameter(ElbParameter* obj);

  /// @brief  パラメータの先頭を得る．
  virtual
  const ElbParameter*
  parameter();

  /// @brief  defparam を追加する．
  virtual
  void
  add_defparam(ElbDefParam* obj);

  /// @brief  defparam の先頭を得る．
  virtual
  const ElbDefParam*
  defparam();

  /// @brief  param assign を追加する．
  virtual
  void
  add_paramassign(ElbParamAssign* obj);

  /// @brief  param assign の先頭を得る．
  virtual
  const ElbParamAssign*
  paramassign();

  /// @brief module array を追加する．
  virtual
  void
  add_modulearray(ElbModuleArray* obj);

  /// @brief module array の先頭を得る．
  virtual
  const ElbModuleArray*
  modulearray();

  /// @brief  module を追加する．
  virtual
  void
  add_module(ElbModule* obj);

  /// @brief  module の先頭を得る．
  virtual
  const ElbModule*
  module();

  /// @brief  primitive array を追加する．
  virtual
  void
  add_primarray(ElbPrimArray* obj);

  /// @brief  primitive array の先頭を得る．
  virtual
  const ElbPrimArray*
  primarray();

  /// @brief  primitive を追加する．
  virtual
  void
  add_primitive(ElbPrimitive* obj);

  /// @brief  primitive の先頭を得る．
  virtual
  const ElbPrimitive*
  primitive();

  /// @brief タスクを追加する．
  virtual
  void
  add_task(ElbTaskFunc* obj);

  /// @brief タスクの先頭を得る．
  virtual
  const ElbTaskFunc*
  task();

  /// @brief 関数を追加する．
  virtual
  void
  add_function(ElbTaskFunc* obj);

  /// @brief 関数の先頭を得る．
  virtual
  const ElbTaskFunc*
  function();

  /// @brief continuous assignment を追加する．
  virtual
  void
  add_contassign(ElbContAssign* obj);

  /// @brief  continuous assignment の先頭を得る．
  virtual
  const ElbContAssign*
  contassign();

  /// @brief  process を追加する．
  virtual
  void
  add_process(ElbProcess* process);

  /// @brief  process の先頭を得る．
  virtual
  const ElbProcess*
  process();

  /// @brief internal scope を追加する．
  virtual
  void
  add_internalscope(ElbScope* obj);

  /// @brief internal scope の先頭を得る．
  virtual
  const ElbScope*
  internalscope();

  /// @brief  要素数を得る．
  virtual
  ymuint
  num() = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のスコープ
  const VlNamedObj* mParent;

  // タグの値
  int mTag;

  // ハッシュ上の次の要素を指すポインタ
  TagDictCell* mLink;

};

END_NAMESPACE_YM_VERILOG

#endif // LIBYM_VERILOG_ELABORATOR_ELB_MGR_TAGDICTCELL_H
