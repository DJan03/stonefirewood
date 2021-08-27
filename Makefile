OUT = bin/game.exe

ODIR = obj
SDIR = src

$(OUT): $(ODIR)/main.o
	gcc.exe $(ODIR)/main.o -o $(OUT)

$(ODIR)/%.o: $(SDIR)/%.c
	gcc.exe -Wall -c -o $@ $<

clean:
	del $(ODIR)\*

fclean:
	del bin\game.exe

run: $(OUT)
	run