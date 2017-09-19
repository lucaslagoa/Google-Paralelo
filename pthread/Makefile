#testando thread utilizar -pthread 
#Carlos Magno
prog: thread.c thread2.c thread3.c thread4.c thread5.c thread7.c thread1.c
	gcc -pthread -o thread thread.c
	gcc -pthread -o thread1 thread1.c
	gcc -pthread -o thread2 thread2.c
	gcc -pthread -o thread3 thread3.c
	gcc -pthread -o thread4 -w thread4.c
	gcc -pthread -o thread5 -w thread5.c
	gcc -pthread -o thread6 thread6.c -lm
	gcc -pthread -o thread7 thread7.c
	@echo Compilado com Sucesso.
	@echo Para limpar digite [make limpar]
limpar:
	rm -rf thread
	rm -rf thread1
	rm -rf thread2
	rm -rf thread3
	rm -rf thread4
	rm -rf thread5
	rm -rf thread6
	rm -rf thread7
	@echo Arquivos apagados com sucesso.
