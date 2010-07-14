
/// @file libym_mvn/verilog_reader/MvVerilogReader.cc
/// @brief MvVerilogReader の実装クラス
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_mvn/MvVerilogReader.h"
#include "ym_mvn/MvMgr.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_MVN

// @brief コンストラクタ
MvVerilogReader::MvVerilogReader() :
  mVlMgr(mMsgMgr)
{
}

// @brief デストラクタ
MvVerilogReader::~MvVerilogReader()
{
}

// @brief 内部のデータをクリアする．
void
MvVerilogReader::clear()
{
  mVlMgr.clear();
}

// @brief verilog 形式のファイルを読み込む．
// @param[in] filename ファイル名
// @param[in] searchpath サーチパス
// @param[in] watcher_list 行番号ウォッチャーのリスト
// @retval true 正常に読み込めた．
// @retval false 読込中にエラーが起こった．
bool
MvVerilogReader::read(const string& filename,
		      const SearchPathList& searchpath,
		      const list<VlLineWatcher*> watcher_list)
{
  return mVlMgr.read_file(filename, searchpath, watcher_list);
}

// @brief 今まで読み込んだ情報からネットワークを生成する．
// @param[in] mgr ネットワーク生成用のマネージャ
// @retval true 正常に処理を行った．
// @retval false 生成中にエラーが起こった．
bool
MvVerilogReader::gen_network(MvMgr& mgr)
{
  using namespace nsVerilog;
  
  list<const VlModule*> tmp_list(mVlMgr.topmodule_list());
  for (list<const VlModule*>::const_iterator p = tmp_list.begin();
       p != tmp_list.end(); ++ p) {
    const VlModule* module = *p;

    // module を実体化
    gen_module(mgr, module);
    
    // module 中の module_array_instance を tmp_list に積む．
    vector<const VlModuleArray*> modulearray_list;
    if ( mVlMgr.find_modulearray_list(module, modulearray_list) ) {
      for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	   p != modulearray_list.end(); ++ p) {
	const VlModuleArray* module_array = *p;
	ymuint32 n = module_array->elem_num();
	for (ymuint32 i = 0; i < n; ++ i) {
	  const VlModule* module1 = module_array->elem_by_offset(i);
	  tmp_list.push_back(module1);
	}
      }
    }
  }

  return true;
}

// @brief メッセージハンドラを付加する．
void
MvVerilogReader::add_msg_handler(MsgHandler* msg_handler)
{
  mMsgMgr.reg_handler(msg_handler);
}

// @brief module を生成する．
// @param[in] mgr MvMgr
// @param[in] vl_module 対象のモジュール
MvModule*
MvVerilogReader::gen_module(MvMgr& mgr,
			    const nsVerilog::VlModule* vl_module)
{
  using namespace nsVerilog;
  
  // ポート数，入出力のビット幅を調べる．
  ymuint np = vl_module->port_num();
  ymuint nio = vl_module->io_num();
  ymuint ni = 0;
  ymuint no = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ++ ni; break;
    case kVpiOutput: ++ no; break;
    default:
      mMsgMgr.put_msg(__FILE__, __LINE__,
		      io->file_region(),
		      kMsgError,
		      "MVN_VL01",
		      "Only Input/Output types are supported");
      return NULL;
    }
  }
  vector<ymuint> ibw_array(ni);
  vector<ymuint> obw_array(no);
  ni = 0;
  no = 0;
  for (ymuint i = 0; i < nio; ++ i) {
    const VlIODecl* io = vl_module->io(i);
    switch ( io->direction() ) {
    case kVpiInput:  ibw_array[ni] = io->bit_size(); ++ ni; break;
    case kVpiOutput: obw_array[no] = io->bit_size(); ++ no; break;
    default: break;
    }
  }
  
  MvModule* module = mgr.new_module(vl_module->name(), np, ibw_array, obw_array);

  // モジュールインスタンスの生成
  vector<const VlModule*> module_list;
  if ( mVlMgr.find_module_list(vl_module, module_list) ) {
    for (vector<const VlModule*>::iterator p = module_list.begin();
	 p != module_list.end(); ++ p) {
      const VlModule* vl_module1 = *p;
      MvModule* module1 = gen_module(mgr, vl_module1);
      if ( module1 == NULL ) {
	return NULL;
      }
      MvNode* node = mgr.new_inst(module, module1);
    }
  }

  // モジュール配列インスタンスの生成
  vector<const VlModuleArray*> modulearray_list;
  if ( mVlMgr.find_modulearray_list(vl_module, modulearray_list) ) {
    for (vector<const VlModuleArray*>::iterator p = modulearray_list.begin();
	 p != modulearray_list.end(); ++ p) {
      const VlModuleArray* vl_module_array = *p;
      ymuint n = vl_module_array->elem_num();
      for (ymuint i = 0; i < n; ++ i) {
	const VlModule* vl_module1 = vl_module_array->elem_by_offset(i);
	MvModule* module1 = gen_module(mgr, vl_module1);
	if ( module1 == NULL ) {
	  return NULL;
	}
	MvNode* node1 = mgr.new_inst(module, module1);
      }
    }
  }

  // プリミティブインスタンスの生成
  vector<const VlPrimitive*> primitive_list;
  if ( mVlMgr.find_primitive_list(vl_module, primitive_list) ) {
    for (vector<const VlPrimitive*>::iterator p = primitive_list.begin();
	 p != primitive_list.end(); ++ p) {
      const VlPrimitive* vl_prim = *p;
    }
  }
  // プリミティブ配列インスタンスの生成
  vector<const VlPrimArray*> primarray_list;
  if ( mVlMgr.find_primarray_list(vl_module, primarray_list) ) {
    for (vector<const VlPrimArray*>::iterator p = primarray_list.begin();
	 p != primarray_list.end(); ++ p) {
      const VlPrimArray* vl_primarray = *p;
      ymuint n = vl_primarray->elem_num();
      for (ymuint i = 0; i < n; ++ i) {
	const VlPrimitive* vl_prim = vl_primarray->elem_by_offset(i);
      }
    }
  }

  // ネットの生成
  vector<const VlDecl*> net_list;
  if ( mVlMgr.find_decl_list(vl_module, vpiNet, net_list) ) {
    for (vector<const VlDecl*>::iterator p = net_list.begin();
	 p != net_list.end(); ++ p) {
      const VlDecl* vl_decl = *p;
      // 仮の through ノードに対応させる．
      ymuint bw = vl_decl->bit_size();
      MvNode* node = mgr.new_through(module, bw);
      
    }
  }

  // ネット配列の生成
  vector<const VlDecl*> netarray_list;
  if ( mVlMgr.find_decl_list(vl_module, vpiNetArray, netarray_list) ) {
    for (vector<const VlDecl*>::iterator p = netarray_list.begin();
	 p != netarray_list.end(); ++ p) {
      const VlDecl* vl_decl = *p;
      ymuint d = vl_decl->dimension();
#if 0
      // 仮の through ノードに対応させる．
      ymuint bw = vl_decl->bit_size();
      MvNode* node = mgr.new_through(bw);
#endif 
    }
  }

  // 継続的代入文の生成
  vector<const VlContAssign*> contassign_list;
  if ( mVlMgr.find_contassign_list(vl_module, contassign_list) ) {
    for (vector<const VlContAssign*>::iterator p = contassign_list.begin();
	 p != contassign_list.end(); ++ p) {
      const VlContAssign* vl_contassign = *p;
      
    }
  }
  
}

END_NAMESPACE_YM_MVN
