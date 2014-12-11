sources := $(wildcard ./src/dbop/*.java)
sources += $(wildcard ./src/global/*.java)
sources += $(wildcard ./src/pdfop/*.java)
sources += $(wildcard ./src/mail/*.java)
sources += $(wildcard ./src/form/*.java)
sources += $(wildcard ./src/cui/*.java)

all:$(sources)
	javac -classpath ./libjar/libpdf.jar:./libjar/libmail.jar -d class $?

.PHONY: run, clean

clean:
	rm -rf ./class/*

run:
	java -classpath ./libjar/libsql.jar:./libjar/libpdf.jar:./libjar/libmail.jar:./libjar/iTextAsian.jar:./libjar/mail.jar:./class hqzs_form
