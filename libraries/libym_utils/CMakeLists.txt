
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  )

if (ZLIB_FOUND)
  include_directories(${ZLIBINCLUDE_DIRS})
endif (ZLIB_FOUND)

if (BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIRS})
endif (BZIP2_FOUND)

if (LIBLZMA_FOUND)
  include_directories(${LIBLZMA_INCLUDE_DIRS})
endif (LIBLZMA_FOUND)

if (POPT_FOUND)
  include_directories(${POPT_INCLUDE_DIR})
endif (POPT_FOUND)


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  src/alloc/Alloc.cc
  src/alloc/FragAlloc.cc
  src/alloc/SimpleAlloc.cc
  src/alloc/UnitAlloc.cc
  )

set (SOURCES
  ${SOURCES}
  src/btg/BtgHeapTree.cc
  src/btg/BtgMatch.cc
  src/btg/BtgMatchImpl.cc
  )

set (SOURCES
  ${SOURCES}
  src/file/File.cc
  src/file/FileInfo.cc
  src/file/FileInfoMgr.cc
  )

set (SOURCES
  ${SOURCES}
  src/gcsolver/GcSolver.cc
  )

set (SOURCES
  ${SOURCES}
  src/gen/CombiGen.cc
  src/gen/GenBase.cc
  src/gen/MultiCombiGen.cc
  src/gen/MultiGenBase.cc
  src/gen/MultiPermGen.cc
  src/gen/MultiSetCombiGen.cc
  src/gen/MultiSetGenBase.cc
  src/gen/MultiSetPermGen.cc
  src/gen/PermGen.cc
  )

set (SOURCES
  ${SOURCES}
  src/io/FileBuff.cc
  src/io/FileBuff.h
  src/io/FileCoder.cc
  src/io/FileDecoder.cc
  src/io/FileIDO.cc
  src/io/FileODO.cc
  src/io/IDO.cc
  src/io/ODO.cc
  src/io/ThruCoder.cc
  src/io/ThruDecoder.cc
  src/io/ZCoder.cc
  src/io/ZDecoder.cc
  )

if (ZLIB_FOUND)
  set (SOURCES
    ${SOURCES}
    src/io/GzCoder.cc
    src/io/GzDecoder.cc
    src/io/zstream.cc
    )
endif (ZLIB_FOUND)

if (BZIP2_FOUND)
  set (SOURCES
    ${SOURCES}
    src/io/BzCoder.cc
    src/io/BzDecoder.cc
    )
endif (BZIP2_FOUND)

if (LIBLZMA_FOUND)
  set (SOURCES
    ${SOURCES}
    src/io/LzmaCoder.cc
    src/io/LzmaDecoder.cc
    )
endif (LIBLZMA_FOUND)

set (SOURCES
  ${SOURCES}
  src/itvl/ItvlMgr.cc
  src/itvl/ItvlMgrImpl.cc
  src/itvl/NameMgr.cc
  )

set (SOURCES
  ${SOURCES}
  src/mfset/MFSet.cc
  )

set (SOURCES
  ${SOURCES}
  src/mincov/LbCS.cc
  src/mincov/LbMAX.cc
  src/mincov/LbMIS1.cc
  src/mincov/LbMIS2.cc
  src/mincov/MaxClique.cc
  src/mincov/McMatrix.cc
  src/mincov/McSolver.cc
  src/mincov/McSolverImpl.cc
  src/mincov/MinCov.cc
  src/mincov/MisNodeHeap.cc
  src/mincov/SelCS.cc
  src/mincov/SelNaive.cc
  src/mincov/SelSimple.cc
  )

set (SOURCES
  ${SOURCES}
  src/misc/Binder.cc
  src/misc/DlList.cc
  src/misc/RandGen.cc
  src/misc/RefCount.cc
  src/misc/Scanner.cc
  src/misc/StopWatch.cc
  )

set (SOURCES
  ${SOURCES}
  src/msg/MsgHandler.cc
  src/msg/MsgMgr.cc
  src/msg/MsgMgrImpl.cc
  )

if (POPT_FOUND)
  set (SOURCES
    ${SOURCES}
    src/popt/PoptMainApp.cc
    )
endif (POPT_FOUND)

