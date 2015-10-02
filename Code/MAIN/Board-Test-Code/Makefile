CC=lcc-x32
CFLAGS=-I$(X32INC)
OBJS=buttterworth20_ord2_fix_board_test.o

.PHONY: clean stubbed stubbedrun uploadrun test

buttterworth20_ord2_fix_board_test.ce: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lib $(X32INC) -o buttterworth20_ord2_fix_board_test.ce

stubbed: CFLAGS+=-DSENSOR_STUB
stubbed: buttterworth20_ord2_fix_board_test.ce

stubbedrun: stubbed uploadrun

clean:
	rm -f buttterworth20_ord2_fix_board_test.ce $(OBJS) $(OBJS:.o=.d)

uploadrun: buttterworth20_ord2_fix_board_test.ce
	x32-upload buttterworth20_ord2_fix_board_test.ce -c $(X32_PACKAGE_SERIAL) -e

test: test.ce
	x32-upload test.ce -c $(X32_PACKAGE_SERIAL)

test.ce: test.c
	$(CC) $(CFLAGS) test.c -lib $(X32INC) -o test.ce

%.o : %.c
	@$(COMPILE.c) -M -o $@ $< > $*.d
	$(COMPILE.c) -o $@ $<
	@cp $*.d $*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
			-e '/^$$/ d' -e 's/$$/ :/' < $*.P >> $*.d; \
		rm -f $*.P

-include $(OBJS:.o=.d)
