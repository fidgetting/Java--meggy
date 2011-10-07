
import re
from functools import partial

contents = open('rdfvisitor.cpp').read()

outer = re.compile('void(.*?)\{((.|\n)*?)\}')  # regex to find methods
inner = re.compile('  ((.|\n)*?);\n')          # regex to expressions in methods

def replace(match):
  mlist = re.findall(inner,  match.group(0))
  mlist.reverse()
  
  return re.sub(inner, partial(sub_replace, mlist.__iter__()),
      match.group())

def sub_replace(miter, match):
  return '  ' + miter.next()[0] + ';\n'

print re.sub(outer, replace, contents);


