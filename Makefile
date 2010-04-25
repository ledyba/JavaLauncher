# Makefile ( n source file 1 execute file version )
# 2002/12/ 4

# ���ܸ�Υ�������ʬ�˽��äƽ������Ʋ�������
# $(HOGEHOGE) �Ȥ����Τϥޥ����ʰ����ѿ��ˤǡ�
# HOGEHOGE=... ��������줿���Ƥ�Ÿ�����Ʋ�ᤵ��ޤ���
# ��Ƭ�Υ��֤ˤϰ�̣������ޤ��Τǡ����ڡ������Ѵ����ʤ��ǲ�������
# �Ǥ���С��ǽ�Ū�����ܸ쥳���ȤϺ�����Ʋ�������

#	 ���ѥå�����̾���ļ¹ԥե�����̾�˽񤭴�����
PACKAGE	= JavaStarter
#	 ���������ե��������¤٤�
SRCS	= JavaStarter.c
#	 ���إå��ե��������¤٤�
HEADS	= 
OBJS	= $(SRCS:.c=.o) $(PACKAGE)_res.o

#	 ���ѥå������˴ޤ��ե�����̾����¤٤�
FILES	= Makefile $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`


### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence
RELEASE = -s

# common (*.o)
LD	= gcc
LDFLAGS	= -g $(DEBUG) $(RELEASE)
LDLIBS	= -lm -lmingw32 -mwindows

# C (*.c)
CC	= gcc
CFLAGS	= -g -O3 -Wall $(DEBUG) $(RELEASE)
CPPFLAGS= -I.

# C++ (*.cc)
CXX	= g++
CXXFLAGS= -g -O3 -Wall $(DEBUG)

# Fortran77 (*.f)
FC	= f77
FFLAGS	= -Wall $(DEBUG)

# Pascal (*.p)
PC	= pc
PFLAGS	= -Wall $(DEBUG)

# etc
SHELL	= /bin/sh
RM	= rm -f
PROF	= gprof


### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cc .f .p

all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

$(OBJS): $(HEADS) Makefile

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
.f.o:
	$(FC) $(FFLAGS) -c $< -o $@
.p.o:
	$(PC) $(PFLAGS) $(CPPFLAGS) -c $< -o $@

$(PACKAGE)_res.o: $(PACKAGE).rc
	windres -i $(PACKAGE).rc -o $(PACKAGE)_res.o

### useful commands ###

clean:
	$(RM) $(PACKAGE) $(OBJS)
	$(RM) core gmon.out *~ #*#

tar:
	@echo $(PACKAGE)-$(VER) > .package
	@$(RM) -r `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | gzip -9 > `cat .package`.tar.gz
	@$(RM) -r `cat .package` .package

zip:
	zip -9 $(PACKAGE)-$(VER).zip $(FILES)


prof: run
	$(PROF) $(PACKAGE) | less

run: all
#	 ������ץ�ǡ����Ǽ¹Ԥ���褦�˽�ľ��
#	    $(PACKAGE) �ϼ¹ԥե�����̾��Ÿ�������
#	    ��Ƭ�ϥ��֡ʥ��ڡ����ǤϤʤ���
	./$(PACKAGE) < sample-data | less