import gdb.printing

class GCHSmallVectorPrinter(object):
  'Print a gch::small_vector'

  class _iterator(object):
    def __init__ (self, start, finish):
      self.item = start
      self.finish = finish
      self.count = 0

    def __iter__(self):
      return self

    def __next__(self):
      count = self.count
      self.count = self.count + 1
      if self.item == self.finish:
          raise StopIteration
      elt = self.item.dereference()
      self.item = self.item + 1
      return ('[%d]' % count, elt)

  def __init__(self, val):
    self.val = val

  def get_base(self):
    return self.val['m_data']

  def get_data_base(self):
    base = self.get_base()
    return base.cast(base.type.fields()[0].type)

  def children(self):
    data_base = self.get_data_base()
    begin = data_base['m_data_ptr']
    end   = begin + data_base['m_size']
    return self._iterator(begin, end)

  def to_string(self):
    data_base = self.get_data_base()
    size     = int (data_base['m_size'])
    capacity = int(data_base['m_capacity'])
    return ('small_vector of length %d, capacity %d' % (size, capacity))

  def display_hint(self):
    return 'array'

def build_printer():
  pp = gdb.printing.RegexpCollectionPrettyPrinter('gch::small_vector')
  pp.add_printer('gch::small_vector', '^gch::small_vector<.*>$', GCHSmallVectorPrinter)
  return pp

gch_small_vector_printer = build_printer()
