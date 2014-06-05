all:
	g++ -g main.cc decode.cc thumbsim_driver.cc parse.cc execute.cc -o thumbsim

run:
	./thumbsim -i -d -f inputs/fib.sim

clean:
	rm -rf ./*.o ./thumbsim

run1:
	./thumbsim -s -c 256 -i -d -f inputs/shang.O1.sim > shang.O1.out

diff1:
	diff shang.O1.completeoutput shang.O1.out
	
run2:
	./thumbsim -s -c 256 -i -d -f inputs/shang.O2.sim > shang.O2.out

diff2:
	diff shang.O2.completeoutput shang.O2.out
	