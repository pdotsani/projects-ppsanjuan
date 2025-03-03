sysreport: sysreport.c lookup.c
	$(CC) -Wall $^ -o $@

install: sysreport
	install -m 755 sysreport /usr/local/bin/
