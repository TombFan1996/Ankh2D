#simple makefile for compiling Ankh2D on PSX

CC = ccpsx
PLATFORM = -DANKH2D_PSX
SRC = graphics.c bmath.c transform.c texture2d.c sprite.c 
OBJS = graphics.obj bmath.obj transform.obj texture2d.obj sprite.obj

main:
	$(CC) $(PLATFORM) -O3 -c $(SRC)
	$(CC) $(PLATFORM) -O3 -Xo$80020000 main.c $(OBJS) -omain.cpe,main.sym
	
for_menu:
	$(CC) $(PLATFORM) -O3 -Xo$80010000 main.c $(OBJS) main.obj, main.sym
	cpe2x main.cpe
	del main.cpe