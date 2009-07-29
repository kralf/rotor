import os

_proc_status = '/proc/%d/status' % os.getpid()

_scale = {'kB': 1024.0, 'mB': 1024.0*1024.0,
          'KB': 1024.0, 'MB': 1024.0*1024.0}

#-------------------------------------------------------------------------------

def _VmB(VmKey):
  global _proc_status, _scale
  try:
      t = open(_proc_status)
      v = t.read()
      t.close()
  except:
      return 0.0  # non-Linux?
  i = v.index(VmKey)
  v = v[i:].split(None, 3)
  if len(v) < 3:
      return 0.0  # invalid format?
  return float(v[1]) * _scale[v[2]]

#-------------------------------------------------------------------------------

def memory(since=0.0):
  '''Return memory usage in bytes.
  '''
  return _VmB('VmSize:') - since

#-------------------------------------------------------------------------------

def resident(since=0.0):
  '''Return resident memory usage in bytes.
  '''
  return _VmB('VmRSS:') - since

#-------------------------------------------------------------------------------

def stacksize(since=0.0):
  '''Return stack size in bytes.
  '''
  return _VmB('VmStk:') - since
