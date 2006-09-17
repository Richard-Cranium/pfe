#! /usr/bin/env python
# -*- coding: UTF-8 -*-
import sys
from pfedoc.match import *
from pfedoc.options import *
from pfedoc.textfile import *
from pfedoc.textfileheader import *
from pfedoc.functionheader import *
from pfedoc.functionprototype import *
from pfedoc.commentmarkup import *
from pfedoc.functionlisthtmlpage import *
from pfedoc.functionlistreference import *
from pfedoc.dbk2htm import *

def _src_to_xml(text):
    return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
def _force_xml(text):
    if not text: return text
    text &= Match(r"<(/\w[\w:]*)>") >> "°[[[°\\1°]]]°"
    text &= Match(r"<(\w[\w:]*)>") >> "°[[[°\\1°]]]°"
    text &= Match(r"<(\w[\w:]*\s+\w+=[^<>]*)>") >> "°[[[°\\1°]]]°"
    text = _src_to_xml(text)
    return _hack_fixup(text.replace("°[[[°", "<").replace("°]]]°", ">"))
def _hack_fixup(text):
    return text & (
        Match("<(cr|closeparen|paren|quote|char|backtick|eol|eof|rp)>")
        >> "&lt;\\1;&gt;") & (
        Match("(>)<([ABN])>(</)") >> "\\1&lt;\\2&gt;\\3") & (
        Match("<br\s*/*>") >> "") & (
        Match("</?ansref>") >> "") & (
        Match("<small>((?:.(?!</small>))*.)</small>") >> "(\\1)")


class PerFileEntry:
    def __init__(self, header, comment):
        self.textfileheader = header
        self.filecomment = comment
class PerFile:
    def __init__(self):
        self.textfileheaders = []
        self.filecomments = []
        self.entries = []
    def add(self, textfileheader, filecomment):
        self.textfileheaders += [ textfileheader ]
        self.filecomments += [ filecomment ]
        self.entries += [ PerFileEntry(textfileheader, filecomment) ]
    def print_list_mainheader(self):
        for t_fileheader in self.textfileheaders:
            print t_fileheader.get_filename(), t_fileheader.src_mainheader()
        
class PerFunctionEntry:
    def __init__(self, header, comment, prototype):
        self.header = header
        self.comment = comment
        self.prototype = prototype
    def get_name(self):
        return self.prototype.get_name()
    def get_titleline(self):
        return self.header.get_titleline()
    def get_head(self):
        return self.prototype
    def get_body(self):
        return self.comment
class PerFunction:
    def __init__(self):
        self.headers = []
        self.comments = []
        self.prototypes = []
        self.entries = []
    def add(self, functionheader, functioncomment, functionprototype):
        self.headers += [ functionheader ]
        self.comments += [ functionprototype ]
        self.prototypes += [ functionprototype ]
        self.entries += [ PerFunctionEntry(functionheader, functioncomment,
                                           functionprototype) ]
    def print_list_titleline(self):
        for funcheader in self.headers:
            print funcheader.get_filename(), "[=>]", funcheader.get_titleline()
    def print_list_name(self):
        for funcheader in self.prototypes:
            print funcheader.get_filename(), "[>>]", funcheader.get_name()

class PerFunctionFamilyEntry:
    def __init__(self, leader):
        self.leader = leader
        self.functions = []
    def contains(self, func):
        for item in self.functions:
            if item == func: return True
        return False
    def add(self, func):
        if not self.contains(func):
            self.functions += [ func ]
    def get_name(self):
        if self.leader is None: return None
        return self.leader.get_name()
class PerFunctionFamily:
    def __init__(self):
        self.functions = []
        self.families = []
        self.retarget = {}
        self.entries = []
    def add_PerFunction(self, per_list):
        for item in per_list.entries:
            add_PerFunctionEntry(item)
    def add_PerFunctionEntry(self, item):
        self.functions += [ item ]
    def get_function(self, name):
        for item in self.functions:
            if item.get_name() == name:
                return item
        return None
    def get_entry(self, name):
        for item in self.entries:
            if item.get_name() == name:
                return item
        return None
    def fill_families(self):
        name_list = {}
        for func in self.functions:
            name = func.get_name()
            name_list[name] = func
        for func in self.functions:
            name = func.get_name()
            line = func.get_titleline()
            is_retarget = Match("=>\s*(\w+)")
            if line & is_retarget:
                into = is_retarget[1]
                self.retarget[name] = is_retarget[1]
        lead_list = []
        for name in self.retarget:
            into = self.retarget[name]
            if into not in name_list:
                print ("function '"+name+"' retarget into '"+into+
                       "' does not exist - keep alone")
            if into in self.retarget:
                other = self.retarget[into]
                print ("function '"+name+"' retarget into '"+into+
                       "' which is itself a retarget into '"+other+"'")
            if into not in lead_list:
                lead_list += [ into ]
        for func in self.functions:
            name = func.get_name()
            if name not in lead_list and name not in self.retarget:
                lead_list += [ name ]
        for name in lead_list:
            func = self.get_function(name)
            if func is not None:
                entry = PerFunctionFamilyEntry(func)
                entry.add(func) # the first
                self.entries += [ entry ]
            else:
                print "head function '"+name+" has no entry"
        for func in self.functions:
            name = func.get_name()
            if name in self.retarget:
                into = self.retarget[name]
                entry = self.get_entry(into)
                if entry is not None:
                    entry.add(func) # will not add duplicates
                else:
                    print "into function '"+name+" has no entry"
    def print_list_name(self):
        for family in self.entries:
            name = family.get_name()
            print name, ":",
            for item in family.functions:
                print item.get_name(), ",",
            print ""