set (SOURCES
  ${SOURCES}
  src/str/StrBuff.cc
  src/str/StrPool.cc
  src/str/StrPool.h
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
#add_library(ym_utils SHARED
#  src/alloc/Alloc.cc
#  src/alloc/FragAlloc.cc
#  src/alloc/SimpleAlloc.cc
#  src/alloc/UnitAlloc.cc
#  src/btg/BtgEdge.h
#  src/btg/BtgHeapTree.cc
#  src/btg/BtgHeapTree.h
#  src/btg/BtgMatch.cc
#  src/btg/BtgMatchImpl.cc
#  src/btg/BtgMatchImpl.h
#  src/btg/BtgNode.h
#  src/btg/btg_nsdef.h
#  src/file/File.cc
#  src/file/FileInfo.cc
#  src/file/FileInfoMgr.cc
#  src/file/FileInfoMgr.h
#  src/gcsolver/GcNode.h
#  src/gcsolver/GcSolver.cc
#  src/gen/CombiGen.cc
#  src/gen/GenBase.cc
#  src/gen/MultiCombiGen.cc
#  src/gen/MultiGenBase.cc
#  src/gen/MultiPermGen.cc
#  src/gen/MultiSetCombiGen.cc
#  src/gen/MultiSetGenBase.cc
#  src/gen/MultiSetPermGen.cc
#  src/gen/PermGen.cc
#  src/io/BzCoder.cc
#  src/io/BzCoder.h
#  src/io/BzDecoder.cc
#  src/io/BzDecoder.h
#  src/io/BzEngine.h
#  src/io/FileBuff.cc
#  src/io/FileBuff.h
#  src/io/FileCoder.cc
#  src/io/FileCoder.h
#  src/io/FileDecoder.cc
#  src/io/FileDecoder.h
#  src/io/FileIDO.cc
#  src/io/FileODO.cc
#  src/io/GzCoder.cc
#  src/io/GzCoder.h
#  src/io/GzDecoder.cc
#  src/io/GzDecoder.h
#  src/io/IDO.cc
#  src/io/LzmaCoder.cc
#  src/io/LzmaCoder.h
#  src/io/LzmaDecoder.cc
#  src/io/LzmaDecoder.h
#  src/io/LzmaEngine.h
#  src/io/ODO.cc
#  src/io/ThruCoder.cc
#  src/io/ThruCoder.h
#  src/io/ThruDecoder.cc
#  src/io/ThruDecoder.h
#  src/io/ZCoder.cc
#  src/io/ZCoder.h
#  src/io/ZDecoder.cc
#  src/io/ZDecoder.h
#  src/io/compress_common.h
#  src/io/gz_common.h
#  src/io/zstream.cc
#  src/io/zstream.h
#  src/itvl/ItvlCell.h
#  src/itvl/ItvlMgr.cc
#  src/itvl/ItvlMgrImpl.cc
#  src/itvl/ItvlMgrImpl.h
#  src/itvl/NameMgr.cc
#  src/mfset/MFSet.cc
#  src/mincov/LbCS.cc
#  src/mincov/LbCS.h
#  src/mincov/LbCalc.h
#  src/mincov/LbMAX.cc
#  src/mincov/LbMAX.h
#  src/mincov/LbMIS1.cc
#  src/mincov/LbMIS1.h
#  src/mincov/LbMIS2.cc
#  src/mincov/LbMIS2.h
#  src/mincov/MaxClique.cc
#  src/mincov/MaxClique.h
#  src/mincov/McCell.h
#  src/mincov/McColHead.h
#  src/mincov/McMatrix.cc
#  src/mincov/McMatrix.h
#  src/mincov/McRowHead.h
#  src/mincov/McSolver.cc
#  src/mincov/McSolver.h
#  src/mincov/McSolverImpl.cc
#  src/mincov/McSolverImpl.h
#  src/mincov/MinCov.cc
#  src/mincov/MisNode.h
#  src/mincov/MisNodeHeap.cc
#  src/mincov/MisNodeHeap.h
#  src/mincov/SelCS.cc
#  src/mincov/SelCS.h
#  src/mincov/SelNaive.cc
#  src/mincov/SelNaive.h
#  src/mincov/SelSimple.cc
#  src/mincov/SelSimple.h
#  src/mincov/Selector.h
#  src/mincov/mincov_nsdef.h
#  src/misc/Binder.cc
#  src/misc/DlList.cc
#  src/misc/RandGen.cc
#  src/misc/RefCount.cc
#  src/misc/Scanner.cc
#  src/misc/StopWatch.cc
#  src/msg/MsgHandler.cc
#  src/msg/MsgMgr.cc
#  src/msg/MsgMgrImpl.cc
#  src/msg/MsgMgrImpl.h
#  src/str/StrBuff.cc
#  src/str/StrPool.cc
#  src/str/StrPool.h
#  )

add_library(ym_utils SHARED
  ${SOURCES}
  )

if (ZLIB_FOUND)
  target_link_libraries(ym_utils
    ${ZLIB_LIBRARIES}
    )
endif (ZLIB_FOUND)

if (BZIP2_FOUND)
  target_link_libraries(ym_utils
    ${BZIP2_LIBRARIES}
    )
endif (BZIP2_FOUND)

if (LIBLZMA_FOUND)
  target_link_libraries(ym_utils
    ${LIBLZMA_LIBRARIES}
    )
endif (LIBLZMA_FOUND)

if (POPT_FOUND)
  target_link_libraries(ym_utils
    ${POPT_LIBRARY}
    )
endif (POPT_FOUND)