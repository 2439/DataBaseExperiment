cc = gcc
prom = db
deps = extmem.h base.h linear_search.h main.h TPMMS.h index_search.h join_with_sort.h
obj = extmem.o base.o linear_search.o main.o TPMMS.o index_search.o join_with_sort.o

$(prom): $(obj)
	$(cc) -o $(prom) $(obj)

%.o: %.c $(deps)
	$(cc) -c $< -o $@

clean:
	del -rf $(obj) $(prom)