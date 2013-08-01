# coding=UTF-8

import sys
import cell_lib


#delay_model = cell_lib.kCellDelayGenericCmos

def print_pin(pin) :
  dir = pin.direction()
  print ''
  print '      Pin: {}'.format(pin.name())
  print '        Direction: {}'.format(dir)
  if dir.is_input() or dir.is_inout() :
    print '        input ID: {}'.format(pin.input_id())
    print '        capacitance: {}'.format(str(pin.capacitance()))
    print '        rise capacitance: {}'.format(str(pin.rise_capacitance()))
    print '        fall capacitance: {}'.format(str(pin.fall_capacitance()))
  elif dir.is_output() or dir.is_inout() :
    print '        output ID: {}'.format(pin.output_id())
    if pin.has_function() :
      print '        function: {}'.format(str(pin.function()))
    if pin.has_three_state() :
      print '        three state: {}'.format(str(pin.three_state()))
    print '        max fanout: {}'.format(str(pin.max_fanout()))
    print '        min fanout: {}'.format(str(pin.min_fanout()))
    print '        max capacitance: {}'.format(str(pin.max_capacitance()))
    print '        min capacitance: {}'.format(str(pin.min_capacitance()))
    print '        max transition: {}'.format(str(pin.max_transition()))
    print '        min transition: {}'.format(str(pin.min_transition()))
  elif dir.is_internal() :
    print '        internal ID: {}'.format(pin.internal_id())

def print_lut(lut) :
  d = lut.dimension()

  for i in range(d) :
    print '          Variable_{} = {}'.format(i, lut.variable_type(i))

  for i in range(d) :
    print '          Index_{} = ('.format(i),
    n = lut.index_num(i)
    comma = ""
    for j in range(n) :
      print '{}{}'.format(comma, lut.index(i, j)),
      comma = ", "
    print ')'

  if d == 1 :
    print '          Values = (',
    n1 = lut.index_num(0)
    comma = ""
    for i1 in range(n1) :
      print '{}{}'.format(comma, lut.grid_value(i1)),
      comma = ", "
    print ')'
  elif d == 2 :
    print '          Values = ('
    n1 = lut.index_num(0)
    n2 = lut.index_num(1)
    for i1 in range(n1) :
      print '                     (',
      comma = ""
      for i2 in range(n2) :
        print '{}{}'.format(comma, lut.grid_value(i1, i2)),
        comma = ", "
      print ')'
    print '                   )'
  elif d == 3 :
    print '          Values = ('
    n1 = lut.index_num(0)
    n2 = lut.index_num(1)
    n3 = lut.index_num(2)
    for i1 in range(n1) :
      print '                     (',
      comma2 = ""
      for i2 in range(n2) :
        print '{}('.format(comma2),
        comma3 = ""
        for i3 in range(n3) :
          print '{}{}'.format(comma3, i1, i2, i3),
          comma3 = ", "
        print ')',
        comma2 = ", "
      print ')'
    print '                   )'

def print_timing(timing) :
  print '        Type: {}'.format(timing.type())
  print '        Condition: {}'.format(str(timing.timing_cond()))
  if delay_model == cell_lib.kCellDelayGenericCmos :
    print '        Rise Intrinsic  = {}'.format(timing.intrinsic_rise())
    print '        Rise Resistance = {}'.format(timing.rise_resistance())
    print '        Fall Intrinsic  = {}'.format(timing.intrinsic_fall())
    print '        Fall Resistance = {}'.format(timing.fall_resistance())
  elif delay_model == cell_lib.kCellDelayTableLookup :
    if timing.cell_rise() :
      print '        Cell Rise'
      print_lut(timing.cell_rise())
    if timing.rise_transition() :
      print '        Rise Transition'
      print_lut(timing.rise_transition())
    if timing.rise_propagation() :
      print '        Rise Propagation'
      print_lut(timing.rise_propagation())
    if timing.cell_fall() :
      print '        Cell Fall'
      print_lut(timing.cell_fall())
    if timing.fall_transition() :
      print '        Fall Transition'
      print_lut(timing.fall_transition())
    if timing.fall_propagation() :
      print '        Fall Propagation'
      print_lut(timing.fall_propagation())

def print_timing_list(cell, ipin, opin, tsense) :
  iid = ipin.input_id()
  oid = opin.output_id()
  timing_list = cell.timing_list2(iid, oid, tsense)
  if len(timing_list) == 0 :
    return
  print ''
  print '      Timing({}, {}, {})'.format(ipin.name(), opin.name(), tsense)
  for timing in timing_list :
    print_timing(timing)

def print_cell(cell) :
  print ''
  print '    Cell: {}'.format(cell.name())
  print '      Area: {}'.format(str(cell.area()))

  pin_list = cell.pin_list()
  for pin in pin_list :
    print_pin(pin)

  ipin_list = cell.input_pin_list()
  opin_list = cell.output_pin_list()
  ts_posi = cell_lib.kCellPosiUnate
  ts_nega = cell_lib.kCellNegaUnate
  for opin in opin_list :
    for ipin in ipin_list :
      print_timing_list(cell, ipin, opin, ts_posi)
      print_timing_list(cell, ipin, opin, ts_nega)

def print_group(group) :
  print '  Group#{}'.format(group.id())
  cell_list = group.cell_list()
  for cell in cell_list :
    print_cell(cell)

def print_class(cclass) :
  print ''
  print 'Class#{}'.format(cclass.id())
  group_list = cclass.cell_group_list()
  for group in group_list :
    print_group(group)

def print_pat(pat, id) :
  (root_id, inv) = pat.root_info()
  inv_char = ''
  if inv :
    inv_char = '~'
  print '  Pat#{}: RepId = {}, Root = {}{}'.format(id, pat.rep_id(), inv_char, root_id)
  print '    EdgeList = ',
  edge_list = pat.edge_list()
  comma = ''
  for edge_id in edge_list :
    print '{}{}'.format(comma, edge_id),
    comma = ', '
  print ''

def print_edge(edge, id) :
  (from_id, to_id, pos, inv) = edge
  print '  Edge#{}: From = {}, To = {}, Pos = {}, Inv = {}'.format(id, from_id, to_id, pos, inv)

path = "/home/matsunaga/data/private/dot-libs/TSMC/tcb670tc.lib"
lib = cell_lib.CellLibrary(path)
delay_model = lib.delay_model()
#cell_list = lib.cell_list()
#for cell in cell_list :
#  print cell.name()
#pg = lib.pg_pat(1)
#print pg.edge_list()
class_list = lib.npn_class_list()
for cclass in class_list :
  print_class(cclass)

pat_list = lib.pg_pat_list()
pos = 0
for pg in pat_list :
  print_pat(pg, pos)
  pos = pos + 1

edge_list = lib.pg_edge_list()
pos = 0
for edge in edge_list :
  print_edge(edge, pos)
  pos = pos + 1
