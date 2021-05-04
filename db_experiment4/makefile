cc = gcc
prom = db
deps = extmem.h base.h linear_search.h main.h TPMMS.h
obj = extmem.o base.o linear_search.o main.o TPMMS.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	rm -rf $(obj) $(prom)