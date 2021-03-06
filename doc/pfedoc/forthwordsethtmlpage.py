class ForthWordsetHtmlPage:
    """ takes a number of WordHtml objects (usually adapters) and creates
    some html page text from it (added to a HtmlDocument() later) """
    def __init__(self, wordset, o):
        self.wordset = wordset   # ForthWordsetAdapter(ForthWordset)
        self.o = o
        self.pages = []          # ForthWordPageAdapter(ForthWordsetEntry),...
        self.title = ""
        self.subtitle = ""
    def add(self, page):
        self.pages += [ page ]
    def get_title(self):
        ee = None
        if self.title: return self.title
        try:   return self.wordset.get_title()
        except Exception, e: ee = e; pass
        try:   return self.wordset.get_wordset_name()
        except Exception, e: print "ForthWordsetHtmlPage/title", ee, e; pass
        return self.o.package+" Forth Wordset Page"
    def get_subtitle(self):
        ee = None
        if self.subtitle: return self.subtitle
        try:   return self.wordset.get_subtitle()
        except Exception, e: ee = e; pass
        try:   return self.wordset.get_wordset_hint()
        except Exception, e: print "ForthWordsetHtmlPage/subtitle", ee, e; pass
        return "Description of Forth Wordset Export Entries"
    def get_filepart(self):
        ee = None
        try: return self.wordset.get_filepart()
        except Exception, e: ee = e; pass
        try: return self.wordset.get_listname()
        except Exception, e: print "ForthWordsetHtmlPage/filepart", ee, e; pass
        return None
    def html_text(self):
        T = ""
        text = self.get_filepart()
        if text:  T += '<a name="'+text+'">'+"</a>\n"
        text = self.get_title()
        if text:  T += "<h2>"+text+"</h2>\n"
        text = self.get_subtitle()
        if text:  T += "<blockquote>"+text+"</blockquote>\n"
        seen = False
        for page in self.pages:
            if page.invalid(): continue
            if not seen: T += "<dl>"; seen = True
            T += '<dt><hr width="90%" align="center" /><big>'
            text = page.xml_name()
            if text: T += "<b><code>"+text+"</code></b>"
            text = page.xml_stack()
            if text: T += "<b><i><code>"+text+"</i></code></b>"
            text = page.xml_hints()
            T += "<code>"+text+"</code>"
            T += "</big>"
            text = page.xml_wordlist()
            if text: T += " &nbsp;=&gt; <code><b> "+text+"</b></code>"
            T += "</dd>\n"
            T += "<dd>"
            text = page.xml_text()
            if text: T += text
            else: T += "<p>(?no text?)</p>\n"
            if not T.endswith("\n"): T += "\n"
            T += ('<p align="right">'+page.xml_typedescriptor()
                  +" = "+page.xml_param()+"</p>\n")
            T += "</dd>\n"
        if seen: T += "</dl>\n"
        if seen: return T+"<p>&nbsp;</p>\n\n"
        print "# empty wordset ",self.get_title()
        return ""
