MAN1_TXT=unsmile.txt
MAN3_TXT=libsmile.txt

DOC_HTML=$(patsubst %.txt,%.html,$(MAN1_TXT) $(MAN3_TXT))

DOC_MAN1=$(patsubst %.txt,%.1,$(MAN1_TXT))
DOC_MAN3=$(patsubst %.txt,%.3,$(MAN3_TXT))

all: html man

html: $(DOC_HTML)

man: man1 man3
man1: $(DOC_MAN1)
man3: $(DOC_MAN3)

clean:
	rm -f *.xml *.html *.1 *.3

%.html : %.txt
	asciidoc -b css-embedded -d manpage $<

%.1 %.3 : %.xml
	xmlto man $<

%.xml : %.txt
	asciidoc -b docbook -d manpage $<