class HtmlManualPageAdapter:
    def __init__(self, entry):
        """ usually takes a PerFunctionEntry """
        self.entry = entry
    def get_name(self):
        return self.entry.get_name()
    def _head(self):
        return self.entry.get_head()
    def _body(self):
        return self.entry.get_body()
    def head_xml_text(self):
        return self._head().xml_text()
    def body_xml_text(self, name):
        return self._body().xml_text(name)
    def head_get_prespec(self):
        return self._head().get_prespec()
    def head_get_namespec(self):
        return self._head().get_namespec()
    def head_get_callspec(self):
        return self._head().get_callspec()
    def get_title(self):
        return self._body().header.get_title()
    def get_filename(self):
        return self._body().header.get_filename()
    def src_mainheader(self):
        return self._body().header.parent.textfile.src_mainheader()
    def get_mainheader(self):
        return _src_to_xml(self.src_mainheader())
    def is_fcode(self):
        return self._head().is_fcode()
class RefEntryManualPageAdapter:
    def __init__(self, entry):
        """ usually takes a PerFunctionEntry """
        self.entry = entry
    def get_name(self):
        name = self.entry.get_name()
        if name: return _force_xml(name)
        return "??name??"
    def _head(self):
        return self.entry.get_head()
    def _body(self):
        return self.entry.get_body()
    def head_xml_text(self):
        return _force_xml(self._head().xml_text())
    def body_xml_text(self, name):
        return _force_xml(self._body().xml_text(name))
    def get_title(self):
        return _force_xml(self._body().header.get_title())
    def src_mainheader(self):
        return self._body().header.parent.textfile.src_mainheader()
    def get_mainheader(self):
        return _src_to_xml(self.src_mainheader())
    def get_includes(self):
        return ""
    def list_seealso(self):
        return self._body().header.get_alsolist()
    def get_authors(self):
        return None
    def get_copyright(self):
        return None
    def is_fcode(self):
        return self._head().is_fcode()

def makedocs(filenames, o):
    textfiles = []
    for filename in filenames:
        textfile = TextFile(filename)
        textfile.parse()
        textfiles += [ textfile ]
    per_file = PerFile()
    for textfile in textfiles:
        textfileheader = TextFileHeader(textfile)
        textfileheader.parse()
        filecomment = CommentMarkup(textfileheader)
        filecomment.parse()
        per_file.add(textfileheader, filecomment)
    funcheaders = []
    for textfile in per_file.textfileheaders:
        funcheader = FunctionHeaderList(textfile)
        funcheader.parse()
        funcheaders += [ funcheader ]
    per_function = PerFunction()
    for funcheader in funcheaders:
        for child in funcheader.get_children():
            funcprototype = FunctionPrototype(child)
            funcprototype.parse()
            funccomment = CommentMarkup(child)
            funccomment.parse()
            per_function.add(child, funccomment, funcprototype)
    per_family = PerFunctionFamily()
    for item in per_function.entries:
        per_family.add_PerFunctionEntry(item)
    per_family.fill_families()
    # debug output....
    # per_file.print_list_mainheader()
    # per_function.print_list_titleline()
    # per_function.print_list_name()
    per_family.print_list_name()
    html = FunctionListHtmlPage(o)
    for item in per_family.entries:
        for func in item.functions:
            func_adapter = HtmlManualPageAdapter(func)
            if o.onlymainheader and not (Match("<"+o.onlymainheader+">")
                                         & func_adapter.src_mainheader()):
                    continue
            html.add(func_adapter)
        html.cut()
    html.cut()
    html_filename = "pfe-words"+o.suffix+".html"
    try:
        print "writing "+html_filename
        fd = open(html_filename, "w")
        print >>fd, section2html(paramdef2html(html.xml_text()))
        fd.close()
    except IOError, e:
        print "could not open '"+html_filename+"'file", e
    man3 = FunctionListReference(o)
    for item in per_family.entries:
        for func in item.functions:
            func_adapter = RefEntryManualPageAdapter(func)
            man3.add(func_adapter)
        man3.cut()
    man3.cut()
    man3_filename = "pfe-words"+o.suffix+".docbook"
    try:
        print "writing "+man3_filename
        fd = open(man3_filename, "w")
        print >>fd, man3.xml_text()
        fd.close()
    except IOError, e:
        print "could not open '"+man3_filename+"'file", e
    
        
if __name__ == "__main__":
    filenames = []
    o = Options()
    o.suffix = ""
    o.package = "PFE/Forth"
    o.program = sys.argv[0]
    for item in sys.argv[1:]:
        if o.scan(item): continue
        filenames += [ item ]
    makedocs(filenames, o)