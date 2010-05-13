
/// @file libym_verilog/elb_impl/EiIODecl.cc
/// @brief EiIODecl の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: EiIODecl.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "EiFactory.h"
#include "EiIODecl.h"
#include "ElbModule.h"
#include "ElbTaskFunc.h"
#include "ElbDecl.h"

#include <ym_verilog/pt/PtDecl.h>



BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// EiFactory の生成関数
//////////////////////////////////////////////////////////////////////

// @brief module IO ヘッダを生成する．
// @param[in] module 親のモジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_ModIOHead(ElbModule* module,
			 const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiModIOHead));
  EiIOHead* head = new (p) EiModIOHead(module, pt_header);
  return head;
}

// @brief task 用の IO ヘッダを生成する．
// @param[in] task 親の task
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_IOHead(ElbTask* task,
		      const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiTaskIOHead));
  EiIOHead* head = new (p) EiTaskIOHead(task, pt_header);
  return head;
}

// @brief function 用の IO ヘッダを生成する．
// @param[in] func 親の function
// @param[in] pt_header パース木のIO宣言ヘッダ
ElbIOHead*
EiFactory::new_IOHead(ElbFunction* func,
		      const PtIOHead* pt_header)
{
  void* p = mAlloc.get_memory(sizeof(EiFuncIOHead));
  EiIOHead* head = new (p) EiFuncIOHead(func, pt_header);
  return head;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] pt_header パース木のIO宣言ヘッダ
EiIOHead::EiIOHead(const PtIOHead* pt_header) :
  mPtHead(pt_header)
{
}
  
// @brief デストラクタ
EiIOHead::~EiIOHead()
{
}

// @brief 方向を返す．
tVpiDirection
EiIOHead::direction() const
{
  switch ( mPtHead->type() ) {
  case kPtIO_Input:  return kVpiInput;
  case kPtIO_Output: return kVpiOutput;
  case kPtIO_Inout:  return kVpiInout;
  default: assert_not_reached(__FILE__, __LINE__);
  }
  // ダミー
  return kVpiNoDirection;
}

// @brief 親のモジュールの取得
ElbModule*
EiIOHead::module() const
{
  return NULL;
}

// @brief 親の task の取得
ElbTask*
EiIOHead::task() const
{
  return NULL;
}

// @brief 親の function の取得
ElbFunction*
EiIOHead::function() const
{
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス EiModIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module 親のモジュール
// @param[in] pt_header パース木のIO宣言ヘッダ
EiModIOHead::EiModIOHead(ElbModule* module,
			 const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mModule(module)
{
}
  
// @brief デストラクタ
EiModIOHead::~EiModIOHead()
{
}

// @brief 親のモジュールの取得
ElbModule*
EiModIOHead::module() const
{
  return mModule;
}


//////////////////////////////////////////////////////////////////////
// クラス EiTaskIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] task 親の task
// @param[in] pt_header パース木のIO宣言ヘッダ
EiTaskIOHead::EiTaskIOHead(ElbTask* task,
			   const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mTask(task)
{
}
  
// @brief デストラクタ
EiTaskIOHead::~EiTaskIOHead()
{
}

// @brief 親の task の取得
ElbTask*
EiTaskIOHead::task() const
{
  return mTask;
}


//////////////////////////////////////////////////////////////////////
// クラス EiFuncIOHead
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 親の function
// @param[in] pt_header パース木のIO宣言ヘッダ
EiFuncIOHead::EiFuncIOHead(ElbFunction* func,
			   const PtIOHead* pt_header) :
  EiIOHead(pt_header),
  mFunction(func)
{
}
  
// @brief デストラクタ
EiFuncIOHead::~EiFuncIOHead()
{
}

// @brief 親の function の取得
ElbFunction*
EiFuncIOHead::function() const
{
  return mFunction;
}


//////////////////////////////////////////////////////////////////////
// クラス EiIODecl
//////////////////////////////////////////////////////////////////////
  
// @brief コンストラクタ
EiIODecl::EiIODecl()
{
}
  
// @brief デストラクタ
EiIODecl::~EiIODecl()
{
}

// @brief 初期化を行う．
// @param[in] head ヘッダ
// @param[in] pt_item パース木のIO宣言要素
// @param[in] decl 対応する宣言要素
void
EiIODecl::init(ElbIOHead* head,
	       const PtIOItem* pt_item,
	       ElbDecl* decl)
{
  mHead = head;
  mPtItem = pt_item;
  mDecl = decl;
}

// @brief 型の取得
tVpiObjType
EiIODecl::type() const
{
  return kVpiIODecl;
}

// @brief ファイル位置の取得
FileRegion
EiIODecl::file_region() const
{
  return mPtItem->file_region();
}

// @brief 名前を返す．
const char*
EiIODecl::name() const
{
  return mPtItem->name();
}

// @brief 方向を返す．
tVpiDirection
EiIODecl::direction() const
{
  return mHead->direction();
}

// @brief 符号の取得
// @retval true 符号つき
// @retval false 符号なし
bool
EiIODecl::is_signed() const
{
  return mDecl->is_signed();
}
  
// @brief 範囲のMSBの取得
// @retval 範囲のMSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
EiIODecl::left_range() const
{
  return mDecl->left_range();
}

// @brief 範囲のLSBの取得
// @retval 範囲のLSB 範囲を持つとき
// @retval NULL 範囲を持たないとき
const VlExpr*
EiIODecl::right_range() const
{
  return mDecl->right_range();
}

// @brief ビット幅を返す．
ymuint32
EiIODecl::bit_size() const
{
  return mDecl->bit_size();
}

// @brief 対応する宣言要素を返す．
const VlDecl*
EiIODecl::decl() const
{
  return mDecl;
}

// @brief 親のモジュールの取得
const VlModule*
EiIODecl::module() const
{
  return mHead->module();
}

// @brief 親の UDP の取得
// @note このクラスでは NULL を返す．
const VlUdpDefn*
EiIODecl::udp_defn() const
{
  return NULL;
}

// @brief 親の task の取得
// @return このクラスは NULL を返す．
const VlTask*
EiIODecl::task() const
{
  return mHead->task();
}

// @brief 親の function の取得
// @return このクラスは NULL を返す．
const VlFunction*
EiIODecl::function() const
{
  return mHead->function();
}
  
// @brief 対応する ElbDecl を返す．
ElbDecl*
EiIODecl::_decl() const
{
  return mDecl;
}

END_NAMESPACE_YM_VERILOG
